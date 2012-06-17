//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _GAMEAPP_H_
#define _GAMEAPP_H_

#include <JApp.h>

#include "common.h"
#include "lib.h"
#include "DSystm.h"
#include "Display.h"


typedef struct
{
	unsigned long        APIVersion;
	char       Credits[32];
	char       VersionName[32];
	char       *BackgroundFilename;   // set to NULL to let menu choose.
	char        * filename;   // The menu will write the selected filename there
}	tMenuApi;


class GameApp:	public JApp
{
private:
#ifdef USE_TTF
	JTTFont* mTTFont1;
#else
	intraFont* mFont;
#endif

	tMenuApi * settings;
	
public:
	GameApp();
	virtual ~GameApp();
	virtual void Create();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();
	virtual void Pause();
	void Resume();
    virtual void OnScroll(int inXVelocity, int inYVelocity);

};

#endif
