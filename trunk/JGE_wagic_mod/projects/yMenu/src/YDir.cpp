
#include "YDir.h"

const int YDir::INTER_Y = 0.2*SCREEN_HEIGHT;
const int YDir::MIN_FOLDER_Y = YEntry::yFolders - YDir::INTER_Y;//SCREEN_HEIGHT/8;
const int YDir::MAX_FOLDER_Y = YEntry::yFolders + YDir::INTER_Y;//(7*SCREEN_HEIGHT)/8;
const int YDir::SLIDE_STEP_X = 8;
const int YDir::RUSH_SLIDE_STEP_X = 39;
const int YDir::RUSH_INTER_MS
								#ifdef PSP
								= 0;
								#else
								= 0;
								#endif
const int YDir::RUSH_TRIGGER_MS
								#ifdef PSP
								= 300000;
								#else
								 = 300;
								#endif
const int YDir::SLIDE_STEP_Y = 6;
map<string,int> YDir::mDirIdxs;


YDir::YDir()
{
	mSlideOut = false;
	mCurFolder = 0;
	mOldFolder = mCurFolder;
	mToCross.x = 0;
	mCurToCross.x = mToCross.x;
	mRush = false;
	mOldRush = false;
	mLastRush = 0;
	mFirstRush = 0;
	mCurInterToCross = 0;
	mInterToCross = 0;
	mInterOffset = 0;
}

YDir::~YDir()
{
	// Save dir's idx
	mDirIdxs[mCurDir] = mCurFolder;
}

void YDir::Create( string deadChild, bool init )
{
	int slideInY;
	if (deadChild != "" || init)	slideInY = YDir::MIN_FOLDER_Y;
	else	slideInY = YDir::MAX_FOLDER_Y;

    //YLOG("initSlideY\n");
	initSlideY( slideInY, YEntry::yFolders );
	
	
	//YLOG("YDir::Create\n");
	for ( int i=0; i< mFolders.size(); ++i )
		mFolders[i].Create();

	mCurDir = DSystm::GetInstance()->getWorkPath();
	
    //YLOG("initCurDirIdx\n");
	initCurDirIdx( deadChild );
	
    zoomCurFolder();
	//YLOG("YDir::Create done\n");
}

void YDir::Destroy()
{
	//YLOG("YDir::Destroy\n");
	for ( int i=0; i< mFolders.size(); ++i )
		mFolders[i].Destroy();

	mFolders.clear();
	//YLOG("YDir::Destroy done\n");
}

int YDir::addCurFolder( int step )
{
	//YLOG("YDir::addCurFolder\n");
	int move = 0;
	mDoRush = false;
	
	mRush = DSystm::GetInstance()->getOldMove();

	if (mRush)
	{
		if (mOldRush)
		{
			if ((clock() - mFirstRush) > RUSH_TRIGGER_MS )	mDoRush = true;
		}
		else if (!mFirstRush)	mFirstRush = clock();
	}
	else	mFirstRush = 0;
	mOldRush = mRush;

	if ( mCurFolder < mFolders.size() && (!mRush || (mDoRush && ((clock() - mLastRush) > RUSH_INTER_MS ))) )
	{
		if (mOldFolder == mCurFolder)
		{
			mOldFolder = mCurFolder;
			if (step >0)
				if ( mCurFolder + step < mFolders.size() )	mCurFolder+= step, move = 1;
				else if (mFolders.size() > 0)	mCurFolder = mFolders.size()-1;
                else    mCurFolder = 0;
			
			else if (step < 0)
				if ( mCurFolder + step >= 0 )	mCurFolder+= step, move = 1;
				else	mCurFolder = 0;

				if ( move )
				{
					int interval;
					if ( (mCurFolder == 1 && step >0) || (mCurFolder == 0 && step <0) )	interval = YEntry::interX;
					else	interval = YEntry::MinInterX;

					mToCross.x = (-1 * step) * (DEFAULT_ICON_W*YEntry::DEF_ZOOM + interval);
					mCurToCross.x = mToCross.x;
					mFocus.x = (SCREEN_WIDTH-DEFAULT_ICON_W) /2;
					mInterToCross = (-1 * step) * YEntry::interX;
					mCurInterToCross = mInterToCross;
				}

			if (mRush)
			{
				mLastRush = clock();
			}
		}
		// If waiting for current slide to finish
		else	move = -1;
	}
	
	//YLOG("YDir::addCurFolder done\n");
	return move;
}

