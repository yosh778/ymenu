
#include "YLaunch.h"
#include "lib.h"


YLaunch::YLaunch( string bootPath )
{
	mAppType = NO_APP;
	this->setBootPath( bootPath );
	mEbootPath = "";
	mRunlevel = -1;
	m_is_pspgo = false;

	mIsoDriver = MODE_DEFAULT;


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
	
	string umdFile = "";
	int ret = -1;

	#ifndef VHBL

	#ifndef TN_CFW
	sctrlSESetBootConfFileIndex(mIsoDriver);

	// ISO Runlevel
	if (this->getRunlevel() == PSP_INIT_APITYPE_UMDEMU_MS)
	{
		
		// PSN EBOOT Support
		if( this->getAppType() == PSN_APP )
		{
			// Disable Galaxy ISO Emulator Patch
			umdFile = "";
		}

		// ISO
		else
		{
			// Enable Galaxy ISO Emulator Patch
			umdFile = mBootPath.c_str();
		}

		sctrlSESetUmdFile(umdFile.c_str());
	}
	#else
	if (this->getRunlevel() == PSP_INIT_APITYPE_DISC)
	{
		umdFile = mBootPath.c_str();
		mBootPath = mEbootPath;
		
		//YLOG("sctrlSEMountUmdFromFile: %s, mIsoDriver: %X\n", umdFile.c_str(), mIsoDriver);
		sctrlSEMountUmdFromFile(umdFile.c_str(), mIsoDriver);
		//YLOG("mountUmdFsctrlSEMountUmdFromFileromFile done");
	}
	#endif


	// PSP GO Fix
	#ifdef PROCFW
		// This call will always fail, but with a different error code depending on the model
		SceUID result = sceIoOpen("ef0:/", 1, 0777);

		// Check for "No such device" error
		m_is_pspgo = !(result == (int)0x80020321);

		if (m_is_pspgo) {
			switch ( mRunlevel )
			{
				case PSP_INIT_APITYPE_MS2:
					mRunlevel = PSP_INIT_APITYPE_EF2;
					break;

				case PSP_INIT_APITYPE_UMDEMU_MS:
					mRunlevel = PSP_INIT_APITYPE_UMDEMU_EF;
					break;

				case PSP_INIT_APITYPE_MS5:
					mRunlevel = PSP_INIT_APITYPE_EF5;
					break;
			}

			if (mEbootPath[0] == 'm' && mEbootPath[1] == 's')
			{
				mEbootPath[0] = 'e';
				mEbootPath[1] = 'f';
			}
		}
	#endif

	//YLOG("mRunlevel: %X, bootPath: %s\n, mParam.key: %s, mParam.argp: %s, mParam.args: %X\n", mRunlevel, this->getBootPath().c_str(), mParam.key, mParam.argp, mParam.args );
	// Trigger Reboot
	ret = sctrlKernelLoadExecVSHWithApitype(mRunlevel, this->getBootPath().c_str(), &mParam);

	//YLOG("sctrlKernelLoadExecVSHWithApitype done");
	#endif

	return ret;
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
			mIsoDriver = MODE_DEFAULT;
			tmpRunlevel = PSP_INIT_APITYPE_MS2;
			break;
			
		case PSN_APP:
			mIsoDriver = MODE_NP9660;
			tmpRunlevel = PSP_INIT_APITYPE_UMDEMU_MS;
			break;
			
		case UNTOUCHED_ISO_APP:
			mIsoDriver = MODE_DEFAULT;
			tmpRunlevel = PSP_INIT_APITYPE_UMDEMU_MS;
			break;
			
		case PATCHED_ISO_APP:
			mIsoDriver = MODE_DEFAULT;
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
			
			
			// Prepare game Reboot
			#ifndef TN_CFW
			mParam.key = "umdemu";
			#else
			if (this->getAppType() == PSN_APP)
			{
				mParam.key = "umdemu";
			}
			else
			{
				mParam.key = "game";
				runlevel = PSP_INIT_APITYPE_DISC;
			}
			#endif
			
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
	// PSP GO Fix
	#ifdef PROCFW
	if (m_is_pspgo) {
		mBootPath[0] = 'e';
		mBootPath[1] = 'f';
	}
	#endif

	return mBootPath;
}

