
#ifndef _COMMON_H_
#define _COMMON_H_


#include <JGE.h>
#include <JRenderer.h>
#include <JLBFont.h>
#include <JTTFont.h>
#include <JSoundSystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <typeinfo>



// DEBUG mode : uncomment to compile a debug build
//#define YDEBUG


// Compile mode : define VHBL to compile a VHBL build, or define TN_CFW to compile a TN_CFW build
//#define VHBL
#define TN_CFW



#define FATAL_ERROR(...) DO_EXIT()
#define YLOG(...) ;
#define SLEEP(a) ;
#ifdef PSP
#include <pspsdk.h>
#include <pspiofilemgr.h>
#include <pspiofilemgr_dirent.h>
#include <psppower.h>
#include <psploadexec_kernel.h>
#include <pspinit.h>
#include <pspsystemctrl_user.h>
#define PSP_INIT_APITYPE_UMDEMU_MS 0x123
#define abs(a) fabs(a)	// avoids float bugs
#define DO_EXIT sceKernelExitGame

#ifdef YDEBUG
#undef YLOG
#define YLOG Lib::psp_dbg
//#undef SLEEP(a)
//#define SLEEP(a) sceKernelDelayThread(1000000*a);
#endif

#include <intraFont.h>
#include <unziprar.h>

#else
#define USE_TTF
#define FATAL_ERROR(a) exit(-1)
#endif



#ifdef wagic
#define PSP_CTRL_UP JGE_BTN_UP
#define PSP_CTRL_DOWN JGE_BTN_DOWN
#define PSP_CTRL_RIGHT JGE_BTN_RIGHT
#define PSP_CTRL_LEFT JGE_BTN_LEFT
#define PSP_CTRL_CROSS JGE_BTN_SEC
#define PSP_CTRL_CIRCLE JGE_BTN_OK
#define PSP_CTRL_TRIANGLE JGE_BTN_CANCEL
#define PSP_CTRL_SQUARE JGE_BTN_PRI
#else
#define JGE_BTN_UP PSP_CTRL_UP
#define JGE_BTN_DOWN PSP_CTRL_DOWN
#define JGE_BTN_RIGHT PSP_CTRL_RIGHT
#define JGE_BTN_LEFT PSP_CTRL_LEFT
#define JGE_BTN_OK PSP_CTRL_CIRCLE
#define JGE_BTN_SEC PSP_CTRL_CROSS
#define JGE_BTN_CANCEL PSP_CTRL_TRIANGLE
#endif

using namespace std;


#define ARGB(a, r, g, b)	(((r)<<16) | ((g)<<8) | (b) | ((a)<<24))
#define FONT_SIZE 28
#define FPS_RATE 60
#define EBOOTNAMES { "VBOOT.PBP", "FBOOT.PBP", "wmenu.bin", "EBOOT.PBP" }
#define HOMEBREW_PATH "ms0:/PSP/GAME"
#define SAVEDATA_PATH "ms0:/PSP/SAVEDATA"
#define DEFAULT_ICON_W 144
#define DEFAULT_ICON_H 80
#define WALLPAPER_PREFIX "bg_"
#define WALLPAPER_EXTS { "png", "jpg", "gif" }
#define ARCHIVE_NAME "install"
#define BG_SWAP_TIME 6
#define BG_SWAP_ASTEP 0.03
#define CAT_PREFIX "CAT_"
#define SAFE_FREE(a) { if (a != NULL) { free(a); a = NULL;} }
#define StrCast(a, b) transform(a.begin(), a.end(), a.begin(), b)

typedef struct {
	int x;
	int y;
	int w;
	int h;
} Coord;

typedef struct {
	int offset;
	int size;
} EbootSlot;

typedef struct {
	int idx;
	int offset;
} dirIdx;


enum name_formats_ {
	ANY_FORMAT, LOWERCASE, UPPERCASE
};


#define RESPATH "."
/*

	enum PspCtrlButtons
	{
		PSP_CTRL_SELECT     = 0x000001,
		PSP_CTRL_START      = 0x000008,
		PSP_CTRL_UP         = 0x000010,
		PSP_CTRL_RIGHT      = 0x000020,
		PSP_CTRL_DOWN      	= 0x000040,
		PSP_CTRL_LEFT      	= 0x000080,
		PSP_CTRL_LTRIGGER   = 0x000100,
		PSP_CTRL_RTRIGGER   = 0x000200,
		PSP_CTRL_TRIANGLE   = 0x001000,
		PSP_CTRL_CIRCLE     = 0x002000,
		PSP_CTRL_CROSS      = 0x004000,
		PSP_CTRL_SQUARE     = 0x008000,
		PSP_CTRL_HOME       = 0x010000,
		PSP_CTRL_HOLD       = 0x020000,
		PSP_CTRL_NOTE       = 0x800000,
	};
	
	typedef enum Buttons
  {
    JGE_BTN_NONE = 0,   // No button pressed
    JGE_BTN_QUIT,   // Home on PSP
    JGE_BTN_MENU,   // Start on PSP
    JGE_BTN_CTRL,   // Select
    JGE_BTN_POWER,  // Hold
    JGE_BTN_SOUND,  // Music note
    JGE_BTN_RIGHT,
    JGE_BTN_LEFT,
    JGE_BTN_UP,
    JGE_BTN_DOWN,
    JGE_BTN_OK,     // Circle in Japan, Cross in Europe
    JGE_BTN_CANCEL, // Triangle
    JGE_BTN_PRI,    // Square (primary)
    JGE_BTN_SEC,    // Cross or Circle (secondary)
    JGE_BTN_PREV,   // Left trigger
    JGE_BTN_NEXT,    // Right trigger
    JGE_BTN_FULLSCREEN,    // Switch to fullscreen (obviously, PC only)

    JGE_BTN_MAX = JGE_BTN_FULLSCREEN + 1
  } JButton;
  
*/

#endif
