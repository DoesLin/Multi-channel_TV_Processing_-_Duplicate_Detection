#ifndef _my_frame_h_
#define _my_frame_h_

#include "header.h"

class CMyFrame
{
private:
  std::string sMFFrameName;
  std::vector<std::vector<uint8_t>> vvMFInfo; // 0 - 255
  AVFrame *pMFFrame;                          // Processing

  int iMFWidth;
  int iMFHeight;

  // No processing no use
  // Faster
  uint8_t *pnMFData;

  size_t nFlgRecord;

  void MFRefreshFrame(std::vector<uint8_t> vData);
  void MFRecord();

  // For rotate : 0 - InBound
  int MFInBound(int r, int c);

  // Convert 64 bits to hex
  void MFBit2Hex(std::string &sTarget);

public:
  CMyFrame();
  // Whether record the vector in txt or not
  CMyFrame(std::string sFrameName, AVFrame *frame,
           size_t flgRecord = RECORD_NO_FRAME);
  // CMyFrame(const CMyFrame &cParam);
  ~CMyFrame();

  void MFInit(std::string sFrameName, AVFrame *frame);
  // For convenience - slower
  void MFInitPixTable();

  void MFRotate(int iTheta);
  void MFInverse();

  void MFExtractTpl();
  void MFExtractTpl2();
  std::string MFObtainKey();
  std::string MFObtainKey2();
};

#endif