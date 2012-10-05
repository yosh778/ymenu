#include "Eboot.h"


Eboot::Eboot( string ebootPath )
: YLaunch ( ebootPath )
{
	mPngData = NULL;
	mTitle = NULL;
	//YLOG("ebootPath: %s\n", ebootPath.c_str());
	readEboot( ebootPath );
	
}

Eboot::~Eboot()
{
	SAFE_FREE(mCategory);
	SAFE_FREE(mTitle);
}

void Eboot::readEboot( string ebootPath )
{
	//YLOG("Eboot::readEboot\n");
	int eID = Lib::openFile( ebootPath );
	
	readOffsets( eID );
	transOffsets( eID );
	readPngData( eID );
	getSfoData( eID );
	//YLOG("Eboot::readEboot done\n");
}

EbootSlot* Eboot::getSlot( int slot )
{
	if (slot >= 0 && slot < mSlots.size())
		return &mSlots[slot];
	
	else	return NULL;
}


int Eboot::readOffsets( int id )
{
	int offset[8];
#ifdef PSP
	sceIoLseek(id, 8, SEEK_SET);
	sceIoRead(id, offset, sizeof(offset));
	
	mEbootSize = sceIoLseek(id, 0, SEEK_END);
	
	
	for ( int i=0;i<(sizeof(offset)/sizeof(int));++i )
		mSlots.push_back( (EbootSlot){offset[i],0} );
		
#else
#endif

	return 0;
}

void Eboot::transOffsets(int id)
{
	// Calc slots's sizes
	for ( int i=0;i<mSlots.size()-2;++i )
		mSlots[i].size = mSlots[i+1].offset - mSlots[i].offset;
	
	mSlots[mSlots.size()-1].size = mEbootSize - mSlots[mSlots.size()-1].offset;
}

void Eboot::readPngData(int id)
{
	#ifdef PSP
	if (mSlots[EBOOT_ICON0].size > 0)
	{
		mPngData = malloc(mSlots[EBOOT_ICON0].size);
		YLOG("mPngData: 0x%08lX\n", mPngData);
		sceIoLseek(id, mSlots[EBOOT_ICON0].offset, SEEK_SET);
		sceIoRead(id, mPngData, mSlots[EBOOT_ICON0].size);
		YLOG("mPngData done\n");
	}
	#endif
}

void Eboot::getSfoData(int id)
{
	#ifdef PSP
	YLOG("Eboot::getSfoData\n");
	if (mSlots[EBOOT_SFO].size > 0)
	{
		void* sfoData = malloc(mSlots[EBOOT_SFO].size);
		sceIoLseek(id, mSlots[EBOOT_SFO].offset, SEEK_SET);
		sceIoRead(id, sfoData, mSlots[EBOOT_SFO].size);
		Lib::closeFile( id );
		YLOG("sfoData offset: 0x%X\n", sfoData);
		
		Sfo sfo(sfoData);
		mTitle = sfo.getTitle();
		mCategory = sfo.getCategory();
		
		SAFE_FREE(sfoData);
	}
	#endif
}


void Eboot::freePngData()
{
	SAFE_FREE(mPngData);
}


void* Eboot::getPngData()
{
	return mPngData;
}


string Eboot::getTitle()
{
	if (mTitle == NULL)	return "";
	else	return mTitle;
}


char* Eboot::getCategory()
{
	return mCategory;
}


unsigned int Eboot::getPngSize()
{
	return mSlots[EBOOT_ICON0].size;
}

int Eboot::appInit()
{
	int errCode;
	
	
	int appType = NO_APP;
	string category = this->getCategory();
	
	if ( category == "ME" )	appType = POPS_APP;
	else if ( category == "MG" )	appType = HOMEBREW_APP;
	else if ( category == "EG" )	appType = PSN_APP;
	
	errCode = this->setAppType( appType );
	
	
	
	return errCode;
}

bool Eboot::isEboot(string filePath)
{
	// Result (Not EBOOT)
	bool result = false;
	
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
			// Valid Magic
			if( !memcmp(header, "\0PBP", sizeof(header)) )
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

