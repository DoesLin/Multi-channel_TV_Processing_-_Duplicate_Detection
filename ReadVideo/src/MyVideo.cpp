#include "MyVideo.h"
#include <fstream>
#include <algorithm>
#include <thread>

using namespace std;
using namespace chrono;

// Les construiteurs et les destruiteurs

CMyVideo::CMyVideo()
{
    pMVFormatCtx = NULL;
    pMVCodecCtxOrig = NULL;
    pMVCodecCtx = NULL;
    pMVCodec = NULL;
    pMVFrame = NULL;
    pMVTargetFrame = NULL;
    pMVSwsCtx = NULL;
    pMVBuffer = NULL;

    dMVRTConvert = 0;
    iMVVideoStream = 0;
    nMVFlgCap = EVENT_NONE;
    nMVFlgRecordFrame = RECORD_NO_FRAME;
    nMVFlgQuit = 1;
    nMVNbrFrame = 0;
    nMVNbrTplFrame = 0;
    nMVFlgFndDup = 0;
}

CMyVideo::CMyVideo(const CMyVideo &cParam)
    : sMVFileName(cParam.sMVFileName),
      nMVFlgCap(cParam.nMVFlgCap),
      nMVFlgRecordFrame(cParam.nMVFlgRecordFrame)
{
    dMVRTConvert = 0;

    pMVFormatCtx = NULL;
    pMVCodecCtxOrig = NULL;
    pMVCodecCtx = NULL;
    pMVCodec = NULL;

    pMVFrame = NULL;
    pMVTargetFrame = NULL;
    pMVBuffer = NULL;

    iMVVideoStream = 0;
    pMVSwsCtx = NULL;

    // No init no quit
    nMVFlgQuit = 1;
    nMVNbrFrame = 0;
    nMVNbrTplFrame = 0;
    nMVFlgFndDup = 0;

    if (sMVFileName != "")
    {
        MVInit(sMVFileName);
    }
}

CMyVideo::CMyVideo(string argv,
                   size_t nFlgCap,
                   size_t nRecordFrameFlag)
{
    pMVFormatCtx = NULL;
    pMVCodecCtxOrig = NULL;
    pMVCodecCtx = NULL;
    pMVCodec = NULL;
    pMVFrame = NULL;
    pMVTargetFrame = NULL;
    pMVSwsCtx = NULL;
    pMVBuffer = NULL;

    dMVRTConvert = 0;
    iMVVideoStream = 0;
    nMVFlgCap = nFlgCap;
    nMVFlgRecordFrame = nRecordFrameFlag;
    // No init no quit
    nMVFlgQuit = 1;
    nMVNbrFrame = 0;
    nMVNbrTplFrame = 0;
    nMVFlgFndDup = 0;

    MVInit(argv);
}

CMyVideo::~CMyVideo()
{
    MVQuit();
}

// Les methodes privees

void CMyVideo::MVInitFrameFormat(AVPixelFormat format,
                                 int width, int height)
{
    // Allocate an AVFrame structure
    pMVTargetFrame = av_frame_alloc();
    if (pMVTargetFrame == NULL)
    {
        exit(-1);
    }

    pMVSwsCtx = sws_getContext(pMVCodecCtx->width,
                               pMVCodecCtx->height,
                               pMVCodecCtx->pix_fmt,
                               width, height,
                               format,
                               SWS_BILINEAR,
                               NULL,
                               NULL,
                               NULL);

    av_image_alloc(pMVTargetFrame->data,
                   pMVTargetFrame->linesize,
                   width, height,
                   format, 32);
}

