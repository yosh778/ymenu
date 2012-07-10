
#include "YCSO.h"


YCSO::YCSO( string csoPath )
: YLaunch ( )
{
	mPngData = NULL;
}

YCSO::~YCSO()
{

}

bool YCSO::isCSO ( string filePath )
{
	// Result (Not CSO)
	int result = false;
	
	// Open File
	SceUID fd = sceIoOpen(filePath.c_str(), PSP_O_RDONLY, 0777);
	
	// Opened File
	if(fd >= 0)
	{
		// Header Buffer
		unsigned char header[4];
		
		// Read Header
		if(sizeof(header) == sceIoRead(fd, header, sizeof(header)))
		{
			// CSO Header Magic
			unsigned char isoFlags[4] = {
				0x43, 0x49, 0x53, 0x4F
			};
			
			// Valid Magic
			if( !memcmp(header, isoFlags, sizeof(header)) )
			{
				// CSO File
				result = true;
			}
		}
		
		// Close File
		sceIoClose(fd);
	}
	
	// Return Result
	return result;
}

void YCSO::freePngData()
{
	SAFE_FREE(mPngData);
}


void* YCSO::getPngData()
{
	return mPngData;
}

string YCSO::getTitle()
{
	return "";
}

int YCSO::appInit( string bootPath )
{
	int errCode;
	
	this->setBootPath( bootPath );
	
	
	int appType = UNTOUCHED_ISO_APP;//NO_APP;
	
	//if ( untouched )	appType = UNTOUCHED_ISO_APP;
	//else if ( patched )	appType = PATCHED_ISO_APP;
	
	errCode = this->setAppType( appType );
	
	
	
	return errCode;
}


