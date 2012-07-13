
#include "YCSO.h"


YCSO::YCSO( string csoPath )
: YISO ( csoPath )
{
	mPngData = NULL;
}

YCSO::~YCSO()
{

}

bool YCSO::isCSO ( string filePath )
{
	// Result (Not CSO)
	int result = false;
	
	// Open File
	SceUID fd = sceIoOpen(filePath.c_str(), PSP_O_RDONLY, 0777);
	
	// Opened File
	if(fd >= 0)
	{
		// Header Buffer
		unsigned char header[4];
		
		// Read Header
		if(sizeof(header) == sceIoRead(fd, header, sizeof(header)))
		{
			// CSO Header Magic
			unsigned char isoFlags[4] = {
				0x43, 0x49, 0x53, 0x4F
			};
			
			// Valid Magic
			if( !memcmp(header, isoFlags, sizeof(header)) )
			{
				// CSO File
				result = true;
			}
		}
		
		// Close File
		sceIoClose(fd);
	}
	
	// Return Result
	return result;
}

bool YCSO::initDecompress()
{
	bool success = true;
	
	
	mFin.read((char*)&mHead,sizeof(mHead));
	
	if ( strncmp((char*)mHead.magic, "CISO", 4) || mHead.block_size == 0 || mHead.total_bytes == 0 )	success = false;
	else
	{
		mTotalBlock = mHead.total_bytes / mHead.block_size;
		int tmp_i;

		for (int i=0; i< mTotalBlock + 1; ++i)
		{
			mFin.read((char*)&tmp_i,sizeof(int));
			mIndexBuf.push_back(tmp_i);
		}
	}
	
	
	return success;
}


/*
 * Returns negative if error, size read else
 *
*/

int YCSO::readSector( char *destBuf, unsigned sector )
{
	int ret = 0;
	
	
	if ( sector < mTotalBlock )
	{
		int index = mIndexBuf[sector];
		int plain = index & 0x80000000;
		index  &= 0x7fffffff;
		int read_pos = index << (mHead.align);
		unsigned read_size;
		
		if ( plain )
			read_size = mHead.block_size;
		else
		{
			int index2 = mIndexBuf[sector+1] & 0x7fffffff;
			// Have to read more bytes if align was set
			if ( mHead.align )
				read_size = (index2-index+1) << (mHead.align);
			else
				read_size = (index2-index) << (mHead.align);
		}
		
		
		void *cso_data = NULL;
		void *destBuf = NULL;
		cso_data = malloc(read_size);
		unsigned decLen = 0;

		mFin.seekg(read_pos,ios::beg);
		mFin.read((char*)cso_data,read_size);
		
		
		if ( plain )
		{
			decLen = read_size;
			memcpy(destBuf, cso_data, decLen);
		}
		else
		{
			int zErr = 0;
			
			
			z_stream stream;
			int err;

			stream.next_in = (Bytef*)cso_data;
			stream.avail_in = (uInt)read_size;
			stream.next_out = (Bytef*)destBuf;
			stream.avail_out = (uInt)YISO::SECTOR_SIZE;

			stream.zalloc = NULL;
			stream.zfree = NULL;

			err = inflateInit2(&stream, -15);
			if (err == Z_OK)
			{
				err = inflate(&stream, Z_FINISH);
				if (err != Z_STREAM_END)	zErr = -2;
				//*destLen = stream.total_out;
				decLen = stream.total_out;

				inflateEnd(&stream);
			}
			else	zErr = -1;
			
			
			
			if ( zErr )	ret = zErr;
		}
		
		
		free(cso_data);
		
		
		if ( !ret )	ret = decLen;
	}
	
	
	return ret;
}