void CMyVideo::MVInitTargetFrame()
{
    switch (nMVFlgCap)
    {
    case EVENT_MY_DET_CAP:
        MVStoreTypeProc("MyDupDetect");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          9,
                          8);
        pMVTargetFrame->width = 9;
        pMVTargetFrame->height = 8;
        iMVRTCount = 0;
        break;
    case EVENT_MY_DET_TPL:
        MVStoreTypeProc("TemplateMyGRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          9,
                          8);
        pMVTargetFrame->width = 9;
        pMVTargetFrame->height = 8;
        break;
    case EVENT_CAP_RCD_MY_DET:
        // MVStoreTypeProc("RecordMyDetect");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          9,
                          8);
        pMVTargetFrame->width = 9;
        pMVTargetFrame->height = 8;
        iMVRTCount = 0;
        break;
    case EVENT_CAP_RCD_DET_GRAY_8:
        // MVStoreTypeProc("Record9X8Detect");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          9,
                          8);
        pMVTargetFrame->width = 9;
        pMVTargetFrame->height = 8;
        iMVRTCount = 0;
        break;
    case EVENT_DET_TPL_FRM_GRAY_8:
        MVStoreTypeProc("Template9X8GRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          9,
                          8);
        pMVTargetFrame->width = 9;
        pMVTargetFrame->height = 8;
        break;
    case EVENT_CAP_DET_FRM_GRAY_8:
        MVStoreTypeProc("Detect9X8GRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          9,
                          8);
        pMVTargetFrame->width = 9;
        pMVTargetFrame->height = 8;
        break;
    case EVENT_CAP_FRM_GRAY_64_SIZE:
        // MVStoreTypeProc("64X64GRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          64,
                          64);
        pMVTargetFrame->width = 64;
        pMVTargetFrame->height = 64;
        iMVRTCount = 0;
        break;
    case EVENT_CAP_INV_GRAY_420:
        MVStoreTypeProc("Inverse420X240GRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          420,
                          240);
        pMVTargetFrame->width = 420;
        pMVTargetFrame->height = 240;
        break;
    case EVENT_CAP_ROT_GRAY_420:
        MVStoreTypeProc("Rotate420X240GRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          420,
                          240);
        pMVTargetFrame->width = 420;
        pMVTargetFrame->height = 240;
        break;
    case EVENT_CAP_MY_FRM_GRAY_420:
        MVStoreTypeProc("Process420X240GRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          420,
                          240);
        pMVTargetFrame->width = 420;
        pMVTargetFrame->height = 240;
        break;
    case EVENT_CAP_FRM_GRAY_420_SIZE:
        MVStoreTypeProc("420X240GRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          420,
                          240);
        pMVTargetFrame->width = 420;
        pMVTargetFrame->height = 240;
        break;
    case EVENT_CAP_FRM_GRAY_DOWN_SIZE:
        MVStoreTypeProc("OnethirdGRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          pMVCodecCtx->width / 3,
                          pMVCodecCtx->height / 3);
        pMVTargetFrame->width = pMVCodecCtx->width / 3;
        pMVTargetFrame->height = pMVCodecCtx->height / 3;
        break;
    case EVENT_CAP_FRM_GRAY:
        MVStoreTypeProc("NormalGRAY");
        MVInitFrameFormat(AV_PIX_FMT_GRAY8,
                          pMVCodecCtx->width,
                          pMVCodecCtx->height);
        pMVTargetFrame->width = pMVCodecCtx->width;
        pMVTargetFrame->height = pMVCodecCtx->height;
        break;

    case EVENT_CAP_FRM_RGB:
    default:
        MVStoreTypeProc("NormalRGB");
        MVInitFrameFormat(AV_PIX_FMT_RGB24,
                          pMVCodecCtx->width,
                          pMVCodecCtx->height);
        pMVTargetFrame->width = pMVCodecCtx->width;
        pMVTargetFrame->height = pMVCodecCtx->height;
    }
}

