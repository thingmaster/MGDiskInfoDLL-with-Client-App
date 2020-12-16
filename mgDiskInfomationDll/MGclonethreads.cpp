#include <pch.h>
#include <strsafe.h>
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
//#include <queue.h>



#include "MGclonethreads.h"


extern int MBR_diskcopy2(LONGLONG copysize); // targetdisk, srcinfo, destinfo, drvinfosrc, drvinfodest, updatequeue = None, event = None, copysize = 0) :

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
    HANDLE hStdout;
    PMYDATA pDataArray;

    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;

    // Make sure there is a console to receive output results. 

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE)
        return 1;

    // Cast the parameter to the correct data type.
    // The pointer is known to be valid because 
    // it was checked for NULL before the thread was created.

    pDataArray = (PMYDATA)lpParam;

    // Print the parameter values using thread-safe functions.

    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"),
        pDataArray->val1, pDataArray->val2);
    long k = 0;
    for (int j = 0; j < 0x7fffffffL; j++)
    {
        k += 1;
    }

    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

    //return 0;

    MBR_diskcopy2(1000000000000ull); // (targetdisk, srcinfo, destinfo, drvinfosrc, drvinfodest, queue, event, sourcesize);

}



void ErrorHandler(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code.

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}


int MBR_diskcopy(int source, int destination)
{

    //srcinfo, destinfo, drvinfosrc, drvinfodest = self.MBR_copyinfo(source, destination)
    MBR_diskcopy2(1000000);// srcinfo, destinfo, drvinfosrc, drvinfodest)
    return 0;
}

extern "C" MGSTORAGEDLL_API HANDLE W32_Read(HANDLE fhnd, LPCWSTR physpath, LPVOID rbytebuf, DWORD readsize, LONG offs);
extern "C" MGSTORAGEDLL_API HANDLE W32_Write(HANDLE fhnd, LPCWSTR physpath, LPCVOID wbytedata, DWORD writecount, int offs);
extern "C" MGSTORAGEDLL_API BOOL W32_lock_volume(HANDLE drivehandle);
extern "C" MGSTORAGEDLL_API BOOL W32_dismount_volume(LPCWSTR volpath);

