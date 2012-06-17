#include "lib.h"


/*
 * hexascii to integer conversion
 * returns 0 if error, else integer result
 */
int Lib::xstrtoi(const char *str) {
	int val;
	int c;
	int i;
	int len = strlen(str);
	int result = 1;

	val = 0;
	for (i = 0, c = *str++; len-- > 0; i++, c = *str++) {
		if (c >= '0' && c <= '9') {
			c -= '0';
		} else if (c >= 'A' && c <= 'F') {
			c = (c - 'A') + 10;
		} else if (c >= 'a' && c <= 'f') {
			c = (c - 'a') + 10;
		} else {
			result = 0;
			break;
		}
		val *= 16;
		val += c;
	}

	if ( result == 1 )	result = val;

	return result;
}


#include <stdarg.h>

int Lib::psp_dbg(char *format, ...)
{

#ifdef PSP
	va_list args;
	char * s = NULL;
	
	
	s = (char*)malloc(strlen(format)+512);
	
	
	va_start(args, format);
	int r = vsprintf(s,format, args);
	va_end(args);
	
	sceIoWrite(2, s, strlen(s));
	
    int fd;
	if ((fd = sceIoOpen("dbg.txt", PSP_O_CREAT | PSP_O_WRONLY | PSP_O_APPEND, 0777)) >= 0)
	{
		sceIoWrite(fd, s, strlen(s));
		sceIoClose(fd);
	}
	
	
	
	free(s);
	
	
	return r;
#else
	return 0;
#endif
}


bool Lib::fileExists( string filename )
{
	bool exists = false;
	
    int id = openFile(filename);
#ifdef PSP
    if (id >= 0)
#else
	if (id != NULL)
#endif
	{
		exists = true;
		closeFile(id);
	}

	return exists;
}


int Lib::openFile( string filename )
{
	int ret = NULL;
	
#ifdef PSP
    ret = sceIoOpen(filename.c_str(), PSP_O_RDONLY, 0777);
#else
	ret = (int)fopen(filename.c_str(), "rb");
#endif

	return ret;
}

int Lib::closeFile( int id )
{
	int ret = NULL;
	
#ifdef PSP
    ret = sceIoClose(id);
#else
	ret = fclose((FILE*)id);
#endif

	return ret;
}


string Lib::GetInfo()
{
	stringstream ss;
	int y, m, d, h, mn, s;
	char semiColon = ':';
	
#ifdef PSP
	pspTime pTime;
	sceRtcGetCurrentClockLocalTime(&pTime);
	
	h = pTime.hour;
	mn = pTime.minutes;
	s = pTime.seconds;
	d = pTime.day;
	m = pTime.month;
	y = pTime.year;
	
#else
	time_t rawtime;
	struct tm *t;
	time ( &rawtime );
	t = localtime ( &rawtime );

	h = t->tm_hour;
	mn = t->tm_min;
	s = t->tm_sec;
	d = t->tm_mday;
	m = t->tm_mon+1;
	y = 1900 + t->tm_year;
	
	//if (t->tm_isdst)	++h;
#endif
	
	
	//if (s%2)	semiColon = ' ';
	ss << setfill('0');
	ss << setw(2) <<  d << '/' << setw(2) <<  m << '/' << y;
	ss << "  ";
	ss << setw(2) << h << semiColon << setw(2) << mn;
	
	
	return ss.str();
}

void Lib::removeDir( string dirPath )
{
	if ( dirPath[dirPath.size()-1] != '/' )	dirPath+= '/';
	
	YLOG("Lib::removeDir: %s\n",dirPath.c_str());
	vector<SceIoDirent>* dir = YDir::getDir ( dirPath );
	YLOG("%X\n",dir);
	
	YLOG("dir->size(): %X\n",dir->size());
	for (int i=0; i<dir->size(); ++i )
	{
		YLOG("%X\n",dir);
		if ((*dir)[i].d_stat.st_attr & FIO_SO_IFDIR && (*dir)[i].d_stat.st_mode & FIO_S_IFDIR)
		{
			YLOG("folder: recursive call: %s\n",(dirPath+(*dir)[i].d_name+"/").c_str());
			Lib::removeDir ( dirPath+(*dir)[i].d_name+"/" );
		}
		else
		{
			sceIoRemove( (dirPath+(*dir)[i].d_name).c_str() );
			YLOG("file: remove it: %s\n",(*dir)[i].d_name);
		}
	}
	
	SAFE_DELETE(dir);
	
	YLOG("end: remove container: %s\n",(dirPath.substr(0,dirPath.size()-1)).c_str());
	sceIoRmdir((dirPath.substr(0,dirPath.size()-1)).c_str());
}

