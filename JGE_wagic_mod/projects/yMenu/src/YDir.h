
#ifndef _YDIR_H_
#define _YDIR_H_

#include "common.h"
#include "lib.h"
#include "YEntry.h"

class YEntry;

class YDir
{
	friend class DSystm;

private:
	vector<YEntry> mFolders;
	int mCurFolder;
	int mOldFolder;
	Coord mToCross;
	Coord mCurToCross;
	Coord mOffset;
	Coord mCurOffset;
	Coord mFocus;
	bool mSlideOut;
	float mCurAlpha;
	float mCurInterToCross;
	float mInterToCross;
	float mInterOffset;

	static map<string,int> mDirIdxs;
	string mCurDir;
	bool mRush;
	bool mOldRush;
	int mLastRush;
	int mFirstRush;
	bool mDoRush;

	bool restoreCurDirIdx();
	void updateLogicPosX();
	bool initCurDirIdx( string deadChild );
	bool findChildIdx( string deadChild );
	
public:
	static const int SLIDE_STEP_X;
	static const int RUSH_INTER_MS;
	static const int RUSH_TRIGGER_MS;
	static const int RUSH_SLIDE_STEP_X;
	static const int SLIDE_STEP_Y;
	static const int INTER_Y;
	static const int MIN_FOLDER_Y;
	static const int MAX_FOLDER_Y;

	YDir();
	~YDir();
	void Create( string deadChild = "", bool init = false );
	void Destroy();

	void update();
	void renderIcons();
	int addCurFolder( int step );
	int getCurFolder();
	void initSlideY( int slideInY, int focusY );
	void slideOut( bool isParent );
	void updateAlpha();
	void initOffsetX();
	
	static string GetParent ( string path );

	#ifdef PSP
	static vector<SceIoDirent>* getDir( string dirName );
	static vector<string>* getDirFolders( string dirName );
	static vector<string>* getDirNames( string dirpath, enum name_formats_ format );
	#endif
};


#endif
