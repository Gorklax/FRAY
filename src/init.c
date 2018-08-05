#include "init.h"

static void *frameBuffer[2] = { NULL, NULL};
static u32 fb = 0;
static GXRModeObj *rmode = NULL;
static f32 yscale;
static u32 xfbHeight;

static Mtx view, mv;
static Mtx44 perspective;

void guPerspective1(Mtx44 mt,f32 fovy,f32 aspect,f32 n,f32 f)
{
	f32 cot,angle,tmp;

	angle = fovy*0.5f;
	angle = DegToRad(angle);
	
	cot = 1.0f/tanf(angle);

	mt[0][0] = cot/aspect;
	mt[0][1] = 0.0f;
	mt[0][2] = 0.0f;
	mt[0][3] = 0.0f;

	mt[1][0] = 0.0f;
	mt[1][1] = cot;
	mt[1][2] = 0.0f;
	mt[1][3] = 0.0f;
	
	tmp = 1.0f/(f-n);
	mt[2][0] = 0.0f;
	mt[2][1] = 0.0f;
	mt[2][2] = -(n)*tmp;
	mt[2][3] = -(f*n)*tmp;
	
	mt[3][0] = 0.0f;
	mt[3][1] = 0.0f;
	mt[3][2] = -1.0f;
	mt[3][3] = 0.0f;
}

//8015FEB4
int main(void){
	GXColor background = {0, 0, 0, 0xff};
	
	SYS_Init();
	VIDEO_Init();
	DVD_Init();
	PAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	
	// allocate 2 framebuffers for double buffering
	frameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	frameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	
	void *gp_fifo = NULL;
	gp_fifo = memalign(32,DEFAULT_FIFO_SIZE);
	memset(gp_fifo,0,DEFAULT_FIFO_SIZE);
	
	GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);
	
	GX_SetCopyClear(background, 0x00ffffff);
	
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
	GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));
 
	GX_SetCullMode(GX_CULL_NONE);
	GX_CopyDisp(frameBuffer[fb],GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);
	
	// setup our camera at the origin
	// looking down the -z axis with y up
	guVector cam = {0.0F, 0.0F, 0.0F},
			up = {0.0F, 1.0F, 0.0F},
		  look = {0.0F, 0.0F, -1.0F};
	guLookAt(view, &cam, &up, &look);
	
	// setup our projection matrix
	// this creates a perspective matrix with a view angle of 90,
	// and aspect ratio based on the display resolution
    f32 w = rmode->viWidth;
    f32 h = rmode->viHeight;
	guMtxIdentity(mv);
	guPerspective1(perspective, 45, (f32)w/h, 0.1F, 300.0F);
	GX_LoadProjectionMtx(perspective, GX_PERSPECTIVE);
	
	Game_Init();
	
	return 0;
}


//801A4510
void Game_Init(){
	u8 curr_major; // r3@11
	
	memset(&GameState, 0, 20);
	for (u32 i = 0; ; i += 1 )
	{
		if ( SceneData[i].idx == 45 )
		{
			/*if ( VI_GetDTVStatus(result) && (*(_WORD *)(v2 - 19312) & 0x200 || OS_GetProgressiveMode() == 1) )
				byte_80479D30 = 39;
			else
				byte_80479D30 = 40;*/
			GameState.curr_major = 40;
			GameState.prev_major = 45;
			while ( 1 )
			{
				curr_major = *Game_MainLoop(GameState.curr_major);
				if ( dword_8046B0F0.unk04 )
					dword_8046B0F0.unk04 = 0;
				GameState.prev_major = GameState.curr_major;
				GameState.curr_major = curr_major;
			}
		}
		if ( *(u32*)(i + 12) )
			break;
	}
}

//801A43A0
u8* Game_MainLoop(u8 scene){
	struct SceneData *v3; // r30@3
	u32 v4; // r4@5
	u32 *result; // r3@7

	for (u32 i = 0; ; i += 1 )
	{
		v4 = SceneData[i].idx;
		if ( v4 == 45 )
			break;
		if ( v4 == scene )
		{
			v3 = &SceneData[i];
			goto LABEL_7;
		}
	}
	v3 = 0;
	LABEL_7:
	GameState.pending = false;
	GameState.unk03 = 0;
	GameState.unk04 = 0;
	GameState.unk05 = 0;
	//result = (u32 *)sub_80018F58(*v3);
	if ( !*((u32*)v3 + 1) )
	{
		result = 0;
		while ( !GameState.pending )
		{
			if ( GameState.unk10 )
				return (u8*)result;
			//result = Scene_RunFunc(v3);
		}
		result = &dword_8046B0F0;
		if ( dword_8046B0F0.unk04 || !*((u32*)v3 + 2) )
			result = &GameState.pending_major;
	}
	return (u8*)result;
}

/*
void Game_Loop(){
	PAD_ScanPads();
	
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	
	//Draw Functions
	
	GX_DrawDone();
	
	
	fb ^= 1;		// flip framebuffer
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
	GX_CopyDisp(frameBuffer[fb],GX_TRUE);

	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_Flush();
	VIDEO_WaitVSync();
}*/