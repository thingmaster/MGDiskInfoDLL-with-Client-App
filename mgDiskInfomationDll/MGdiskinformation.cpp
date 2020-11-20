// MGSTORAGE.cpp : Defines the exported functions for the demo MGstorage DLL.
// see the complementary client app MGstoragetest.cpp
//  mg 2020-07-11

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include <cstdio>
#include <iostream>
//#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <regex>
#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
//#include <stdio.h>
#include "MGdiskinformation.h"

// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position


// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
void fibonacci_init(
    const unsigned long long a,
    const unsigned long long b)
{
    index_ = 0;
    current_ = a;
    previous_ = b; // see special case when initialized
}

// Produce the next value in the sequence.
// Returns true on success, false on overflow.
bool fibonacci_next()
{
    // check to see if we'd overflow result or position
    if ((ULLONG_MAX - previous_ < current_) ||
        (UINT_MAX == index_))
    {
        return false;
    }

    // Special case when index == 0, just return b value
    if (index_ > 0)
    {
        // otherwise, calculate next sequence value
        previous_ += current_;
    }
    std::swap(current_, previous_);
    ++index_;
    return true;
}

// Get the current value in the sequence.
unsigned long long fibonacci_current()
{
    return current_;
}

// Get the current index position in the sequence.
unsigned fibonacci_index()
{
    return index_;
}

// scan the smart information
unsigned MGSMARTscan()
{
    int result = system("smartctl.exe  --scan");

    char   psBuffer[128];
    FILE* pPipe;

    if ((pPipe = _popen("smartctl --scan", "rt")) == NULL)
        exit(1);
    while (fgets(psBuffer, 128, pPipe)) {
        printf(psBuffer);
    }
    if (feof(pPipe))
        printf("\nProcess returned %d\n", _pclose(pPipe));

    if ((pPipe = _popen("smartctl -x /dev/sda", "rt")) == NULL)
        exit(1);

    //const std::regex e(".*(SMART).*(A.*)\\s+.*");
    //const std::regex e(".*([0-9]*,){3}.*[[]{1}(.*)[]]{1}.*");
    const std::regex e(".*([0-9]*,){3}.*");
    //SMART attributes data section
    const std::regex e1("\\s*([0-9]+)\\s+(\\S+)\\s+[-]{6}\\s+([0-9]+\\s+){3}\\S+\\s+(.*)");
    // SMART device statistics summary 
    const std::regex e2("\\s*(0x[0-9A-Z]+)\\s+(0x[0-9A-Z]+)\\s+[0-9]*\\s+([0-9]*)\\s+[-]{3}\\s+(.*)");
    // SMART device info summary
    const std::regex e3("\\s*(.*)(Model Family|Device Model|Serial Number|Capacity|Model Number|Firmware Version)(.*)\\s*:\\s*(.*)");
    std::cmatch m;
    while (fgets(psBuffer, 128, pPipe)) {
        printf(psBuffer);

        if (std::regex_search(psBuffer, m, e1)) {
            std::cout << "a = " << m.str(1) << '\n';
            std::cout << "b = " << m.str(2) << '\n';
            std::cout << "c = " << m.str(3) << '\n';
            std::cout << "D = " << m.str(4) << '\n';
            //std::cout << "E = " << m.str(5) << '\n';
            std::cout << '\n';
        }
        if (std::regex_search(psBuffer, m, e2)) {
            std::cout << "a = " << m.str(1) << '\n';
            std::cout << "b = " << m.str(2) << '\n';
            std::cout << "c = " << m.str(3) << '\n';
            std::cout << "D = " << m.str(4) << '\n';
            //std::cout << "E = " << m.str(5) << '\n';
            std::cout << '\n';
        }
        if (std::regex_search(psBuffer, m, e3)) {

            std::cout << "a = " << m.str(1) << '\n';
            std::cout << "b = " << m.str(2) << '\n';
            std::cout << "c = " << m.str(3) << '\n';
            std::cout << "D = " << m.str(4) << '\n';
            //std::cout << "E = " << m.str(5) << '\n';
            std::cout << '\n';
        }
    }

    if (feof(pPipe))
        printf("\nProcess returned %d\n", _pclose(pPipe));
    return 0;
}

