
#include "Sfo.h"


Sfo::Sfo( void *data )
{
	mData = data;
	if (mData != NULL)
	{
		this->init();
	}
}

Sfo::~Sfo()
{
	mEntries.clear();
}

void Sfo::init()
{
	void* p = (void*)((int)mData);
	
	mHeader = *((sfo_header*)p);
	p = (void*)((int)p + sizeof(sfo_header));
	//YLOG("mHeader: %X, %X, %X, %X, %X\n",mHeader.magicNb, mHeader.version, mHeader.valOffset, mHeader.paramOffset, mHeader.nbEntries);
	
	for (int i=0; i<mHeader.nbEntries; ++i )
	{
		mEntries.push_back( *((sfo_entry*)p) );
		p = (void*)((int)p + sizeof(sfo_entry));
		//YLOG("mEntries[%i]: %X, %X, %X, %X, %X, %X\n", i,mEntries[i].valOffset, mEntries[i].unknown0, mEntries[i].unknown1, mEntries[i].minValSize, mEntries[i].maxValSize, mEntries[i].paramOffset);
	}


	//YLOG("init done\n");
}


int Sfo::findValueIdx( string name )
{
	int valIdx = -1;
	void* p = (void*)((int)mData + mHeader.valOffset);
	char* valName = NULL;
	
	
	//YLOG("findValueIdx\n");
	for (int i=0; i<mHeader.nbEntries && valIdx==-1; ++i )
	{
		//YLOG("findValueIdx in for\n");
		p = (void*)((int)mData + mHeader.valOffset);
		p = (void*)((int)p + mEntries[i].valOffset);
		
		valName = (char*)p;
		//YLOG("valName[%i]: %X_=_", i,valName);
		//YLOG("%s\n",valName);
		
		
		if (!strcmp(name.c_str(),valName))	valIdx = i;
	}
	
	
	//YLOG("findValueIdx done\n");
	return valIdx;
}



void* Sfo::getParamOffset( int idx )
{
	//YLOG("getParamOffset\n");
	if (idx < mEntries.size() && idx > -1)
		return (void*)((int)mData + mHeader.paramOffset + mEntries[idx].paramOffset);

	else	return (void*)-1;
}

char* Sfo::getTitle()
{
	//YLOG("getTitle\n");
	int idx = this->findValueIdx("TITLE");
	char *res = (char*)this->getParamOffset(idx);
	//YLOG("title: %X_=_",res);
	//YLOG("%s\n",res);
	
	if ( (int)res > -1)
	{
		// Copy title to new allocated space, as the sfo is gonna get freed
		int len = mEntries[idx].minValSize + 1;
		char *title = (char*)malloc( len );
		strncpy(title, res, len);
		
		return title;
	}
	else	return NULL;
}

char* Sfo::getCategory()
{
	//YLOG("getCategory\n");
	int idx = this->findValueIdx("CATEGORY");
	char *res = (char*)this->getParamOffset(idx);
	//YLOG("category: %X_=_",res);
	//YLOG("%s\n",res);
	
	if ( (int)res > -1)
	{
		// Copy category to new allocated space, as the sfo is gonna get freed
		int len = mEntries[idx].minValSize + 1;
		char *category = (char*)malloc( len );
		strncpy(category, res, len);
		
		return category;
	}
	else	return NULL;
}

