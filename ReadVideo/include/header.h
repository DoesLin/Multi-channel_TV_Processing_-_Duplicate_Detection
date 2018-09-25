#ifndef _header_h
#define _header_h

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <dlfcn.h>
}

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#ifndef _my_record_
#define _my_record_

#define RECORD_NO_FRAME 0
#define RECORD_FRAME 1

#define NOT_TPL 0
#define TPL 1

#endif

#ifndef _my_event_
#define _my_event_

#define EVENT_NONE 0                   // NormalRGB
#define EVENT_CAP_FRM_RGB 1            // NormalRGB
#define EVENT_CAP_FRM_GRAY 2           // NormalGRAY
#define EVENT_CAP_FRM_GRAY_DOWN_SIZE 3 // OnethirdGRAY
#define EVENT_CAP_FRM_GRAY_420_SIZE 4  // 420X240GRAY
#define EVENT_CAP_MY_FRM_GRAY_420 5    // Process420X240GRAY
#define EVENT_CAP_ROT_GRAY_420 6       // Rotate420X240GRAY
#define EVENT_CAP_INV_GRAY_420 7       // Inverse420X240GRAY
#define EVENT_CAP_FRM_GRAY_64_SIZE 8   // 64X64GRAY
#define EVENT_DET_TPL_FRM_GRAY_8 9     // Template9X8GRAY
#define EVENT_CAP_DET_FRM_GRAY_8 10    // Detect9X8GRAY
#define EVENT_CAP_RCD_DET_GRAY_8 11    // Record9X8Detect
#define EVENT_CAP_RCD_MY_DET 12        // RecordMyDetect
#define EVENT_MY_DET_TPL 13            // TemplateMyGRAY
#define EVENT_MY_DET_CAP 14            // MyDupDetect

#define MAX_FRAME_NOMBRE 100
#define MAX_TEMPLATE_FRAME_NOMBRE 23 * 60 //50s

#endif

#endif
