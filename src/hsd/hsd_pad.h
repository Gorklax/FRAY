#ifndef _hsd_pad_h_
#define _hsd_pad_h_

#include <gctypes.h>
#include <ogc/irq.h>
#include <ogc/pad.h>

typedef enum _HSD_FlushType {
    HSD_PAD_FLUSH_QUEUE_MERGE = 0,
    HSD_PAD_FLUSH_QUEUE_THROWAWAY = 1,
    HSD_PAD_FLUSH_QUEUE_LEAVE1 = 2,
    HSD_PAD_FLUSH_QUEUE_TERMINATE = 3
} HSD_FlushType;

typedef struct _RumbleCommand {
    u16 op;
    u16 frame;
} RumbleCommand;

typedef union _HSD_Rumble {
    u16 def;
    struct _RumbleCommand command;
} HSD_Rumble;

typedef struct _HSD_RumbleData {
    u8 last_status;
    u8 status;
    u8 direct_status;
    u16 nb_list;
    struct _HSD_PadRumbleListData* listdatap;
} HSD_RumbleData;

typedef struct _HSD_PadRumbleListData {
    struct _HSD_PadRumbleListData* next;
    u32 id;
    u8 pause;
    u8 pri;
    u8 status;
    u16 loop_count;
    u16 wait;
    s32 frame;
    HSD_Rumble* stack;
    HSD_Rumble* listp;
    HSD_Rumble* headp;
} HSD_PadRumbleListData;

typedef struct _HSD_PadData {
    PADStatus stat[4];
    u32 rumble_mask;
} HSD_PadData;

typedef struct _HSD_PadStatus {
    u32 button;
    u32 last_button;
    u32 trigger;
    u32 repeat;
    u32 release;
    s32 repeat_count;
    s8 stickX;
    s8 stickY;
    s8 subStickX;
    s8 subStickY;
    u8 analogL;
    u8 analogR;
    u8 analogA;
    u8 analogB;
    f32 nml_stickX;
    f32 nml_stickY;
    f32 nml_subStickX;
    f32 nml_subStickY;
    f32 nml_analogL;
    f32 nml_analogR;
    f32 nml_analogA;
    f32 nml_analogB;
    u8 cross_dir;
    u8 support_rumble;
    s8 err;
} HSD_PadStatus;

typedef struct _PadLibData {
    u8 qnum;
    u8 qread;
    u8 qwrite;
    u8 qcount;
    u8 qtype;
    struct _HSD_PadData* queue;
    s32 repeat_start;
    s32 repeat_interval;
    u8 adc_type;
    s8 adc_th;
    f32 adc_angle;
    u8 clamp_stickType;
    u8 clamp_stickShift;
    s8 clamp_stickMax;
    s8 clamp_stickMin;
    u8 clamp_analogLRShift;
    u8 clamp_analogLRMax;
    u8 clamp_analogLRMin;
    u8 clamp_analogABShift;
    u8 clamp_analogABMax;
    u8 clamp_analogABMin;
    s8 scale_stick;
    u8 scale_analogLR;
    u8 scale_analogAB;
    u8 cross_dir;
    u8 reset_switch_status;
    u8 reset_switch;
    struct {
        u16 max_list;
        struct _HSD_PadRumbleListData* listdatap;
    } rumble_info;
} PadLibData;

extern PadLibData HSD_PadLibData;

u8 HSD_PadGetRawQueueCount(void);
s32 HSD_PadGetResetSwitch(void);
void HSD_PadRenewRawStatus(void);
void HSD_PadFlushQueue(HSD_FlushType);
void HSD_PadClampCheck1(u8*, u8, u8, u8);
void HSD_PadClampCheck3(s8*, s8*, u8, s8, s8);
void HSD_PadRenewMasterStatus(void);
void HSD_PadRenewCopyStatus(void);
void HSD_PadZeroQueue(void);
void HSD_PadRenewStatus(void);
void HSD_PadReset(void);
void HSD_PadInit(u8, HSD_PadData*, u16, HSD_PadRumbleListData*);
void HSD_PadRumbleOn(u8);
void HSD_PadRumbleOffH(u8);
void HSD_PadRumbleFree(HSD_RumbleData*, HSD_PadRumbleListData*);
void HSD_PadRumbleRemoveAll(void);
void HSD_PadRumbleInterpret(void);
void HSD_PadRumbleInit(u16, HSD_PadRumbleListData*);

#endif