int YDir::getCurFolder()
{
	return mCurFolder;
}

void YDir::renderIcons()
{
	JRenderer* renderer = JRenderer::GetInstance();
	bool smoothing;
	
	this->update();
	this->updateLogicPosX();

	for ( int i=0; i<mFolders.size(); ++i )
	{
		this->updateAlpha();
		mFolders[i].updateAlpha( mCurAlpha );

		// Effective display coordinates calculation
		float xDispPos, yDispPos, dispAngle = 0, xScale, yScale;
		xDispPos = mOffset.x + mFolders[i].mPos.x;
		yDispPos = mOffset.y + mFolders[i].mPos.y-(DEFAULT_ICON_H*mFolders[i].getZoom())/2;//(1-mFolders[i].getZoom());
		xScale = mFolders[i].getZoom()*((float)DEFAULT_ICON_W/mFolders[i].mIcon->getWidth());
		yScale = mFolders[i].getZoom()*((float)DEFAULT_ICON_H/mFolders[i].mIcon->getHeight());

		if ( abs(1 - xScale) < 0.0001  &&  abs(1 - yScale) < 0.0001 )
			smoothing = false;
		else
			smoothing = true;
		
		JRenderer::GetInstance()->EnableTextureFilter(smoothing);

		// Display
		renderer->RenderQuad(mFolders[i].mIcon, xDispPos, yDispPos, dispAngle, xScale, yScale);
	}
}

void YDir::update()
{


	// Nb of pixels passed at each Xslide frame
	int step;

	if (mDoRush)	step = YDir::RUSH_SLIDE_STEP_X;
	else	step = YDir::SLIDE_STEP_X;

	// If a slide is happening
	if ( mCurToCross.x )
	{
		// X sliding calc

		int curStep;/*, curFolderRealX = mOffset.x + mFolders[mCurFolder].mPos.x;
		mCurToCross.x = mFocus.x-curFolderRealX;*/

		if (abs(mCurToCross.x) < step)
			curStep = abs(mCurToCross.x);
		else
			curStep = step;


		if ( mCurToCross.x < 0 )	curStep*= -1;

		mOffset.x+= curStep;
		mCurToCross.x-= curStep;
		//mCurInterToCross-= ((float)curStep)/mToCross.x * mInterToCross;
		mCurInterToCross = ((float)mCurToCross.x)/mToCross.x * mInterToCross;
		mInterOffset+= ((float)curStep)/mToCross.x * mInterToCross;
	}
	// If no slide is happening, finish sliding
	else
	{
		// Force zoom rates to default
		if (mOldFolder < mFolders.size())	mFolders[mOldFolder].setZoom(0.5);
		if (mCurFolder < mFolders.size())mFolders[mCurFolder].setZoom(1);

		mOldFolder = mCurFolder;

		/*
		// 1px weïrd bug fix when sliding on the left
		if (mOffset.x%2 )	mOffset.x++;
		*/
		mInterToCross = 0;
		mCurInterToCross = mInterToCross;
		mInterOffset = 0;
	}

	// If no slide is happening, finish sliding
	if ( mOldFolder != mCurFolder )
	{
		// Zoom sliding calc

		// rate = proportion of step * the zoom difference to cross
		// *0.93 : to decelerate zoom a little, else tiny display bug somehow :S
		const float rate = ((float)step*0.93/(float)abs(mToCross.x))*(1-YEntry::DEF_ZOOM);
		float curRate, fZoom;

		int i = mOldFolder, end = mCurFolder;
		if ( mOldFolder > mCurFolder )
		{
			i = mCurFolder;
			end = mOldFolder;
		}

		for ( i; i < end + 1; ++i )
		{
			curRate = rate;

			if ( i == mOldFolder )	fZoom = YEntry::DEF_ZOOM;
			else	fZoom = 1;

			// If final zoom not reached yet
			if ( (i == mOldFolder && mFolders[i].getZoom() > fZoom) || \
					(i == mCurFolder && mFolders[i].getZoom() < fZoom)
				)
			{

				// If the rate step is bigger than what's left to our final zoom, add the difference
				if ( abs(fZoom-mFolders[i].getZoom()) < rate )
					curRate = fZoom - mFolders[i].getZoom();

				// If it's the previous folder, reduce it
				if ( i == mOldFolder )	curRate*= -1;
				

				mFolders[i].setZoom(mFolders[i].getZoom()+curRate);
			}
		}
	}


	
	// Nb of pixels passed at each Yslide frame
	const int Ystep = YDir::SLIDE_STEP_Y;

	// If an Yslide is happening
	if ( mCurToCross.y )
	{
		// Y sliding calc

		int curStep, curFolderRealY = mOffset.y + YEntry::yFolders;
		mCurToCross.y = mFocus.y-curFolderRealY;

		if (abs(mCurToCross.y) < Ystep)
			curStep = abs(mCurToCross.y);
		else
			curStep = Ystep;


		if ( mCurToCross.y < 0 )	curStep*= -1;

		mOffset.y+= curStep;
		mCurToCross.y-= curStep;
	}
}

