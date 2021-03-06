#pragma once
// MGSTORAGELIBrary.h - Contains declarations of math functions
#pragma once

#ifdef MGDISKINFOMATIONDLL_EXPORTS
#define MGSTORAGEDLL_API __declspec(dllexport)
#else
#define MGSTORAGEDLL_API __declspec(dllimport)
#endif

#include "MGdiskdlldefs.h"

// The Fibonacci recurrence relation describes a sequence F
// where F(n) is { n = 0, a
//               { n = 1, b
//               { n > 1, F(n-2) + F(n-1)
// for some initial integral values a and b.
// If the sequence is initiaized F(0) = 1, F(1) = 1,
// then this relation produces the well-known Fibonacci
// sequence: 1, 1, 2, 3, 5, 8, 13, 21, 34, ...

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
extern "C" MGSTORAGEDLL_API void fibonacci_init(
    const unsigned long long a, const unsigned long long b);

// Produce the next value in the sequence.
// Returns true on success and updates current value and index;
// false on overflow, leaves current value and index unchanged.
extern "C" MGSTORAGEDLL_API bool fibonacci_next();

// Get the current value in the sequence.
extern "C" MGSTORAGEDLL_API unsigned long long fibonacci_current();

// Get the position of the current value in the sequence.
extern "C" MGSTORAGEDLL_API unsigned fibonacci_index();

//scan SMART information TEST VERSION.
extern "C" MGSTORAGEDLL_API unsigned MGSMARTscan();

//scan SMART information TEST VERSION.
extern "C" MGSTORAGEDLL_API unsigned MGdiskpart(TCHAR * tmpfileptr);

extern "C" MGSTORAGEDLL_API HANDLE W32_Read(HANDLE fhnd, LPCWSTR physpath, LPVOID rbytebuf, DWORD readsize, LONG offs);
extern "C" MGSTORAGEDLL_API HANDLE W32_Write(HANDLE fhnd, LPCWSTR physpath, LPCVOID wbytedata, DWORD writecount, int offs);
extern "C" MGSTORAGEDLL_API BOOL W32_lock_volume(HANDLE drivehandle);
extern "C" MGSTORAGEDLL_API BOOL W32_dismount_volume(LPCWSTR volpath );
//extern "C" MGSTORAGEDLL_API int diskgeometry(LPWSTR physicaldevpath);
//extern "C" MGSTORAGEDLL_API int disklayout(LPWSTR physicaldevpath);

//extern "C" MGSTORAGEDLL_API class mg_diskoperations;

