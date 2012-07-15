
#ifndef _YLAUNCH_H_
#define _YLAUNCH_H_

#include "common.h"

enum appTypes_ {
	NO_APP, HOMEBREW_APP, PSN_APP, POPS_APP, UNTOUCHED_ISO_APP, PATCHED_ISO_APP
};


class YLaunch
{

private:
	int mAppType;
	int mRunlevel;
	string mBootPath;
	string mEbootPath;
	
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam mParam;
	
	
	int setRunlevel( int runlevel );
	int getRunlevel();
	void setBootPath( string bootPath );
	string getBootPath();
	
	
public:
	YLaunch( string bootPath );
	virtual ~YLaunch();
	
	int launch();
	int getAppType();
	int setAppType( int appType );
	virtual string getTitle() = 0;
	virtual void* getPngData() = 0;
	virtual void freePngData() = 0;
	virtual int appInit() = 0;
	
};


#endif
