
#ifndef _YCSO_H_
#define _YCSO_H_

#include "YLaunch.h"


class YCSO : public YLaunch
{

private:
	void* mPngData;
	
	
public:
	YCSO( string csoPath );
	~YCSO();
	
	string getTitle();
	static bool isCSO ( string filePath );
	void* getPngData();
	void freePngData();
	int appInit();
};


#endif
