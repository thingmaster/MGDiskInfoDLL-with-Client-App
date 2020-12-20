// MGstoragetest.cpp : This file contains the 'main' test program entry point for Client & DLL example
//         example has a command shell _popen() which includes 'p' for pipe of std output to a buffer
//          testing demonstrates executing 'smartctl -x /dev/sdb'  which dumps SATA drive SMART information
//          this has dependency that SMARTMONTOOLS.ORG smartctl utility is present. and execute in admin mode for full info
// 
// MGstoragetest.cpp : Client app for demonstrating MGstorage DLL.
//     see the complementary DLL MGstorage.DLL project which this invokes


// #include "pch.h" Uncomment for Visual Studio 2017 and earlier
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <regex>
#include <windows.h>
#include <tchar.h>
#include "MGdiskinformation.h"
#include "MGclonethreads.h"
//#include <stdio.h>

//#include "MGstorage.h"

extern int SMARTscan(); // the local scan function (not the DLL one)

TCHAR  ptmpf[MAX_PATH];
int tmpfilewrite(int, const char**, TCHAR* lptmpf);

int main()
{
    int i = 2;
    const char** argv;
    const char* args[2];
    args[0] = "select disk 1\n";
    args[1] = "detail disk\n";
    //args[1] =  (const TCHAR *) "ABC";// = { "a",  "b" };
    //args[2] = (const TCHAR *) "BCD";
    printf("args %s", args[0]);
    printf("args %s", args[1]);
    argv = args;
    fibonacci_init( 0L, 1L);

    class mgclonethread* mycloner;
    mycloner = new  mgclonethread();

    printf("SYSTEMDISKS CLASS -------------");
    mg_systemdisks *mysystemdisks = new mg_systemdisks(-1);
    mysystemdisks->mg_printdisks();
    //mg_deviceioctl myioctl ( );
    //printf("%lx", myioctl.mg_printtest());
    if (FALSE)
    {
        printf("Call IOCTL wrapper classes -------------");
        wchar_t buf[128];
        // extract DiskExtent information by-Volume C: - Z:
        for (char i = '0'; i <= '9'; i++)
        {
            swprintf(buf, sizeof(buf) / sizeof(*buf), L"\\\\.\\PhysicalDrive%c", i);
            mg_diskgeometry mggeometry(buf);  // (LPWSTR)L"\\\\.\\PhysicalDrive1");
            mggeometry.mg_geometryextractdata();
            mg_disklayout mgdisklayout(buf);// (LPWSTR)L"\\\\.\\PhysicalDrive0");
            mgdisklayout.mg_layoutextractdata();
            mg_partitioninfo  mgpartitioninfo(buf);// (LPWSTR)L"\\\\.\\PhysicalDrive0");
            mgpartitioninfo.mg_partitionextractdata();
        }
        //mg_disklayout mgdisklayout((LPWSTR)L"\\\\.\\PhysicalDrive0");
        //mgdisklayout.mg_layoutextractdata();
        //mg_disklayout mgdisklayout1((LPWSTR)L"\\\\.\\PhysicalDrive1");
        //mgdisklayout1.mg_layoutextractdata();
        //mg_partitioninfo  mgpartitioninfo((LPWSTR)L"\\\\.\\PhysicalDrive0");
        //mgpartitioninfo.mg_partitionextractdata();
        //mg_partitioninfo  mgpartitioninfo1((LPWSTR)L"\\\\.\\PhysicalDrive1");
        //mgpartitioninfo1.mg_partitionextractdata();


        // extract DiskExtent information by-Volume C: - Z:
        for (char i = 'C'; i <= 'Z'; i++)
        {
            swprintf(buf, sizeof(buf) / sizeof(*buf), L"\\\\.\\%c:", i);
            mg_diskextents    mgdiskextents(buf); //(LPWSTR)L"\\\\.\\\\C:");
            mgdiskextents.mg_extentsextractdata();
            mg_partitioninfo  mgpartitioninfoF(buf); // (LPWSTR)L"\\\\.\\G:");
            mgpartitioninfoF.mg_partitionextractdata();
        }
    }

    printf("EXECUTE SOME READ IO & LOCK VOLUME  -------------");
    // read some chunks of D: at 1,000,000 offset increments
    BYTE rbytebuf[2048];
    HANDLE fhnd = W32_Read((HANDLE)0, (LPWSTR)L"\\\\.\\\\D:", rbytebuf, 2048, 0);
    BOOL lockstat = W32_lock_volume(fhnd);
    if (lockstat)
    {
        printf("D: LOCKED");
        for (int i = 16384; i < 400000000; i += 1000000)
        {
            printf("Read D:  @ i=%d: %2x %2x %2x\n", i, rbytebuf[0], rbytebuf[1], rbytebuf[2]);
            fhnd = W32_Read(fhnd, (LPWSTR)L"\\\\.\\\\D:", rbytebuf, 2048, i);
            if (!fhnd) 
            {
                break;
            }
        }
    }
    CloseHandle(fhnd);

    W32_dismount_volume((LPWSTR)L"\\\\.\\\\D:");


    char mychars[128], newchars[128];
    memcpy(mychars, "abcdefg\0", 8);


#ifndef DOSMARTMG
    printf("SMART MONITOR CLASS -------------");

    mg_diskoperations myinfo;

    myinfo.MGSMARTscan(0);
    myinfo.MGSMARTscan(1);
    myinfo.MGSMARTscan(2);
    myinfo.MGSMARTscan(3);
    myinfo.MGSMARTscan(4);
    myinfo.MGSMARTscan(5);
#endif

    return 0;
    myinfo.set_str(mychars);
    int getstrflg = myinfo.get_str(newchars);
    printf("mystr: %s", newchars);

    myinfo.get_tempfilename(newchars);
    printf("mystr: %s", newchars);

    //const char* tmpfcontent[2] = { (const char*)"select disk 1\n", (const char*)"detail disk\n" };

    //myinfo.writetempfile(0, tmpfcontent, 2);

    //int tmpfileflg = tmpfilewrite(i, args, ptmpf);// argv);
    //_tprintf(TEXT("tmpfname %s\n"), ptmpf);

    myinfo.diskpart(0);
    myinfo.diskpart(1);
    myinfo.diskpart(2);
    myinfo.diskpart(3);
    myinfo.diskpart(4);
    myinfo.diskpart(5);
    return 0;
    unsigned v = MGdiskpart(ptmpf);

// Initialize a Fibonacci relation sequence.
    fibonacci_init(1, 1);
    // Write out the sequence values until overflow.
    do {
        std::cout << fibonacci_index() << ": "
            << fibonacci_current() << std::endl;
    } while (fibonacci_next());
    // Report count of values written before overflow.
    std::cout << fibonacci_index() + 1 <<
        " Fibonacci sequence values fit in an " <<
        "unsigned 64-bit integer." << std::endl;
    if (i < 10) {
        std::cout << "hello";
    }
    else {
        std::cout << "bye";
    };
    // DLL version
    i = MGSMARTscan();

    // the Local version
    i = SMARTscan();
}

