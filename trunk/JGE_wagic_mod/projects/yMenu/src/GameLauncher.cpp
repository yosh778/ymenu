//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include <JGameLauncher.h>

#include "GameApp.h"
#include "lib.h"


//-------------------------------------------------------------------------------------
JApp* JGameLauncher::GetGameApp()
{
	SLEEP(7);
	return new GameApp();
};


//-------------------------------------------------------------------------------------
char *JGameLauncher::GetName()
{
	return "yMenu";
}


//-------------------------------------------------------------------------------------
u32 JGameLauncher::GetInitFlags()
{
	return JINIT_FLAG_NORMAL;
}
