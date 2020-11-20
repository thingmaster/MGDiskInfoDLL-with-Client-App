#pragma once
// MGSTORAGELIBrary.h - Contains declarations of math functions
#pragma once

#ifdef MGDISKINFOMATIONDLL_EXPORTS
#define MGSTORAGEDLL_API __declspec(dllexport)
#else
#define MGSTORAGEDLL_API __declspec(dllimport)
#endif

// The Fibonacci recurrence relation describes a sequence F
// where F(n) is { n = 0, a
//               { n = 1, b
//               { n > 1, F(n-2) + F(n-1)
// for some initial integral values a and b.
// If the sequence is initialized F(0) = 1, F(1) = 1,
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
extern "C" MGSTORAGEDLL_API BOOL W32_Write(HANDLE fhnd, LPCWSTR physpath, LPCVOID wbytedata, DWORD writecount, int offs);
extern "C" MGSTORAGEDLL_API BOOL W32_lock_volume(HANDLE drivehandle);
extern "C" MGSTORAGEDLL_API BOOL W32_dismount_volume(LPCWSTR volpath );
extern "C" MGSTORAGEDLL_API int diskgeometry(LPWSTR physicaldevpath);
extern "C" MGSTORAGEDLL_API int disklayout(LPWSTR physicaldevpath);
extern "C" MGSTORAGEDLL_API int getvolumediskinfo(LPWSTR wsvolpath);


extern class mgdiskinfo(None);


#include "mgjsonreader.h"
#include <winioctl.h>

// Disk Information  
// Disk Information  
class MGSTORAGEDLL_API mg_diskoperations {

    // ...
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

        if ((pPipe = _popen("smartctl --scan", "rt")) == NULL)
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
        sprintf_s(cmdbuf, "smartctl.exe -x /dev/sd%c", drivechar);
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
    int get_partitioninfo(int diskno = 0) {
        HANDLE hDevice = 0;
        int stat;
        LPVOID lpOutBuffer = NULL;
        DWORD nOutBufferSize = 0;
        LPDWORD lpBytesReturned = NULL;
        LPOVERLAPPED lpOverlapped = NULL;
        PARTITION_INFORMATION_EX pdg;
        // open device //physicaldisk<n>
        //hDevice = CreateFile(TEXT("\\\\.\\C:"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |
        //        FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        LPCWSTR diskpath[6] = { L"\\\\.\\PhysicalDrive0",
            L"\\\\.\\PhysicalDrive1",
            L"\\\\.\\PhysicalDrive2",
            L"\\\\.\\PhysicalDrive3",
            L"\\\\.\\PhysicalDrive4",
            L"\\\\.\\PhysicalDrive5" };
        printf("*** DISK PARTITION INFO DISK <%d> ***\n", diskno);
        if (diskno >= sizeof(diskpath)/sizeof(LPCWSTR)) {
            return -1;
        }
        hDevice = CreateFile(diskpath[diskno], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
        if (hDevice) {
            //W32_lock_volume(handle)
        }
        if (hDevice <= 0) {
            return 0;
        }
        stat = DeviceIoControl(
            (HANDLE)hDevice,                 // handle to a partition
            IOCTL_DISK_GET_PARTITION_INFO_EX, // dwIoControlCode
            NULL,                             // lpInBuffer
            0,                                // nInBufferSize
            (LPVOID)&pdg,             // output buffer
            (DWORD)sizeof(pdg),           // size of output buffer
            (LPDWORD)lpBytesReturned,        // number of bytes returned
            (LPOVERLAPPED)lpOverlapped);      // OVERLAPPED structure 

        unsigned long long myval64 = pdg.PartitionLength.HighPart;
        myval64 <<= 32;
        myval64 += pdg.PartitionLength.LowPart;

        if (stat == 1) {
            printf("GetParitionInfo_EX IOCTL: Disk %d; Parition type: GPT:%d, Len: %llu: Partition #: %d \n", diskno, pdg.PartitionStyle, myval64, pdg.PartitionNumber);
        }

        // close the device(hDevice)
        CloseHandle(hDevice);
        return stat;
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
