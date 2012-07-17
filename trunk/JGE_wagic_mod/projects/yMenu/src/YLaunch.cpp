
#include "YLaunch.h"
#include "lib.h"


YLaunch::YLaunch( string bootPath )
{
	mAppType = NO_APP;
	this->setBootPath( bootPath );
	mEbootPath = "";
	mRunlevel = -1;
	mIsoDriver = MODE_INFERNO;
	
	
	// Clear Memory
	memset(&mParam, 0, sizeof(mParam));
	
	// Set Common Parameters
	mParam.size = sizeof(mParam);
}

YLaunch::~YLaunch()
{

}

int YLaunch::launch()
{
	// If not an app or no bootPath, don't launch
	if ( this->getAppType() == NO_APP || this->getBootPath() == "" )	return -1;
	
	
	sctrlSESetBootConfFileIndex(mIsoDriver);
	
	// ISO Runlevel
	if(this->getRunlevel() == PSP_INIT_APITYPE_UMDEMU_MS)
	{
		
		// PSN EBOOT Support
		if( this->getAppType() == PSN_APP )
		{
			// Disable Galaxy ISO Emulator Patch
			sctrlSESetUmdFile("");
		}
		
		// ISO
		else
		{
			// Enable Galaxy ISO Emulator Patch
			sctrlSESetUmdFile(mBootPath.c_str());
		}
	}
	
	
	YLOG("mRunlevel: %X, bootPath: %s\n, mParam.key: %s, mParam.argp: %s, mParam.args: %X\n", mRunlevel, this->getBootPath().c_str(), mParam.key, mParam.argp, mParam.args );
	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(mRunlevel, this->getBootPath().c_str(), &mParam);
}

int YLaunch::setAppType( int appType )
{
	int errCode = 1, tmpRunlevel = -1;
	
	
	switch ( appType )
	{
		case NO_APP:
			tmpRunlevel = -1;
			break;
			
		case HOMEBREW_APP:
			mIsoDriver = MODE_INFERNO;
			tmpRunlevel = PSP_INIT_APITYPE_MS2;
			break;
			
		case PSN_APP:
			mIsoDriver = MODE_NP9660;
			tmpRunlevel = PSP_INIT_APITYPE_UMDEMU_MS;
			break;
			
		case UNTOUCHED_ISO_APP:
			mIsoDriver = MODE_INFERNO;
			tmpRunlevel = PSP_INIT_APITYPE_UMDEMU_MS;
			break;
			
		case PATCHED_ISO_APP:
			mIsoDriver = MODE_INFERNO;
			tmpRunlevel = PSP_INIT_APITYPE_UMDEMU_MS;
			break;
			
		case POPS_APP:
			tmpRunlevel = PSP_INIT_APITYPE_MS5;
			break;
			
		default:
			errCode = 0;
	}
	
	
	if ( errCode )
	{
		mAppType = appType;
		this->setRunlevel( tmpRunlevel );
	}
	
	
	return errCode;
}

int YLaunch::getAppType()
{
	return mAppType;
}

int YLaunch::setRunlevel( int runlevel )
{
	int errCode = 1;
	
	
	switch ( runlevel )
	{
		case PSP_INIT_APITYPE_MS2:
		
			// Prepare Homebrew Reboot
			mEbootPath = mBootPath;
			mParam.key = "game";
			
			break;
			
		case PSP_INIT_APITYPE_UMDEMU_MS:
			
			// EBOOT Path
			mEbootPath = "disc0:/PSP_GAME/SYSDIR/EBOOT";
			

			if ( this->getAppType() == UNTOUCHED_ISO_APP || this->getAppType() == PSN_APP )	mEbootPath += ".BIN";
			
			// Else PATCHED_ISO_APP
			else	mEbootPath += ".OLD";
			
			
			// Prepare ISO Reboot
			mParam.key = "umdemu";
			
			break;
			
			
		case PSP_INIT_APITYPE_MS5:
		
			// Prepare Pops Reboot
			mEbootPath = mBootPath;
			mParam.key = "pops";
			
			break;
			
		default:
			errCode = 0;
	}
	
	
	if ( errCode )
	{
		mParam.args = mEbootPath.size() + 1;
		mParam.argp = (void*)mEbootPath.c_str();
		mRunlevel = runlevel;
	}
	
	
	return errCode;
}

int YLaunch::getRunlevel()
{
	return mRunlevel;
}

void YLaunch::setBootPath( string bootPath )
{
	mBootPath = bootPath;
}

string YLaunch::getBootPath()
{
	return mBootPath;
}

