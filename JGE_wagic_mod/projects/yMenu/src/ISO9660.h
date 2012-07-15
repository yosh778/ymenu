
#ifndef _ISO9660_H_
#define _ISO9660_H_



typedef struct __attribute__((packed)) lbe32_ {
	u32 LE;
	u32 BE;
} lbe32;


typedef struct __attribute__((packed)) lbe16_ {
	u16 LE;
	u16 BE;
} lbe16;


typedef struct  __attribute__((packed)) PrimVolDateTime_ {
	char year[4];
	char month[2];
	char day[2];
	char hour[2];
	char minute[2];
	char second[2];
	char centiSeconds[2];
	u8 gmtOffset;
} PrimVolDateTime;


typedef struct  __attribute__((packed)) PrimaryVolumeDescriptor_ {
	u8 typeCode;	// Always 1 for a Primary Volume Descriptor
	char id[5];	// Always CD001
	u8 version;	// Always 1
	u8 unused0;	// Always 0
	char systmId[0x20];
	char volId[0x20];
	u8 unused1[8];
	lbe32 isoSize;	// Unit = sectorSize
	u8 unused2[0x20];
	lbe16 volSetSize;
	lbe16 volSeqNb;
	lbe16 sectorSize;
	lbe32 pathTableSize;
	u32 lbaPathTableL;
	u32 lbaPathTableBackupL;
	u32 lbaPathTableM;
	u32 lbaPathTableBackupM;
	u8 rootDirRecord[0x22];	// DirectoryRecord with 0 sized name, so always 34 bytes
	char volSetId[0x80];
	char publisherId[0x80];
	char dataPreparerId[0x80];
	char appId[0x80];
	char copyrightFileId[0x26];
	char abstractFileId[0x24];
	char bibliographicFileId[0x25];
	PrimVolDateTime volCreatDate;
	PrimVolDateTime volModDate;
	PrimVolDateTime volExpDate;
	PrimVolDateTime volEffDate;	// Volume Effective Date and Time : Date and time from which the volume should be used
	u8 structVersion;	// Always 1 : directory records and path table version
	u8 unused3;	// Always 0
	u8 appUsed[0x200];	// Not defined by ISO 9660
	u8 reserved[0x28D];	// Reserved by ISO
} PrimaryVolumeDescriptor;


typedef struct  __attribute__((packed)) PathTableRecord_ {
	u8 nameSize;
	u8 xarSize;
	u32 lba;
	u16 parentIdx;
	char name[];	// If nameSize odd, followed by pad byte
} PathTableRecord;


typedef struct  __attribute__((packed)) DirRecDateTime_ {
	u8	year;
	u8	month;
	u8	day;
	u8	hour;
	u8	minute;
	u8	second;
	u8	gmtOffset;
} DirRecDateTime;


enum ISO9660FileFlags {
	HIDDEN = 0, DIRECTORY = 1, ASSOCIATED = 2, XAR_FORMAT = 3, XAR_PERM = 4, RESERVED_0 = 5, RESERVED_1 = 6, NOT_FINAL_DIR = 7
};


typedef struct __attribute__((packed))
{
	u8 size;
	u8 xarSize;		//	Extended Attribute Record length
	lbe32 lba;
	lbe32 fileSize;
	DirRecDateTime time;
	u8 fileFlags;	// One of enum ISO9660FileFlags
	u8 interleaveUnit;
	u8 interleaveSkip;
	lbe16 volSeqNb;
	u8 nameSize;
	char name[];	// If nameSize odd, followed by pad byte
} DirectoryRecord;




#endif