void CMyVideo::MVConvertFrame()
{
    // Convert the image from its native format to target format
    sws_scale(pMVSwsCtx,
              (uint8_t const *const *)pMVFrame->data,
              pMVFrame->linesize, 0, pMVCodecCtx->height,
              pMVTargetFrame->data, pMVTargetFrame->linesize);

    switch (nMVFlgCap)
    {
    // My optimal processing record detection
    case EVENT_MY_DET_CAP:
    {
        // Five more times we found it is duplicate
        // Then we consider it is duplicate
        MVDetectDup2();
        break;
    }
    // My optimal processing template
    case EVENT_MY_DET_TPL:
    {
        break;
    }
    // My optimal processing record detection
    case EVENT_CAP_RCD_MY_DET:
    {
        // Five more times we found it is duplicate
        // Then we consider it is duplicate
        MVDetectWithRT2();
        break;
    }
    // My processing record detection
    case EVENT_CAP_RCD_DET_GRAY_8:
    {
        // Five more times we found it is duplicate
        // Then we consider it is duplicate
        MVDetectWithRT();
        break;
    }
    // My processing template
    case EVENT_DET_TPL_FRM_GRAY_8:
    {
        break;
    }
    // My processing detection
    case EVENT_CAP_DET_FRM_GRAY_8:
    {
        // Five more times we found it is duplicate
        // Then we consider it is duplicate
        MVDetectDup();
        break;
    }
    // My processing - Inverse
    case EVENT_CAP_INV_GRAY_420:
    {
        CMyFrame MFInv(sMVFileName.substr(0, sMVFileName.find_first_of('.')),
                       pMVTargetFrame,
                       RECORD_NO_FRAME);
        MFInv.MFInverse();
        break;
    }
    // My processing - Rotate
    case EVENT_CAP_ROT_GRAY_420:
    {
        CMyFrame MFRot(sMVFileName.substr(0, sMVFileName.find_first_of('.')),
                       pMVTargetFrame,
                       RECORD_NO_FRAME);
        MFRot.MFRotate(180);
        break;
    }
    // My processing
    case EVENT_CAP_MY_FRM_GRAY_420:
    {
        CMyFrame(sMVFileName.substr(0, sMVFileName.find_first_of('.')),
                 pMVTargetFrame,
                 RECORD_NO_FRAME);
        break;
    }
    default:;
    }
}

void CMyVideo::MVConvertWithRT()
{
    auto msRTConvert1 = high_resolution_clock::now();
    MVConvertFrame();
    auto msRTConvert2 = high_resolution_clock::now();
    // floating-point duration: no duration_cast needed
    duration<double, std::milli> msRTConvert = msRTConvert2 - msRTConvert1;
    dMVRTConvert = msRTConvert.count();

    switch (nMVFlgCap)
    {
    case EVENT_CAP_RCD_MY_DET:
        // Store only the avg
        break;
    case EVENT_CAP_RCD_DET_GRAY_8:
        // Store only the avg
        break;
    case EVENT_CAP_FRM_GRAY_64_SIZE:
        // Store only the avg
        dMVRTAverage = (dMVRTAverage * iMVRTCount + dMVRTConvert) / ++iMVRTCount;
        break;
    default:
        MVStoreRT("Convert", dMVRTConvert);
    }
}

void CMyVideo::MVSaveFrame(int iFrame)
{
    cout << "Saving frame" << iFrame << endl;

    FILE *pFile;
    char szFilename[32];
    int y;

    // Open file
    sprintf(szFilename, "frame/%s%d-%d.pgm",
            sMVFileName.substr(0, sMVFileName.find_last_of('.')).c_str(),
            nMVFlgCap, iFrame);

    // Store in vector to observer
    string sFrameName(szFilename);
    sFrameName = sFrameName.substr(sFrameName.find_first_of('/') + 1,
                                   sFrameName.find_last_of('.') -
                                       sFrameName.find_first_of('/') - 1);

    CMyFrame MFRecord(sFrameName,
                      pMVTargetFrame,
                      RECORD_FRAME);
    vMVFrames.push_back(MFRecord);

    // Write in ppm
    pFile = fopen(szFilename, "wb");
    if (pFile == NULL)
    {
        return;
    }

    // Write header
    fprintf(pFile, "P5\n%d %d\n255\n",
            pMVTargetFrame->width,
            pMVTargetFrame->height);

    // Write pixel data
    for (y = 0; y < pMVTargetFrame->height; y++)
    {
        fwrite(pMVTargetFrame->data[0] +
                   y * pMVTargetFrame->linesize[0],
               1, pMVTargetFrame->width, pFile);
    }

    // Close file
    fclose(pFile);
}

