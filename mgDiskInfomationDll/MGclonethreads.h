#pragma once

#pragma once

#ifdef MGDISKINFOMATIONDLL_EXPORTS
#define MGSTORAGEDLL_API __declspec(dllexport)
#else
#define MGSTORAGEDLL_API __declspec(dllimport)
#endif

extern "C" MGSTORAGEDLL_API  class mgclonethread ;
extern "C" MGSTORAGEDLL_API DWORD WINAPI MyThreadFunction(LPVOID lpParam);
extern "C" MGSTORAGEDLL_API void ErrorHandler(LPTSTR lpszFunction);



#define MAX_THREADS 1
#define BUF_SIZE 255

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
typedef struct MyData {
    int val1;
    int val2;
} MYDATA, * PMYDATA;


extern "C" MGSTORAGEDLL_API  class mgclonethread
{
    PMYDATA pDataArray[MAX_THREADS];
    DWORD   dwThreadIdArray[MAX_THREADS];
    HANDLE  hThreadArray[MAX_THREADS];
public:
    mgclonethread(void)
    {
        this->mgcreatethreads();
        for (BOOL bactive = FALSE; !bactive; )
        {
            bactive = TRUE;
            // keep checking as long as Anybody alive?
            for (int i = 0; i < MAX_THREADS; i++)
            {
                if (this->isThreadAlive(this->hThreadArray[i]))
                {
                    //printf("thread NOT finished i=%d\n", i);
                    bactive = FALSE;
                    break;
                }
                printf("thread finished i=%d\n", i);
            }
        }
        for (int i = 0; i < MAX_THREADS; i++)
        {
            CloseHandle(hThreadArray[i]);
            if (this->pDataArray[i] != NULL)
            {
                HeapFree(GetProcessHeap(), 0, this->pDataArray[i]);
                this->pDataArray[i] = NULL;    // Ensure address is not reused.
            }
        }
    }

    int mgcreatethreads()
    {
        // Create MAX_THREADS worker threads.

        for (int i = 0; i < MAX_THREADS; i++)
        {
            // Allocate memory for thread data.

            this->pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(MYDATA));

            if (this->pDataArray[i] == NULL)
            {
                // If the array allocation fails, the system is out of memory
                // so there is no point in trying to print an error message.
                // Just terminate execution.
                ExitProcess(2);
            }

            // Generate unique data for each thread to work with.

            this->pDataArray[i]->val1 = i;
            this->pDataArray[i]->val2 = i + 100;

            // Create the thread to begin execution on its own.

            this->hThreadArray[i] = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size  
                MyThreadFunction,       // thread function name
                this->pDataArray[i],          // argument to thread function 
                0,                      // use default creation flags 
                &this->dwThreadIdArray[i]);   // returns the thread identifier 


            // Check the return value for success.
            // If CreateThread fails, terminate execution. 
            // This will automatically clean up threads and memory. 

            if (this->hThreadArray[i] == NULL)
            {
                ErrorHandler((LPTSTR)TEXT("CreateThread"));
                ExitProcess(3);
            }
        } // End of main thread creation loop.

        // Wait until all threads have terminated.
    }

    int blockingwaitallthreads()
    {
        WaitForMultipleObjects(MAX_THREADS, this->hThreadArray, TRUE, INFINITE);
        WAIT_TIMEOUT;//timed out. not ready
        WAIT_OBJECT_0; //first thread, ...
        WAIT_OBJECT_0 + 1;
        WAIT_OBJECT_0 + 2;
        WAIT_FAILED;
        return 0;
    }

    BOOL isThreadAlive(HANDLE hthread)
    {
        DWORD waitresult = WaitForMultipleObjects(1, &hthread, TRUE, 0);
        //WAIT_TIMEOUT;//timed out. not ready
        if (waitresult == WAIT_OBJECT_0)
        {    //first thread, ...
            return FALSE;
        }
        if (waitresult == WAIT_TIMEOUT)
        {
            return TRUE;
        }
        if (waitresult == WAIT_FAILED)
        { //            The function has failed.To get extended error information, call GetLastError.WAIT_FAILED
            printf("waiting for thread WaitForMultipleObjects() failed.");
            return FALSE;
        }
        return FALSE;
    }
    int closethreadh()
    {
        // Close all thread handles and free memory allocations.

        for (int i = 0; i < MAX_THREADS; i++)
        {
            CloseHandle(this->hThreadArray[i]);
            if (this->pDataArray[i] != NULL)
            {
                HeapFree(GetProcessHeap(), 0, this->pDataArray[i]);
                this->pDataArray[i] = NULL;    // Ensure address is not reused.
            }
        }
        return 0;
    }
};