// partition_decode is a service function to decode ioctl output partition structure 
BOOL partition_decode(PARTITION_INFORMATION_EX* p_pinfo, P_MG_DISKINFO returninfo, 
    int partitiontype, int partitioncount=1)
{
    /*
PARTITION_BASIC_DATA_GUID
    ebd0a0a2 - b9e5 - 4433 - 87c0 - 68b6b72699c7
    The data partition type that is created and recognized by Windows.
    Only partitions of this type can be assigned drive letters, receive volume GUID paths, host mounted folders(also called volume mount points), and be enumerated by calls to FindFirstVolumeand FindNextVolume.

    This value can be set only for basic disks, with one exception.If both PARTITION_BASIC_DATA_GUIDand GPT_ATTRIBUTE_PLATFORM_REQUIRED are set for a partition on a basic disk that is subsequently converted to a dynamic disk, the partition remains a basic partition, even though the rest of the disk is a dynamic disk.This is because the partition is considered to be an OEM partition on a GPT disk.

    PARTITION_ENTRY_UNUSED_GUID
    00000000 - 0000 - 0000 - 0000 - 000000000000
    There is no partition.
    This value can be set for basicand dynamic disks.

    PARTITION_SYSTEM_GUID
    c12a7328 - f81f - 11d2 - ba4b - 00a0c93ec93b
    The partition is an EFI system partition.
    This value can be set for basicand dynamic disks.

    PARTITION_MSFT_RESERVED_GUID
    e3c9e316 - 0b5c - 4db8 - 817d - f92df00215ae
    The partition is a Microsoft reserved partition.
    This value can be set for basicand dynamic disks.

    PARTITION_LDM_METADATA_GUID
    5808c8aa - 7e8f - 42e0 - 85d2 - e1e90434cfb3
    The partition is a Logical Disk Manager(LDM) metadata partition on a dynamic disk.
    This value can be set only for dynamic disks.

    PARTITION_LDM_DATA_GUID
    af9b60a0 - 1431 - 4f62 - bc68 - 3311714a69ad
    The partition is an LDM data partition on a dynamic disk.
    This value can be set only for dynamic disks.

    PARTITION_MSFT_RECOVERY_GUID
    de94bba4 - 06d1 - 4d40 - a16a - bfd50179d6ac
    The partition is a Microsoft recovery partition.
    This value can be set for basicand dynamic disks.
    */
    struct partitionguidlookup {
        int guid_value;
        const char* guid_name_str;
        const char* guid_value_str;
    } windows_definitions[7] = {
        {0, (const char*)"PARTITION_BASIC_DATA_GUID", (const char*)"ebd0a0a2-b9e5-4433-87c0-68b6b72699c7"},
        {1, (const char*)"PARTITION_ENTRY_UNUSED_GUID", (const char*)"00000000-0000-0000-0000-000000000000"},
        {2, (const char*)"PARTITION_SYSTEM_GUID",(const char*)"c12a7328-f81f-11d2-ba4b-00a0c93ec93b"},
        {3, (const char*)"PARTITION_MSFT_RESERVED_GUID", (const char*)"e3c9e316-0b5c-4db8-817d-f92df00215ae"},
        {4, (const char*)"PARTITION_LDM_METADATA_GUID",(const char*)"5808c8aa-7e8f-42e0-85d2-e1e90434cfb3"},
        {5, (const char*)"PARTITION_LDM_DATA_GUID",(const char*)"af9b60a0-1431-4f62-bc68-3311714a69ad"},
        {6, (const char*)"PARTITION_MSFT_RECOVERY_GUID",(const char*)"de94bba4-06d1-4d40-a16a-bfd50179d6ac"} };

    PARTITION_INFORMATION_MBR* p_pinfo_mbr;
    PARTITION_INFORMATION_GPT* p_pinfo_gpt;
    P_MG_PARTITIONINFO ppartinfo = &returninfo->diskpartitions[0];
    switch (partitiontype)
    {
    case PARTITION_STYLE_MBR: // = 0,
        if (TRUE)
        {
            //returninfo->Sig
            //layoutinfo[2] = pdli_mbr->Signature;
            //layoutinfo[3] = pdli_mbr->CheckSum;
        }
        for (int i = 0; i < partitioncount; i++, p_pinfo++)
        {
            p_pinfo_mbr = &p_pinfo->Mbr;
            p_pinfo_gpt = &p_pinfo->Gpt;
            //------PARTITION_INFO_EX
            //PARTITION_STYLE PartitionStyle;
            //LARGE_INTEGER   StartingOffset;
            //LARGE_INTEGER   PartitionLength;
            //DWORD           PartitionNumber;
            //BOOLEAN         RewritePartition;
            //BOOLEAN         IsServicePartition;
            //union {
            //    PARTITION_INFORMATION_MBR Mbr;
            //    PARTITION_INFORMATION_GPT Gpt;
            //} DUMMYUNIONNAME;
            ppartinfo = &returninfo->diskpartitions[p_pinfo->PartitionNumber];
            ppartinfo->PartitionStyle = p_pinfo->PartitionStyle;
            ppartinfo->StartingOffset = p_pinfo->StartingOffset.QuadPart;
            ppartinfo->PartitionLength = p_pinfo->PartitionLength.QuadPart;
            ppartinfo->PartitionNumber = p_pinfo->PartitionNumber;

            wprintf(L"\nPart Style = %ld  [MBR %d, GPT %d, RAW %d]\n", (ULONG)p_pinfo->PartitionStyle,
                PARTITION_STYLE::PARTITION_STYLE_MBR,
                PARTITION_STYLE::PARTITION_STYLE_GPT,
                PARTITION_STYLE::PARTITION_STYLE_RAW);

            wprintf(L"Start offs   = %I64d\n", p_pinfo->StartingOffset);
            wprintf(L"Part length  = %I64d\n", p_pinfo->PartitionLength);
            wprintf(L"Part Number  = %ld\n", (ULONG)p_pinfo->PartitionNumber);
            //typedef struct _PARTITION_INFORMATION_MBR {
            //    BYTE    PartitionType;
            //    BOOLEAN BootIndicator;
            //    BOOLEAN RecognizedPartition;
            //    DWORD   HiddenSectors;
            //    GUID    PartitionId;
            //} PARTITION_INFORMATION_MBR, * PPARTITION_INFORMATION_MBR
            wprintf(L"P Type = %d\n", p_pinfo_mbr->PartitionType);
            wprintf(L"Boot indicator   = %d\n", p_pinfo_mbr->BootIndicator);
            wprintf(L"Recognized  = %d\n", p_pinfo_mbr->RecognizedPartition);
            wprintf(L"Hidden sect  = %0lX\n", (ULONG)p_pinfo_mbr->HiddenSectors);
            wprintf(L"GUID id  = %0lX\n", (GUID)p_pinfo_mbr->PartitionId);
        }
        break;
    case PARTITION_STYLE_GPT: // = 1,
        //------PARTITION_INFO_EX
        //PARTITION_STYLE PartitionStyle;
        //LARGE_INTEGER   StartingOffset;
        //LARGE_INTEGER   PartitionLength;
        //DWORD           PartitionNumber;
        //BOOLEAN         RewritePartition;
        //BOOLEAN         IsServicePartition;
        //union {
        //    PARTITION_INFORMATION_MBR Mbr;
        //    PARTITION_INFORMATION_GPT Gpt;
        //} DUMMYUNIONNAME;
        for (int i = 0; i < partitioncount; i++, p_pinfo++)
        {
            p_pinfo_mbr = &p_pinfo->Mbr;
            p_pinfo_gpt = &p_pinfo->Gpt;
            //------PARTITION_INFO_EX
            //PARTITION_STYLE PartitionStyle;
            //LARGE_INTEGER   StartingOffset;
            //LARGE_INTEGER   PartitionLength;
            //DWORD           PartitionNumber;
            //BOOLEAN         RewritePartition;
            //BOOLEAN         IsServicePartition;
            //union {
            //    PARTITION_INFORMATION_MBR Mbr;
            //    PARTITION_INFORMATION_GPT Gpt;
            //} DUMMYUNIONNAME;
            ppartinfo = &returninfo->diskpartitions[p_pinfo->PartitionNumber];
            ppartinfo->PartitionStyle = p_pinfo->PartitionStyle;
            ppartinfo->StartingOffset = p_pinfo->StartingOffset.QuadPart;
            ppartinfo->PartitionLength= p_pinfo->PartitionLength.QuadPart;
            ppartinfo->PartitionNumber = p_pinfo->PartitionNumber;

            wprintf(L"\nPart Style = %ld [MBR %d, GPT %d, RAW %d]\n", (ULONG)p_pinfo->PartitionStyle,
                PARTITION_STYLE::PARTITION_STYLE_MBR,
                PARTITION_STYLE::PARTITION_STYLE_GPT,
                PARTITION_STYLE::PARTITION_STYLE_RAW);
            wprintf(L"Start offs   = %016I64XX (%I64d)\n", p_pinfo->StartingOffset, p_pinfo->StartingOffset);
            wprintf(L"Part length  = %016I64XX (%I64d)\n", p_pinfo->PartitionLength, p_pinfo->PartitionLength);
            wprintf(L"Part Number  = %ld\n", (ULONG)p_pinfo->PartitionNumber);
            //typedef struct _PARTITION_INFORMATION_GPT {
            //    GUID    PartitionType;
            //    GUID    PartitionId;
            //    DWORD64 Attributes;
            //    WCHAR   Name[36];
            //} PARTITION_INFORMATION_GPT, * PPARTITION_INFORMATION_GPT;
            wprintf(L"GPT Partition Type GUID = %lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n", p_pinfo_gpt->PartitionType.Data1, p_pinfo_gpt->PartitionType.Data2, p_pinfo_gpt->PartitionType.Data3,
                p_pinfo_gpt->PartitionType.Data4[0], p_pinfo_gpt->PartitionType.Data4[1], p_pinfo_gpt->PartitionType.Data4[2], p_pinfo_gpt->PartitionType.Data4[3],
                p_pinfo_gpt->PartitionType.Data4[4], p_pinfo_gpt->PartitionType.Data4[5], p_pinfo_gpt->PartitionType.Data4[6], p_pinfo_gpt->PartitionType.Data4[7]);
            wsprintf(ppartinfo->guidtypebuf, L"%lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", p_pinfo_gpt->PartitionType.Data1, p_pinfo_gpt->PartitionType.Data2, p_pinfo_gpt->PartitionType.Data3,
                p_pinfo_gpt->PartitionType.Data4[0], p_pinfo_gpt->PartitionType.Data4[1], p_pinfo_gpt->PartitionType.Data4[2], p_pinfo_gpt->PartitionType.Data4[3],
                p_pinfo_gpt->PartitionType.Data4[4], p_pinfo_gpt->PartitionType.Data4[5], p_pinfo_gpt->PartitionType.Data4[6], p_pinfo_gpt->PartitionType.Data4[7]);
            char guidbuf[64] = "";
            sprintf_s(guidbuf, 64, "%lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", p_pinfo_gpt->PartitionType.Data1, p_pinfo_gpt->PartitionType.Data2, p_pinfo_gpt->PartitionType.Data3,
                p_pinfo_gpt->PartitionType.Data4[0], p_pinfo_gpt->PartitionType.Data4[1], p_pinfo_gpt->PartitionType.Data4[2], p_pinfo_gpt->PartitionType.Data4[3],
                p_pinfo_gpt->PartitionType.Data4[4], p_pinfo_gpt->PartitionType.Data4[5], p_pinfo_gpt->PartitionType.Data4[6], p_pinfo_gpt->PartitionType.Data4[7]);
            for (int guidtype = 0; guidtype < 7; guidtype++)
            {
                if (!strcmp(windows_definitions[guidtype].guid_value_str, guidbuf))
                {
                    /* it's a match */
                    printf("GUIDTYPE=%s\n", windows_definitions[guidtype].guid_name_str);
                }
            }
            wprintf(L"GPT Partition ID GUID = %lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n", p_pinfo_gpt->PartitionId.Data1, p_pinfo_gpt->PartitionId.Data2, p_pinfo_gpt->PartitionId.Data3,
                p_pinfo_gpt->PartitionId.Data4[0], p_pinfo_gpt->PartitionId.Data4[1], p_pinfo_gpt->PartitionId.Data4[2], p_pinfo_gpt->PartitionId.Data4[3],
                p_pinfo_gpt->PartitionId.Data4[4], p_pinfo_gpt->PartitionId.Data4[5], p_pinfo_gpt->PartitionId.Data4[6], p_pinfo_gpt->PartitionId.Data4[7]);
            wsprintf(ppartinfo->guidbuf, L"%lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", p_pinfo_gpt->PartitionId.Data1, p_pinfo_gpt->PartitionId.Data2, p_pinfo_gpt->PartitionId.Data3,
                p_pinfo_gpt->PartitionId.Data4[0], p_pinfo_gpt->PartitionId.Data4[1], p_pinfo_gpt->PartitionId.Data4[2], p_pinfo_gpt->PartitionId.Data4[3],
                p_pinfo_gpt->PartitionId.Data4[4], p_pinfo_gpt->PartitionId.Data4[5], p_pinfo_gpt->PartitionId.Data4[6], p_pinfo_gpt->PartitionId.Data4[7]);
            wprintf(L"Attributes  =  %016I64X\n", p_pinfo_gpt->Attributes);
            wprintf(L"Name = %ws\n", p_pinfo_gpt->Name);
            wsprintf(ppartinfo->partitionname, L"%ws", p_pinfo_gpt->Name);
        }
        break;

    case PARTITION_STYLE_RAW: // = 2
    default:
        break;
    }
    return TRUE;
}



