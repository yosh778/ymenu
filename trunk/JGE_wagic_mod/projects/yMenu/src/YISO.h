
#ifndef _YISO_H_
#define _YISO_H_

#include "YLaunch.h"


class YISO : public YLaunch
{

private:
	void* mPngData;
	
	
public:
	YISO( string isoPath );
	~YISO();
	
	string getTitle();
	static bool isISO ( string filePath );
	void* getPngData();
	void freePngData();
	int appInit( string bootPath );
};


#endif