// execute a diskpart command
unsigned MGdiskpart(TCHAR * tmpfileptr)
{
    int result = system("smartctl.exe  --scan");
    int i;

    char   psBuffer[2048], psBuffer1[MAX_PATH+20]; // MAXPATH];
    FILE* pPipe;
    //string s = str(format("%2% %2% %1%\n") % "world" % "hello");
    printf("%s", tmpfileptr);
    i = snprintf(psBuffer1, MAX_PATH+20, "diskpart /s %s", tmpfileptr);
    printf("%s", psBuffer1);
    if ((pPipe = _popen(psBuffer1, "rt")) == NULL)
        exit(1);
    while (fgets(psBuffer, 128, pPipe)) {
        printf(psBuffer);
    }
    if (feof(pPipe))
        printf("\nProcess returned %d\n", _pclose(pPipe));

    if ((pPipe = _popen("smartctl -x /dev/sda", "rt")) == NULL)
        exit(1);

    //const std::regex e(".*(SMART).*(A.*)\\s+.*");
    //const std::regex e(".*([0-9]*,){3}.*[[]{1}(.*)[]]{1}.*");
    const std::regex e(".*([0-9]*,){3}.*");
    //SMART attributes data section
    const std::regex e1("\\s*([0-9]+)\\s+(\\S+)\\s+[-]{6}\\s+([0-9]+\\s+){3}\\S+\\s+(.*)");
    // SMART device statistics summary 
    const std::regex e2("\\s*(0x[0-9A-Z]+)\\s+(0x[0-9A-Z]+)\\s+[0-9]*\\s+([0-9]*)\\s+[-]{3}\\s+(.*)");
    // SMART device info summary
    const std::regex e3("\\s*(.*)(Model Family|Device Model|Serial Number|Capacity|Model Number|Firmware Version)(.*)\\s*:\\s*(.*)");
    std::cmatch m;
    while (fgets(psBuffer, 128, pPipe)) {
        printf(psBuffer);

        if (std::regex_search(psBuffer, m, e1)) {
            std::cout << "a = " << m.str(1) << '\n';
            std::cout << "b = " << m.str(2) << '\n';
            std::cout << "c = " << m.str(3) << '\n';
            std::cout << "D = " << m.str(4) << '\n';
            //std::cout << "E = " << m.str(5) << '\n';
            std::cout << '\n';
        }
        if (std::regex_search(psBuffer, m, e2)) {
            std::cout << "a = " << m.str(1) << '\n';
            std::cout << "b = " << m.str(2) << '\n';
            std::cout << "c = " << m.str(3) << '\n';
            std::cout << "D = " << m.str(4) << '\n';
            //std::cout << "E = " << m.str(5) << '\n';
            std::cout << '\n';
        }
        if (std::regex_search(psBuffer, m, e3)) {

            std::cout << "a = " << m.str(1) << '\n';
            std::cout << "b = " << m.str(2) << '\n';
            std::cout << "c = " << m.str(3) << '\n';
            std::cout << "D = " << m.str(4) << '\n';
            //std::cout << "E = " << m.str(5) << '\n';
            std::cout << '\n';
        }
    }

    if (feof(pPipe))
        printf("\nProcess returned %d\n", _pclose(pPipe));
    return 0;
}


int testfunc(void)
{
    HANDLE fhnd = 0, drivehandle = 0;
    BYTE wbytedata[4096] , rbytedata[4096] ;
    DWORD writecount = 0, readsize = 0;
    int offs = 0;
    HANDLE readstat = W32_Read(fhnd, (LPCWSTR)"\\\\1", rbytedata, readsize, offs);
    //BOOL writestat = W32_Write(fhnd, (LPCWSTR)"\\\\1", wbytedata, writecount, offs);
    BOOL lockstat = W32_lock_volume( drivehandle);
    BOOL dismountstat = W32_dismount_volume((LPCWSTR) "F");
    return 0;
}

