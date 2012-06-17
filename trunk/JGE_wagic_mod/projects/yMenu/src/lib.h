
#ifndef _LIB_H_
#define _LIB_H_

#include "common.h"
#include "math.h"
#include "time.h"
#include "YDir.h"

class Lib
{
private:

public:

	/*
	 * hexascii to integer conversion
	 */
	static int xstrtoi(const char *str);
	static int psp_dbg(char *format, ...);
	static bool fileExists( string filename );
	static int openFile( string filename );
	static int closeFile( int id );
	static string GetInfo();
	static void removeDir( string dirPath );
	
	
};


#endif
