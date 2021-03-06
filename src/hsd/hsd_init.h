#ifndef _hsd_init_h_
#define _hsd_init_h_

#include <gctypes.h>
#include <ogc/system.h>
#include <ogc/video_types.h>
#include <stdarg.h>

#include "hsd_debug.h"
#include "hsd_memory.h"
#include "hsd_video.h"

#define HSD_DEFAULT_FIFO_SIZE 0x40000
#define HSD_DEFAULT_XFB_MAX_NUM 2
#define HSD_DEFAULT_AUDIO_SIZE 0x80000

#define ROUNDDOWN32(v) (((u32)(v)) & ~(32 - 1))
#define ROUNDUP32(v) (((u32)(v) + 32 - 1) & ~(32 - 1))

typedef enum _HSD_InitParam {
    HSD_INIT_FIFO_SIZE,
    HSD_INIT_XFB_MAX_NUM,
    HSD_INIT_HEAP_MAX_NUM,
    HSD_INIT_AUDIO_HEAP_SIZE,
    HSD_INIT_RENDER_MODE_OBJ
} HSD_InitParam;

typedef enum _HSD_CacheBit {
    HSD_CACHE_NONE = 0,
    HSD_CACHE_VTX = 1,
    HSD_CACHE_TEX = 2
} HSD_CacheBit;

void HSD_InitComponent(void);
void HSD_GXSetFifoObj(GXFifoObj*);
void HSD_DVDInit(void);
void** HSD_AllocateXFB(u32, GXRModeObj*);
void* HSD_AllocateFIFO(u32);
void HSD_GXInit(void);

HSD_RenderPass HSD_GetCurrentRenderPass(void);
void HSD_StartRender(HSD_RenderPass);
void _HSD_NeedCacheInvalidate(HSD_CacheBit);

void HSD_ObjInit(void);
void HSD_ObjDumpStat(void);
BOOL HSD_SetInitParameter(HSD_InitParam, ...);

#endif