int SMARTscan()
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

    if ((pPipe = _popen("smartctl -x /dev/sdb", "rt")) == NULL)
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

/* parse these for the information fields
Model Family : Silicon Motion based SSDs
Device Model : ADATA SU800
Serial Number : 2K0820058778
LU WWN Device Id : 5 707c18 1008cab23
Firmware Version : R0427ANR
User Capacity : 512, 110, 190, 592 bytes[512 GB]
Sector Size : 512 bytes logical / physical
Rotation Rate : Solid State Device
Form Factor : 2.5 inches
Device is : In smartctl database[for details use : -P show]
ATA Version is : ACS - 3 T13 / 2161 - D revision 5
SATA Version is : SATA 3.3, 6.0 Gb / s(current : 6.0 Gb / s)
Local Time is : Sat Jul 11 09 : 25 : 57 2020 PDT
SMART support is : Available - device has SMART capability.
SMART support is : Enabled
AAM feature is : Unavailable
APM level is : 254 (maximum performance)
Rd look - ahead is : Enabled
Write cache is : Enabled
DSN feature is : Unavailable
ATA Security is : Disabled, frozen[SEC2]
Wt Cache Reorder : Unknown
*/


// temp file service

#define BUFSIZE 1024

void PrintError(LPCTSTR errDesc);


