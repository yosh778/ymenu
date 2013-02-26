
#include "DSystm.h"

DSystm* DSystm::mDSystm = NULL;


DSystm::DSystm(
				#ifdef USE_TTF
				JTTFont* font
				#else
				intraFont* font
				#endif
				)
{
	mDefIconTex = NULL;
	mFolderIconTex = NULL;
	mCorruptIconTex = NULL;
	mFileIconTex = NULL;
	mRarIconTex = NULL;
	mZipIconTex = NULL;
	mDefIcon = NULL;
	mFolderIcon = NULL;
	mCorruptIcon = NULL;
	mFileIcon = NULL;
	mRarIcon = NULL;
	mZipIcon = NULL;
	setAppPath( "quit" );
	mFont = font;
	mClic = NULL;
	mDir = NULL;
	mOldDir = NULL;
	mOldMove = false;
	mToAddDir = 0;
    mCutCache.on = false;
    mExit = false;
}

DSystm::~DSystm()
{
}


DSystm* DSystm::GetInstance(
							#ifdef USE_TTF
							JTTFont* font
							#else
							intraFont* font
							#endif
							)
{
//#ifdef USE_TTF
	if ( DSystm::mDSystm == NULL && font != NULL )
		DSystm::mDSystm = new DSystm( font );
/*#else
	if ( DSystm::mDSystm == NULL )
		DSystm::mDSystm = new DSystm();
#endif*/
	
	return DSystm::mDSystm;
}


void DSystm::Create( string workPath )
{
	Display::LoadTextQuad ( "def_ico.png", TEX_TYPE_USE_VRAM, mDefIconTex, mDefIcon );
	Display::LoadTextQuad ( "folder.png", TEX_TYPE_USE_VRAM, mFolderIconTex, mFolderIcon );
	Display::LoadTextQuad ( "corrupt.png", TEX_TYPE_USE_VRAM, mCorruptIconTex, mCorruptIcon );
	Display::LoadTextQuad ( "def_ico.png", TEX_TYPE_USE_VRAM, mFileIconTex, mFileIcon );
	Display::LoadTextQuad ( "rar_ico.png", TEX_TYPE_USE_VRAM, mRarIconTex, mRarIcon );
	Display::LoadTextQuad ( "zip_ico.png", TEX_TYPE_USE_VRAM, mZipIconTex, mZipIcon );

	mClic = JSoundSystem::GetInstance()->LoadSample("s.wav");


	mDir = new YDir();
	setWorkPath( workPath );
	mDir->Create( "", true );
}

void DSystm::Destroy()
{
	if (mOldDir != mDir && mOldDir != NULL)
	{
		mOldDir->Destroy();
		SAFE_DELETE(mOldDir);
	}
	if (mDir != NULL)
	{
		mDir->Destroy();
		SAFE_DELETE(mDir);
	}
	SAFE_DELETE(mDefIconTex);
	SAFE_DELETE(mFolderIconTex);
	SAFE_DELETE(mCorruptIconTex);
	SAFE_DELETE(mFileIconTex);
	SAFE_DELETE(mRarIconTex);
	SAFE_DELETE(mZipIconTex);
	SAFE_DELETE(mDefIcon);
	SAFE_DELETE(mFolderIcon);
	SAFE_DELETE(mCorruptIcon);
	SAFE_DELETE(mFileIcon);
	SAFE_DELETE(mZipIcon);
	SAFE_DELETE(mRarIcon);
	SAFE_DELETE(mClic);
	
	SAFE_DELETE(DSystm::mDSystm);
}


