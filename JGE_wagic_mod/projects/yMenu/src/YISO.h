
#ifndef _YISO_H_
#define _YISO_H_

#include "YLaunch.h"
#include "ISO9660.h"



class YISO : public YLaunch
{

private:
	

protected:
	ifstream mFin;
	void* mPngData;
	vector<PathTableRecord*> mPathTable;
	int mBackupType;
	char* mTitle;
	
	virtual bool open( string path );
	virtual int readSector( char *destBuf, unsigned sector );
	void* read( u32 sector, u32 len );
	virtual void close();
	
	
	void processPathTable( PathTableRecord* pathTable, u32 pathTableSize );
	
	u16 findDirPathTable( string dirPath, u16 parent = 1 );
	DirectoryRecord* findFile( string fileName, DirectoryRecord* dir );
	
	vector<DirectoryRecord*>* getDir( DirectoryRecord* dir );
	void* getFile( DirectoryRecord* fileRecord );
	
	
	
	static u32 lba2Pos( u32 lba );
	
	
public:
	static const u32 SECTOR_SIZE;
	

	YISO( string isoPath );
	virtual ~YISO();
	
	virtual string getTitle();
	virtual void* getPngData();
	virtual void freePngData();
	virtual int appInit();
	void Create();
	
	static bool isISO ( string filePath );
};


#endif
