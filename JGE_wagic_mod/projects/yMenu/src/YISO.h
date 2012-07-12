
#ifndef _YISO_H_
#define _YISO_H_

#include "YLaunch.h"


class YISO : public YLaunch
{

private:
	

protected:
	void* mPngData;
	
	
public:
	YISO( string isoPath );
	~YISO();
	
	virtual string getTitle();
	virtual void* getPngData();
	virtual void freePngData();
	virtual int appInit();
	
	static bool isISO ( string filePath );
};


#endif
