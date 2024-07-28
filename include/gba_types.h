#ifndef _GBA_TYPES_H_
#define _GBA_TYPES_H_

#include "gba_util_macros.h"
#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#include <stdbool.h>
#endif
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

typedef union u_obj_attr0 Obj_Attr0_t;

union u_obj_attr0 {
  u16 raw;
  struct {
    u16 y : 8;
    u16 obj_mode : 2;
    u16 gfx_mode : 2;
    u16 mosaic : 1;
    u16 bpp8 : 1;
    u16 shape : 2; 
  } ALIGN(2) attrs;
};

typedef union u_obj_attr1 Obj_Attr1_t;

union u_obj_attr1 {
  u16 raw;
  struct {
    u16 x : 9;
    u16 fill : 3;
    u16 hflip : 1;
    u16 vflip : 1;
    u16 size : 2;

  } ALIGN(2) attrs_reg;  // Regular sprite attr
  
  struct {
    u16 x : 9;
    u16 aff_idx : 5;
    u16 size : 2;

  } ALIGN(2) attrs_aff;  // Affine sprite attr

};

typedef union u_obj_attr2 Obj_Attr2_t;

union u_obj_attr2 {
  u16 raw;
  struct {
    u16 tile_idx : 10;
    u16 priority : 2;
    u16 palbank : 4;
  } ALIGN(2) attr;
};


typedef struct s_obj_attr Obj_Attrs_t;


struct s_obj_attr {
  Obj_Attr0_t attr0;
  Obj_Attr1_t attr1;
  Obj_Attr2_t attr2;
  i16 fill;

} ALIGN(4);


typedef struct s_obj_aff Obj_Affine_t;

struct s_obj_aff {

  u16 fill0[3];
  i16 pa;
  u16 fill1[3];
  i16 pb;
  u16 fill2[3];
  i16 pc;
  u16 fill3[3];
  i16 pd;
} ALIGN(4);


typedef u32 Tile[8];
typedef u32 Tile8[16];


typedef Tile Charblock[512];
typedef Tile8 Charblock8[256];

typedef void (*IRQ_Callback_t)(void);

typedef struct bmp_rect {
    u32 x, y, width, height;
    u16 color;
} BMP_Rect_t;

#ifdef __cplusplus
}
#endif





#endif  /* _GBA_TYPES_H_ */
