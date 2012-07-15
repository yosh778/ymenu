
#ifndef _YISO_H_
#define _YISO_H_

#include "YLaunch.h"
#include "ISO9660.h"


enum BackupTypes_ {
	BACKUP_UNKNOWN, BACKUP_UNTOUCHED, BACKUP_PATCHED
};


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
	virtual u16 findDirPathTable( u16 parent, string dirPath );
	void readPathTable();
	vector<DirectoryRecord*>* getDir( DirectoryRecord* dir );
	DirectoryRecord* findFile( string fileName, DirectoryRecord* dir );
	void* getFile( DirectoryRecord* fileRecord );
	
public:
	static const u32 SECTOR_SIZE;
	

	YISO( string isoPath );
	~YISO();
	
	virtual string getTitle();
	virtual void* getPngData();
	virtual void freePngData();
	virtual int appInit();
	void Create();
	
	static bool isISO ( string filePath );
	static u32 lba2Pos( u32 lba );
};


#endif