int DSystm::readDir()
{
    YLOG("DSystm::readDir\n");
	
#ifdef PSP

    //YLOG("getDir\n");
	vector<SceIoDirent>* dir = YDir::getDir ( DSystm::mWorkPath );
	
    //YLOG("for\n");
	for (int i=0; i<dir->size(); ++i )
	{
		string tmpName = (*dir)[i].d_name;
        //YLOG("StrCast\n");
		StrCast(tmpName, ::tolower);
        //YLOG("StrCast done\n");
		
		if (tmpName.size() > 3)  tmpName = tmpName.substr(tmpName.size()-3, 3);
        else    tmpName = "";
        //YLOG("substr done\n");
		
        #ifndef PSP_VERSION

        //YLOG("test\n");
		if ( tmpName == YEntry::ARCHIVE_EXTS[0] || tmpName == YEntry::ARCHIVE_EXTS[1] \
			|| tmpName == "iso" || tmpName == "cso" \
			|| ( (*dir)[i].d_stat.st_attr & FIO_SO_IFDIR && (*dir)[i].d_stat.st_mode & FIO_S_IFDIR) )
        
        #endif
		{
            //YLOG("YEntry auto\n");
			YEntry yEntry((*dir)[i]);
            //YLOG("mDir->mFolders.push_back\n");
			mDir->mFolders.push_back(yEntry);
		}
        //YLOG("i++\n");
	}
	
    //YLOG("SAFE_DELETE(dir)\n");
	SAFE_DELETE(dir);
	
#else
	stringstream sstm;
	for (int i=0;i<30;++i)
	{
		sstm << "Test" << i;
		YEntry yEntry(sstm.str());
		mDir->mFolders.push_back(yEntry);
		sstm.str("");
	}
#endif


    //YLOG("DSystm::readDir done\n");
	return 0;
}

void DSystm::render()
{
	if (mOldDir != NULL)	mOldDir->renderIcons();
	mDir->renderIcons();

	JRenderer* renderer = JRenderer::GetInstance();
	renderer->EnableTextureFilter(false);


	// Display message
	string mess;
	Coord name, path;

	if (mDir->mCurFolder < mDir->mFolders.size())
		mess = mDir->mFolders[mDir->mCurFolder].getDispName();
	else	mess = "No content";
	
	
	name.x = (SCREEN_WIDTH
						#ifdef USE_TTF
							-mFont->GetWidth(mess.c_str())
						#endif
							) /2;
	name.y = YEntry::yFolders - 60;
	path.x = SCREEN_WIDTH/11;
	path.y = SCREEN_HEIGHT/8 + 3;

	
#ifdef USE_TTF
	mFont->SetColor(ARGB(255,255,255,255));
	mFont->RenderString(mess.c_str(), name.x, name.y-20);
	mFont->RenderString(mWorkPath.c_str(), path.x, path.y);
#else
	intraFontSetStyle(mFont,1.f,ARGB(255,255,255,255),ARGB(240,0,0,0),0.f,INTRAFONT_ALIGN_CENTER);
	intraFontPrint(mFont, name.x, name.y, mess.c_str());
	intraFontSetStyle(mFont,0.8f,ARGB(255,255,255,255),ARGB(128,0,0,0),0.f,0);
	intraFontPrint(mFont, path.x, path.y, mWorkPath.c_str());
#endif
	
}


void DSystm::setWorkPath( string workPath )
{
    //YLOG("setWorkPath in\n");
	if ( workPath[workPath.size()-1] != '/' )	workPath+= '/';
	
    //YLOG("if ( DSystm::mWorkPath != workPath )\n");
	if ( DSystm::mWorkPath != workPath )
	{
        //YLOG("=\n");
		DSystm::mWorkPath = workPath;
        //YLOG("readDir\n");
		readDir();
        //YLOG("readDir done\n");
	}
    //YLOG("setWorkPath done\n");
}

void DSystm::setAppPath( string appPath )
{
	mAppPath = appPath;
}

string DSystm::getWorkPath()
{
	return DSystm::mWorkPath;
}

string DSystm::getAppPath()
{
	return mAppPath;
}

