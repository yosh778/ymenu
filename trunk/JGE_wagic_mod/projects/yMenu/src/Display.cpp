
#include "Display.h"


Display* Display::mDisplay = NULL;
int Display::MAX_HIDE_OPACITY = 246;



Display::Display(
				#ifdef USE_TTF
				JTTFont* font
				#else
				intraFont* font
				#endif
				)
{
	mFont = font;
	mSwapBgState = -1;
	mLastSwap = clock();
	mBgAlpha = 1;
	mOldBgTex = NULL;
	mOldBg = NULL;
	mBgTex = NULL;
	mBg = NULL;
	mSwapBg = false;
	mCurBg = -1;
	mLTTid = -1;
	mBatteryTex = NULL;
	mBatteryQ = NULL;
	mNotify = NOTIFICATION_INACTIVE;
	mHideOpacity = 0;
	mEventType = -1;
	
	this->FindBgs();
	
	int dummy;
	this->SwapBg( dummy, 0 );

	Display::LoadTextQuad ( "battery.png", TEX_TYPE_USE_VRAM, mBatteryTex, mBatteryQ );
}

Display::~Display()
{
	SAFE_DELETE(mOldBgTex);
	SAFE_DELETE(mOldBg);
	SAFE_DELETE(mBgTex);
	SAFE_DELETE(mBg);
	SAFE_DELETE(mBatteryTex);
	SAFE_DELETE(mBatteryQ);
}

void Display::Destroy()
{
#ifdef PSP
	if ( !getSwapBgState() )
	{
		// While load tex thread running, wait
		while ( !getSwapBgState() )	sceKernelDelayThread(1000);
		
		sceKernelDeleteThread(mLTTid);
	}
#endif
	
	SAFE_DELETE(mDisplay);
}


Display* Display::GetInstance(
								#ifdef USE_TTF
								JTTFont* font
								#else
								intraFont* font
								#endif
								)
{
	if ( Display::mDisplay == NULL && font != NULL )
		Display::mDisplay = new Display( font );
	
	return Display::mDisplay;
}




void Display::SetQuadAlpha( JQuad* quad, float a )
{
	unsigned char alpha = a*255;
	quad->SetColor(0x00FFFFFF | (alpha<<24));
}


void Display::LoadTextQuad ( string path, int options, JTexture*& tex, JQuad*& quad )
{
	tex = JRenderer::GetInstance()->LoadTexture(path.c_str(), options);
	if ( tex == NULL )	FATAL_ERROR("LoadTextQuad failed to load tex\n");

	quad = new JQuad(tex, 0, 0, tex->mWidth, tex->mHeight);
	quad = quad;
}


void Display::SwapBg( int& finished, int mode )
{
	if ( mWallpapers.size() )
	{
		int alea;

		do	alea = rand()%mWallpapers.size();
		while (alea == mCurBg);

		mCurBg = alea;

		#ifdef PSP
		if (mode)	LoadTQThread ( mWallpapers[mCurBg], TEX_TYPE_NONE, mBgTex, mBg, finished );
		else	LoadTextQuad ( mWallpapers[mCurBg], TEX_TYPE_NONE, mBgTex, mBg );
		#else
		LoadTextQuad ( mWallpapers[mCurBg], TEX_TYPE_NONE, mBgTex, mBg );
		finished = 1;
		#endif
	}
}


