
#ifndef _YISO_H_
#define _YISO_H_

#include "YLaunch.h"


class YISO : public YLaunch
{

private:
	

protected:
	ifstream mFin;
	void* mPngData;
	
	virtual bool open( string path );
	virtual int readSector( char *destBuf, unsigned sector );
	virtual void close();
	
	
public:
	static unsigned SECTOR_SIZE;
	

	YISO( string isoPath );
	~YISO();
	
	virtual string getTitle();
	virtual void* getPngData();
	virtual void freePngData();
	virtual int appInit();
	
	static bool isISO ( string filePath );
};


#endif