#define IOCTLBUFFERSZ 2048

// class mg_deviceioctl - wrapper for generic Windows IOCTL service request.
// caller must: 
// instantiate this. provide IOCTL code for the operation
// call open method here. open the target physical disk device or volume
//  -  ioctl_status is set TRUE (open succeeded)  or FALSE (failed) 
// call execute method here. executes the actual ioctl request. 
//     - activeioctlbuffer is filled with response data
//     = ioctl_status is set TRUE (it worked)  or FALSE (failed)
// 
// WARNING any new requests not already supported here should be carefully reviewed for
// detailed inputs and outputs data type and size!
//
extern "C" MGSTORAGEDLL_API class mg_deviceioctl {
    HANDLE activehandle;
    DWORD activeioctl;
    DWORD lastioerror;

public:
    BYTE  activeioctlbuffer[2048];
    BOOL ioctl_status;
    //wchar_t* activedevicepath;  

public:
    mg_deviceioctl(DWORD newioctl = NULL)
    {
        this->activeioctl = newioctl;
        this->ioctl_status = FALSE;
    };

    BOOL mg_openforioctl(LPWSTR wszpath)
    {
        HANDLE hDevice = CreateFileW(wszpath,          // drive to open
            0,                // no access to the drive
            FILE_SHARE_READ | // share mode
            FILE_SHARE_WRITE,
            NULL,             // default security attributes
            OPEN_EXISTING,    // disposition
            0,                // file attributes
            NULL);            // do not copy file attributes

        if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
        {
            this->activehandle = 0;
            this->lastioerror = GetLastError();
            return (FALSE);
        }
        this->activehandle = hDevice;
        //wsprintf(this->activedevicepath, L"%s", wszpath);
        this->lastioerror = 0;
        return TRUE;
    }
    BOOL mg_closeioctl()
    {
        return CloseHandle(this->activehandle);
    }
    BOOL mg_executeioctl(LPWSTR wszpath)
    {
        if (!this->activehandle)
        {
            if (!this->mg_openforioctl(wszpath))
            {
                return FALSE; // unable to open path.
            }
        }
        // now execute on the active handle
        DWORD retbytecount = 0;
        this->ioctl_status = DeviceIoControl(this->activehandle, // target device 
            this->activeioctl, // operation to perform
            NULL, 0,                       // no input buffer
            this->activeioctlbuffer, IOCTLBUFFERSZ,      // output buffer
            &retbytecount,                 // # bytes returned
            (LPOVERLAPPED)NULL);           // synchronous I/O
        if (!this->ioctl_status)
        {
            this->lastioerror = GetLastError();
        }
        this->mg_closeioctl();
        return this->ioctl_status;
    }
    DWORD mg_printtest(void)
    {
        printf("test");
        return this->activeioctl;
    }
    int getactiveioctl()
    {
        return this->activeioctl;
    }
};


// Execute the IOCTL_DISK_GET_DRIVE_GEOMETRY ioctl
extern "C" MGSTORAGEDLL_API class mg_diskgeometry : mg_deviceioctl {
    int otherintval;
    LPWSTR ppathbuf;
    wchar_t pathbuf[128];

public:
    mg_diskgeometry(int devicenumber) : mg_deviceioctl(IOCTL_DISK_GET_DRIVE_GEOMETRY)
    {
        this->ppathbuf = (LPWSTR) new BYTE[129 * sizeof(wchar_t)]();
        swprintf(this->ppathbuf, 128*sizeof(wchar_t), L"\\\\.\\PhysicalDrive%d\0", devicenumber);
        this->ioctl_status = this->mg_executeioctl(this->ppathbuf);  // (LPWSTR)L"\\\\.\\PhysicalDrive1");
    }
    mg_diskgeometry(LPWSTR wszpath = NULL, int othervar = 0) : mg_deviceioctl(IOCTL_DISK_GET_DRIVE_GEOMETRY)
    {
        this->ppathbuf = (LPWSTR) new BYTE[129 * sizeof(wchar_t)]();
        swprintf(this->ppathbuf, 128 * sizeof(wchar_t), L"%ws", wszpath);
        this->ioctl_status = this->mg_executeioctl(wszpath);
        //COPY THE wszpath to pathbuf
    };
    BOOL mg_geometry_info_valid()
    {
        return this->ioctl_status;
    }
    LPWSTR mg_devicepath()
    {
        if (this->mg_geometry_info_valid())
            return (LPWSTR)&this->ppathbuf;
        return NULL;
    }
    DWORD mg_printdisks(void)
    {
        printf("\ndisks %d\n", this->getactiveioctl());
        return this->getactiveioctl();
    }

    BOOL mg_geometryextractdata(P_MG_DISKINFO returninfo = NULL, BOOL outputflag = FALSE)
    {
        if (!this->ioctl_status)
        {
            return FALSE;
        }
        DISK_GEOMETRY* tmptr = (DISK_GEOMETRY*)&this->activeioctlbuffer;
        if (!tmptr)
        {
            return FALSE;
        }
        //LARGE_INTEGER Cylinders;
        //MEDIA_TYPE    MediaType; https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ne-winioctl-media_type
        //DWORD         TracksPerCylinder;
        //DWORD         SectorsPerTrack;
        //DWORD         BytesPerSector;
        //} DISK_GEOMETRY, * PDISK_GEOMETRY;
        if (returninfo)
        {
            returninfo->cylinders = tmptr->Cylinders.QuadPart;
            returninfo->mediatype = tmptr->MediaType;
            returninfo->mgsupportedmedia = (tmptr->MediaType == MEDIA_TYPE::FixedMedia) || (tmptr->MediaType == MEDIA_TYPE::FixedMedia);
            returninfo->trackspercyl = tmptr->TracksPerCylinder;
            returninfo->sectorspertrack = (ULONG)tmptr->SectorsPerTrack;
            returninfo->bytespersector = tmptr->BytesPerSector;
            returninfo->disksize = tmptr->Cylinders.QuadPart * (ULONG)tmptr->TracksPerCylinder *
                (ULONG)tmptr->SectorsPerTrack * (ULONG)tmptr->BytesPerSector;
            returninfo->disksizegb = returninfo->disksize / (1024 * 1024 * 1024);
        }
        if (!outputflag)
            return TRUE;
        wprintf(L"\n **IOCTL_DISK_GET_DRIVE_GEOMETRY** Drive path      = %ws\n", (LPWSTR)this->ppathbuf);
        wprintf(L"Media type: %d [Fixed %d, Removable %d, Unknown %d, others are Floppy media ]\n", tmptr->MediaType, MEDIA_TYPE::FixedMedia, MEDIA_TYPE::RemovableMedia, MEDIA_TYPE::Unknown);
        wprintf(L"Cylinders       = %I64d\n", tmptr->Cylinders);
        wprintf(L"Tracks/cylinder = %ld\n", (ULONG)tmptr->TracksPerCylinder);
        wprintf(L"Sectors/track   = %ld\n", (ULONG)tmptr->SectorsPerTrack);
        wprintf(L"Bytes/sector    = %ld\n", (ULONG)tmptr->BytesPerSector);
        ULONGLONG DiskSize = tmptr->Cylinders.QuadPart * (ULONG)tmptr->TracksPerCylinder *
            (ULONG)tmptr->SectorsPerTrack * (ULONG)tmptr->BytesPerSector;
        wprintf(L"Disk size       = %I64d (Bytes)\n"
            L"                = %.2f (Gb)\n",
            DiskSize, (double)DiskSize / (1024 * 1024 * 1024));
        return TRUE;
    }
};

