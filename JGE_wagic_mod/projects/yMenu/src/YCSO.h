
#ifndef _YCSO_H_
#define _YCSO_H_

#include "YISO.h"
#include <zlib.h>


/*
 *
 * CSO decompress algorithm ported from Virtuous Flame's ciso.py's decompress_cso code :
 * http://code.google.com/p/procfw/downloads/detail?name=ciso.py
 *
 * CSO header struct taken from procfw source code :
 * http://code.google.com/p/procfw/source/browse/Vshctrl/isoreader.c
 *
*/


typedef struct _CISOHeader {
	u8 magic[4];			/* +00 : 'C','I','S','O'                           */
	u32 header_size;
	u64 total_bytes;	/* +08 : number of original data size              */
	u32 block_size;		/* +10 : number of compressed block size           */
	u8 ver;				/* +14 : version 01                                */
	u8 align;			/* +15 : align of index (offset = index[n]<<align) */
	u8 rsv_06[2];		/* +16 : reserved                                  */
} __attribute__ ((packed)) CISOHeader;


class YCSO : public YISO
{

private:
	CISOHeader mHead;
	unsigned mTotalBlock;
	
	bool initDecompress();
	virtual int readSector( char *destBuf, unsigned sector );
	
	
public:
	YCSO( string csoPath );
	~YCSO();
	
	static bool isCSO ( string filePath );
};


#endif
