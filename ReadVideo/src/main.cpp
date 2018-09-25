#include "header.h"
#include "MyThread.h"

using namespace std;

int main(int argc, const char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: ./MyVideo.out <file>\n");
    exit(1);
  }

  // Init template
  CMyVideo MVTpl;
  MVTpl.MVTask("Alone30s.mp4",
               EVENT_MY_DET_TPL,
               RECORD_NO_FRAME);

  // Multi input videos
  CMyThread CMTTest(argc, argv,
                    EVENT_MY_DET_CAP,
                    RECORD_NO_FRAME);
  CMTTest.MTInvokeAll();

  // string sNbr(argv[1]);
  // CMyThread CMTTest(argv[2],
  //                   stoi(sNbr),
  //                   EVENT_CAP_RCD_MY_DET);
  // CMTTest.MTInvokeAll();

  // getchar();
  return 0;
}