void DSystm::update()
{
	JGE* engine = JGE::GetInstance();

	
	// now updating everything, just before rendering, to somehow solve some bugs
	//if (mOldDir != NULL)	mOldDir->update();
	//mDir->update();
	
	int move = 0;
	
	// Exit button
	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))
	{
        #ifdef PSP_VERSION

		YLOG("Exit button pressed\n");
        mExit = true;
		engine->End();
		return;

        #endif
	}

	Display* displayer = Display::GetInstance();
	if ( displayer->getNotifyState() == NOTIFICATION_INACTIVE )
	{
		bool action_done = false;
		int step = 0;
	
		// Browse dir
		if (engine->GetButtonState(PSP_CTRL_RIGHT) || engine->GetAnalogX() > 255-80 )
		{
			step = 1;
		}
		else if (engine->GetButtonState(PSP_CTRL_LEFT) || engine->GetAnalogX() < 80 )
		{
			step = -1;
		}
		else	mOldMove = false;

		// If no more action asked, treat saved actions
		if (!step && mToAddDir)
		{
			if (mToAddDir >0)	step = 1;
			else	step = -1;

			mToAddDir-=step;
		}

		// If there's an action to do
		if (step)
		{
			move = mDir->addCurFolder(step);
			mOldMove = true;
		}
		
		// If action succeeded
		if (move == 1)
			JSoundSystem::GetInstance()->PlaySample(mClic);

		// If Dir still sliding a folder and couldn't increment, keep in memory to increment later
		else if (move == -1)
		{
			int memStep = 0;

			// Only save action if it's a click
			if (engine->GetButtonClick(PSP_CTRL_RIGHT) || engine->GetAnalogX() > 255-80)	memStep = 1;
			else if (engine->GetButtonClick(PSP_CTRL_LEFT) || engine->GetAnalogX() < 80)	memStep = -1;
			mToAddDir+= memStep;
		}
		


		if (mDir->mCurFolder < mDir->mFolders.size() && \
			(engine->GetButtonClick(PSP_CTRL_CROSS) || engine->GetButtonClick(PSP_CTRL_CIRCLE) )
			)
		{
			// If we want to start homebrew
			if ( mDir->mFolders[mDir->mCurFolder].isApp() )
			{
                #ifndef PSP_VERSION

				setAppPath( mDir->mFolders[mDir->mCurFolder].getAppPath() );
				engine->End();
				YLOG("Start homebrew requested\n");
				return;

                #endif
			}
			// If we want to install save
			else if ( mDir->mFolders[mDir->mCurFolder].inSavePath() )
			{
				// Check for archives only if required to prevent lags
				//mDir->mFolders[mDir->mCurFolder].findArchive();
				
				if ( mDir->mFolders[mDir->mCurFolder].getArchiveType() != NO_FILE )
					displayer->setEventType(0), Display::GetInstance()->Notify( "Install "+mDir->mFolders[mDir->mCurFolder].getDispName()+" ?" ), action_done = true;
			}
		}
		
		if (mDir->mCurFolder < mDir->mFolders.size() && engine->GetButtonClick(PSP_CTRL_SQUARE))
		{
			displayer->setEventType(1);
			Display::GetInstance()->Notify( "Delete "+mDir->mFolders[mDir->mCurFolder].getDispName()+" ?" ), action_done = true;
		}

		// If we want to change directory
		if ( !mDir->mCurToCross.y && mOldDir == NULL )
		{
			if (engine->GetButtonClick(PSP_CTRL_UP)/* || engine->GetAnalogY() < 80*/)
			{
				ChYDir( true );
			}
			else if ( (engine->GetButtonClick(PSP_CTRL_CROSS) \
					|| engine->GetButtonClick(PSP_CTRL_CIRCLE) \
					|| engine->GetButtonClick(PSP_CTRL_DOWN) \
					/*|| engine->GetAnalogY() > 255-80*/) \
					&& (mDir->mCurFolder < mDir->mFolders.size()) \
					&& !mDir->mFolders[mDir->mCurFolder].isApp() \
					&& mDir->mFolders[mDir->mCurFolder].IsFolder() \
					&& !action_done )
			{
				ChYDir( false );
			}
		}

        #ifdef PSP_VERSION
        if ( engine->GetButtonClick(PSP_CTRL_START) && !mDir->mFolders.empty() )
        {
            // If not cutting yet, prepare new cut
            if ( !mCutCache.on )
            {
                mCutCache.path = mDir->mFolders[mDir->mCurFolder].getPathName();
                mCutCache.isFolder = mDir->mFolders[mDir->mCurFolder].IsFolder();
                JSoundSystem::GetInstance()->PlaySample(mClic);
            }

            // Toggle cut state
            mCutCache.on = !mCutCache.on;
        }

        if ( mCutCache.on && engine->GetButtonClick(PSP_CTRL_SELECT) )
        {
            string srcPath = mCutCache.path;
            string fullname = srcPath.substr(srcPath.rfind('/') + 1);
            string destPath = this->getWorkPath() + fullname;

            if ( srcPath != destPath )
            {
                if ( Lib::IoMove(srcPath, destPath) >= 0)
                {
                    mDir->addEntry(fullname, mCutCache.isFolder);
                    JSoundSystem::GetInstance()->PlaySample(mClic);
                }
            }

            mCutCache.on = false;
        }
        #endif
	}
	// If a notification is taking place and user says yes
	else if ( displayer->getNotifyChoice() )
	{
		if (displayer->getEventType() == 0)
		{
			//install archive
			int type = mDir->mFolders[mDir->mCurFolder].getArchiveType();
			string filePath = mDir->mFolders[mDir->mCurFolder].getPathName(),
					destPath = HOMEBREW_PATH"/";

			switch (type)
			{
				case RAR_FILE:
					#ifdef PSP
					YLOG("Unraring %s to %s\n", filePath.c_str(), destPath.c_str());
					DoExtractRAR(filePath.c_str(), destPath.c_str());
					#endif
					break;
				case ZIP_FILE:
					#ifdef PSP
					YLOG("Unzipping %s to %s\n", filePath.c_str(), destPath.c_str());
					unzipToDir(filePath.c_str(), destPath.c_str());
					#endif
					break;
				default:
					YLOG("Not an archive !\n");
			}
		}
		else if (displayer->getEventType() == 1)
		{
			if ( mDir->mFolders[mDir->mCurFolder].IsFolder() )	Lib::removeDir( mDir->mFolders[mDir->mCurFolder].getPathName() );
			else	sceIoRemove( mDir->mFolders[mDir->mCurFolder].getPathName().c_str() );
			
            mDir->removeEntry(mDir->mCurFolder);
		}
		displayer->setEventType(-1);
	}

	// Once our old Dir's Yslide is over, free it
	if (mOldDir != NULL && !mOldDir->mCurToCross.y )
	{
		mOldDir->Destroy();
		SAFE_DELETE(mOldDir);
	}

}

