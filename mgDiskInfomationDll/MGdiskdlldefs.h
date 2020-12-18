#pragma once
// MGdiskdlldefs.h

#define MAXPARTITIONCOUNT 128

typedef struct mgpartitioninfo {
	int PartitionStyle;
	int PartitionNumber;
	int Signature;

	long long StartingOffset;
	long long PartitionLength;
	BOOL IsServicePartition;
	BOOL RewritePartition;
	wchar_t guidbuf[64];
	wchar_t guidtypebuf[64];
	wchar_t partitionname[64];
}  MG_PARTITIONINFO, *P_MG_PARTITIONINFO ;


typedef struct mgdiskinfo {
	BOOL summaryinfovalid;
	wchar_t physpath[MAX_PATH];
	char volumelist[128];
	long long mediasize;
	BOOL isbootdisk;
	int npartitions;
	// Geometry
	BOOL geometryinfovalid;
	long long cylinders;
	long long trackspercyl;
	long long sectorspertrack;
	long long bytespersector;
	long long disksize;
	float disksizegb;
	//GetDriveLayout
	BOOL drivelayoutinfovalid;
	int diskpartitionstyle;
	int diskpartitioncount;
	// partitions
	P_MG_PARTITIONINFO diskpartitions;// array of partitions
} MG_DISKINFO, *P_MG_DISKINFO ;

extern BOOL mggetdiskinfo(int diskn, P_MG_DISKINFO pinfo); 

class mg_diskitem2 {
	BOOL validvolume = FALSE;
	CHAR  RootPathName[1024];
	DWORD SectorsPerCluster;
	DWORD BytesPerSector;
	DWORD NumberOfFreeClusters;
	DWORD TotalNumberOfClusters;

public:
	MG_DISKINFO mydiskinfo;
	P_MG_PARTITIONINFO mypartitions;
	mg_diskitem2(int disknumber);
};