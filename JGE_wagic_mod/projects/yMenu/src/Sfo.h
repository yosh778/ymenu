
#ifndef _SFO_H_
#define _SFO_H_

#include "common.h"
#include "lib.h"


typedef struct {
	u32 magicNb;
	u32 version;
	u32 valOffset;
	u32 paramOffset;
	u32 nbEntries;
} sfo_header;


enum sfo_param_type {
	PSF_TYPE_BIN = 0, PSF_TYPE_STR = 2, PSF_TYPE_VAL = 4
};

typedef struct {
	u16 valOffset;
	u8 alignment;
	u8 type;	// sfo_param_type
	u32 minValSize;
	u32 maxValSize;
	u32 paramOffset;
} sfo_entry;


class Sfo
{

private:
	void *mData;
	sfo_header mHeader;
	vector<sfo_entry> mEntries;
	
	void init();
	
public:
	Sfo( void *data );
	~Sfo();

	int findValueIdx( string name );
	void* getParamOffset( int paramIdx );
	char* getTitle();
	char* getCategory();
	
};


#endif
