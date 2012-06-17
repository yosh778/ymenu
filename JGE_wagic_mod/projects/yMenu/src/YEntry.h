
#ifndef _YENTRY_H_
#define _YENTRY_H_

#include "common.h"
#include "DSystm.h"
#include "Eboot.h"
#include "GameApp.h"

enum {
	NO_FILE, ZIP_FILE, RAR_FILE
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
	bool mIsHomebrew;
	JTexture* mIconTex;
	JQuad* mIcon;
	string mEbootName;
	string mArchiveName;
	float mZoom;
	Eboot *mEboot;
	int mArchiveType;
	bool mEbootExists;
	bool mIsFolder;

	bool containsEboot();
	int findEboot();
	int findArchive();
	void initDispName();
	void setFileType();
	
	
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
	string getEbootPath();
	string getArchivePath();
	bool inHbPath();
	bool inSavePath();
	bool isHomebrew();
	Coord& getPos();
	float getZoom();
	void setZoom ( float zoom );
	void updateAlpha( float intensity );
	bool IsFolder();
};


#endif
