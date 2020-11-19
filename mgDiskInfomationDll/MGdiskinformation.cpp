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
    BOOL readstat = W32_Read(fhnd, (LPCWSTR)"\\\\1", rbytedata, readsize, offs);
    BOOL writestat = W32_Write(fhnd, (LPCWSTR)"\\\\1", wbytedata, writecount, offs);
    BOOL lockstat = W32_lock_volume( drivehandle);
    BOOL dismountstat = W32_dismount_volume((LPCWSTR) "F");
    return 0;
}

BOOL W32_Read(HANDLE fhnd, LPCWSTR physpath, LPVOID rbytebuf, DWORD readsize, LONG offs)
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
        if (fhnd == 0)
        {
            return FALSE; //, -1, 0 # couldnot open it
        }
    }

    LONG movedisthigh = 0;
    SetFilePointer(fhnd, offs, &movedisthigh, FILE_BEGIN); //win32file.SetFilePointer(fhnd, offs, win32file.FILE_BEGIN)
    //werr, rbytebuf = win32file.ReadFile(fhnd, readsize, None) #rbytebuf, None) #, overlapped )
    DWORD readcountresult;
    BOOL readstat = FALSE;
    readstat = ReadFile(fhnd, rbytebuf, readsize, &readcountresult, 0); //#, overlapped )

    return readstat ; // fhnd, werr, rbytebuf
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