// Execute the IOCTL_DISK_GET_DRIVE_LAYOUT_EX ioctl
extern "C" MGSTORAGEDLL_API class mg_disklayout : mg_deviceioctl {
    int otherintval;
    LPWSTR ppathbuf;
    wchar_t pathbuf[128];

public:
    mg_disklayout(int devicenumber) : mg_deviceioctl(IOCTL_DISK_GET_DRIVE_LAYOUT_EX)
    {
        this->ppathbuf = (LPWSTR) new BYTE[129 * sizeof(wchar_t)];
        swprintf(this->ppathbuf, 128 * sizeof(wchar_t), L"\\\\.\\PhysicalDrive%d", devicenumber);
        this->ioctl_status = this->mg_executeioctl(this->ppathbuf);  // (LPWSTR)L"\\\\.\\PhysicalDrive1");
    }

    mg_disklayout(LPWSTR wszpath = NULL, int othervar = 0) : mg_deviceioctl(IOCTL_DISK_GET_DRIVE_LAYOUT_EX)
    {
        this->ppathbuf = (LPWSTR) new BYTE[129 * sizeof(wchar_t)]();
        swprintf(this->ppathbuf, 128 * sizeof(wchar_t), L"%ws", wszpath);
        this->mg_executeioctl(wszpath);
    }
    DWORD mg_printdisks(void)
    {
        printf("\ndisks %d\n", this->getactiveioctl());
        return this->getactiveioctl();
    }

    BOOL mg_layoutextractdata(P_MG_DISKINFO returninfo=NULL, BOOL outputflag=FALSE)
    {
        DRIVE_LAYOUT_INFORMATION_EX* tmptr = (DRIVE_LAYOUT_INFORMATION_EX*)this->activeioctlbuffer;
        if (!tmptr)
        {
            return FALSE;
        }
        //typedef struct _DRIVE_LAYOUT_INFORMATION_EX {
        //    ULONG                    PartitionStyle;
        //    ULONG                    PartitionCount;
        //    union {
        //        DRIVE_LAYOUT_INFORMATION_MBR Mbr;
        //        DRIVE_LAYOUT_INFORMATION_GPT Gpt;
        //    } DUMMYUNIONNAME;
        //    PARTITION_INFORMATION_EX PartitionEntry[1];
        //} DRIVE_LAYOUT_INFORMATION_EX, * PDRIVE_LAYOUT_INFORMATION_EX; 
        DRIVE_LAYOUT_INFORMATION_EX* pdg = (DRIVE_LAYOUT_INFORMATION_EX*)tmptr; 
        //typedef struct _DRIVE_LAYOUT_INFORMATION_GPT {
        //    GUID          DiskId;
        //    LARGE_INTEGER StartingUsableOffset;
        //    LARGE_INTEGER UsableLength;
        //    DWORD         MaxPartitionCount;
        //} DRIVE_LAYOUT_INFORMATION_GPT, * PDRIVE_LAYOUT_INFORMATION_GPT;
        DRIVE_LAYOUT_INFORMATION_GPT* pdli_gpt = &pdg->Gpt;
        //typedef struct _DRIVE_LAYOUT_INFORMATION_MBR {
        //    DWORD Signature;
        //    DWORD CheckSum;
        //} DRIVE_LAYOUT_INFORMATION_MBR, * PDRIVE_LAYOUT_INFORMATION_MBR;
        DRIVE_LAYOUT_INFORMATION_MBR* pdli_mbr = &pdg->Mbr;
        PARTITION_INFORMATION_EX* p_pinfo = &pdg->PartitionEntry[0];
        PARTITION_INFORMATION_MBR* p_pinfo_mbr;
        PARTITION_INFORMATION_GPT* p_pinfo_gpt;
        if (returninfo)
        {
            returninfo->diskpartitionstyle = tmptr->PartitionStyle;
            returninfo->diskpartitioncount = tmptr->PartitionCount;
        }
        wprintf(L"\n **IOCTL_DISK_GET_DRIVE_LAYOUT_EX** Drive path      = %ws\n", (LPWSTR)this->ppathbuf);
        wprintf(L"PartitionStyle = %ld [MBR %d, GPT %d, RAW %d]\n", (ULONG)pdg->PartitionStyle,
            PARTITION_STYLE::PARTITION_STYLE_MBR,
            PARTITION_STYLE::PARTITION_STYLE_GPT,
            PARTITION_STYLE::PARTITION_STYLE_RAW);
        wprintf(L"PartitionCount   = %ld\n", (ULONG)pdg->PartitionCount);
        BOOL pinfoxx = partition_decode(p_pinfo, returninfo, pdg->PartitionStyle, pdg->PartitionCount);
        return TRUE;
    }
};


extern "C" MGSTORAGEDLL_API class mg_diskextents : mg_deviceioctl {
    int otherintval;
    LPWSTR ppathbuf;
    wchar_t  pathbuf[MAX_PATH];
public:
    mg_diskextents(int devicenumber) : mg_deviceioctl(IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS)
    {
        this->ppathbuf = (LPWSTR) new BYTE[129 * sizeof(wchar_t)];
        swprintf(this->ppathbuf, 128 * sizeof(wchar_t), L"\\\\.\\PhysicalDrive%d", devicenumber);
        this->ioctl_status = this->mg_executeioctl(this->ppathbuf);  // (LPWSTR)L"\\\\.\\PhysicalDrive1");
    }

    mg_diskextents(LPWSTR wszpath = NULL, int othervar = 0) : mg_deviceioctl(IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS)
    {
        this->ppathbuf = (LPWSTR) new BYTE[129 * sizeof(wchar_t)]();
        swprintf(this->ppathbuf, 128 * sizeof(wchar_t), L"%ws", wszpath);
        this->ioctl_status = this->mg_executeioctl(wszpath);
    }
    DWORD mg_printdisks(void)
    {
        printf("\ndisks %d\n", this->getactiveioctl());
        return this->getactiveioctl();
    }

    BOOL mg_extentsextractdata(long long *pextents4=NULL)
    {
        VOLUME_DISK_EXTENTS* tmptr = (VOLUME_DISK_EXTENTS*)this->activeioctlbuffer;
        if (!tmptr)
        {
            return FALSE;
        }
        //typedef struct _VOLUME_DISK_EXTENTS {
        //    DWORD       NumberOfDiskExtents;
        //    DISK_EXTENT Extents[ANYSIZE_ARRAY];
        //} VOLUME_DISK_EXTENTS, * PVOLUME_DISK_EXTENTS;
        //typedef struct _DISK_EXTENT {
        //   DWORD         DiskNumber;
        //    LARGE_INTEGER StartingOffset;
        //    LARGE_INTEGER ExtentLength;
        //} DISK_EXTENT, * PDISK_EXTENT;

        DISK_EXTENT* p_pinfo = &tmptr->Extents[0];
        if (!tmptr->NumberOfDiskExtents)
        {
            //wprintf(L"\n**IOCTL_DISK_GET_PARTITION_INFO_EX** Device %s. Partition length ZERO.\n", (LPWSTR)this->activedevicepath);
            return FALSE;
        }
        if (pextents4)
        {
            pextents4[0] = tmptr->NumberOfDiskExtents;
            pextents4[1] = p_pinfo->DiskNumber;
            pextents4[2] = (long long)(p_pinfo->StartingOffset.HighPart);
            pextents4[2] = pextents4[2] << 32;
            pextents4[2] += p_pinfo->StartingOffset.LowPart;;
            pextents4[3] = (long long)p_pinfo->ExtentLength.HighPart;
            pextents4[3] = pextents4[3] << 32;
            pextents4[3] += p_pinfo->ExtentLength.LowPart;
        }
        wprintf(L"\n **IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS** Drive path      = %ws\n", this->ppathbuf);
        wprintf(L"N Extents = %ld\n", (ULONG)tmptr->NumberOfDiskExtents);
        for (int i = 0; i < tmptr->NumberOfDiskExtents; i++, p_pinfo++)
        {
            wprintf(L"P Disk Num = %ld\n", (ULONG)p_pinfo->DiskNumber);
            wprintf(L"Start offs   = %I64d\n", p_pinfo->StartingOffset);
            wprintf(L"Part length  = %I64d\n", p_pinfo->ExtentLength);
        }
        return TRUE;
    }
};

