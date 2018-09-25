#include "MyFrame.h"
#include <fstream>
#include <sstream>
#include <bitset>

using namespace std;

CMyFrame::CMyFrame()
{
    iMFWidth = 0;
    iMFHeight = 0;
    pnMFData = NULL;
    nFlgRecord = RECORD_NO_FRAME;
}

CMyFrame::CMyFrame(string sFrameName,
                   AVFrame *frame,
                   size_t flgRecord)
{
    iMFWidth = 0;
    iMFHeight = 0;
    pnMFData = NULL;
    nFlgRecord = flgRecord;

    MFInit(sFrameName, frame);
}

/*
CMyFrame::CMyFrame(const CMyFrame &cParam)
    : sMFFrameName(cParam.sMFFrameName),
      vvMFInfo(cParam.vvMFInfo),
      pMFFrame(cParam.pMFFrame),
      nFlgRecord(cParam.nFlgRecord)
{
    MFInit(sMFFrameName, pMFFrame);
}
*/

CMyFrame::~CMyFrame()
{
}

// Private methodes

void CMyFrame::MFRefreshFrame(vector<uint8_t> vData)
{
    pnMFData = (uint8_t *)av_malloc(pMFFrame->linesize[0] *
                                    iMFHeight *
                                    sizeof(uint8_t));
    copy(vData.begin(), vData.end(), pnMFData);

    // Remplace the ex
    av_free(pMFFrame->data[0]);
    pMFFrame->data[0] = pnMFData;
}

void CMyFrame::MFRecord()
{
    cout << "Recording frame " << sMFFrameName << endl;

    MFInitPixTable();

    ofstream ofs;
    ofs.open(("frame/" + sMFFrameName + ".txt").c_str(),
             ofstream::out | ofstream::trunc);

    for (int y = 0; y < iMFHeight; y++)
    {
        // if (y % 40 == 0)
        // {
        for (int x = 0; x < iMFWidth; x++)
        {
            ofs << (int)vvMFInfo[y][x] << " ";
        }
        ofs << "\n";
        // }
    }

    ofs.close();
}

