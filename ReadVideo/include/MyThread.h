#ifndef _my_thread_h_
#define _my_thread_h_

#include "MyVideo.h"
#include <thread>

class CMyThread
{
private:
  std::vector<std::thread> MTList;

public:
  CMyThread();
  CMyThread(int argc, const char *argv[],
            size_t nFlgCap = EVENT_CAP_MY_FRM_GRAY_420,
            size_t nRecordFrameFlag = RECORD_FRAME);
  CMyThread(std::string sVideoName, size_t nNbr = 1,
            size_t nFlgCap = EVENT_CAP_FRM_GRAY_64_SIZE,
            size_t nRecordFrameFlag = RECORD_NO_FRAME);

  ~CMyThread();

  void MTInvokeAll();
};

#endif