int tmpfilewrite(int argc, const char* argv[], TCHAR* fnbuffer)
{
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
    char  chBuffer[BUFSIZE];


    //LPCTSTR errMsg;

    printf("args %s\n", argv[0]);
    printf("args %s\n", argv[1]);
    if (argc != 2)
    {
        _tprintf(TEXT("Usage: %s <file>\n"), argv[0]);
        return NULL;
    }
    const char* mystr = "tempfilename0";
    //  Opens the existing file. 
/*    hFile = CreateFile( mystr,// argv[1],               // file name
        GENERIC_READ,          // open for reading
        0,                     // do not share
        NULL,                  // default security
        OPEN_EXISTING,         // existing file only
        FILE_ATTRIBUTE_NORMAL, // normal file
        NULL);                 // no template
    if (hFile == INVALID_HANDLE_VALUE)
    {
        PrintError(TEXT("First CreateFile failed"));
        return (1);
    }
*/
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
    _tprintf(TEXT("Tempfilename %s \n"), szTempFileName);// argv[1]);
    //TCHAR* str = _T("Hello");
    //TCHAR* str2 = new TCHAR[23];   // let us say this is big enough 

    _tcscpy_s(fnbuffer, MAX_PATH, szTempFileName);
    //memcpy(&str2, &str, (_tcslen(str) + 1) * sizeof(TCHAR)); 
    //strcpy(fnbuffer, lpTempPathBuffer);

    //  Creates the new file to write to for the upper-case version.
    hTempFile = CreateFile((LPTSTR)szTempFileName, // file name 
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
    const char* chBuffer2 = "select disk 1";
    dwBytesRead = 13;
    for (int i = 0; i < 2; i++) {
        dwBytesRead = strlen(argv[i]);
        fSuccess = WriteFile(hTempFile,
            argv[i],
            dwBytesRead,
            &dwBytesWritten,
            NULL);
        if (!fSuccess)
        {
            PrintError(TEXT("WriteFile failed"));
            return (NULL);
        }
    }
    //  Reads BUFSIZE blocks to the buffer and converts all characters in 
    //  the buffer to upper case, then writes the buffer to the temporary 
    //  file. 
/*    do
    {
        if (ReadFile(hFile, chBuffer, BUFSIZE, &dwBytesRead, NULL))
        {
            //  Replaces lower case letters with upper case
            //  in place (using the same buffer). The return
            //  value is the number of replacements performed,
            //  which we aren't interested in for this demo.
            CharUpperBuffA(chBuffer, dwBytesRead);

            fSuccess = WriteFile(hTempFile,
                chBuffer,
                dwBytesRead,
                &dwBytesWritten,
                NULL);
            if (!fSuccess)
            {
                PrintError(TEXT("WriteFile failed"));
                return (5);
            }
        }
        else
        {
            PrintError(TEXT("ReadFile failed"));
            return (6);
        }
        //  Continues until the whole file is processed.
    } while (dwBytesRead == BUFSIZE);
*/
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
        fSuccess = MoveFileEx(szTempFileName,
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
    return (1);
}

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

//  PrintError support function.
//  Simple wrapper function for error output.
void PrintError(LPCTSTR errDesc)
{
    LPCTSTR errMsg = ErrorMessage(GetLastError());
    _tprintf(TEXT("\n** ERROR ** %s: %s\n"), errDesc, errMsg);
    LocalFree((LPVOID)errMsg);
}