void YDir::initSlideY( int slideInY, int focusY )
{
	mFocus.y = focusY;
	mOffset.y = slideInY - YEntry::yFolders;
	mToCross.y = focusY - slideInY;
	mCurToCross.y = mToCross.y;
}

void YDir::slideOut( bool isParent )
{
	int focusY;
	if (isParent)	focusY = YDir::MIN_FOLDER_Y;
	else	focusY = YDir::MAX_FOLDER_Y;

	initSlideY( YEntry::yFolders, focusY );

	mSlideOut = true;
}

void YDir::updateAlpha()
{
	if (mSlideOut)
		mCurAlpha = ((float)abs(mCurToCross.y))/abs(mToCross.y);
	else
		mCurAlpha = ((float)abs(mToCross.y)-abs(mCurToCross.y))/abs(mToCross.y);
}

void YDir::updateLogicPosX()
{
	int leftSize = mFocus.x;
	for ( int i=0; i<mFolders.size(); ++i )
	{
		// Folders new logical position calculation
		mFolders[i].mPos.x = leftSize;
		mFolders[i].mPos.y = YEntry::yFolders;


		int interval = YEntry::MinInterX;

		// If interSlide not finished
		if ( mInterOffset  != 0 )
		{
			int b[2], base[2] = { YEntry::interX, 0 };

			b[0] = mCurFolder - 2;
			// If left sliding
			if ( mInterOffset >0)
			{
				++b[0];
				int mem = base[0];
				base[0] = base[1];
				base[1] = mem;
			}
			b[1] = b[0] + 2;

			if ( i == b[0] )	interval = base[0] + mInterOffset;
			else if ( i == b[1] )	interval = base[1] + -1*mInterOffset;
			else if ( i == b[1]-1 )	interval = YEntry::interX;
			else	interval = YEntry::MinInterX;
		}
		else if ( i == mCurFolder-1 || i == mCurFolder )	interval = YEntry::interX;

		// Add current folder's width to get the next folder's x position
		leftSize+= DEFAULT_ICON_W*mFolders[i].getZoom() + interval;
	}
}




string YDir::GetParent ( string path )
{
	string parDir;
	int nbSlash = count(path.begin(), path.end(), '/');

	if ( nbSlash >1 || ( nbSlash == 1 && path[path.size()-1] != '/' ) )
		parDir = path.substr(0,1+path.rfind('/',path.size()-2));
	else
		parDir = path;

	return parDir;
}

