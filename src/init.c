#include "init.h"

#include <gccore.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "hsd/hsd_init.h"
#include "hsd/hsd_pad.h"
#include "hsd/hsd_video.h"

#include "ogcext/dvd.h"

#include "scene/scene.h"
#include "util.h"

static u32 arena_size; //-0x5110(r13)
static u64 sys_time;

GXRModeObj* rmode = NULL;

HSD_PadData pad_queue[PAD_QUEUE_SIZE]; //8046B108
HSD_PadRumbleListData rumble_list[PAD_RUMBLE_LIST_SIZE]; //8036B1F8

//80019AAC
void Init_PadReader(void (*init_func)(void))
{
    (*init_func)();
    //TODO
}

//8015FD24
void Pad_InitCallback()
{
    PAD_SetSpec(PAD_QUEUE_SIZE);
    HSD_PadInit(PAD_QUEUE_SIZE, pad_queue, PAD_RUMBLE_LIST_SIZE, rumble_list);
    HSD_PadLibData.clamp_stickType = 0;
    HSD_PadLibData.clamp_stickShift = 1;
    HSD_PadLibData.clamp_stickMax = 80;
    HSD_PadLibData.clamp_stickMin = 0;
    HSD_PadLibData.clamp_analogLRShift = 1;
    HSD_PadLibData.clamp_analogLRMax = 140;
    HSD_PadLibData.clamp_analogLRMin = 0;
    HSD_PadLibData.scale_stick = 80;
    HSD_PadLibData.scale_analogLR = 140;
}

//8015FEB4
int main(void)
{
    VIDEO_Init();
    DVDInit(); //Calls FRAY's custom OGCExt DVDInit

    arena_size = (u32)SYS_GetArena1Hi() - (u32)SYS_GetArena1Lo();
    rmode = &TVNtsc480IntDf;

    HSD_SetInitParameter(1, 2); //Set XFB Max Num
    HSD_SetInitParameter(4, rmode); //Set RModeObj
    HSD_SetInitParameter(0, 0x40000); //Set FifoSize
    HSD_SetInitParameter(2, 4); //Set Heap Size
    //sub_80228C4C - Checks DebuggerIsPresent, so may be used to init the leak checker or something of that nature

    HSD_AllocateXFB(2, rmode);
    void* fifo = HSD_AllocateFIFO(0x40000);
    HSD_GXSetFifoObj(GX_Init(fifo, 0x40000));

    HSD_InitComponent();

    GX_SetMisc(GX_MT_XF_FLUSH, GX_XF_FLUSH_SAFE); //Normally GX_SetMisc(1, 8), which isn't valid

    sys_time = SYS_Time();

    //sub_8002838C(); Inits AR, ARQ, and AI - "Audio_Init"

    Init_PadReader(Pad_InitCallback);

    HSD_VISetUserPostRetraceCallback(stub);
    HSD_VISetUserGXDrawDoneCallback(HSD_VIDrawDoneXFB);
    HSD_VISetBlack(0);
    //sub_8001564C(); Allocs memory in AR
    //sub_80015F3C();
    //sub_80018F68();
    //sub_80014D2C();
    //sub_8001C5BC();
    //sub_8001D21C();
    //sub_8001E290();
    //sub_8015FCC0();
    //sub_8001F87C();

    //sub_803A6048(0xC000); FirstHeapAlloc
    //InitializeStaticMemRegions();
    printf("# -------------------------------------------\n");
    printf("#\tSuper Smash Bros. Melee\n");
    printf("# Arena Size %u MB\n", arena_size >> 0x14);
    printf("# DATE %s TIME %s\n", __DATE__, __TIME__);

    Init_Game();
    return 0;
}

//801A4510
void Init_Game()
{
    memset(&gamestate, 0, 20);

    MajorScene* major_scenes = Scene_GetMajorScenes();
    for (u32 i = 0; major_scenes[i].idx != 45; i += 1) {
        if (major_scenes[i].Init) { //For things such as VS, this points to a function that allocates the memory for StartMelee, etc..
            major_scenes[i].Init();
        }
    }

    if ((VIDEO_HaveComponentCable() /* && (rmode->viTVMode & 0x02)*/) /*|| OS_GetProgressiveMode() == 1)*/) {
        gamestate.curr_major = 39;
    } else {
        gamestate.curr_major = 40;
    }
    gamestate.curr_major = 0; //DEBUG
    gamestate.prev_major = 45;

    while (1) {
        u8 curr_major = *Scene_ProcessMajor(gamestate.curr_major);
        if (dword_8046B0F0.unk04)
            dword_8046B0F0.unk04 = 0;
        gamestate.prev_major = gamestate.curr_major;
        gamestate.curr_major = curr_major;
    }
}