HANDLE W32_Read(HANDLE fhnd, LPCWSTR physpath, LPVOID rbytebuf, DWORD readsize, LONG offs)
{
    if (fhnd==0)
    {
        fhnd = CreateFile(physpath, // win32file.CreateFile(physpath,
            GENERIC_READ,  //win32file.GENERIC_READ, #access
            FILE_SHARE_READ,    //win32file.FILE_SHARE_READ, #share mode
            0, //# attributes
            OPEN_EXISTING, //win32file.OPEN_EXISTING, #create disposition
            FILE_ATTRIBUTE_NORMAL, // #flags and attributes FILE_ATTRIBUTE_NORMAL 128 (0x80)
            0);
        if (fhnd <= 0)
        {
            return fhnd; //, -1, 0 # couldnot open it
        }
    }

    LONG movedisthigh = 0;
    SetFilePointer(fhnd, offs, &movedisthigh, FILE_BEGIN); //win32file.SetFilePointer(fhnd, offs, win32file.FILE_BEGIN)
    //werr, rbytebuf = win32file.ReadFile(fhnd, readsize, None) #rbytebuf, None) #, overlapped )
    DWORD readcountresult;
    BOOL readstat = FALSE;
    readstat = ReadFile(fhnd, rbytebuf, readsize, &readcountresult, 0); //#, overlapped )
    if (!readstat)
    {
        return (HANDLE) 0;
    }
    return fhnd ; // fhnd, werr, rbytebuf
}
// W32_Write(fhnd, physpath, rbytebuf, readsize, offs) - execute the win32file Win32 API Write function, optionally open a handle
BOOL  W32_Write(HANDLE fhnd, LPCWSTR physpath, LPCVOID wbytedata, DWORD writecount, int offs)
{
    //if fhnd is Null, open the deviceand return handle
    //physpath is a fully constructed Win32 compliant \\physicaldevice0 path for writing
    //wbytedata is a win32 compliant write buffer, size is implied in the buffer size
    //offs is a file byte offset from start of file ONLY USED IF FHND IS NONE!
    //returns handle, errorstatus, bytescount
    //def W32_Write(fhnd, physpath, wbytedata, offs) :
    if (!fhnd )
    {
        fhnd = CreateFile(physpath,  //fhnd = win32file.CreateFile(physpath,
            GENERIC_READ | GENERIC_WRITE, //win32file.GENERIC_READ | win32file.GENERIC_WRITE, #access
            FILE_SHARE_READ | FILE_SHARE_WRITE, //win32file.FILE_SHARE_READ | win32file.FILE_SHARE_WRITE, #share mode
            0, //# attributes
            OPEN_EXISTING, //win32file.OPEN_EXISTING, #create disposition
            FILE_ATTRIBUTE_NORMAL, //  0x80, #FILE_FLAG_BACKUP_SEMANTICS 2 normal 0x80, #flags and attributes FILE_ATTRIBUTE_NORMAL 128 (0x80)
            0);
        if (! fhnd)
        {
            return FALSE ; //0, -1, 0 # couldnot open it
        }
    }

    LONG movedisthigh = 0;
    SetFilePointer(fhnd, offs, &movedisthigh, FILE_BEGIN); //win32file.SetFilePointer(fhnd, offs, win32file.FILE_BEGIN)
    //werr, nbytes = win32file.WriteFile(fhnd, wbytedata)
    DWORD writecountresult;
    BOOL werr = WriteFile(fhnd, wbytedata, writecount, &writecountresult, 0 );
    //#win32file.CloseHandle(fhnd) #win32file.FlushFileBuffers(fhnd)
    //#fhnd = 0
    return FALSE ;  //  fhnd, werr, nbytes
}


BOOL W32_lock_volume(HANDLE drivehandle)
{
    if (drivehandle != 0)
    {
        DWORD  ioctl = FSCTL_LOCK_VOLUME; // winioctl['FSCTL_LOCK_VOLUME']
        BOOL info = DeviceIoControl(drivehandle, ioctl, 0, 0, 0, 0, 0, 0); //#, None) #24)//info = win32file.DeviceIoControl(drivehandle, ioctl, None, None, None)#, None) #24)
        if (!info)
        {
            int lasterr = GetLastError();
            printf("LOCK FAIL GetLasterror() = %d", lasterr);
            return FALSE;
        }
        return TRUE;
    }
    //print('lock_physical_drive(), LOCK IOCTL **FAILED** Check for open files on device??')
    return FALSE;
}

