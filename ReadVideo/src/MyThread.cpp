#include "MyThread.h"

using namespace std;

CMyThread::CMyThread()
{
    ;
}

CMyThread::CMyThread(int argc, const char *argv[],
                     size_t nFlgCap,
                     size_t nRecordFrameFlag)
{
    cout << "Testing : " << nFlgCap << endl;

    for (size_t nBoucle = 1; nBoucle < (size_t)argc; nBoucle++)
    {
        MTList.push_back(thread(&CMyVideo::MVTask,
                                CMyVideo(),
                                argv[nBoucle],
                                nFlgCap,
                                nRecordFrameFlag));
    }
}

CMyThread::CMyThread(string sVideoName, size_t nNbr,
                     size_t nFlgCap,
                     size_t nRecordFrameFlag)
{
    cout << "Testing : " << nFlgCap << endl;
    cout << "Thread Number : " << nNbr << endl;

    for (size_t nBoucle = 0; nBoucle < nNbr; nBoucle++)
    {
        MTList.push_back(thread(&CMyVideo::MVTask,
                                CMyVideo(),
                                sVideoName,
                                nFlgCap,
                                nRecordFrameFlag));
    }
}

CMyThread::~CMyThread()
{
    ;
}

void CMyThread::MTInvokeAll()
{
    cout << "Begin test" << endl;

    for (size_t nBoucle = 0; nBoucle < MTList.size(); nBoucle++)
    {
        MTList[nBoucle].join();
        // MTList[nBoucle].detach();
    }

    // End until all threads finished
    cout << "End test" << endl;
}
