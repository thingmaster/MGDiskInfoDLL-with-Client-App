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


/*
TCHAR* tmpfilewrite(int argc, const char* argv[], char * fnbuffer )
*/

