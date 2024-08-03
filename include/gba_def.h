#ifndef _GBA_DEF_H_
#define _GBA_DEF_H_

#include "gba_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REG_BASE          0x04000000
#define MEM_PAL           0x05000000
#define MEM_VRAM          0x06000000
#define MEM_OAM           0x07000000
#define MEM_GAMEPAK_SRAM  0x0E000000

#define SCREEN_HEIGHT 160
#define SCREEN_WIDTH  240

#define REG_DISPLAY_CNT (*((volatile u32*) (REG_BASE)))
#define REG_DISPLAY_STAT (*((volatile u16*) (REG_BASE+0x0004)))
#define REG_DISPLAY_VCOUNT (*((volatile u16*) (REG_BASE+0x0006)))

#define REG_IME (*((volatile u16*) (REG_BASE + 0x0208)))
#define REG_IE  (*((volatile u16*) (REG_BASE + 0x0200)))
#define REG_IF  (*((volatile u16*) (REG_BASE + 0x0202)))
#define REG_IFBIOS (*((volatile u16*) (0x03007FF8)))

#define REG_ISR_MAIN (*((IRQ_Callback_t*) (0x03007FFC)))

#define REG_KEY_STAT (*((volatile u16*) (REG_BASE+0x0130)))

#define KEY_A     1
#define KEY_B     2
#define KEY_SEL   4
#define KEY_START 8
#define KEY_RIGHT 16
#define KEY_LEFT  32
#define KEY_UP    64
#define KEY_DOWN  128
#define KEY_R     256
#define KEY_L     512


#define VRAM_BUF ((u16*) MEM_VRAM)

#define TILE_MEM ((Charblock*) MEM_VRAM)
#define TILE8_MEM ((Charblock8*) MEM_VRAM)

#define PAL_BG_MEM ((u16*) MEM_PAL)
#define PAL_OBJ_MEM ((u16*) (MEM_PAL + 0x0200))

#define OAM_MEM ((Obj_Attrs_t*) MEM_OAM)
#define AFFINE_MEM ((Obj_Affine_t*) MEM_OAM)

#define SRAM_BUF ((u8*) (MEM_GAMEPAK_SRAM))


#define OBJ_MODE_REGULAR 0
#define OBJ_MODE_AFFINE 1
#define OBJ_MODE_HIDE 2
#define OBJ_MODE_AFFINE_X2RENDERING 3


#ifdef __cplusplus
}
#endif









#endif  /* _GBA_DEF_H_ */
