
#include "YEntry.h"


const float YEntry::yFolders = (SCREEN_HEIGHT/2 + 32);
const int YEntry::interX = 10;
const int YEntry::MinInterX = 0;//YEntry::interX;
const float YEntry::DEF_ZOOM = 0.5;
const float YEntry::DEF_INT_Y = 20;
const unsigned YEntry::DEF_ALPHA = 240;
const string YEntry::ARCHIVE_EXTS[] = { "zip", "rar" };
string YEntry::EBOOT_NAMES[] = EBOOTNAMES;

YEntry::YEntry( string name )
{
	mName = name;
	mDispName = "";
	mIconTex = NULL;
	mIcon = NULL;
	mIsHomebrew = false;
	mEbootName = "";
	mZoom = YEntry::DEF_ZOOM;
	mEboot = NULL;
	mArchiveType = NO_FILE;
	mEbootExists = false;
}

YEntry::~YEntry()
{
	
}


void YEntry::Create()
{
	// Give default icon before loadup completed
	if (this->mIcon == NULL)	this->mIcon =  DSystm::GetInstance()->getFolderIcon();
	
}

void YEntry::Destroy()
{
	SAFE_DELETE(mIconTex);
	
	DSystm* dirSys = DSystm::GetInstance();

	if (mIcon != dirSys->getDefIcon() && \
		mIcon != dirSys->getFolderIcon() && \
		mIcon != dirSys->getCorruptIcon())
		SAFE_DELETE(mIcon);
	
	SAFE_DELETE(mEboot);
}


string YEntry::getPathName()
{
	return DSystm::GetInstance()->getWorkPath() + this->mName;
}

string YEntry::getEbootPath()
{
	return this->getPathName()+"/"+this->mEbootName;
}

string YEntry::getArchivePath()
{
	return this->getPathName() + "/" + mArchiveName;
}

string YEntry::getRealName()
{
	return this->mName;
}

void YEntry::initDispName()
{
	string fName;
	bool done = false;
	
	if (mEboot != NULL)
	{
		char* name = mEboot->getTitle();
		if (name != NULL)	fName = name, done = true;
	}

	if (!done)
	{
		string dispName = mName, prefix = CAT_PREFIX;
		StrCast(dispName, ::tolower);
		StrCast(prefix, ::tolower);


		if ( dispName.find(prefix) == 0)	dispName = mName.substr(prefix.size());
		else	dispName = mName;

		fName = dispName;
	}

	mDispName = fName;
}

string YEntry::getDispName()
{
	return mDispName;
}

bool YEntry::inHbPath()
{
	if (DSystm::GetInstance()->getWorkPath().find(HOMEBREW_PATH)!=string::npos)	return true;
	else return false;
}

bool YEntry::inSavePath()
{
	if (DSystm::GetInstance()->getWorkPath().find(SAVEDATA_PATH)!=string::npos)	return true;
	else return false;
}

bool YEntry::isHomebrew()
{
	if ( mEboot != NULL && inHbPath() )
		this->mIsHomebrew = true;
	
	return this->mIsHomebrew;
}

Coord& YEntry::getPos()
{
	return this->mPos;
}

void YEntry::setZoom ( float zoom )
{
	if ( zoom > 1 )	zoom = 1;
	
	this->mZoom = zoom;
}

float YEntry::getZoom()
{
	return this->mZoom;
}


int YEntry::findEboot()
{
	bool done = false;
	string cache;
	
	#ifdef PSP
	SceUID id;
	SceIoDirent entry;

	id = sceIoDopen((this->getPathName()+"/").c_str());

	if (id < 0)
		return -1;

	
	for (int i=0;i<(sizeof(EBOOT_NAMES)/sizeof(string)) && !done;++i )
		StrCast(EBOOT_NAMES[i], ::tolower);
	
	
	memset(&entry, 0, sizeof(SceIoDirent));
	while (!done && sceIoDread(id, &entry) > 0)
	{
		cache = entry.d_name;
		StrCast(cache, ::tolower);
		for (int i=0;i<(sizeof(EBOOT_NAMES)/sizeof(string)) && !done;++i )
		{

			if ( cache == EBOOT_NAMES[i] )
			{
				mEbootExists = true;
				done = true;
				mEbootName = entry.d_name;
				//YLOG("entry.d_name: \n",entry.d_name);
				//YLOG("mEbootName: %s\n",mEbootName.c_str());
			}
		}
		
		
		memset(&entry, 0, sizeof(SceIoDirent));
	}
	
	sceIoDclose(id);
	#endif

	return 0;
}


int YEntry::findArchive()
{
	bool done = false;
	string cache, archiveName = ARCHIVE_NAME, temp;
	StrCast(archiveName, ::tolower);
	
	#ifdef PSP
	SceUID id;
	SceIoDirent entry;

	id = sceIoDopen((this->getPathName()+"/").c_str());

	if (id < 0)
		return -1;
	
	
	memset(&entry, 0, sizeof(SceIoDirent));
	while (!done && sceIoDread(id, &entry) > 0)
	{
		cache = entry.d_name;
		StrCast(cache, ::tolower);
		
		for (int i=0;i<(sizeof(ARCHIVE_EXTS)/sizeof(string)) && !done;++i )
		{
			temp = archiveName + "." + ARCHIVE_EXTS[i];
			//YLOG("%s == %s ?\n",cache[0].c_str(),temp.c_str());
			if ( cache == temp )
			{
				if (i)	mArchiveType = RAR_FILE;
				else	mArchiveType = ZIP_FILE;
				mArchiveName = entry.d_name;

				done = true;
				//YLOG("entry.d_name: %s\n",entry.d_name);
				//YLOG("mArchiveName: %s, mArchiveType: %i\n",mArchiveName.c_str(), mArchiveType);
			}
		}
		
		
		memset(&entry, 0, sizeof(SceIoDirent));
	}
	
	sceIoDclose(id);
	#endif

	return 0;
}

bool YEntry::containsEboot()
{
	return mEbootExists;
}


void YEntry::updateAlpha( float intensity )
{
	// Alpha = minimal value + value left * zoom percentage
	const u8 alpha = (u8)(intensity*(YEntry::DEF_ALPHA+(u8)((this->getZoom()-YEntry::DEF_ZOOM)/YEntry::DEF_ZOOM*(0xFF-YEntry::DEF_ALPHA))));
	Display::SetQuadAlpha( mIcon, ((float)alpha)/255 );
}

int YEntry::getArchiveType()
{
	return mArchiveType;
}

Eboot* YEntry::getEboot()
{
	return mEboot;
}

JTexture* YEntry::getIconTex()
{
	return mIconTex;
}

JQuad* YEntry::getIcon()
{
	return mIcon;
}


void YEntry::setEboot( Eboot* eboot )
{
	mEboot = eboot;
}

void YEntry::setIconTex( JTexture* iconTex )
{
	mIconTex = iconTex;
}

void YEntry::setIcon( JQuad* icon )
{
	mIcon = icon;
}