extern "C" MGSTORAGEDLL_API class mg_partitioninfo : mg_deviceioctl {
    int otherintval;
    LPWSTR ppathbuf;
    wchar_t volumepathbuf[128];
    BOOL mg_partition_info_valid;
public:
    mg_partitioninfo(char driveletter, int othervar = 0) : mg_deviceioctl(IOCTL_DISK_GET_PARTITION_INFO_EX)
    {
        this->ppathbuf = (LPWSTR) new BYTE[129 * sizeof(wchar_t)];
        swprintf(this->ppathbuf, 128*sizeof(wchar_t), L"\\\\.\\%c:", driveletter);
        this->mg_partition_info_valid = this->mg_executeioctl(this->ppathbuf);
    }

    mg_partitioninfo(LPWSTR wszpath = NULL, int othervar = 0) : mg_deviceioctl(IOCTL_DISK_GET_PARTITION_INFO_EX)
    {
        this->ppathbuf = NULL;
        this->mg_partition_info_valid = this->mg_executeioctl(wszpath);
    }

    BOOL mg_is_partition_info_valid()
    {
        return this->mg_partition_info_valid;
    }

    LPWSTR mg_getpartitionvolumepath()
    {
        if (!this->mg_partition_info_valid)
        {
            return NULL;
        }
        return this->ppathbuf;
    }
    DWORD mg_printdisks(void)
    {
        printf("\ndisks %d\n", this->getactiveioctl());
        return this->getactiveioctl();
    }

    BOOL mg_partitionextractdata(P_MG_DISKINFO returninfo = NULL, BOOL  outputflag=TRUE)
    {
        if (!this->mg_partition_info_valid)
            return FALSE;
        PARTITION_INFORMATION_EX* tmptr = (PARTITION_INFORMATION_EX*)this->activeioctlbuffer;
        if (!tmptr)
        {
            return FALSE;
        }
        //typedef struct _PARTITION_INFORMATION_EX {
        //    PARTITION_STYLE PartitionStyle;
        //    LARGE_INTEGER   StartingOffset;
        //    LARGE_INTEGER   PartitionLength;
        //    DWORD           PartitionNumber;
        //    BOOLEAN         RewritePartition;
        //    BOOLEAN         IsServicePartition;
        //    union {
        //        PARTITION_INFORMATION_MBR Mbr;
        //        PARTITION_INFORMATION_GPT Gpt;
        //    } DUMMYUNIONNAME;
        //} PARTITION_INFORMATION_EX, * PPARTITION_INFORMATION_EX;
        if (!tmptr->PartitionLength.HighPart && !tmptr->PartitionLength.LowPart)
        {
            //wprintf(L"\n**IOCTL_DISK_GET_PARTITION_INFO_EX** Device %s. Partition length ZERO.\n", (LPWSTR)this->activedevicepath);
            return FALSE;
        }
        wprintf(L"\n**IOCTL_DISK_GET_PARTITION_INFO_EX** Device %s, Partition style: %d\n", (LPWSTR)this->ppathbuf, tmptr->PartitionStyle);
        wprintf(L"Start offs   = %016I64XX (%I64d)\n", tmptr->StartingOffset, tmptr->StartingOffset);
        wprintf(L"Part length  = %016I64XX (%I64d)\n", tmptr->PartitionLength, tmptr->PartitionLength);
        wprintf(L"Part Number  = %ld\n", (ULONG)tmptr->PartitionNumber);
        printf("IsServicePartition %d\n", tmptr->IsServicePartition);
        printf("RewritePartition %d\n", tmptr->RewritePartition);

        unsigned long long myval64 = tmptr->PartitionLength.HighPart;
        myval64 <<= 32;
        myval64 += tmptr->PartitionLength.LowPart;
        PARTITION_INFORMATION_MBR* p_pinfo_mbr = &tmptr->Mbr;
        PARTITION_INFORMATION_GPT* p_pinfo_gpt = &tmptr->Gpt;

        if (returninfo)
        {
            P_MG_PARTITIONINFO ppartinfo = &returninfo->diskpartitions[tmptr->PartitionNumber];
            ppartinfo->PartitionStyle = tmptr->PartitionStyle;
            ppartinfo->PartitionNumber = tmptr->PartitionNumber;
            ppartinfo->StartingOffset = tmptr->StartingOffset.HighPart;
            ppartinfo->StartingOffset <<= 32;
            ppartinfo->StartingOffset += tmptr->StartingOffset.LowPart;
            ppartinfo->PartitionLength = tmptr->PartitionLength.HighPart;
            ppartinfo->PartitionLength <<= 32;
            ppartinfo->PartitionLength += tmptr->PartitionLength.LowPart;
            ppartinfo->IsServicePartition = tmptr->IsServicePartition;
            ppartinfo->RewritePartition = tmptr->RewritePartition;
        }
        BOOL pinfoxx = partition_decode(tmptr, returninfo, tmptr->PartitionStyle, 1);

        switch (tmptr->PartitionStyle)
        {
        case PARTITION_STYLE_MBR: // = 0,
            //typedef struct _PARTITION_INFORMATION_MBR {
            //    BYTE    PartitionType;
            //    BOOLEAN BootIndicator;
            //    BOOLEAN RecognizedPartition;
            //    DWORD   HiddenSectors;
            //    GUID    PartitionId;
            //} PARTITION_INFORMATION_MBR, * PPARTITION_INFORMATION_MBR
            wprintf(L"MBR P Type = %d\n", p_pinfo_mbr->PartitionType);
            wprintf(L"Boot indicator   = %d\n", p_pinfo_mbr->BootIndicator);
            wprintf(L"Recognized  = %d\n", p_pinfo_mbr->RecognizedPartition);
            wprintf(L"Hidden sect  = %0lX\n", (ULONG)p_pinfo_mbr->HiddenSectors);
            wprintf(L"GUID id  = %0lX\n", (GUID)p_pinfo_mbr->PartitionId);
            break;
        case PARTITION_STYLE_GPT: // = 1,
            //typedef struct _PARTITION_INFORMATION_GPT {
            //    GUID    PartitionType;
            //    GUID    PartitionId;
            //    DWORD64 Attributes;
            //    WCHAR   Name[36];
            //} PARTITION_INFORMATION_GPT, * PPARTITION_INFORMATION_GPT;
            wprintf(L"GPT Partition Type GUID = %lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n", p_pinfo_gpt->PartitionType.Data1, p_pinfo_gpt->PartitionType.Data2, p_pinfo_gpt->PartitionType.Data3,
                p_pinfo_gpt->PartitionType.Data4[0], p_pinfo_gpt->PartitionType.Data4[1], p_pinfo_gpt->PartitionType.Data4[2], p_pinfo_gpt->PartitionType.Data4[3],
                p_pinfo_gpt->PartitionType.Data4[4], p_pinfo_gpt->PartitionType.Data4[5], p_pinfo_gpt->PartitionType.Data4[6], p_pinfo_gpt->PartitionType.Data4[7]);
            wprintf(L"GPT Partition ID GUID = %lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n", p_pinfo_gpt->PartitionId.Data1, p_pinfo_gpt->PartitionId.Data2, p_pinfo_gpt->PartitionId.Data3,
                p_pinfo_gpt->PartitionId.Data4[0], p_pinfo_gpt->PartitionId.Data4[1], p_pinfo_gpt->PartitionId.Data4[2], p_pinfo_gpt->PartitionId.Data4[3],
                p_pinfo_gpt->PartitionId.Data4[4], p_pinfo_gpt->PartitionId.Data4[5], p_pinfo_gpt->PartitionId.Data4[6], p_pinfo_gpt->PartitionId.Data4[7]);

            wprintf(L"Boot indicator   = %lx\n", p_pinfo_gpt->PartitionId);
            wprintf(L"Attributes  =  %016I64X\n", p_pinfo_gpt->Attributes);
            wprintf(L"Name = %ws\n", (ULONG)p_pinfo_gpt->Name);
            break;

        case PARTITION_STYLE_RAW: // = 2
            wprintf(L"RAW Partition style not decoded\n");
        default:
            break;
        }
        return TRUE;
    }
};