BOOL W32_dismount_volume(LPCWSTR targetvolpath)
{
    HANDLE fhnd = 0;
    fhnd = CreateFileW(targetvolpath,  // fhnd = win32file.CreateFile(targetdev,
        GENERIC_READ | GENERIC_WRITE, // #access
        FILE_SHARE_READ | FILE_SHARE_WRITE, //#share mode
        0, //# sec attributes
        OPEN_EXISTING, //#create disposition
        FILE_ATTRIBUTE_NORMAL, //#flags and attributes FILE_ATTRIBUTE_NORMAL 128 (0x80)
        0);
    if (!fhnd)
    {
        return FALSE;
    }
    //print('Unexpected CreateFile error in unmount_volumes(%s)' % targetdev)

    DWORD ioctl = FSCTL_DISMOUNT_VOLUME; //0x00090020;// winioctl['FSCTL_DISMOUNT_VOLUME']
    BOOL info = DeviceIoControl(fhnd, ioctl, 0, 0, 0, 0, 0, 0);
    // add Handle to the Instance LIST OF HANDLES BEING DISMOUNTED ## fhandle.append(fhnd)
    return info;
}



/*
TCHAR* tmpfilewrite(int argc, const char* argv[], char * fnbuffer )
*/


#define UNICODE 1
#define _UNICODE 1

/* The code of interest is in the subroutine GetDriveGeometry. The
   code in main shows how to interpret the results of the call. */

#define wszDrive L"\\\\.\\PhysicalDrive1"

BOOL GetDriveGeometry(LPWSTR wszPath, DISK_GEOMETRY* pdg)
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to the drive to be examined 
    BOOL bResult = FALSE;                 // results flag
    DWORD junk = 0;                     // discard results

    hDevice = CreateFileW(wszPath,          // drive to open
        0,                // no access to the drive
        FILE_SHARE_READ | // share mode
        FILE_SHARE_WRITE,
        NULL,             // default security attributes
        OPEN_EXISTING,    // disposition
        0,                // file attributes
        NULL);            // do not copy file attributes

    if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
    {
        return (FALSE);
    }

    bResult = DeviceIoControl(hDevice,                       // device to be queried
        IOCTL_DISK_GET_DRIVE_GEOMETRY, // operation to perform
        NULL, 0,                       // no input buffer
        pdg, sizeof(*pdg),            // output buffer
        &junk,                         // # bytes returned
        (LPOVERLAPPED)NULL);          // synchronous I/O

    CloseHandle(hDevice);

    return (bResult);
}

int diskgeometry(LPWSTR physicaldevpath ) //L"\\\\.\\PhysicalDrive1"
{
    DISK_GEOMETRY pdg = { 0 }; // disk drive geometry structure
    BOOL bResult = FALSE;      // generic results flag
    ULONGLONG DiskSize = 0;    // size of the drive, in bytes

    bResult = GetDriveGeometry((LPWSTR) physicaldevpath, (DISK_GEOMETRY *) &pdg);

    if (bResult)
    {
        wprintf(L"\n *** Drive path      = %ws\n", physicaldevpath);
        wprintf(L"Cylinders       = %I64d\n", pdg.Cylinders);
        wprintf(L"Tracks/cylinder = %ld\n", (ULONG)pdg.TracksPerCylinder);
        wprintf(L"Sectors/track   = %ld\n", (ULONG)pdg.SectorsPerTrack);
        wprintf(L"Bytes/sector    = %ld\n", (ULONG)pdg.BytesPerSector);

        DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
            (ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
        wprintf(L"Disk size       = %I64d (Bytes)\n"
            L"                = %.2f (Gb)\n",
            DiskSize, (double)DiskSize / (1024 * 1024 * 1024));
    }
    else
    {
        wprintf(L"GetDriveGeometry failed. Error %ld.\n", GetLastError());
    }

    return ((int)bResult);
}



BOOL GetDriveLayoutInfo(LPWSTR wszPath, DRIVE_LAYOUT_INFORMATION_EX* pdg)
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to the drive to be examined 
    BOOL bResult = FALSE;                 // results flag
    DWORD junk = 0;                     // discard results

    hDevice = CreateFileW(wszPath,          // drive to open
        0,                // no access to the drive
        FILE_SHARE_READ | // share mode
        FILE_SHARE_WRITE,
        NULL,             // default security attributes
        OPEN_EXISTING,    // disposition
        0,                // file attributes
        NULL);            // do not copy file attributes

    if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
    {
        return (FALSE);
    }
    int lpretbytes = 0;
    bResult = DeviceIoControl(hDevice,                       // device to be queried
        IOCTL_DISK_GET_DRIVE_LAYOUT_EX, //IOCTL_DISK_GET_PARTITION_INFO_EX, // operation to perform  IOCTL_DISK_GET_DRIVE_LAYOUT_EX
        NULL, 0,                       // no input buffer
        pdg, 2048, //sizeof(*pdg),            // output buffer
        &junk,                         // # bytes returned
        (LPOVERLAPPED)NULL);          // synchronous I/O

    CloseHandle(hDevice);

    return (bResult);
}


