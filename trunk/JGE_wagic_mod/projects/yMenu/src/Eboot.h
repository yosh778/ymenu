
#ifndef _EBOOT_H_
#define _EBOOT_H_

#include "common.h"
#include "lib.h"
#include "Sfo.h"


enum eboot_slots_ {
	EBOOT_SFO, EBOOT_ICON0
};


class Eboot
{

private:
	vector<EbootSlot> mSlots;
	int mEbootSize;
	void* mPngData;
	char* mTitle;
	char* mCategory;

	void readEboot( string ebootPath );
	int readOffsets( int id );
	void readPngData(int id);
	void transOffsets( int id );
	void getSfoData(int id);
	
public:
	
	Eboot( string ebootPath );
	~Eboot();

	EbootSlot* getSlot( int slot );
	void* getPngData();
	void freePngData();
	char* getTitle();
	char* getCategory();
	unsigned int getPngSize();
};


#endif