int MBR_diskcopy2(long long  copysize = 0) // targetdisk, srcinfo, destinfo, drvinfosrc, drvinfodest, updatequeue = None, event = None, copysize = 0) :
{
    if (FALSE) // src bigger than dest?  srcinfo['bytestocopy'] > destinfo['bytestocopy'] 
    {
        printf("dest smaller than src disk");
        return -1;
    }
    //------queue updatequeue = new queue;
    unsigned long  COPYUNIT = (32768 * 8 * 16);// # CHANGE TO X16 = 4MB0
    unsigned long long  writeops = copysize /(long long) COPYUNIT;
    unsigned long writerem = copysize % COPYUNIT;
    //BYTE buf = win32file.AllocateReadBuffer(COPYUNIT);
    /*
    thisitemstatus = 'COPYING: Dev(%s-->%s), Write size(%d) Disk size %d.(0x%x) ' % (
        drvinfosrc[1],
        drvinfodest[1],
        COPYUNIT,
        copysize,
        copysize
        )
        print(thisitemstatus)
    */
    HANDLE fhnd = 0;
    HANDLE fhnd_read = 0;
    wchar_t rd_physpath[MAX_PATH], wrt_physpath[MAX_PATH];
    LPVOID rbytebuf;
    // NOTE READ() buffer has to be a windows resource; using local function heap will result in Read fail 998 (access error)
    rbytebuf = (LPVOID)  HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, COPYUNIT );


    unsigned long  writesize;
    unsigned long long  cbytesoffs;
    unsigned long   retrylimit = 100;
    int thisitemstatus = 0;
    int statetracking = 0;
    wsprintf(rd_physpath, L"\\\\.\\PhysicalDrive%d", 0);
    wsprintf(wrt_physpath, L"\\\\.\\PhysicalDrive%d", 3);

    //BOOL bLocked = W32_lock_volume(fhnd);
    BOOL bdmD = W32_dismount_volume((LPWSTR)L"\\\\.\\\\I:");
    BOOL bdmF = W32_dismount_volume((LPWSTR)L"\\\\.\\\\J:");
    BOOL bdmG = W32_dismount_volume((LPWSTR)L"\\\\.\\\\L:");

    for (unsigned long long writtencount = 0; writtencount <= writeops; writtencount += 1)
    {
        //try :
        if (writtencount == writeops)
        {
            writesize = writerem;
        }
        else
        {
            writesize = COPYUNIT;
        }
        cbytesoffs = writtencount * COPYUNIT;
        if (FALSE)
        {
            /*
            thisitemstatus = 'Dev(%s-->%s),copying %d bytes @ %d (0x%x)/ %d  (%3.1f%%)' % (
                drvinfosrc[1],
                drvinfodest[1],
                writesize,
                cbytesoffs,
                cbytesoffs,
                srcinfo['bytestocopy'],
                (float(cbytesoffs + COPYUNIT) / float(srcinfo['bytestocopy'])) * 100.0
                )
            # read()
            */
        }
        statetracking += 1;

        printf("rd: @ %lld, writesize %ld\n", cbytesoffs, writesize);
        //fhnd_read, ioerr, buf = W32_Read(fhnd_read, drvinfosrc[1], buf, writesize, cbytesoffs)
        fhnd_read = W32_Read(fhnd_read, (LPCWSTR)rd_physpath, (LPVOID) rbytebuf, (DWORD) writesize, cbytesoffs );

        statetracking += 1;
        //# write()
        printf("wrt: @ %lld, writesize %ld\n", cbytesoffs, writesize);
        if (FALSE) //WRITES DISABLED by default UNTIL DEV COMPLETE!
        {
            fhnd = W32_Write(fhnd, wrt_physpath, rbytebuf, (DWORD)writesize, cbytesoffs);//fhnd, ioerr, nby = W32_Write(fhnd, drvinfodest[1], buf, cbytesoffs)
            //#ret, _data = win32file.WriteFile(dest_hnd, buf)
        }
        /*
        #if ret:
        #    msg = ("Error occurred while sending data, 
        #            "err=%s, sentlen=%s" % (ret, _data))
        #    print("FAIL: " + msg)
        #    raise IOError(msg)
        */
        statetracking += 1;
        if (FALSE) // updatequeue)
        {
            //------updatequeue.put((targetdisk, cbytesoffs + writesize));
            //#event.set()
        }
        //except:
        /*
        thisitemstatus = 'Dev(%s-->%s) %d bytes, @ %d (0x%x)/ %d  (%3.1f%%)' % (
                drvinfosrc[1],
                drvinfodest[1],
                writesize,
                cbytesoffs,
                cbytesoffs,
                srcinfo['bytestocopy'],
                (float(cbytesoffs + COPYUNIT) / float(srcinfo['bytestocopy'])) * 100.0
                )
        */
        if (FALSE) //Something failed?
        {
            printf("Exception in copy: %s. Statetracker@<%d>; RETRY COUNT %d", thisitemstatus, statetracking, retrylimit);
            if (retrylimit <= 0)
            {
                break;
            }
            else
            {
                retrylimit -= 1;
            }
            continue;
        }
    }

    if (fhnd_read)
    {
        CloseHandle(fhnd_read);
        if (fhnd)
        {
            CloseHandle(fhnd);
            /*
            thisitemstatus = 'Dev(%s-->%s),copying %d bytes @ %d (0x%x)/ %d  (%3.1f%%)' % (
                drvinfosrc[1],
                drvinfodest[1],
                writesize,
                cbytesoffs,
                cbytesoffs,
                srcinfo['bytestocopy'],
                (float(cbytesoffs + COPYUNIT) / float(srcinfo['bytestocopy'])) * 100.0
                )
            */
            printf("End of diskcopy: %s. Statetracker@<%d>", thisitemstatus, statetracking);
        }
    }
}