int disklayout(LPWSTR physicaldevpath)
{
    //typedef struct _DRIVE_LAYOUT_INFORMATION_EX {
    //    ULONG                    PartitionStyle;
    //    ULONG                    PartitionCount;
    //    union {
    //        DRIVE_LAYOUT_INFORMATION_MBR Mbr;
    //        DRIVE_LAYOUT_INFORMATION_GPT Gpt;
    //    } DUMMYUNIONNAME;
    //    PARTITION_INFORMATION_EX PartitionEntry[1];
    //} DRIVE_LAYOUT_INFORMATION_EX, * PDRIVE_LAYOUT_INFORMATION_EX; 
    BYTE xferbuffer[2048];
    DRIVE_LAYOUT_INFORMATION_EX* pdg = (DRIVE_LAYOUT_INFORMATION_EX*)xferbuffer; // = { 0 }; // disk drive layout structure

    BOOL bResult = FALSE;      // generic results flag
    ULONGLONG DiskSize = 0;    // size of the drive, in bytes

    bResult = GetDriveLayoutInfo((LPWSTR) physicaldevpath, (DRIVE_LAYOUT_INFORMATION_EX*) xferbuffer );

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
    PARTITION_INFORMATION_MBR* p_pinfo_mbr ;
    PARTITION_INFORMATION_GPT* p_pinfo_gpt ;
    if (bResult)
    {
        wprintf(L"\n *** Drive path      = %ws\n", physicaldevpath);
        //wprintf(L"Cylinders       = %I64d\n", pdg.Cylinders);
        wprintf(L"PartitionStyle = %ld\n", (ULONG)pdg->PartitionStyle);
        wprintf(L"PartitionCount   = %ld\n", (ULONG)pdg->PartitionCount);
        switch (pdg->PartitionStyle)
        {
        case PARTITION_STYLE_MBR: // = 0,
            wprintf(L"Signature = %ld\n", (ULONG)pdli_mbr->Signature);
            wprintf(L"Checksum   = %ld\n", (ULONG)pdli_mbr->CheckSum);
            for (int i = 0; i < pdg->PartitionCount; i++, p_pinfo++ )
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
                wprintf(L"P Style = %ld\n", (ULONG)p_pinfo->PartitionStyle);
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
                wprintf(L"P Type = %02d\n", p_pinfo_mbr->PartitionType);
                wprintf(L"Boot indicator   = %02X\n", p_pinfo_mbr->BootIndicator);
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
            wprintf(L"DiskID = %lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n", pdli_gpt->DiskId.Data1, pdli_gpt->DiskId.Data2, pdli_gpt->DiskId.Data3,
                pdli_gpt->DiskId.Data4[0], pdli_gpt->DiskId.Data4[1], pdli_gpt->DiskId.Data4[2], pdli_gpt->DiskId.Data4[3],
                pdli_gpt->DiskId.Data4[4], pdli_gpt->DiskId.Data4[5], pdli_gpt->DiskId.Data4[6], pdli_gpt->DiskId.Data4[7]);
                // % I64X\n", pdli_gpt->DiskId);
            wprintf(L"MaxPartCount   = %ld\n", (ULONG)pdli_gpt->MaxPartitionCount);
            wprintf(L"Start usable offs= %016I64XX (%I64d)\n", pdli_gpt->StartingUsableOffset, pdli_gpt->StartingUsableOffset);
            wprintf(L"MaxPart= %ld\n", (ULONG)pdli_gpt->MaxPartitionCount);
            for (int i = 0; i < pdg->PartitionCount; i++, p_pinfo++)
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
                wprintf(L"P Style = %ld\n", (ULONG)p_pinfo->PartitionStyle);
                wprintf(L"Start offs   = %016I64XX (%I64d)\n", p_pinfo->StartingOffset, p_pinfo->StartingOffset);
                wprintf(L"Part length  = %016I64XX (%I64d)\n", p_pinfo->PartitionLength, p_pinfo->PartitionLength);
                wprintf(L"Part Number  = %ld\n", (ULONG)p_pinfo->PartitionNumber);
                //typedef struct _PARTITION_INFORMATION_GPT {
                //    GUID    PartitionType;
                //    GUID    PartitionId;
                //    DWORD64 Attributes;
                //    WCHAR   Name[36];
                //} PARTITION_INFORMATION_GPT, * PPARTITION_INFORMATION_GPT;
                wprintf(L"P Type = %lx\n", p_pinfo_gpt->PartitionType);
                wprintf(L"Boot indicator   = %lx\n", p_pinfo_gpt->PartitionId);
                wprintf(L"Attributes  =  %016I64X\n", p_pinfo_gpt->Attributes);
                wprintf(L"Name = %ws\n", (ULONG)p_pinfo_gpt->Name); 
            }
            break;

        case PARTITION_STYLE_RAW: // = 2
        default:
            break;
        }
    }

    return ((int)bResult);
} 