int CMyFrame::MFInBound(int r, int c)
{
    if (r < iMFHeight && r > 0 &&
        c < iMFWidth && c > 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void CMyFrame::MFBit2Hex(string &sTarget)
{
    bitset<64> bTmp(sTarget);
    stringstream ssTmp;
    ssTmp << hex << bTmp.to_ulong();
    sTarget = ssTmp.str();
}

// Public methodes

void CMyFrame::MFInit(string sFrameName, AVFrame *frame)
{
    sMFFrameName = sFrameName;
    pMFFrame = frame;

    iMFWidth = pMFFrame->width;
    iMFHeight = pMFFrame->height;

    if (nFlgRecord == RECORD_FRAME)
    {
        MFRecord();
    }
}

void CMyFrame::MFInitPixTable()
{
    for (int y = 0; y < iMFHeight; y++)
    {
        uint8_t *pnPixH = pMFFrame->data[0] +
                          y * pMFFrame->linesize[0];
        vvMFInfo.push_back(vector<uint8_t>());
        for (int x = 0; x < iMFWidth; x++)
        {
            vvMFInfo[y].push_back(pnPixH[x]);
        }
    }
}

void CMyFrame::MFRotate(int iTheta)
{
    MFInitPixTable();

    // r’ = r0 + (r – r0)cos(theta) – (c – c0)sin(theta)
    // c’ = c0 + (r - r0)sin(theta) + (c – c0)cos(theta)

    vector<uint8_t> vData(pMFFrame->data[0],
                          pMFFrame->data[0] +
                              pMFFrame->linesize[0] *
                                  iMFHeight);

    //converts the degree given by user into radians
    float rads = (iTheta * 3.14159265) / 180.0;
    int r0 = iMFHeight / 2;
    int c0 = iMFWidth / 2;
    int r1 = 0, c1 = 0;

    // goes through the array of the oldImage, uses the formulas
    // to find where the pixel should go
    // then puts the old pixel value into the new
    // pixel position on the tempImage
    for (int r = 0; r < iMFHeight; r++)
    {
        for (int c = 0; c < iMFWidth; c++)
        {
            r1 = (int)(r0 + ((r - r0) * cos(rads)) -
                       ((c - c0) * sin(rads)));
            c1 = (int)(c0 + ((r - r0) * sin(rads)) +
                       ((c - c0) * cos(rads)));

            if (MFInBound(r1, c1) == 0)
            {
                vData[r * pMFFrame->linesize[0] +
                      c] = vvMFInfo[r1]
                                   [c1];
            }
        }
    }

    MFRefreshFrame(vData);
}

void CMyFrame::MFInverse()
{
    MFInitPixTable();

    vector<uint8_t> vData(pMFFrame->data[0],
                          pMFFrame->data[0] +
                              pMFFrame->linesize[0] *
                                  iMFHeight);

    for (int y = 0; y < iMFHeight; y++)
    {
        for (int x = 0; x < iMFWidth; x++)
        {
            vData[y * pMFFrame->linesize[0] +
                  x] = vvMFInfo[y]
                               [iMFWidth - 1 - x];
        }
    }

    MFRefreshFrame(vData);
}

void CMyFrame::MFExtractTpl()
{
    MFInitPixTable();

    cout << "Templating frame " << sMFFrameName << endl;
    ofstream ofs;
    ofs.open(("frame/" + sMFFrameName + ".txt").c_str(),
             ofstream::out | ofstream::app);

    string sTmp;
    for (int y = 0; y < iMFHeight; y++)
    {
        for (int x = 0; x < iMFWidth - 1; x++)
        {
            sTmp += to_string((bool)(vvMFInfo[y][x + 1] - vvMFInfo[y][x]));
        }
    }
    MFBit2Hex(sTmp);
    ofs << sTmp << "\n";

    ofs.close();
}

void CMyFrame::MFExtractTpl2()
{
    MFInitPixTable();

    cout << "Templating frame " << sMFFrameName << endl;
    ofstream ofs;
    ofs.open(("frame/" + sMFFrameName + ".txt").c_str(),
             ofstream::out | ofstream::app);

    string sTmp;
    for (int y = 0; y < iMFHeight; y++)
    {
        for (int x = 0; x < iMFWidth - 1; x++)
        {
            sTmp += to_string((bool)(vvMFInfo[y][x + 1] - vvMFInfo[y][x]));
        }
    }
    // MFBit2Hex(sTmp);
    ofs << sTmp << "\n";

    ofs.close();
}

string CMyFrame::MFObtainKey()
{
    string sKey;
    for (int y = 0; y < iMFHeight; y++)
    {
        uint8_t *pnPixH = pMFFrame->data[0] +
                          y * pMFFrame->linesize[0];
        for (int x = 0; x < iMFWidth - 1; x++)
        {
            sKey += to_string((bool)(pnPixH[x + 1] - pnPixH[x]));
            // sTmp += to_string((bool)(vvMFInfo[y][x + 1] - vvMFInfo[y][x]));
        }
    }
    MFBit2Hex(sKey);
    return sKey;
}

string CMyFrame::MFObtainKey2()
{
    string sKey;
    for (int y = 0; y < iMFHeight; y++)
    {
        uint8_t *pnPixH = pMFFrame->data[0] +
                          y * pMFFrame->linesize[0];
        for (int x = 0; x < iMFWidth - 1; x++)
        {
            sKey += to_string((bool)(pnPixH[x + 1] - pnPixH[x]));
            // sTmp += to_string((bool)(vvMFInfo[y][x + 1] - vvMFInfo[y][x]));
        }
    }
    // MFBit2Hex(sKey);
    return sKey;
}