#ifdef PSP
vector<SceIoDirent>* YDir::getDir( string dirName )
{
	vector<SceIoDirent>* dirC = new vector<SceIoDirent>();
	
	SceUID id;
	SceIoDirent entry;

	id = sceIoDopen(dirName.c_str());

	if (id < 0)
	{
		//YLOG("ERROR: sceIoDopen failed to open dir %s\n",dirName.c_str());
		return NULL;
	}

	memset(&entry, 0, sizeof(SceIoDirent));

	while (sceIoDread(id, &entry) > 0)
	{
		// skip . / .. as inexistent on PSVita
		if (strcmp(".", entry.d_name) && strcmp("..", entry.d_name))
		{
			dirC->push_back(entry);
		}
		
		memset(&entry, 0, sizeof(SceIoDirent));
	}

	sceIoDclose(id);
	
	
	return dirC;
}

vector<string>* YDir::getDirFolders( string dirName )
{
	vector<SceIoDirent>* dir = YDir::getDir(dirName);
	vector<string>* dirF = new vector<string>();
	
	
	SceIoDirent* entry;
	
	for (int i=0; i<dir->size(); ++i )
	{
		entry = &(*dir)[i];
		// Keep folders only
		if (entry->d_stat.st_attr & FIO_SO_IFDIR && entry->d_stat.st_mode & FIO_S_IFDIR)
		{
			dirF->push_back(entry->d_name);
		}
	}
	
	SAFE_DELETE(dir);
	
	
	return dirF;
}

vector<string>* YDir::getDirNames( string dirpath, enum name_formats_ format )
{
	vector<SceIoDirent>* dir = YDir::getDir(dirpath);
	vector<string>* dirF = new vector<string>();
	
	for (int i=0; i<dir->size(); ++i )
	{
		dirF->push_back((*dir)[i].d_name);
		string* s = &(*dirF)[dirF->size()-1];
		
		
		switch (format)
		{
			case LOWERCASE:
				StrCast((*s), ::tolower);
				break;
			case UPPERCASE:
				StrCast((*s), ::toupper);
				break;
			case ANY_FORMAT:
				break;
				
			default:
				break;
		}
	}
	
	SAFE_DELETE(dir);
	
	
	return dirF;
}

#endif


bool YDir::restoreCurDirIdx()
{
	bool found = false;
	map<string,int>::iterator it = mDirIdxs.find(mCurDir);

	if ( it != mDirIdxs.end() )	mCurFolder = it->second, found = true;

    if (mCurFolder >= mFolders.size() && mCurFolder > 0)   mCurFolder = mFolders.size() - 1;
	
	
	return found;
}

// Calcs offset to avoid storing it in our mDirIdxs map
void YDir::initOffsetX()
{
	mOffset.x = -1*mCurFolder*(DEFAULT_ICON_W/2 + YEntry::MinInterX);
	if ( mCurFolder > 0 )	mOffset.x-= YEntry::interX-YEntry::MinInterX;
}

bool YDir::initCurDirIdx( string deadChild )
{
	bool success = true;
	
	
	if ( !restoreCurDirIdx() && deadChild != "" )
		if ( !findChildIdx(deadChild) )	success = false;
	
	initOffsetX();
	
	
	return success;
}

bool YDir::findChildIdx( string deadChild )
{
    bool found = false;
    
    for (int i=0; !found && i<mFolders.size(); ++i )
        if ( mFolders[i].getRealName() == deadChild )   mCurFolder = i, found = true;
    
    return found;
}

void YDir::removeEntry( int index )
{
    mFolders[index].Destroy();
    mFolders.erase(mFolders.begin()+index);
    if (index == mFolders.size() && index>0)
    {
        mCurFolder--;
        this->initOffsetX();
    }
}

void YDir::addEntry( string name, bool isFolder )
{
    YEntry yEntry(name, isFolder);
    mFolders.push_back(yEntry);

    mFolders[mFolders.size()-1].Create();

    YLOG("mCurFolder: %d\nmFolders.size(): %d", mCurFolder, mFolders.size());
    if (mFolders.size() == 1)
        zoomCurFolder();
}

void YDir::zoomCurFolder()
{
    if (mCurFolder < mFolders.size())
    {
        mFolders[mCurFolder].setZoom(1);
        mFocus.x = (SCREEN_WIDTH-DEFAULT_ICON_W) /2;
    }
}




