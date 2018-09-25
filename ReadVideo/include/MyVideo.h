#ifndef _my_video_h_
#define _my_video_h_

#include "header.h"
#include "MyFrame.h"

class CMyVideo
{
private:
  // Init infos
  std::string sMVFileName;
  std::string sMVFile;

  // RT infos
  double dMVRTConvert;
  double dMVRTDetect;
  int iMVRTCount;
  double dMVRTAverage;

  // Codec infos
  AVFormatContext *pMVFormatCtx;
  AVCodecContext *pMVCodecCtxOrig;
  AVCodecContext *pMVCodecCtx;
  AVCodec *pMVCodec;

  // Frame infos
  size_t nMVNbrFrame;
  size_t nMVNbrTplFrame;
  AVFrame *pMVFrame;
  AVFrame *pMVTargetFrame;
  uint8_t *pMVBuffer;
  std::vector<CMyFrame> vMVFrames;

  // Video stream infos
  int iMVVideoStream;
  AVPacket MVPacket;
  struct SwsContext *pMVSwsCtx;

  // Flags
  size_t nMVFlgCap;
  size_t nMVFlgRecordFrame;
  size_t nMVFlgQuit;
  size_t nMVFlgFndDup;

  // Detection db
  std::vector<std::string> vMVTemplate;

  // Private methodes

  // Init
  void MVInitFrameFormat(AVPixelFormat format,
                         int width, int height);
  void MVInitTargetFrame();

  // Frame
  void MVConvertFrame();
  void MVConvertWithRT();
  void MVSaveFrame(int iFrame);

  // Detection
  void MVLoadDb();
  void MVDetectDup();
  void MVDetectWithRT();
  void MVDetectDup2();
  void MVDetectWithRT2();

  // Event
  void MVEvent();

  // Store
  void MVStoreTypeProc(std::string sType);
  void MVStoreRT(std::string sRTType, double dRT);

public:
  CMyVideo();
  CMyVideo(const CMyVideo &cParam);
  CMyVideo(std::string argv,
           size_t nFlgCap = EVENT_NONE,
           size_t nRecordFrameFlag = RECORD_NO_FRAME);
  ~CMyVideo();

  // Modification
  void MVModifierCapFlag(size_t nFlgCap);

  // Initialization
  void MVInit(std::string argv);
  void MVReadFrame();
  void MVQuit();

  // Task for thread
  void MVTask(std::string argv,
              size_t nFlgCap = EVENT_CAP_FRM_GRAY_420_SIZE,
              size_t nRecordFrameFlag = RECORD_FRAME);
};

#endif