JQuad* DSystm::getDefIcon()
{
	return mDefIcon;
}

JQuad* DSystm::getFolderIcon()
{
	return mFolderIcon;
}

JQuad* DSystm::getCorruptIcon()
{
	return mCorruptIcon;
}

JQuad* DSystm::getFileIcon()
{
	return mFileIcon;
}

JQuad* DSystm::getRarIcon()
{
	return mRarIcon;
}

JQuad* DSystm::getZipIcon()
{
	return mZipIcon;
}

bool DSystm::getOldMove()
{
	return mOldMove;
}

void DSystm::ChYDir( bool getParent )
{
    YLOG("DSystm::ChYDir %X\n", getParent );
	string workpath;

	if (getParent)	workpath = YDir::GetParent(mWorkPath);
	else	workpath = mWorkPath+mDir->mFolders[mDir->mCurFolder].mName;

	if ( workpath != mWorkPath )
	{
        YLOG("mDir->slideOut\n" );
		mDir->slideOut( !getParent );


		mOldDir = mDir;
        YLOG("mDir = new YDir\n" );
		mDir = new YDir();

		string deadChild;
		if ( getParent )
		{
			int pos = mWorkPath.rfind('/', mWorkPath.size()-2 )+1;
			deadChild = mWorkPath.substr( pos, mWorkPath.size()-pos-1 );
			//YLOG("%X\n", mWorkPath.size()-pos-1 );
		}
		else	deadChild = "";
		//YLOG("mWorkPath: %s, deadChild: %s, %X\n", mWorkPath.c_str(), deadChild.c_str(), mWorkPath.rfind('/', mWorkPath.size()-2 ));

        YLOG("setWorkPath\n" );
		setWorkPath( workpath );
		
        YLOG("mDir->Create\n" );
		mDir->Create( deadChild );
        YLOG("mDir->Create done\n" );
	}
}

YLaunch* DSystm::getCurApp()
{
    return mDir->mFolders[mDir->getCurFolder()].mApp;
}

bool DSystm::isExit()
{
    return mExit;
}


