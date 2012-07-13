
#include "YISO.h"


const unsigned YISO::SECTOR_SIZE = 0x800;



YISO::YISO( string isoPath )
: YLaunch ( isoPath )
{
	mPngData = NULL;
}

YISO::~YISO()
{

}

bool YISO::isISO ( string filePath )
{
	// Result (Not ISO)
	bool result = false;
	
	// Open File
	SceUID fd = sceIoOpen(filePath.c_str(), PSP_O_RDONLY, 0777);
	
	// Opened File
	if(fd >= 0)
	{
		// Move to ISO Header
		sceIoLseek32(fd, 0x8000, PSP_SEEK_SET);
		
		// Header Buffer
		unsigned char header[16];
		
		// Read Header
		if(sizeof(header) == sceIoRead(fd, header, sizeof(header)))
		{
			// ISO Header Magic
			unsigned char isoFlags[16] = {
				0x01, 0x43, 0x44, 0x30, 0x30, 0x31, 0x01, 0x00,
				0x50, 0x53, 0x50, 0x20, 0x47, 0x41, 0x4D, 0x45
			};
			
			// Valid Magic
			if( !memcmp(header, isoFlags, sizeof(header)) )
			{
				// ISO File
				result = true;
			}
		}
		
		// Close File
		sceIoClose(fd);
	}
	
	// Return Result
	return result;
}

void YISO::freePngData()
{
	SAFE_FREE(mPngData);
}


void* YISO::getPngData()
{
	return mPngData;
}

string YISO::getTitle()
{
	return "";
}

int YISO::appInit()
{
	int errCode;
	
	
	int appType = UNTOUCHED_ISO_APP;//NO_APP;
	
	//if ( untouched )	appType = UNTOUCHED_ISO_APP;
	//else if ( patched )	appType = PATCHED_ISO_APP;
	
	errCode = this->setAppType( appType );
	
	
	
	return errCode;
}

bool YISO::open( string path )
{
	mFin.open(path.c_str(),std::ios::binary);
	
	return mFin.is_open();
}

int YISO::readSector( char *destBuf, unsigned sector )
{
	mFin.seekg(sector*YISO::SECTOR_SIZE, ios::beg);
	mFin.read(destBuf, YISO::SECTOR_SIZE);
	
	return YISO::SECTOR_SIZE;
}

void YISO::close()
{
	mFin.close();
}


