#ifndef _hsd_video_h_
#define _hsd_video_h_

#include <assert.h>

#include <gctypes.h>
#include <ogc/gx.h>
#include <ogc/irq.h>
#include <ogc/video.h>
#include <ogc/video_types.h>

#define HSD_VI_XFB_MAX 3
#define HSD_ANTIALIAS_OVERLAP 4
#define VI_DISPLAY_PIX_SZ 2
#define HSD_ANTIALIAS_GARBAGE_SIZE (640 * HSD_ANTIALIAS_OVERLAP * VI_DISPLAY_PIX_SZ)

typedef void (*HSD_VIGXDrawDoneCallback)(s32);

typedef enum _HSD_VIXFBDrawDispStatus {
    HSD_VI_XFB_NONE,
    HSD_VI_XFB_NOUSE,
    HSD_VI_XFB_FREE,
    HSD_VI_XFB_DRAWING,
    HSD_VI_XFB_WAITDONE,
    HSD_VI_XFB_DRAWDONE,
    HSD_VI_XFB_NEXT,
    HSD_VI_XFB_DISPLAY,
    HSD_VI_XFB_COPYEFB,
    HSD_VI_XFB_TERMINATE
} HSD_VIXFBDrawDispStatus;

typedef enum _HSD_VIEFBDrawDispStatus {
    HSD_VI_EFB_FREE,
    HSD_VI_EFB_DRAWDONE,
    HSD_VI_EFB_TERMINATE
} HSD_VIEFBDrawDispStatus;

typedef enum _HSD_RenderPass {
    HSD_RP_SCREEN,
    HSD_RP_TOPHALF,
    HSD_RP_BOTTOMHALF,
    HSD_RP_OFFSCREEN,
    HSD_RP_NUM
} HSD_RenderPass;

typedef struct _HSD_VIStatus {
    GXRModeObj rmode;
    u8 black;
    u8 vf;
    u8 gamma;
    GXColor clear_clr;
    u32 clear_z;
    u8 update_clr;
    u8 update_alpha;
    u8 update_z;
} HSD_VIStatus;

typedef struct _current {
    struct _HSD_VIStatus vi;
    BOOL chg_flag;
} Current;

typedef struct _XFB {
    void* buffer;
    HSD_VIXFBDrawDispStatus status;
    Current vi_all;
} XFB;

typedef struct _HSD_VIInfo {
    Current current;

    XFB xfb[HSD_VI_XFB_MAX];

    struct EFB {
        HSD_VIEFBDrawDispStatus status;
        Current vi_all;
    } efb;

    u32 nb_xfb;
    
    VIRetraceCallback pre_cb;
    VIRetraceCallback post_cb;
    
    struct drawdone {
        s32 waiting;
        s32 arg;
        HSD_VIGXDrawDoneCallback cb;
    } drawdone;

    struct perf {
        u32 frame_period;
        u32 frame_renew;
    } perf;

} HSD_VIInfo;

VIRetraceCallback HSD_VISetUserPreRetraceCallback(VIRetraceCallback);
VIRetraceCallback HSD_VISetUserPostRetraceCallback(VIRetraceCallback);
HSD_VIGXDrawDoneCallback HSD_VISetUserGXDrawDoneCallback(HSD_VIGXDrawDoneCallback);
s32 HSD_VIGetXFBDrawEnable(void);
void HSD_VICopyEFB2XFBPtr(HSD_VIStatus*, void*, HSD_RenderPass);
s32 HSD_VIWaitXFBDrawEnable(void);
void HSD_VIGXDrawDone(s32);
void HSD_VICopyXFBASync(HSD_RenderPass);
void HSD_VIDrawDoneXFB(s32);
void HSD_VISetXFBWaitDone(s32);
void HSD_VISetXFBDrawDone(void);
s32 HSD_VIGetXFBLastDrawDone(void);
void HSD_VISetEFBDrawDone(void);
void HSD_VISetConfigure(GXRModeObj*);
void HSD_VISetBlack(u8);
void HSD_VIInit(HSD_VIStatus*, void*, void*, void*);

#endif
