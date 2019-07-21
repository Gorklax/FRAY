#ifndef _hsd_util_h_
#define _hsd_util_h_

#include <gctypes.h>

#include <ogc/gu.h>

#define FLT_EPSILON 1.19209290E-07F

#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

typedef struct _HSD_SList {
    struct _HSD_SList *next;
    void* data;
} HSD_SList;

typedef struct _HSD_Spline {
    u16 type; //0x00
    u16 numcv; //0x02
    f32 tension; //0x04
    guVector* cv; //0x08
    f32 totalLength; //0x0C
    f32* segLength; //0x10
    f32* segPoly[5]; //0x14
} HSD_Spline;

typedef union _update {
	f32 fv;
    s32 iv;
    guVector p;
} update;

f32 HSD_ClampFloat(f32 val, f32 min, f32 max);
f32 splGetHermite(f32, f32, f32, f32, f32, f32);
void splArcLengthPoint(HSD_Spline *, f32, guVector *);

#endif