BOOL GetVolumeDiskExtents(LPWSTR wsvolpath, VOLUME_DISK_EXTENTS* pdg)
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to the drive to be examined 
    BOOL bResult = FALSE;                 // results flag
    DWORD junk = 0;                     // discard results

    hDevice = CreateFileW(wsvolpath,          // drive to open
        0,                // no access to the drive
        FILE_SHARE_READ | // share mode
        FILE_SHARE_WRITE,
        NULL,             // default security attributes
        OPEN_EXISTING,    // disposition
        0,                // file attributes
        NULL);            // do not copy file attributes

    DWORD lasterr;
    if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
    {
        if (hDevice<=0)
        {
            lasterr = GetLastError();
        }
        return (FALSE);
    }
    int lpretbytes = 0;
    bResult = DeviceIoControl(hDevice,                       // device to be queried
                IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, // dwIoControlCode
                NULL, 0,                       // no input buffer
                pdg, 2048, //sizeof(*pdg),            // output buffer
                &junk,                         // # bytes returned
                (LPOVERLAPPED)NULL);          // synchronous I/O
    if (!bResult)
    {
        lasterr = GetLastError();
    }
    CloseHandle(hDevice);

    return (bResult);
}

int getvolumediskinfo(LPWSTR wsvolpath)
{
    //typedef struct _VOLUME_DISK_EXTENTS {
    //    DWORD       NumberOfDiskExtents;
    //    DISK_EXTENT Extents[ANYSIZE_ARRAY];
    //} VOLUME_DISK_EXTENTS, * PVOLUME_DISK_EXTENTS;
    //typedef struct _DISK_EXTENT {
    //   DWORD         DiskNumber;
    //    LARGE_INTEGER StartingOffset;
    //    LARGE_INTEGER ExtentLength;
    //} DISK_EXTENT, * PDISK_EXTENT;
    BYTE xferbuffer[2048];
    VOLUME_DISK_EXTENTS* pdg = (VOLUME_DISK_EXTENTS*)xferbuffer; // = { 0 }; // disk drive layout structure

    BOOL bResult = FALSE;      // generic results flag
    ULONGLONG DiskSize = 0;    // size of the drive, in bytes

    bResult = GetVolumeDiskExtents((LPWSTR)wsvolpath, (VOLUME_DISK_EXTENTS*)xferbuffer);

    DISK_EXTENT* p_pinfo = &pdg->Extents[0];
    if (bResult)
    {
        wprintf(L"\n *** Drive path      = %ws\n", wsvolpath);
        //wprintf(L"Cylinders       = %I64d\n", pdg->);
        wprintf(L"N Extents = %ld\n", (ULONG)pdg->NumberOfDiskExtents);
        for (int i = 0; i < pdg->NumberOfDiskExtents; i++, p_pinfo++)
        {
            wprintf(L"P Disk Num = %ld\n", (ULONG)p_pinfo->DiskNumber);
            wprintf(L"Start offs   = %I64d\n", p_pinfo->StartingOffset);
            wprintf(L"Part length  = %I64d\n", p_pinfo->ExtentLength);
        }
    }
    return 0;
}