//extern "C" MGSTORAGEDLL_API mg_deviceioctl* mg_deviceioctl_create(HANDLE a, DWORD b) {
//    return new mg_deviceioctl(a, b);
//}

//IOCTL_VOLUME_LOGICAL_TO_PHYSICAL 
//typedef struct _VOLUME_PHYSICAL_OFFSETS {
//    ULONG                  NumberOfPhysicalOffsets;
//    VOLUME_PHYSICAL_OFFSET PhysicalOffset[ANYSIZE_ARRAY];
//} VOLUME_PHYSICAL_OFFSETS, * PVOLUME_PHYSICAL_OFFSETS;

//IOCTL_VOLUME_PHYSICAL_TO_LOGICAL
//typedef struct _VOLUME_PHYSICAL_OFFSET {
//   ULONG    DiskNumber;
//    LONGLONG Offset;
//} VOLUME_PHYSICAL_OFFSET, * PVOLUME_PHYSICAL_OFFSET;

//IOCTL_DISK_GET_LENGTH_INFO  
//typedef struct _GET_LENGTH_INFORMATION {
//    LARGE_INTEGER Length;
//} GET_LENGTH_INFORMATION, * PGET_LENGTH_INFORMATION;

//IOCTL_STORAGE_GET_MEDIA_TYPES 
//typedef struct _DISK_GEOMETRY {
//LARGE_INTEGER Cylinders;
//MEDIA_TYPE    MediaType;
//DWORD         TracksPerCylinder;
//DWORD         SectorsPerTrack;
//DWORD         BytesPerSector;
//} DISK_GEOMETRY, * PDISK_GEOMETRY;
extern class mgdiskinfo(None);


#include "mgjsonreader.h"
#include <winioctl.h>

// Disk Information  
// Disk Information  
class MGSTORAGEDLL_API mg_diskoperations {

    // 
    char m_mystr[128]{ "hello\0" };
    TCHAR m_mytempfilename[sizeof(WCHAR) * MAX_PATH];
    enum m_diskfields { DP_ID, DP_Type, DP_Status, DP_Path };
    const char* m_disktags[19] = {
        (const char*)"Disk ID",
        (const char*)"Type",
        (const char*)"Status",
        (const char*)"Path",
        (const char*)"Target",
        (const char*)"LUN ID",
        (const char*)"Location Path",
        (const char*)"Current Read-only State",
        (const char*)"Read-only",
        (const char*)"Boot Disk",
        (const char*)"Pagefile Disk",
        (const char*)"Hibernation File Disk",
        (const char*)"Crashdump Disk",
        (const char*)"Clustered Disk",
        (const char*)"Volume 1",
        (const char*)"Volume 2",
        (const char*)"Volume 3",
        (const char*)"Volume 4",
        (const char*)"  Volume",
    };
    /*         Disk 1 is now the selected disk. WDC WDS256G1X0C - 00ENX0
    "Disk ID", //{12F1EA0E - 2CB7 - 4A7E - 862C - BB1E7770212E}
    "Type",  //NVMe
    "Status", // : Online
    "Path", // : 0
    "Target", // 0
    "LUN ID", //  : 0
    "Location Path", //  : PCIROOT(0)#PCI(1D00)#PCI(0000)#NVME(P00T00L00)
    "Current Read - only State", //  : No
    "Read - only", //  : No
    "Boot Disk", //  : Yes
    "Pagefile Disk", //  : Yes
    "Hibernation File Disk", //  : No
    "Crashdump Disk", //  : Yes
    "Clustered Disk", //  : No  // Volume ###  Ltr  Label        Fs     Type        Size     Status     Info
    "Volume", //  3     C   Windows      NTFS   Partition    193 GB  Healthy    Boot
    "Volume", //  4     E   NUC - E        NTFS   Partition     41 GB  Healthy
    "Volume", //  5     Q   NUC - Q        FAT32  Partition   3000 MB  Healthy
    "Volume", //  6         SYSTEM       FAT32  Partition    100 MB  Healthy    System
    */
public:
    mg_diskoperations()
    {
        get_tempfilename(NULL); //sets the instance var for my_tempfilename
    }
public:
    int get_str(char* c128buff) {
        memcpy(c128buff, m_mystr, strlen(m_mystr) + 1);
        return 0;
    }
public:
    int set_str(char* p) {
        memcpy(m_mystr, p, strlen(p) + 1);
        return 0;
    }

private:
    char *tmpnametochar(void) {
        size_t iresult;
        char chbuff[MAX_PATH];  
        wcstombs_s( &iresult, chbuff, (const wchar_t *) m_mytempfilename, wcslen(m_mytempfilename) + 1);
        return chbuff;
    }
public:
    unsigned diskpart(int diskno) //, TCHAR* tmpfileptr)
    {
        char tmpsysstring[(MAX_PATH + 14) * sizeof(TCHAR)];
        //sprintf_s(tmpsysstring, (MAX_PATH + 14) * sizeof(TCHAR), "diskpart -s %s", (TCHAR*)m_mytempfilename);
        //int result = system(tmpsysstring);
        int i;
        size_t retval;
        printf("*** DISKPART info Disk <%d> ***\n",diskno);
        char   tmpcmdstr[MAX_PATH], psBuffer[2048], psBuffer1[MAX_PATH + 20]; // MAXPATH];
        FILE* pPipe;
        char  tmpfcontent1[128];
        char tmpfcontent2[128];
        char* tmpptrs[2] = { tmpfcontent1, tmpfcontent2 };
        sprintf_s(tmpfcontent1, "select disk %d\n", diskno);
        sprintf_s(tmpfcontent2, "detail disk\n");

        writetempfile(diskno, (const char **) tmpptrs, 2);
        //const char* tmpfcontent[2] = { (const char*)"select disk 1\n", (const char*)"detail disk\n" };

        //myinfo.writetempfile(0, tmpfcontent, 2);

        //int tmpfileflg = tmpfilewrite(i, args, ptmpf);// argv);
        //_tprintf(TEXT("tmpfname %s\n"), ptmpf);
        //string s = str(format("%2% %2% %1%\n") % "world" % "hello");
        wcstombs_s(&retval, tmpcmdstr,(size_t) MAX_PATH, m_mytempfilename, wcslen((const wchar_t*)m_mytempfilename));
        printf("%s\n", tmpcmdstr);
        i = snprintf(psBuffer1, MAX_PATH + 20, "diskpart /s %s", tmpcmdstr);
        printf("%s\n", psBuffer1);
        if ((pPipe = _popen(psBuffer1, "rt")) == NULL)
            exit(1);
        int tagix;
        while (fgets(psBuffer, 128, pPipe)) {
            //printf(psBuffer);

            for (tagix = 0; tagix < 19; tagix++) {
                if (0 == strncmp(m_disktags[tagix], psBuffer, strlen(m_disktags[tagix]))) {
                    printf("%s:  <%*.*s>\n", m_disktags[tagix], strlen(psBuffer) - 1, strlen(psBuffer) - 1, psBuffer);
                    break;
                }
            }
        }
        if (feof(pPipe))
            printf("\nProcess returned %d\n", _pclose(pPipe));

    }
public:
    // scan the smart information
    unsigned MGSMARTscan(int diskno=0)
    {
        char   psBuffer[128];
        FILE* pPipe;

        if ((pPipe = _popen("..\\x64\\release\\mgsmartctl --scan", "rt")) == NULL)
            exit(1);
        while (fgets(psBuffer, 128, pPipe)) {
            printf(psBuffer);
        }
        if (feof(pPipe))
            printf("\nProcess returned %d\n", _pclose(pPipe));

        char drivechar = 'a' + diskno;
        printf("*** SMART INFO DISK  /dev/sd<%c> ***\n", drivechar);

        //const std::regex e(".*(SMART).*(A.*)\\s+.*");
        //const std::regex e(".*([0-9]*,){3}.*[[]{1}(.*)[]]{1}.*");
        //const std::regex e(".*(SMART).*(A.*)\\s+.*");
        //const std::regex e(".*([0-9]*,){3}.*[[]{1}(.*)[]]{1}.*");
        const std::regex e(".*([0-9]*,){3}.*");
        //SMART attributes data section
        const std::regex e1("\\s*([0-9]+)\\s+(\\S+)\\s+[-]{6}\\s+([0-9]+\\s+){3}\\S+\\s+(.*)");
        // SMART device statistics summary 
        const std::regex e2("\\s*(0x[0-9A-Z]+)\\s+(0x[0-9A-Z]+)\\s+[0-9]*\\s+([0-9]*)\\s+[-]{3}\\s+(.*)");
        // SMART device info summary
        const std::regex e3("\\s*(..*Temperature.*|Power on hours|.*Percentage.*|.*Model.*|PCI Vendor ID|.*LBA Size|Serial Number|.*Capacity|Model Number|Firmware Version|.*Spare.*|Data Units Read|Power Cycles)(.*)\\s*:\\s*(.*)");
        std::cmatch m;
        //mgjsonscanner mgj ;

        char cmdbuf[128];
        sprintf_s(cmdbuf, "..\\x64\\release\\mgsmartctl.exe -x /dev/sd%c", drivechar);
        if ((pPipe = _popen(cmdbuf, "rt")) == NULL)
            exit(1);
        while (fgets(psBuffer, 128, pPipe)) {
            // let json scanner output printf(psBuffer);
            //i = mgj.mgjsonscanline(psBuffer);
            if (std::regex_search(psBuffer, m, e2)) {
                std::cout << "" << m.str(1) << "";
                std::cout << ":" << m.str(2) << ". ";
                std::cout << "Val = " << m.str(3) << ", ";
                std::cout << "Name = " << m.str(4) << '\n';
                //std::cout << "E = " << m.str(5) << '\n';
                std::cout << '\n';
            }
            if (std::regex_search(psBuffer, m, e1)) {
                std::cout << "" << m.str(1) << ". ";
                //std::cout << "c = " << m.str(3) << '\n';
                std::cout << "Val = " << m.str(4) << ", ";
                std::cout << "Name =" << m.str(2) << '\n';

                //std::cout << "E = " << m.str(5) << '\n';
                std::cout << '\n';
            }
            if (std::regex_search(psBuffer, m, e3)) {

                //std::cout << "e3 a = " << m.str(1) << '\n';
                std::cout << "Info item: " << m.str(1) << " <";
                //std::cout << " " << m.str(2) << ' ';
                std::cout << "" << m.str(3) << ">\n";
                //std::cout << "E = " << m.str(5) << '\n';
                std::cout << '\n';
            }
        }
        if (feof(pPipe))
            printf("\nProcess returned %d\n", _pclose(pPipe));
        return 0;
    }

public:
    TCHAR *get_tempfilename(char* c128buff) {
        HANDLE hFile = INVALID_HANDLE_VALUE;
        HANDLE hTempFile = INVALID_HANDLE_VALUE;
        BOOL fDoMove = FALSE;
        BOOL fSuccess = FALSE;
        DWORD dwRetVal = 0;
        UINT uRetVal = 0;

        DWORD dwBytesRead = 0;
        DWORD dwBytesWritten = 0;

        TCHAR szTempFileName[MAX_PATH];
        TCHAR lpTempPathBuffer[MAX_PATH];
#define BUFSIZE 1024
        char  chBuffer[BUFSIZE];


        //LPCTSTR errMsg;

        const char* mystr = "tempfilename0";

        //  Gets the temp path env string (no guarantee it's a valid path).
        dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
            lpTempPathBuffer); // buffer for path 
        if (dwRetVal > MAX_PATH || (dwRetVal == 0))
        {
            PrintError(TEXT("GetTempPath failed"));
            if (!CloseHandle(hFile))
            {
                PrintError(TEXT("CloseHandle(hFile) failed"));
                return (NULL);
            }
            return NULL;
        }