void Display::FindBgs()
{
	bool checkNextBg = true;
	const char* exts[] = WALLPAPER_EXTS;
	srand(time(NULL));
	#ifdef PSP
	vector<string>* dir = YDir::getDirNames("./", LOWERCASE);
	vector<string> dirDest;
	
	string prefix = WALLPAPER_PREFIX;
	StrCast(prefix, ::tolower);
	
	string curName;
	
	for (int i=0; i<dir->size(); ++i )
	{
		//YLOG("%s begins with %s ?  ",(*dir)[i].c_str(),prefix.c_str());
		if ( (*dir)[i].find(prefix) == 0 )
		{
			dirDest.push_back((*dir)[i]);
			//YLOG("Yes\n");
		}
		//else	YLOG("No\n");
	}
	SAFE_DELETE(dir);
	
	
	stringstream path;
	for ( int i=0; checkNextBg; ++i )
	{
		for ( int j=0; j<(sizeof(exts)/sizeof(char*)) && checkNextBg; ++j )
		{
			path << WALLPAPER_PREFIX << i << "." << exts[j];
			string p = path.str();
			StrCast(p, ::tolower);
			
			
			for (int i=0; i<dirDest.size() && checkNextBg; ++i )
			{
				//YLOG("%s == %s ?  ",dirDest[i].c_str(),p.c_str());
				if ( !dirDest[i].compare(p) )
				{
					mWallpapers.push_back(p), checkNextBg = false;
					//YLOG("Yes\n");
				}
				//else	YLOG("No\n");
			}
			
			path.clear();
			path.str("");
		}
		checkNextBg = !checkNextBg;
	}
	#else 
	stringstream path;
	string cache;

	
	SceIoDirent entry;

	id = sceIoDopen("./");

	if (id < 0)
		return -1;
		
	
	memset(&entry, 0, sizeof(SceIoDirent));
	while (checkNextBg && sceIoDread(id, &entry) > 0)
	{
		cache = entry.d_name;
		StrCast(cache, ::tolower);
		for ( int i=0; checkNextBg; ++i )
		{
			for ( int j=0; j<(sizeof(exts)/sizeof(char*)) && checkNextBg; ++j )
			{
				path << WALLPAPER_PREFIX << i << "." << exts[j];
				string p = path.str();
				StrCast(p, ::tolower);

				if ( cache == p )
					mWallpapers.push_back(p), checkNextBg = false;
					
				
				path.str("");
			}
			checkNextBg = !checkNextBg;
		}
		
		
		memset(&entry, 0, sizeof(SceIoDirent));
	}
	
	sceIoDclose(id);
	#endif
	
	
}


#ifdef PSP

struct LoadTexThread {
	string path;
	JTexture** tex;
	JQuad** quad;
	int options;
	int* finished;
};


int Lib_LoadTexThread(SceSize args, void *argp)
{
	struct LoadTexThread* lTT = (struct LoadTexThread*) argp;
	
	JTexture* tex = JRenderer::GetInstance()->LoadTexture(lTT->path.c_str(), lTT->options);
	if ( tex == NULL )	FATAL_ERROR("Lib_LoadTexThread failed to load tex\n");
	
	JQuad* quad = new JQuad(tex, 0, 0, tex->mWidth, tex->mHeight);
	
    // TODO : Secure tex / quad / finished multithread access ..
	*(lTT->tex) = tex;
	*(lTT->quad) = quad;
	*(lTT->finished) = 1;
	
	
	sceKernelExitThread(0);
	
	return 0;
}


void Display::LoadTQThread ( string path, int options, JTexture*& tex, JQuad*& quad, int& finished )
{
	struct LoadTexThread lTT = { path, &tex, &quad, options, &finished };
	
	if (mLTTid < 0)
		mLTTid = sceKernelCreateThread( "LoadTexThread", &Lib_LoadTexThread, 50, 0x1000, 0, NULL );
	
	if (mLTTid >= 0)
	{
		sceKernelStartThread(mLTTid, sizeof(lTT), &lTT);
	}
	
}
#endif



