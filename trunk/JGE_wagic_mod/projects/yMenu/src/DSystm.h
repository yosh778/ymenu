
#ifndef _DSYSTM_H_
#define _DSYSTM_H_

#include "common.h"
#include "lib.h"
#include "YDir.h"
#include "Display.h"
#include "YLaunch.h"


class YDir;
class YEntry;

class DSystm
{

private:
	YDir *mDir;
	YDir *mOldDir;
	string mAppPath;
	JTexture* mFolderIconTex;
	JQuad* mFolderIcon;
	JTexture* mDefIconTex;
	JQuad* mDefIcon;
	JTexture* mCorruptIconTex;
	JQuad* mCorruptIcon;
	JTexture* mFileIconTex;
	JQuad* mFileIcon;
	JTexture* mRarIconTex;
	JQuad* mRarIcon;
	JTexture* mZipIconTex;
	JQuad* mZipIcon;
#ifdef USE_TTF
	JTTFont* mFont;
#else
	intraFont* mFont;
#endif
	bool mOldMove;
	int mToAddDir;

    CutCache mCutCache;
    bool mExit;

	string mWorkPath;
	JSample *mClic;
	
	static DSystm *mDSystm;
	
	DSystm(
			#ifdef USE_TTF
			JTTFont* font
			#else
			intraFont* mFont
			#endif
			);
	~DSystm();
	
	
public:
	static DSystm* GetInstance(
								#ifdef USE_TTF
								JTTFont* font = NULL
								#else
								intraFont* font = NULL
								#endif
								);
	void Create(string workPath);
	void Destroy();
	
	//	-1 on fatal error, 0 on success
	int readDir();
	void render();
	void update();
	void ChYDir( bool getParent );
	void setWorkPath( string workPath );
	void setAppPath( string ebootPath );
	string getWorkPath();
	string getAppPath();
	JQuad* getDefIcon();
	JQuad* getFolderIcon();
	JQuad* getCorruptIcon();
	JQuad* getFileIcon();
	JQuad* getRarIcon();
	JQuad* getZipIcon();
	bool getOldMove();
	YLaunch* getCurApp();
    bool isExit();
};


#endif