        //  Generates a temporary file name. 
        uRetVal = GetTempFileName(lpTempPathBuffer, // directory for tmp files
            TEXT("DEMO"),     // temp file name prefix 
            0,                // create unique name 
            szTempFileName);  // buffer for name 
        if (uRetVal == 0)
        {
            PrintError(TEXT("GetTempFileName failed"));
            if (!CloseHandle(hFile))
            {
                PrintError(TEXT("CloseHandle(hFile) failed"));
                return (NULL);
            }
            return (NULL);
        }
        size_t iresult;
        wcsncpy_s(m_mytempfilename, 256, (const wchar_t*)szTempFileName, wcslen(szTempFileName));
        _tprintf(TEXT("Tempfilename %s \n"), m_mytempfilename);// argv[1]);
        //if (c128buff) {
        //    memcpy(c128buff, m_mytempfilename, (strlen(m_mytempfilename) + 1));// *sizeof(TCHAR));
        //}
        return szTempFileName;
    }

    int writetempfile(int diskno, const char** pstrings, int nstrs) {
        HANDLE hFile = INVALID_HANDLE_VALUE;
        HANDLE hTempFile = INVALID_HANDLE_VALUE;
        BOOL fDoMove = FALSE;
        BOOL fSuccess = FALSE;
        DWORD dwRetVal = 0;
        UINT uRetVal = 0;

        DWORD dwBytesRead = 0;
        DWORD dwBytesWritten = 0;
        //(LPTSTR)tmpnametochar()
        //  Creates the new file to write to for the upper-case version.
        hTempFile = CreateFile(m_mytempfilename, // file name 
            GENERIC_WRITE,        // open for write 
            0,                    // do not share 
            NULL,                 // default security 
            CREATE_ALWAYS,        // overwrite existing
            FILE_ATTRIBUTE_NORMAL,// normal file 
            NULL);                // no template 
        if (hTempFile == INVALID_HANDLE_VALUE)
        {
            PrintError(TEXT("Second CreateFile failed"));
            if (!CloseHandle(hFile))
            {
                PrintError(TEXT("CloseHandle(hFile) failed"));
                return (NULL);
            }
            return (NULL);
        }
        const char* chBuffer2 = "select disk %d";
        char cmdbuf[128];
        sprintf_s(cmdbuf, chBuffer2, diskno);
        dwBytesRead = 13;
        for (int i = 0; i < nstrs; i++) {
            dwBytesRead = strlen(pstrings[i]);
            fSuccess = WriteFile(hTempFile,
                pstrings[i],
                dwBytesRead,
                &dwBytesWritten,
                NULL);
            if (!fSuccess)
            {
                PrintError(TEXT("WriteFile failed"));
                return (NULL);
            }
        }

        //  The handles to the files are no longer needed, so
        //  they are closed prior to moving the new file.
        if (!CloseHandle(hFile))
        {
            PrintError(TEXT("CloseHandle(hFile) failed"));
            return (NULL);
        }

        if (!CloseHandle(hTempFile))
        {
            PrintError(TEXT("CloseHandle(hTempFile) failed"));
            return (NULL);
        }

        if (fDoMove)
        {

            //  Moves the temporary file to the new text file, allowing for differnt
            //  drive letters or volume names.
            fSuccess = MoveFileEx((LPCWSTR)m_mytempfilename,
                TEXT("AllCaps.txt"),
                MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
            if (!fSuccess)
            {
                PrintError(TEXT("MoveFileEx failed"));
                return NULL;
            }
            else
                _tprintf(TEXT("All-caps version of %s written to AllCaps.txt\n"), TEXT("XXX"));// argv[1]);
            return NULL;
        }
        return (0);
    }
private:
    void PrintError(LPCTSTR errDesc)
    {
        LPCTSTR errMsg = ErrorMessage(GetLastError());
        _tprintf(TEXT("\n** ERROR ** %s: %s\n"), errDesc, errMsg);
        LocalFree((LPVOID)errMsg);
    }
    //void printing();
    //void printing(int a);
//  ErrorMessage support function.
//  Retrieves the system error message for the GetLastError() code.
//  Note: caller must use LocalFree() on the returned LPCTSTR buffer.
    LPCTSTR ErrorMessage(DWORD error)
    {
        LPVOID lpMsgBuf;

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0,
            NULL);

        return((LPCTSTR)lpMsgBuf);
    }
};


