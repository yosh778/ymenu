
#ifndef _DSYSTM_H_
#define _DSYSTM_H_

#include "common.h"
#include "lib.h"
#include "YDir.h"
#include "Display.h"


class YDir;
class YEntry;

class DSystm
{
	friend class GameApp;

private:
	YDir *mDir;
	YDir *mOldDir;
	string mEbootPath;
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
	void setEbootPath( string ebootPath );
	string getWorkPath();
	string getEbootPath();
	JQuad* getDefIcon();
	JQuad* getFolderIcon();
	JQuad* getCorruptIcon();
	JQuad* getFileIcon();
	JQuad* getRarIcon();
	JQuad* getZipIcon();
	bool getOldMove();
};


#endif
