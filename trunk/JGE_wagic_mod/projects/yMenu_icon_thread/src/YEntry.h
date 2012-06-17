
#ifndef _YENTRY_H_
#define _YENTRY_H_

#include "common.h"
#include "DSystm.h"
#include "Eboot.h"
#include "GameApp.h"

enum {
	NO_FILE, ZIP_FILE, RAR_FILE
};


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

	int findArchive();
	
	
public:
	static const string ARCHIVE_EXTS[];
	static string EBOOT_NAMES[];
	static const float yFolders;
	static const int MinInterX;
	static const int interX;
	static const float DEF_ZOOM;
	static const unsigned DEF_ALPHA;
	static const float DEF_INT_Y;

	YEntry( string name );
	~YEntry();
	void Create();
	void Destroy();
	
	void initDispName();
	bool containsEboot();
	int findEboot();
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
	Eboot* getEboot();
	JTexture* getIconTex();
	JQuad* getIcon();
	void setEboot( Eboot* eboot );
	void setIconTex( JTexture* iconTex );
	void setIcon( JQuad* icon );

};


#endif