void CMyVideo::MVLoadDb()
{
    cout << "Loading template db" << endl;
    ifstream ifs;
    ifs.open(string("frame/Template.txt").c_str(),
             ifstream::in);
    if (ifs.is_open())
    {
        string sTpl;
        while (getline(ifs, sTpl))
        {
            // cout << sTpl << endl;
            vMVTemplate.push_back(sTpl);
        }
        ifs.close();
    }
    else
    {
        cout << "Can not find template" << endl;
    }
}

void CMyVideo::MVDetectDup()
{
    CMyFrame MFDupDet(sMVFileName.substr(0, sMVFileName.find_first_of('.')),
                      pMVTargetFrame,
                      RECORD_NO_FRAME);
    string sCurKey = MFDupDet.MFObtainKey();
    if (find(vMVTemplate.begin(), vMVTemplate.end(),
             sCurKey) != vMVTemplate.end())
    {
        // Five more times we found it is duplicate
        // Then we consider it is duplicate
        if (++nMVFlgFndDup == 5)
        {
            cout << "Found a duplicate video ! " << sMVFileName << endl;
            // terminate();
        }
    }
}

void CMyVideo::MVDetectDup2()
{
    CMyFrame MFDupDet(sMVFileName.substr(0, sMVFileName.find_first_of('.')),
                      pMVTargetFrame,
                      RECORD_NO_FRAME);
    string sCurKey = MFDupDet.MFObtainKey2();
    if (find(vMVTemplate.begin(), vMVTemplate.end(),
             sCurKey) != vMVTemplate.end())
    {
        // Five more times we found it is duplicate
        // Then we consider it is duplicate
        if (++nMVFlgFndDup == 5)
        {
            cout << "Found a duplicate video ! " << sMVFileName << endl;
            // exit(0);
        }
    }
}

void CMyVideo::MVDetectWithRT()
{
    auto msRTDetect1 = high_resolution_clock::now();
    MVDetectDup();
    auto msRTDetect2 = high_resolution_clock::now();

    // floating-point duration: no duration_cast needed
    duration<double, milli> msRTDetect = msRTDetect2 - msRTDetect1;
    dMVRTDetect = msRTDetect.count();

    // Store only the avg
    dMVRTAverage = (dMVRTAverage * iMVRTCount + dMVRTDetect) / ++iMVRTCount;
}

void CMyVideo::MVDetectWithRT2()
{
    auto msRTDetect1 = high_resolution_clock::now();
    MVDetectDup2();
    auto msRTDetect2 = high_resolution_clock::now();

    // floating-point duration: no duration_cast needed
    duration<double, milli> msRTDetect = msRTDetect2 - msRTDetect1;
    dMVRTDetect = msRTDetect.count();

    // Store only the avg
    dMVRTAverage = (dMVRTAverage * iMVRTCount + dMVRTDetect) / ++iMVRTCount;
}

