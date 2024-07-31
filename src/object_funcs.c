#include "gba_def.h"
#include "gba_types.h"



void oam_cpy(Obj_Attrs_t *dst, const Obj_Attrs_t *src, u32 ct) {
#if 1
  while (ct--) {
    *dst++=*src++;
  }
#else
  u32 *rawd = (u32*)dst, *raws = (u32*)src;
  while (ct--) {
    *rawd++ = *raws++;
    *rawd++ = *raws++;
  }

#endif

}

void oam_init(Obj_Attrs_t *objs, u32 ct) {
  u32 *raw = (u32*)objs, cur = ct;
   
  while (cur--) {
    *raw++ = 0x0200;
    *raw++ = 0;
  }
  oam_cpy(OAM_MEM, objs, ct);
}

void oam_init_ofs(Obj_Attrs_t *objs, u32 ct, u32 ofs) {
  u32 *raw = (u32*)objs, cur=ct;
  while (cur--)
    *raw++=0x0200,*raw++=0;
  oam_cpy(&OAM_MEM[ofs], objs, ct);
}


void obj_affine_cpy(Obj_Affine_t *dst, const Obj_Affine_t *src, u32 ct) {
  while (ct--) {
    dst->pa = src->pa;
    dst->pb = src->pb;
    dst->pc = src->pc;
    dst++->pd = src++->pd;
  }
}
