#ifndef _gobj_h_
#define _gobj_h_

#include <assert.h>

#include <gctypes.h>

#include "hsd/hsd_jobj.h"

#define GOBJ_CLASS_PLAYER 0x04
#define GOBJ_CLASS_ITEM 0x06

#define GOBJ_NOREF 0xFF

//This has 0x20 aligned size - assuming this is actually something related to HSD_Memory and can be scrapped later
typedef struct _GObj_LinkedList {
	struct _GObj_LinkedList* prev;
	struct _GObj_LinkedList* next;
	int size;
	int data[5];
} GObj_LinkedList;

typedef struct _HSD_GObj {
	u16 classifier;
	u8 p_link;
	u8 gx_link;
	u8 p_priority;
	u8 render_priority;
	s8 obj_kind;
	s8 data_kind;
	struct _HSD_GObj* next;
	struct _HSD_GObj* prev;
	struct _HSD_GObj* next_gx;
	struct _HSD_GObj* prev_gx;
	GObj_LinkedList* unk_linkedlist;
	void (*render_cb)(struct _HSD_GObj *gobj, int code);
	void* unk20;
	void* unk24;
	void* data;
	void* hsd_obj; //JObj or CObj known
	void (*user_data_remove_func)(void* data);
	GObj_LinkedList* unk_linkedlist2;
	GObj_LinkedList* unk_linkedlist3;
} HSD_GObj;

void GObj_Free(HSD_GObj* gobj);
void GObj_CallHSDDestructor(HSD_GObj* gobj);
void GObj_CallDestructor(HSD_GObj *gobj);
bool GObj_IsPlayer(HSD_GObj* gobj);

#endif