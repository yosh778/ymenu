
#include "YISO.h"
#include "lib.h"


const u32 YISO::SECTOR_SIZE = 0x800;



YISO::YISO( string isoPath )
: YLaunch ( isoPath )
{
	mPngData = NULL;
	mBackupType = NO_APP;
	mTitle = NULL;
	
	this->open( isoPath );
}

YISO::~YISO()
{
	SAFE_FREE(mTitle);
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
		unsigned char header[8];
		
		// Read Header
		if(sizeof(header) == sceIoRead(fd, header, sizeof(header)))
		{
			// ISO Header Magic
			unsigned char isoFlags[8] = {
				0x01, 0x43, 0x44, 0x30, 0x30, 0x31, 0x01, 0x00
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
	if (mTitle == NULL)	return "";
	else	return mTitle;
}

int YISO::appInit()
{
	return this->setAppType( mBackupType );
}

bool YISO::open( string path )
{
	mFin.open(path.c_str(),std::ios::binary);
	
	return mFin.is_open();
}

int YISO::readSector( char *destBuf, unsigned sector )
{
	mFin.seekg(lba2Pos(sector), ios::beg);
	mFin.read(destBuf, YISO::SECTOR_SIZE);
	
	return YISO::SECTOR_SIZE;
}

void YISO::close()
{
	mFin.close();
}

void* YISO::read( u32 sector, u32 len )
{
	u32 bufSize = ((len / YISO::SECTOR_SIZE) + 1)*YISO::SECTOR_SIZE;
	void* data = malloc(bufSize);
	u32 sizeRead = 0;
	u32 curSector = sector;
	
	while ( sizeRead < len )
	{
		int ret = this->readSector((char*)((u32)data+sizeRead), curSector );
		
		if ( ret < 0 )	break;
		else
		{
			sizeRead+= ret;
			++curSector;
		}
	}
	
	//YLOG("read: %X\n", data);
	
	return data;
}

void YISO::processPathTable( PathTableRecord* pathTable, u32 pathTableSize )
{
	PathTableRecord* curRecord = pathTable;
	u32 recordSize;
	
	while ( (u32)curRecord < (u32)pathTable+pathTableSize )
	{
		mPathTable.push_back( curRecord );
		
		recordSize = sizeof(PathTableRecord) + curRecord->nameSize;
		if ( recordSize%2 )	++recordSize;
		
		curRecord = (PathTableRecord*) ((u32)curRecord + recordSize);
	}
}


u16 YISO::findDirPathTable( string dirPath, u16 parent )
{
	u32 curIdx = -1;
	bool found = false;
	//YLOG("findDirPathTable: %X %s\n", parent, dirPath.c_str());
	
	// If path contains at least one directory, search
	if ( dirPath.find('/') != string::npos )
	{
		while ( !found &&  ++curIdx < mPathTable.size() )
		{
			if ( mPathTable[curIdx]->parentIdx == parent )
				if ( !strnicmp(mPathTable[curIdx]->name, dirPath.c_str(), mPathTable[curIdx]->nameSize) )
					found = true;
		}
	}
	
	
	if (found)
	{
		return this->findDirPathTable( dirPath.substr( dirPath.find('/')+1), curIdx+1 );
	}
	else	return parent;
}

vector<DirectoryRecord*>* YISO::getDir( DirectoryRecord* dir )
{
	DirectoryRecord* curRecord = dir;
	vector<DirectoryRecord*>* dirList = new vector<DirectoryRecord*>();
	
	
	while ( curRecord->size != 0 )
	{
		dirList->push_back( curRecord );
		
		curRecord = (DirectoryRecord*) ((u32)curRecord + curRecord->size);
	}
	
	
	return dirList;
}

DirectoryRecord* YISO::findFile( string fileName, DirectoryRecord* dir )
{
	u32 curIdx = -1;
	bool found = false;
	vector<DirectoryRecord*>* dirList = this->getDir(dir);
	DirectoryRecord* ret = NULL;
	
	
	while ( !found &&  ++curIdx < dirList->size() )
	{
		if ( !strnicmp((*dirList)[curIdx]->name, fileName.c_str(), (*dirList)[curIdx]->nameSize) )	found = true;
	}
	
	
	if ( found )	ret = (*dirList)[curIdx];
	
	SAFE_DELETE(dirList);
	
	
	return ret;
}

void* YISO::getFile( DirectoryRecord* fileRecord )
{
	return this->read( fileRecord->lba.LE, fileRecord->fileSize.LE );
}


void YISO::Create()
{
	if ( mFin.is_open() )
	{
		char *sectorBuf = (char*)malloc( YISO::SECTOR_SIZE );
		
		int err;
		err = this->readSector( sectorBuf, 16);
		if ( err >= 0 )
		{
			PrimaryVolumeDescriptor* pvd = (PrimaryVolumeDescriptor*)sectorBuf;
			u32 lbaPathTableL = pvd->lbaPathTableL;
			u32 pathTableSize = pvd->pathTableSize.LE;
			
			
			YLOG("PrimaryVolumeDescriptor: %X %X\n", lbaPathTableL, pathTableSize);
			void *pathTableBuf = this->read(lbaPathTableL, pathTableSize);
			
			this->processPathTable( (PathTableRecord*)pathTableBuf, pathTableSize );
			
			
			u16 dirId;
			if ( (dirId = this->findDirPathTable( "PSP_GAME/SYSDIR/" )) > 1 )
			{
				YLOG("findDirPathTable done: %X %X\n", dirId, mPathTable[dirId-1]->lba);
				this->readSector( sectorBuf, mPathTable[dirId-1]->lba);
				
				DirectoryRecord* dir = (DirectoryRecord*)sectorBuf;
				DirectoryRecord* old = findFile( "EBOOT.OLD", dir );
				DirectoryRecord* bin = findFile( "EBOOT.BIN", dir );
				
				if (old != NULL)
				{
					YLOG("OLD: %08lX %08lX\n", old->fileSize.LE, old->lba.LE);
					mBackupType = PATCHED_ISO_APP;
				}
				else if (bin != NULL)
				{
					YLOG("BIN: %08lX %08lX\n", bin->fileSize.LE, bin->lba.LE);
					mBackupType = UNTOUCHED_ISO_APP;
				}
				
			}
			
			
			//YLOG("findDirPathTable go\n");
			if ( (dirId = this->findDirPathTable( "PSP_GAME/" )) > 1 )
			{
				YLOG("findDirPathTable done: %X %X\n", dirId, mPathTable[dirId-1]->lba);
				this->readSector( sectorBuf, mPathTable[dirId-1]->lba);
				
				DirectoryRecord* dir = (DirectoryRecord*)sectorBuf;
				DirectoryRecord* png = findFile( "ICON0.PNG", dir );
				DirectoryRecord* sfo = findFile( "PARAM.SFO", dir );
				
				if (png != NULL)
				{
					mPngData = this->getFile(png);
					YLOG("mPngData: %X\n", mPngData);
				}
				if (sfo != NULL)
				{
					void* sfoData = this->getFile(sfo);
					YLOG("sfoData: %X\n", sfoData);
					
					Sfo sfo(sfoData);
					YLOG("sfo initialized\n");
					mTitle = sfo.getTitle();
					YLOG("sfo getTitle done\n");
					//mCategory = sfo.getCategory();
					
					SAFE_FREE(sfoData);
				}
				
			}
			
			SAFE_FREE(pathTableBuf);
			mPathTable.clear();
		}
		else	YLOG("Read error: %X\n", err);
		
		
		
		SAFE_FREE(sectorBuf);
		
		this->close();
	}

	YLOG("Create done\n");
}

inline u32 YISO::lba2Pos( u32 lba )
{
	return lba*YISO::SECTOR_SIZE;
}