void Display::updateBg()
{

	if ( clock()-mLastSwap > BG_SWAP_TIME*CLOCKS_PER_SEC && mSwapBgState==-1 )
	{
		mSwapBg = true;
		mLastSwap = clock();
	}

	
	if ( mOldBg != NULL && mSwapBgState==-1 )
	{
		mLastSwap = clock();
		const float aStep = BG_SWAP_ASTEP;

		if (mBgAlpha < 1)
		{
			if ( mBgAlpha+aStep <= 1)
				mBgAlpha+= aStep;
			else
				mBgAlpha = 1;
		}
		else
		{
			SAFE_DELETE(mOldBg);
			SAFE_DELETE(mOldBgTex);
			mBgAlpha = 1;
		}
	}

	if ( mSwapBg )
	{
		mSwapBgState = 0;
		mOldBgTex = mBgTex;
		mOldBg = mBg;
		mBgTex = NULL;
		mBg = NULL;
		#ifdef PSP
		this->SwapBg(mSwapBgState, 1);
		#else
		this->SwapBg(mSwapBgState, 0);
		#endif
		mSwapBg = false;
	}
	
	
	// Once our next wallpaper is loaded
	if ( mSwapBgState==1 )
	{
		mBgAlpha = 0;
		mSwapBgState = -1;
	}
}


void Display::renderBg()
{
	JRenderer* renderer = JRenderer::GetInstance();
	renderer->EnableTextureFilter(false);

	if (this->mOldBg != NULL )
	{
		Display::SetQuadAlpha(mOldBg, 1);
		renderer->RenderQuad(mOldBg, 0, 0);
	}
	
	// Display current bg only if loading finished
	if (this->mBg != NULL && mSwapBgState==-1)
	{
		Display::SetQuadAlpha(mBg, mBgAlpha);
		renderer->RenderQuad(mBg, 0, 0);
	}
	
}


int Display::getSwapBgState()
{
	return mSwapBgState;
}

void Display::renderInfoQuad()
{
	JRenderer* renderer = JRenderer::GetInstance();
	renderer->EnableTextureFilter(false);


	mBatteryPos.x = SCREEN_WIDTH- mBatteryQ->getWidth()-17;//14;
	mBatteryPos.y = SCREEN_HEIGHT- mBatteryQ->getHeight()-11;
	Coord battPwr = { 6, 2, 17, 10}, battPwrDisp;
	float pwrLeft;
	
	#ifdef PSP
	mBatteryP = scePowerGetBatteryLifePercent();
	if ( mBatteryP >=0)	pwrLeft = ((float)mBatteryP)/100;
	else	mBatteryP = 0;
	#else
		mBatteryP = 100;
	#endif
	pwrLeft = ((float)mBatteryP)/100;

	battPwrDisp.x= (battPwr.x + battPwr.w) - battPwr.w*pwrLeft;
	battPwrDisp.w= (battPwr.x + battPwr.w) - battPwrDisp.x;
	
	mBatteryQ->SetColor( ARGB(255,255,255,255) );
	renderer->RenderQuad(mBatteryQ, mBatteryPos.x, mBatteryPos.y );
	renderer->FillRect( mBatteryPos.x + battPwrDisp.x, mBatteryPos.y + battPwr.y, battPwrDisp.w, battPwr.h, ARGB(255,246,246,246) );



}

void Display::renderInfoFont()
{
	Coord time = { 24, mBatteryPos.y + 12 };
	string info = Lib::GetInfo().c_str();
	stringstream battTxt;
	battTxt << mBatteryP << "% ";
	
#ifdef USE_TTF
	time.y-= 20;
	mFont->SetColor(ARGB(255,255,255,255));
	mFont->RenderString(info.c_str(), time.x, time.y);
	mFont->RenderString(battTxt.str().c_str(), mBatteryPos.x - mFont->GetWidth(battTxt.str().c_str()), time.y);
#else
	intraFontSetStyle(mFont,0.8f,ARGB(255,255,255,255),ARGB(128,0,0,0),0.f,0);
	intraFontPrint(mFont, time.x, time.y, info.c_str());
	intraFontPrint(mFont, mBatteryPos.x - intraFontMeasureText(mFont, battTxt.str().c_str()), time.y, battTxt.str().c_str());
#endif
}