/*
    ''' mgSSDclone( parentframe, mgDiskpart ssdinfo, MGsmartdecoder smartinfo )
    draw and manage Disk Cloning user interface
    '''
    class mgSSDclone(mgMultiZoneGui) :
    def __init__(self, parentframe, ssdinfo, smartinfo, extraframe = None) :
    super().__init__(parentframe, ssdinfo, smartinfo)
    self.diskcanvassegment = dict()
    self.clonethreads = dict()
    self.ssdinfo = ssdinfo
    self.smartinfo = smartinfo
    self.disks = mgDiskAccess()
    self.cloningobj = None
    self.clonestates = (MGguicanvasregion.STARTBUTTON,
        MGguicanvasregion.STATUSLABEL,
        MGguicanvasregion.DONELABEL)
    self.actiontext = ("Start writing...", "Status...", "Done...")
    self.clonestate = 0
    self.r0 = None
    self.r1 = None
    self.r2 = None
    self.r3 = None
    self.r4a = None
    drivenums, driveletters = ssdinfo.MGgetvolumesummary()
    self.refreshdisks("")

    def cloningaction(self, srcdisk, Onetgtdisk) :

    # do a cloning operation one source to one destination
    if int(Onetgtdisk) in self.bootdisks:
print("Bootdisk (%s) cannot be target drive for cloning" % Onetgtdisk)
    return

    disk = self.ssdinfo.MGgetdiskbynumber(int(Onetgtdisk))
    sourcesize = self.ssdinfo.MGgetdiskbynumber(int(srcdisk))['Media Size'] #self.disks.mbrs[int(srcdisk)][2]['IOCTL-DiskLength']
    targetsize = self.ssdinfo.MGgetdiskbynumber(int(Onetgtdisk))['Media Size'] #self.disks.mbrs[int(Onetgtdisk)][2]['IOCTL-DiskLength']

    if sourcesize > targetsize:
print("Cloning not executed; Source disk %d (%d bytes) is larger than Target disk %d(%d bytes)" % (
    int(srcdisk),
    sourcesize,
    int(Onetgtdisk),
    targetsize))
    return False

    if int(srcdisk) == int(Onetgtdisk) :
        print('Cloning not executed; Source and Target are same disk! (Disk %s)' % (srcdisk))
        return False

        umttgts, vols, otherpart, allparts = self.ssdinfo.MGgetdiskvolumes(int(Onetgtdisk))
        if len(umttgts) > 0: # there may be zero volumes on a disk
            umount_handles = unmount_volumes(umttgts)
        else:
umount_handles = []
    #print('Target drive #: %s, Volumes on target disk: ' % Onetgtdisk, umttgts)

    # create a new thread to do clone operation
# #mbrinfo.MBR_diskcopy( source=src, destination=Onetgtdisk)
    test01 = MGcloningclass(int(srcdisk), int(Onetgtdisk), parentwidget = None, buttonstiedtoprocess = None)
    self.clonethreads[str(Onetgtdisk.strip())] = test01
    test01.volumehandles = umount_handles
    test01.starttime = mg_timenow(bTimestring = False)[1]#  reference time value
    test01.diskcanvas = self.r3.getcanvas()
    test01.diskcanvassegment = self.diskcanvassegment[Onetgtdisk.strip()]
    test01.diskcopylength = sourcesize
    test01.executediskclonethread(self.disks)
    test01.onGetValue()
    #input()
#
    #self.cloningupdates()

    def cloningupdates(self, parentitem) :
    #starttime = self.clonestart[str(Onetgtdisk.strip())]
    secondsadjust = 60
    N256G = 256060162048
    ONEMB = 1024 * 1024
    TIME256G = 45 # use this representative 256G time - to - clone. not precise, but good enough for rough gui updates
    for diskno in self.clonethreads:
athread = self.clonethreads[diskno]
    capacity = athread.diskcopylength
    minsreference = TIME256G * capacity / N256G #
    tmpc = athread.diskcanvas
    updatestr = '...'
    while not athread.queue.empty() :
        rslt, values = athread.getqueuedata()
        if rslt :
            if len(values) > 1:
value = values[1]
            else :
                value = values
                currtime = mg_timenow(bTimestring = False) # get current time value
                deltaseconds = (currtime[1] - athread.starttime)
                pctcomplete = value / capacity
                if not deltaseconds:
deltaseconds = 999
    bytespersec = value / deltaseconds
    minsleft = (capacity - value) / bytespersec / secondsadjust   #minsreference - pctcomplete * minsreference
    updatestr = '  %3.0f%% copied; time remaining %d minutes (@%5.1f MB/S) ' % (pctcomplete * 100, minsleft, bytespersec / ONEMB)

    if not athread.testisalive() :
        break #
        if not updatestr == '...' :
            self.docloningvisualupdate(tmpc, athread.diskcanvassegment, updatestr, pctcomplete, deltaseconds, minsleft, bytespersec)

            #check the cloning threads anytime we see all queues queue in empty state
            someonealive = False
            for diskno in self.clonethreads:
#if anyone is alive, retrigger an update refresh
athread = self.clonethreads[diskno]
    if athread.testisalive() :
        parentitem.after(500, lambda wid = parentitem:self.cloningupdates(wid))
        someonealive = True
        break

# if clonethread execution is completed, use diskpart to modify the new-disk GUID to avoid collision
        # After GUID update, online the diskand release any volume handles that were locked for cloning
        if not athread.onlinedguidchange:
returncode, onlined = self.ssdinfo.MGdiskpartGUID(int(diskno), int(diskno))
    athread.onlinedguidchange = onlined
    if len(athread.volumehandles) > 0:
unmount_volumes(None, athread.volumehandles)
    athread.volumehandles = []

    # need some work to lock out cloning features at least until the app can restart,
    # but desireable to ask user for system restart with cloned disks removed
    if not someonealive:
parentitem['text'] = 'Restart...'
*/
/*
# class MGcloningclass creates a thread to execute cloningand post updates to a queue readable by the GUI for progress info
# sourcedisk is a list currently identifying a single soure disk for cloning eg['0']
# targetdisks is a single target disk for cloning eg.['2']
# parentwidget is a parent progress widget enabled / disabled
class MGcloningclass(object) :
    PROGRESSDELAY = 100
    def __init__(self, sourcedisk, targetdisk, parentwidget = None, buttonstiedtoprocess = None) :
    self.queue = Queue()
    self.event = Event()#TBD how this is used
    self.diskcanvas = None
    self.diskcanvassegment = None
    self.onlinedguidchange = False
    self.diskcopylength = 0
    self.starttime = None
    self.volumehandles = []
    self.parentwidget = parentwidget
    self.sourcedisk = sourcedisk
    self.targetdisk = targetdisk
    self.queue.put((1, 999)) # try a complex payload ?
    self.buttons_tied = buttonstiedtoprocess


    def updatebuttons(self, button_state = DISABLED) :

    if not self.buttons_tied :
        return

        for btn in self.buttons_tied :
            btn.config(button_state)
            pass

            def getqueue(self) :
            return self.queue

            def executediskclonethread(self, ssdinf) :
            self.updatebuttons()
            #freeze_support()
            #print(ssdinf)
            srcinfo, destinfo, drvinfosrc, drvinfodest = ssdinf.MBR_copyinfo(self.sourcedisk, self.targetdisk)

            self.p1 = Thread(target = self.docloningthreadproc, args = (self.queue, self.event, self.targetdisk, self.diskcopylength, srcinfo, destinfo, drvinfosrc, drvinfodest))  #self.sourcedisk, self.targetdisks))
            self.p1.start()
            if self.parentwidget:
self.parentwidget.start(PROGRESSDELAY)
self.parentwidget.after(MGcloningclass.PROGRESSDELAY, self.onGetValue)
if self.testisalive() :
    #print('executediskclonethread is ALIVE')
    pass
else:
print('executediskclonethread is NOT ALIVE. it did not start??', self.sourcedisk, self.targetdisk)

def testisalive(self) :
    return self.p1.isAlive()

    def onGetValue(self, parentwidget = None) :
    if not self.parentwidget :
        return
        if self.p1.isAlive() :
            #print("callback")
            self.parentwidget.after(MGcloningclass.PROGRESSDELAY, self.onGetValue)
            return
        else:
try :
    #self.txt.insert('end', self.queue.get(0))
    #self.txt.insert('end', "\n")
    #print('q', self.getqueuedata())
    self.parentwidget.stop()
    #self.startBtn.config(state = NORMAL)

    except queue.Empty:
print("queue is empty")

def getqueuedata(self) :
    try :
    #self.txt.insert('end', self.queue.get(0))
    #self.txt.insert('end', "\n")
    x = self.queue.get(0)
    return True, x

    except queue.Empty:
#print("queue is empty")
return False, None

# a threadproc to clone a source disk to a dest disk
def docloningthreadproc(self, queue, event, targetdisk, sourcesize, srcinfo, destinfo, drvinfosrc, drvinfodest) :
    #print('in thread')  #, devicespecs)
    # do a cloning operation one source to one destination
    MBR_diskcopy2(targetdisk, srcinfo, destinfo, drvinfosrc, drvinfodest, queue, event, sourcesize)

*/


int serverside(void)
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;


    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
        1,
        1024 * 16,
        1024 * 16,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
        {
            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                /* add terminating zero */
                buffer[dwRead] = '\0';

                /* do something with data in buffer */
                printf("%s", buffer);
            }
        }

        DisconnectNamedPipe(hPipe);
    }

    return 0;
}

int clientside(void)
{
    HANDLE hPipe;
    DWORD dwWritten;


    hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        WriteFile(hPipe,
            "Hello Pipe\n",
            12,   // = length of string + terminating '\0' !!!
            &dwWritten,
            NULL);

        CloseHandle(hPipe);
    }

    return (0);
}