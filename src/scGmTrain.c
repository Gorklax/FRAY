#include "scGmTrain.h"

//801B2298
u32 Menu_GmTrain_Init(){
  s8 *v1; // r31@1
  s32 v2; // ctr@1
  u8 *v3; // r6@1
  s32 v4; // r5@1
  u32 v5; // r7@1
  u32 v6; // r7@4
  u32 v7; // r0@4
  u32 v8; // r0@4
  u32 v9; // r0@4
  u32 v10; // r0@4
  u32 *v11; // r6@4
  s32 *v12; // r5@4
  u32 v13; // r0@6
  u32 v14; // r0@6
  u32 v15; // r0@6
  u32 result; // r3@6
  u32 v17; // r0@6
  u32 v18; // r0@6

  v1 = (s8*)&sm_train; //StartMelee ptr
  Scene_InitUsableStructs(v1);
  v2 = 2;
  v3 = 0; //byte_80473814
  v4 = (s32)v1;
  v5 = 0;
  do
  {
    *(u8 *)(v4 + 107) = v5;
    *(u8 *)(v4 + 118) = 0;
    if ( v5 )
      v1[140] = 33;
    v6 = v5 + 1;
    v7 = *(s32 *)(v4 + 108);
    *((s32 *)v3 + 5) = *(s32 *)(v4 + 104);
    *((s32 *)v3 + 6) = v7;
    v8 = *(s32 *)(v4 + 116);
    *((s32 *)v3 + 7) = *(s32 *)(v4 + 112);
    *((s32 *)v3 + 8) = v8;
    v9 = *(s32 *)(v4 + 124);
    *((s32 *)v3 + 9) = *(s32 *)(v4 + 120);
    *((s32 *)v3 + 10) = v9;
    v10 = *(s32 *)(v4 + 132);
    *((s32 *)v3 + 11) = *(s32 *)(v4 + 128);
    *((s32 *)v3 + 12) = v10;
    *((s32 *)v3 + 13) = *(s32 *)(v4 + 136);
    v11 = v3 + 36;
    *(u8 *)(v4 + 143) = v6;
    *(u8 *)(v4 + 154) = 0;
    v12 = (s32 *)(v4 + 36);
    if ( v6 )
      v1[140] = 33;
    v5 = v6 + 1;
    v13 = v12[27];
    v11[5] = v12[26];
    v11[6] = v13;
    v14 = v12[29];
    v11[7] = v12[28];
    v11[8] = v14;
    v15 = v12[31];
    v11[9] = v12[30];
    v11[10] = v15;
    result = v12[32];
    v17 = v12[33];
    v11[11] = result;
    v11[12] = v17;
    v18 = v12[34];
    v4 = (s32)(v12 + 9);
    v11[13] = v18;
    v3 = (u8 *)(v11 + 9);
    --v2;
  }
  while ( v2 );
  return result;
}