void Display::updateNotify()
{
	JGE* engine = JGE::GetInstance();

	switch (mNotify)
	{
		case NOTIFICATION_ACTIVE:
			if (engine->GetButtonClick(PSP_CTRL_CIRCLE))	mChoice = false, mNotify = NOTIFICATION_DIE;
			else if (engine->GetButtonClick(PSP_CTRL_CROSS))
			{
				mChoice = true, mNotify = NOTIFICATION_WORKING;
				
				if (this->getEventType() == 0)	mNotifyMess = "Installing ...";
				else if (this->getEventType() == 1)	mNotifyMess = "Deleting ...";
			}
			break;
			
		case NOTIFICATION_WORKING:
			if (mChoice)	mNotify = NOTIFICATION_DIE;
			break;
			
		case NOTIFICATION_INIT:
			if (mHideOpacity < Display::MAX_HIDE_OPACITY)	mHideOpacity+= 20;
			else	mNotify = NOTIFICATION_ACTIVE;

			if (mHideOpacity > Display::MAX_HIDE_OPACITY)	mHideOpacity = Display::MAX_HIDE_OPACITY;
			break;
			
		case NOTIFICATION_DIE:
			if (mHideOpacity > 0)	mHideOpacity-=20;
			else	mNotify = NOTIFICATION_INACTIVE;

			if (mHideOpacity < 0)	mHideOpacity = 0;
			break;
			
		default:
			mNotify = NOTIFICATION_INACTIVE;
	}
}

void Display::renderNotify()
{
	
	if ( mNotify != NOTIFICATION_INACTIVE)
	{
		JRenderer::GetInstance()->FillRect( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (PIXEL_TYPE)ARGB(mHideOpacity,0,0,0) );
		

		if ( mHideOpacity > 128 )
		{
			int inter = 50, yControls, yInter = 43, yTitle = 0;
			string control[] = { "X Yes", "O No" };

		#ifdef USE_TTF
			yInter-= 30;
			yTitle+= (SCREEN_HEIGHT - FONT_SIZE )/2;
			yControls = yTitle + yInter;
			mFont->SetColor(ARGB(255,255,255,255));
			mFont->RenderString(mNotifyMess.c_str(), (SCREEN_WIDTH - mFont->GetWidth(mNotifyMess.c_str()) )/2 , yTitle);
			
			if ( !mChoice )
			{
				mFont->RenderString(control[0].c_str(), (SCREEN_WIDTH - inter )/2  - mFont->GetWidth(control[0].c_str()), yControls);
				mFont->RenderString(control[1].c_str(), (SCREEN_WIDTH + inter )/2 , yControls);
			}
		#else
			yTitle+= (SCREEN_HEIGHT +10 )/2;
			yControls = yTitle + yInter;
			intraFontSetStyle(mFont,0.9f,ARGB(255,255,255,255),ARGB(64,0,0,0),0.f,0);
			intraFontPrint(mFont, (SCREEN_WIDTH - intraFontMeasureText(mFont, mNotifyMess.c_str()) )/2 , yTitle, mNotifyMess.c_str());
			
			if ( !mChoice )
			{
				intraFontSetStyle(mFont,0.87f,ARGB(255,255,255,255),ARGB(64,0,0,0),0.f,0);
				intraFontPrint(mFont, (SCREEN_WIDTH - inter )/2  - intraFontMeasureText(mFont, control[0].c_str()), yControls, control[0].c_str());
				intraFontPrint(mFont, (SCREEN_WIDTH + inter )/2, yControls, control[1].c_str());
			}
		#endif
		}
	}
}

void Display::Notify( string mess )
{
	if (mNotify == NOTIFICATION_INACTIVE)
	{
		mNotifyMess = mess;
		mNotify = NOTIFICATION_INIT;
		mHideOpacity = 0;
		mChoice = false;
		mChoiceDone = false;
	}
}

int Display::getNotifyState()
{
	return mNotify;
}

bool Display::getNotifyChoice()
{
	bool res = false;
	
	// If confirmation didn't occur yet, check it
	if (!mChoiceDone)
	{
		res = mChoice;
		if (res)	mChoiceDone = true;
	}
	
	return res;
}

void Display::setEventType( int eventType )
{
	mEventType = eventType;
}

int Display::getEventType()
{
	return mEventType;
}