class mg_volumeitem {
    BOOL validvolume = FALSE;
    CHAR  RootPathName[1024];
    DWORD SectorsPerCluster;
    DWORD BytesPerSector;
    DWORD NumberOfFreeClusters;
    DWORD TotalNumberOfClusters;
    long long extentsinfo[6];
    MG_DISKINFO diskinfo;
    P_MG_PARTITIONINFO partitioninfo;

public:
    mg_volumeitem(char volroot) {
        mg_partitioninfo* mypartitioninfo = new mg_partitioninfo(volroot); // A (LPWSTR)L"\\\\.\\G:");
        //char volroot = 'A' + j;
        if (!mypartitioninfo->mg_is_partition_info_valid())
        {
            return;
        }
        this->partitioninfo = (P_MG_PARTITIONINFO) new BYTE[MAXPARTITIONCOUNT * sizeof(MG_PARTITIONINFO)];

        this->diskinfo.diskpartitions = this->partitioninfo;
        mypartitioninfo->mg_partitionextractdata(&this->diskinfo);
        P_MG_PARTITIONINFO p0 = partitioninfo;
        P_MG_PARTITIONINFO p1 = &partitioninfo[1];
        P_MG_PARTITIONINFO p2 = &partitioninfo[2];
        P_MG_PARTITIONINFO p3 = &partitioninfo[3];
        P_MG_PARTITIONINFO p4 = &partitioninfo[4];

        LPWSTR buf = mypartitioninfo->mg_getpartitionvolumepath();
        //LPCSTR buf1 = (LPCSTR) strrchr((char *)buf, '\\');
        char buf1[] = { volroot, ':','\\', '\0' };
        BOOL freeresult = GetDiskFreeSpaceA(
            (LPCSTR)buf1,
            &SectorsPerCluster,
            &BytesPerSector,
            &NumberOfFreeClusters,
            &TotalNumberOfClusters);
        if (!freeresult)
        {
            return;
        }
        long long freespace = NumberOfFreeClusters;
        freespace *= BytesPerSector * SectorsPerCluster;
        long long totalspace = TotalNumberOfClusters;
        totalspace *= BytesPerSector * SectorsPerCluster;

        printf("Freespace: %lld, UsedSpace: %lld, Totalspace: %lld, ",
            freespace,
            totalspace - freespace,
            totalspace);
        printf("free: spc %ld,\n bps %ld,\n BPC %ld,\n freeclusters %ld,\n totalclusters %ld \n",
            SectorsPerCluster,
            BytesPerSector,
            SectorsPerCluster * BytesPerSector,
            NumberOfFreeClusters,
            TotalNumberOfClusters);

        // Volume Info Request
        char VolumeNameBuffer[MAX_PATH + 1];
        DWORD   nVolumeNameSize = MAX_PATH + 1;
        DWORD VolumeSerialNumber;
        DWORD MaximumComponentLength;
        DWORD FileSystemFlags;
        char    lpFileSystemNameBuffer[MAX_PATH + 1];
        DWORD   nFileSystemNameSize = MAX_PATH + 1;
        BOOL viresult = GetVolumeInformationA(
            (LPCSTR)buf1,
            (LPSTR)VolumeNameBuffer,
            nVolumeNameSize,
            &VolumeSerialNumber,
            &MaximumComponentLength,
            &FileSystemFlags,
            (LPSTR)lpFileSystemNameBuffer,
            nFileSystemNameSize
        );
        if (!viresult)
        {
            return;
        }
        printf("VolumeInfo: name(%s),\n filesys(%s),\n serial#(%0lxH)\n",
            VolumeNameBuffer,
            lpFileSystemNameBuffer,
            VolumeSerialNumber);
        mg_diskextents* mydiskextents = new mg_diskextents(buf); //(LPWSTR)L"\\\\.\\\\C:");
        mydiskextents->mg_extentsextractdata(extentsinfo);
        printf("extents: nextents: %lld, disk #: %lld, start: %lld, size: %lld\n",
            extentsinfo[0],
            extentsinfo[1],
            extentsinfo[2],
            extentsinfo[3]);
        this->validvolume = TRUE;
    }
};


mg_diskitem::mg_diskitem(int disknumber) {
    this->mydiskinfo.mgsupportedmedia = FALSE; // just to initialize it
    this->mydiskinfo.diskpartitioncount = 0;
    mg_diskgeometry* mygeometry = new mg_diskgeometry(disknumber);
    mygeometry->mg_geometryextractdata(&this->mydiskinfo, TRUE);
    if (!this->mydiskinfo.mgsupportedmedia)
    {
        return;
    }
    this->mypartitions = (P_MG_PARTITIONINFO) new BYTE[MAXPARTITIONCOUNT * sizeof(MG_PARTITIONINFO)];
    this->mydiskinfo.diskpartitions  = this->mypartitions;

    // retrieve whole disk partitioning scheme MBR or GPT data as partition #0
    mg_partitioninfo* mydiskpartitioninfo = new mg_partitioninfo(disknumber);
    mydiskpartitioninfo->mg_partitionextractdata(&this->mydiskinfo);

    // retrieve the partition table entries partition #1...N
    mg_disklayout* mydisklayoutinfo = new mg_disklayout(disknumber);
    mydisklayoutinfo->mg_layoutextractdata(&this->mydiskinfo, TRUE);
}

extern "C" MGSTORAGEDLL_API class mg_systemdisks {
    /* 
    * from answers.microsoft.com: 
    * Each hard drive requires one drive letter per partition, and there are 24 drive letters (C-Z) available.  
    * So, in theory, you can have up to 24 hard drives, if each one has only one partition.
    * (my comment: in filesystem context it's true, but a drive is r/w as a physical device without a mounted filesystem volume
    *  so I'll use 24 but I don't consider that a technically complete answer)
    */
    #define MAXDEVICES 24
    mg_volumeitem* logicalvolumes[26] ;
    P_MG_DISKINFO pdiskinfo[MAXDEVICES];
    unsigned int systemdiskbitflags;


public:
    mg_systemdisks(int i=-1)
    {
        LPWSTR buf;
        int j;
        CHAR  RootPathName[1024];
        this->systemdiskbitflags = 0;
        if (i == -1)
        {
            unsigned int maskbitvar=1;
            for (j = 0; j < MAXDEVICES; j++, maskbitvar <<=1)
            {
                pdiskinfo[j] = NULL;
                class mg_diskitem *ptmp = new mg_diskitem(j);
                if (ptmp->mydiskinfo.disksize)
                {
                    pdiskinfo[j] = (P_MG_DISKINFO) new BYTE[sizeof(MG_DISKINFO)];
                    memcpy((void*)pdiskinfo[j], (void*)&ptmp->mydiskinfo, sizeof(MG_DISKINFO));
                    this->systemdiskbitflags |= maskbitvar;
                }
            }
        }
        if (TRUE)
        {
           //diskitems
        }
        DWORD rslt = GetLogicalDrives();// (nBufferLength, (LPWSTR)logdrives);
        j = 0;
        for (long i = 0x1; j < 26; i = i << 1, j++)
        {
            if (i & rslt)
            {
                printf("volume %lx, %c\n", i, ('A' + j));
                mg_volumeitem* ptmpv = new mg_volumeitem('A'+j);
                mg_partitioninfo* mypartitioninfo = new mg_partitioninfo('A'+j); // A (LPWSTR)L"\\\\.\\G:");
                char volroot = 'A' + j;
                if (!mypartitioninfo->mg_is_partition_info_valid())
                {
                    //break;
                }
            }
        }
    }
    DWORD mg_printdisks(void)
    {
        printf("\ndisks %d\n", 0);
        return 0;
    }
};

