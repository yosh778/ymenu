//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include "GameApp.h"
#include "lib.h"


//-------------------------------------------------------------------------------------
// Constructor. Variables can be initialized here.
//
//-------------------------------------------------------------------------------------
GameApp::GameApp()
{
	this->settings = NULL;
#ifdef USE_TTF
	mTTFont1 = NULL;
#endif
}


//-------------------------------------------------------------------------------------
// Destructor.
//
//-------------------------------------------------------------------------------------
GameApp::~GameApp()
{

}


//-------------------------------------------------------------------------------------
// This is the init callback function. You should load and create your in-game 
// resources here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Create()
{
	JRenderer* renderer = JRenderer::GetInstance();
	
	renderer->EnableVSync(true);
	
	YLOG("GameApp::Create\n");
	
	YLOG("GameApp::Create intraFont\n");
#ifdef USE_TTF
	mTTFont1 = new JTTFont();
	mTTFont1->Load("BorisBlackBloxx.ttf", FONT_SIZE);
#else
	intraFontInit();
	mFont = intraFontLoad("flash0:/font/jpn0.pgf",INTRAFONT_STRING_UTF8);
	
	if ( mFont == NULL)	FATAL_ERROR("Failed to load font\n");
#endif
	YLOG("GameApp::Create intraFont done\n");
	
	
	JGE* engine = JGE::GetInstance();
	vector<string> argv = engine->GetARGV();
	string workPath;
	
	int settingsAddr = 0;

	if (argv.size() > 1) {
		string hex = argv[1];
#ifdef PSP
		hex[8] = 0;	//bug in HBL ?
#endif
		settingsAddr = Lib::xstrtoi(hex.c_str());
	}
	if (settingsAddr) {
		this->settings = (tMenuApi *) settingsAddr;
		
#ifdef PSP
		workPath = this->settings->filename;
#else
		workPath = "ms0:/";
#endif
		// Set to quit by default
		strcpy(this->settings->filename, "quit");
		
	} else {
		workPath = "ms0:/PSP/GAME";
	}

	
	Display::GetInstance(
						#ifdef USE_TTF
							mTTFont1
						#else
							mFont
						#endif
							);

	DSystm* dirSys = DSystm::GetInstance(
									#ifdef USE_TTF
										mTTFont1
									#else
										mFont
									#endif
										);
	
	
	YLOG("GameApp::Create dirSys\n");
	dirSys->Create(workPath);
	YLOG("GameApp::Create dirSys done\n");
}


//-------------------------------------------------------------------------------------
// This is the clean up callback function. You should delete all your in-game 
// resources, for example texture and quads, here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Destroy()
{

	DSystm* dirSys = DSystm::GetInstance();
	YLOG("quit ymenu requested\n");
#ifdef PSP
	YLOG("copying eboot path: %s\n", dirSys->getAppPath().c_str());
	if (this->settings != NULL)	strcpy(this->settings->filename, dirSys->getAppPath().c_str());
	
	dirSys->getCurApp()->launch();
	
#endif
	
	Display::GetInstance()->Destroy();
	dirSys->Destroy();
	

#ifdef USE_TTF
	SAFE_DELETE(mTTFont1);
#else
	intraFontUnload(mFont);
	intraFontShutdown();
#endif
	
	
	//End message infinite loop
	
}


//-------------------------------------------------------------------------------------
// This is the update callback function and is called at each update frame
// before rendering. You should update the game logic here.
//
//-------------------------------------------------------------------------------------
void GameApp::Update()
{
	Display* displayer = Display::GetInstance();

	displayer->updateBg();
	DSystm::GetInstance()->update();
	
	displayer->updateNotify();
}


//-------------------------------------------------------------------------------------
// All rendering operations should be done in Render() only.
// 
//-------------------------------------------------------------------------------------
void GameApp::Render()
{

	JRenderer* renderer = JRenderer::GetInstance();
	
	Display* displayer = Display::GetInstance();
	displayer->renderBg();
	displayer->renderInfoQuad();
	DSystm::GetInstance()->render();
	displayer->renderInfoFont();
	displayer->renderNotify();


#ifdef WIN32
	Sleep(1000/FPS_RATE);
	// emulates psp's max 60 fps
#endif
}


//-------------------------------------------------------------------------------------
// This function is called when the system wants to pause the game. You can set a flag
// here to stop the update loop and audio playback.
//
//-------------------------------------------------------------------------------------
void GameApp::Pause()
{
	
}


//-------------------------------------------------------------------------------------
// This function is called when the game returns from the pause state.
//
//-------------------------------------------------------------------------------------
void GameApp::Resume()
{
	
}


void GameApp::OnScroll(int inXVelocity, int inYVelocity)
{
}