void CMyVideo::MVEvent()
{
    switch (nMVFlgCap)
    {
    // Extract template for detection
    case EVENT_MY_DET_TPL:
    {
        if (nMVNbrTplFrame == 0)
        {
            ofstream ofs;
            ofs.open(string("frame/Template.txt").c_str(),
                     ofstream::out | ofstream::trunc);
            ofs.close();
        }

        if (nMVNbrTplFrame <= MAX_TEMPLATE_FRAME_NOMBRE)
        {
            nMVNbrTplFrame++;
            // Five second one template
            if (nMVNbrTplFrame % (23 * 5) == 0)
            {
                CMyFrame MFRecordTpl("Template",
                                     pMVTargetFrame,
                                     RECORD_NO_FRAME);

                MFRecordTpl.MFExtractTpl2();
            }
        }
        break;
    }
    // Extract template for detection
    case EVENT_DET_TPL_FRM_GRAY_8:
    {
        if (nMVNbrTplFrame == 0)
        {
            ofstream ofs;
            ofs.open(string("frame/Template.txt").c_str(),
                     ofstream::out | ofstream::trunc);
            ofs.close();
        }

        if (nMVNbrTplFrame <= MAX_TEMPLATE_FRAME_NOMBRE)
        {
            nMVNbrTplFrame++;
            // Five second one template
            if (nMVNbrTplFrame % (23 * 5) == 0)
            {
                CMyFrame MFRecordTpl("Template",
                                     pMVTargetFrame,
                                     RECORD_NO_FRAME);

                MFRecordTpl.MFExtractTpl();
            }
        }
        break;
    }
    default:;
    }

    // Record frame and its pix table
    if (nMVFlgRecordFrame != RECORD_NO_FRAME)
    {
        // Save the frame to disk
        if (nMVNbrFrame <= MAX_FRAME_NOMBRE)
        {
            nMVNbrFrame++;
            if (nMVNbrFrame % 20 == 0)
            {
                MVSaveFrame(nMVNbrFrame);
            }
        }
    }
}

void CMyVideo::MVStoreTypeProc(string sType)
{
    // Record RT
    ofstream ofsFile;
    ofsFile.open(sMVFile, ofstream::out | ofstream::app);

    // Record the type we are processing
    ofsFile << "TypeProcessing " << sType << endl;

    ofsFile.close();
}

void CMyVideo::MVStoreRT(string sRTType, double dRT)
{
    // Record RT
    ofstream ofsFile;
    ofsFile.open(sMVFile, ofstream::out | ofstream::app);

    ofsFile << sRTType << " " << dRT << endl;

    ofsFile.close();
}

// Les methodes publiques

void CMyVideo::MVModifierCapFlag(size_t nFlgCap)
{
    nMVFlgCap = nFlgCap;
}

void CMyVideo::MVInit(string argv)
{
    // If it has not been init yet
    if (nMVFlgQuit == 1)
    {
        // cout << "Current thread : " << this_thread::get_id() << endl;

        // No init no quit
        nMVFlgQuit = 0;
        sMVFileName = argv;
        sMVFile = "record/" +
                  sMVFileName.substr(0, argv.find_last_of('.')) +
                  to_string(nMVFlgCap) + ".txt";

        // Record RT and reset the file
        if (nMVFlgCap != EVENT_CAP_FRM_GRAY_64_SIZE &&
            nMVFlgCap != EVENT_CAP_RCD_DET_GRAY_8 &&
            nMVFlgCap != EVENT_CAP_RCD_MY_DET)
        {
            ofstream ofsFile;
            ofsFile.open(sMVFile, ofstream::out | ofstream::trunc);
            ofsFile.close();
        }

        // Register all formats and codecs
        av_register_all();

        // Open video file
        if (avformat_open_input(&pMVFormatCtx, argv.c_str(), NULL, NULL) != 0)
        {
            exit(-1); // Couldn't open file
        }

        // Retrieve stream information
        if (avformat_find_stream_info(pMVFormatCtx, NULL) < 0)
        {
            exit(-1); // Couldn't find stream information
        }

        // Dump information about file onto standard error
        av_dump_format(pMVFormatCtx, 0, argv.c_str(), 0);

        // Find the first video stream
        iMVVideoStream = -1;
        for (int i = 0; i < pMVFormatCtx->nb_streams; i++)
        {
            if (pMVFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                iMVVideoStream = i;
                break;
            }
        }
        if (iMVVideoStream == -1)
        {
            exit(-1); // Didn't find a video stream
        }

        // Get a pointer to the codec context for the video stream
        pMVCodecCtxOrig = pMVFormatCtx->streams[iMVVideoStream]->codec;

        // Find the decoder for the video stream
        pMVCodec = avcodec_find_decoder(pMVCodecCtxOrig->codec_id);
        if (pMVCodec == NULL)
        {
            fprintf(stderr, "Unsupported codec!\n");
            exit(-1); // Codec not found
        }

        // Copy context
        pMVCodecCtx = avcodec_alloc_context3(pMVCodec);
        if (avcodec_copy_context(pMVCodecCtx, pMVCodecCtxOrig) != 0)
        {
            fprintf(stderr, "Couldn't copy codec context");
            exit(-1); // Error copying codec context
        }

        // Open codec
        if (avcodec_open2(pMVCodecCtx, pMVCodec, NULL) < 0)
        {
            exit(-1); // Could not open codec
        }

        // Allocate video frame
        pMVFrame = av_frame_alloc();

        MVInitTargetFrame();

        if (nMVFlgCap == EVENT_CAP_DET_FRM_GRAY_8 ||
            nMVFlgCap == EVENT_CAP_RCD_DET_GRAY_8 ||
            nMVFlgCap == EVENT_CAP_RCD_MY_DET ||
            nMVFlgCap == EVENT_MY_DET_CAP)
        {
            MVLoadDb();
        }
    }
}

