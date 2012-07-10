
#ifndef _YENTRY_H_
#define _YENTRY_H_

#include "common.h"
#include "DSystm.h"
#include "Eboot.h"
#include "GameApp.h"
#include "YLaunch.h"
#include "YISO.h"
#include "YCSO.h"

enum {
	NO_FILE, ZIP_FILE, RAR_FILE, ISO_FILE, CSO_FILE
};

class Eboot;

class YEntry
{
	friend class DSystm;
	friend class YDir;


private:
	string mName;
	string mDispName;
	Coord mPos;
	bool mIsApp;
	JTexture* mIconTex;
	JQuad* mIcon;
	string mEbootName;
	string mArchiveName;
	float mZoom;
	YLaunch *mApp;
	int mFileType;
	bool mEbootExists;
	bool mIsFolder;

	bool containsEboot();
	bool findEboot();
	int findArchive();
	void initDispName();
	void setFileType();
	void checkBackup();
	
	
public:
	static const string ARCHIVE_EXTS[];
	static string EBOOT_NAMES[];
	static const float yFolders;
	static const int MinInterX;
	static const int interX;
	static const float DEF_ZOOM;
	static const unsigned DEF_ALPHA;
	static const float DEF_INT_Y;

	YEntry( SceIoDirent name );
	~YEntry();
	void Create();
	void Destroy();
	
	int getArchiveType();
	string getPathName();
	string getRealName();
	string getDispName();
	string getAppPath();
	string getArchivePath();
	bool inHbPath();
	bool inSavePath();
	bool isApp();
	Coord& getPos();
	float getZoom();
	void setZoom ( float zoom );
	void updateAlpha( float intensity );
	bool IsFolder();
};


#endif
