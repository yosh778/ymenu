
#include "Eboot.h"


Eboot::Eboot( string ebootPath )
{
	mPngData = NULL;
	mTitle = NULL;
	
	readEboot( ebootPath );
	
}

Eboot::~Eboot()
{
	SAFE_FREE(mTitle);
}

void Eboot::readEboot( string ebootPath )
{
	int eID = Lib::openFile( ebootPath );
	
	readOffsets( eID );
	transOffsets( eID );
	readPngData( eID );
	getSfoData( eID );
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
		sceIoLseek(id, mSlots[EBOOT_ICON0].offset, SEEK_SET);
		sceIoRead(id, mPngData, mSlots[EBOOT_ICON0].size);
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


char* Eboot::getTitle()
{
	return mTitle;
}


unsigned int Eboot::getPngSize()
{
	return mSlots[EBOOT_ICON0].size;
}