void CMyVideo::MVReadFrame()
{
    int frameFinished = 0;

    while (av_read_frame(pMVFormatCtx, &MVPacket) >= 0)
    {
        // Is this a packet from the video stream?
        if (MVPacket.stream_index == iMVVideoStream)
        {
            // Decode video frame
            avcodec_decode_video2(pMVCodecCtx, pMVFrame,
                                  &frameFinished, &MVPacket);

            // Did we get a video frame?
            if (frameFinished)
            {
                MVConvertWithRT();
                MVEvent();
            }
        }

        // Free the packet that was allocated by av_read_frame
        av_free_packet(&MVPacket);
    }
}

void CMyVideo::MVQuit()
{
    if (nMVFlgQuit == 0)
    {
        switch (nMVFlgCap)
        {
        case EVENT_CAP_RCD_MY_DET:
        {
            // Store only the avg
            // Record RT
            ofstream ofsFile;
            ofsFile.open(sMVFile, ofstream::out | ofstream::app);
            ofsFile << dMVRTAverage << endl;
            ofsFile.close();
            break;
        }
        case EVENT_CAP_RCD_DET_GRAY_8:
        {
            // Store only the avg
            // Record RT
            ofstream ofsFile;
            ofsFile.open(sMVFile, ofstream::out | ofstream::app);
            ofsFile << dMVRTAverage << endl;
            ofsFile.close();
            break;
        }
        case EVENT_CAP_FRM_GRAY_64_SIZE:
        {
            // Store only the avg
            // Record RT
            ofstream ofsFile;
            ofsFile.open(sMVFile, ofstream::out | ofstream::app);
            ofsFile << dMVRTAverage << endl;
            ofsFile.close();

            // MVStoreRT("ConvertAvg", dMVRTAverage);
            break;
        }
        default:;
        }

        // Free the RGB image
        av_free(pMVBuffer);
        av_free(pMVTargetFrame->data[0]);
        av_frame_free(&pMVTargetFrame);

        // Free the YUV frame
        av_frame_free(&pMVFrame);

        // Close the codecs
        avcodec_close(pMVCodecCtx);
        avcodec_close(pMVCodecCtxOrig);

        // Close the video file
        avformat_close_input(&pMVFormatCtx);

        avcodec_free_context(&pMVCodecCtx);

        // Free sws
        sws_freeContext(pMVSwsCtx);

        nMVFlgQuit = 1;
    }
}

void CMyVideo::MVTask(string argv,
                      size_t nFlgCap,
                      size_t nRecordFrameFlag)
{
    nMVFlgCap = nFlgCap;
    nMVFlgRecordFrame = nRecordFrameFlag;
    MVInit(argv);
    MVReadFrame();
    MVQuit();
}
