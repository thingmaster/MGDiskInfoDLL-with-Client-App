#pragma once
// MGdiskdlldefs.h
typedef struct mgdiskinfo {
	wchar_t phypath[MAX_PATH];
	char volumelist[128];
	long long mediasize;
	BOOL isbootdisk;
	int npartitions;
} MG_DISKINFO, *P_MG_DISKINFO;

extern BOOL mggetdiskinfo(int diskn, P_MG_DISKINFO pinfo); 
