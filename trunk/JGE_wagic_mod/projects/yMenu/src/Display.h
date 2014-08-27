
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"
#include "DSystm.h"


enum {
	NOTIFICATION_INACTIVE, NOTIFICATION_ACTIVE, NOTIFICATION_OK, NOTIFICATION_CANCEL, NOTIFICATION_WORKING, NOTIFICATION_DIE, NOTIFICATION_INIT
};


class Display
{

private:
	vector<string> mWallpapers;
	int mCurBg;
	JTexture* mBgTex;
	JQuad* mBg;
	JTexture* mOldBgTex;
	JQuad* mOldBg;
	float mBgAlpha;
	unsigned long mLastSwap;
	bool mSwapBg;
	int mLTTid;
#ifdef USE_TTF
	JTTFont* mFont;
#else
	intraFont* mFont;
#endif
	JTexture* mBatteryTex;
	JQuad* mBatteryQ;
	int mNotify;
	bool mChoiceDone;
	bool mChoice;
	string mNotifyMess;
	int mBatteryP;
	Coord mBatteryPos;
	int mHideOpacity;
	int mEventType;
	
	static Display *mDisplay;
	
	Display(
			#ifdef USE_TTF
			JTTFont* font
			#else
			intraFont* mFont
			#endif
			);
	~Display();
	
	void FindBgs();


public:
	static int swapBgState(int mSwapBgState = -2);
	static int MAX_HIDE_OPACITY;
	
	void Destroy();
	
	void SwapBg( int& finished, int mode );
	void updateBg();
	void renderBg();
	void LoadTQThread ( string path, int options, JTexture*& tex, JQuad*& quad, int& finished );
	int getSwapBgState();
	void renderInfoQuad();
	void renderInfoFont();
	void updateNotify();
	bool getNotifyChoice();
	int getNotifyState();
	void renderNotify();
	void Notify( string mess );
	
	static Display* GetInstance(
								#ifdef USE_TTF
								JTTFont* font = NULL
								#else
								intraFont* font = NULL
								#endif
								);
	static void SetQuadAlpha( JQuad* quad, float alpha );
	static void LoadTextQuad ( string path, int options, JTexture*& tex, JQuad*& quad );
	void setEventType( int eventType );
	int getEventType();

};


#endif
