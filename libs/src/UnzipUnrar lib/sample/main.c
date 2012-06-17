#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <unziprar.h>


PSP_MODULE_INFO("Unzip/Unrar sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);


int main(void)
{
	const char * zip[] = { "z.zip", "z/" },
				* rar[] = { "r.rar", "r/" };
	
	pspDebugScreenInit();
	pspDebugScreenSetXY(2, 3);
	pspDebugScreenPrintf("Starting\n\n");
	
	
	pspDebugScreenPrintf("Extracting zip file %s to %s ... ", zip[0], zip[1]);
	unzipToDir( zip[0], zip[1], NULL );
	pspDebugScreenPrintf("done !\n");
	
	pspDebugScreenPrintf("Extracting rar file %s to %s ... ", rar[0], rar[1]);
	DoExtractRAR ( rar[0], rar[1], NULL );
	pspDebugScreenPrintf("done !\n");

    sceKernelExitGame();
    return 0;
}

