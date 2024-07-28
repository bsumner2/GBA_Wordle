#include "gba_def.h"
#include "input.h"
#include "mode3_io.h"
#include "gba_funcs.h"
#include "gba_types.h"
#include "rng.h"
#include "save_interface.h"
#include "wordle_letters.h"
#include <string.h>
#include <sys/unistd.h>


#ifdef TEST_TXT_IO

int main(void) {
  REG_DISPLAY_CNT = 0x0403;
  memset(VRAM_BUF, 0x10A510A5, SCREEN_WIDTH*SCREEN_HEIGHT*2);
  mode3_printf(0,0, "Hello, world! How are you doing today?");

  while (1) {
  }

}

#else
const char *KBD_LAYOUT = "qwertyuiopasdfghjklzxcvbnm";


static const int KBD_ROW_LENS[3] = {10,9,7};

typedef struct {
  Obj_Attrs_t *keys;
  const char *letters[3];
  u32 cur_row, cur_col, cur_letter;
} Keyboard_Ctx_t;


void enable_interrupts(void) {
  REG_IME = 0;
  REG_IE = 1;
  REG_DISPLAY_STAT = 8;
  REG_ISR_MAIN = isr_handler_cb;
  REG_IME = 1;
}


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

void init_board(Obj_Attrs_t *objs) {
  int cur=0;
  static const int x_ofs_init = 98; /* (240 - ((4 margin px) + (5 tiles * 8 px/tile)))/2=(240-44)/2=  */

  int x, 
      y=16;  /* Arbitrarily-chosen top margin ofs. 
              * Set aligned to top so as to leave room for keyboard */ 
  for (int i = 0; i < 6; ++i) {
    x = x_ofs_init;
    for (int j = 0; j < 5; ++j) {
      objs[cur].attr0.attrs.y=y;
      objs[cur].attr1.attrs_reg.x=x;

      objs[cur+30].attr0.raw = 0x0200|y;  // set hide flag and y pos.
      objs[cur+++30].attr1.attrs_reg.x=x;
      x+=9;
    }
    y+=9;
  }
  oam_cpy(OAM_MEM, objs, 60);
}





void init_kbd(Keyboard_Ctx_t *kbd) {
  Obj_Attrs_t *objs = kbd->keys;
  int row_len, cur=0;
  int x, y = 80;
  const char *row_layout = KBD_LAYOUT;
  for (int row=0; row < 3; ++row) {
    row_len = KBD_ROW_LENS[row];
    kbd->letters[row] = row_layout;
    x = (240 - ((row_len-1) + (row_len*8)))/2;
    for (int i = 0; i < row_len; ++i) {
      objs[cur].attr0.attrs.y=y;
      objs[cur].attr1.attrs_reg.x=x;
      objs[cur++].attr2.attr.tile_idx = (row_layout[i] - 'a')+1;
      x+=9;
    }
    y+=9;
    row_layout += row_len;
  }

  objs[0].attr2.attr.palbank = 4;

  oam_cpy(&OAM_MEM[60], objs, 26);

}

Obj_Attrs_t obj_buf[128];

void pause_game(void) {
  fast_memset32(VRAM_BUF, 0, SCREEN_WIDTH*SCREEN_HEIGHT/2  /* unit conversion: (SCR_W*SCR_H) pixels / 2 pixels / word */);
  REG_DISPLAY_CNT = 0x0403;
  int pause_tally;
  SRAM_Read(&pause_tally, sizeof(int), 0);
  if (pause_tally < 0) {
    pause_tally=1;
    SRAM_Write(&pause_tally, sizeof(int), 0);
  } else {
    ++pause_tally;
    SRAM_Write(&pause_tally, sizeof(int), 0);
  }

  mode3_printf(0, 0, 0x8000, "pause count: %d", pause_tally);
  

  while ((REG_KEY_STAT&KEY_START))
    vsync();

  do vsync(); while (!(REG_KEY_STAT&KEY_START));

  REG_DISPLAY_CNT = 0x1000;
  memcpy(&TILE_MEM[4][0], WordleLettersTiles, WordleLettersTilesLen);
}


void StartScreen(void) {
  REG_DISPLAY_CNT = 0x0403;
  
 // mode3_printf((240-30*4)/2, 16, 0x8000, "A Burt \"Lin\" Sumner Production");
  mode3_printf((240-18*4)/2, 48, 0x8000, "Wordle Boy Advance");
  vsync();

  u32 rand_seed = 0;
  u16 key_stat_snapshot;
  _Bool start_prompted = 0;
  for ( ; ; ) {
    vsync();
    if ((key_stat_snapshot = (~REG_KEY_STAT)&0x03FF)) {
      rand_seed ^= (rand_seed<<10) ^ key_stat_snapshot;
      if (!start_prompted) {
          mode3_printf((SCREEN_WIDTH - 26*4)/2, SCREEN_HEIGHT - 16, 0x10A5, "[Press START to Continue!]");
          start_prompted = 1;
          while (!(REG_KEY_STAT&KEY_START)) vsync();
      } else if (((KEY_START&key_stat_snapshot))) {
        break;
      } else {
        continue;
      }

    }
  }
  
  LFSR_Init((((~rand_seed)<<16)&0xFFFF0000)|(0x0000FFFF&rand_seed), (u32[FEEDBACK_POLY_DEGREE]){23, 22, 21, 16});
  fast_memset32(VRAM_BUF, 0, SCREEN_WIDTH*SCREEN_HEIGHT/2);
  while (!(REG_KEY_STAT&KEY_START)) vsync(); 
  
}


void MainMenu(SaveProfile_t *profile);
int main(void) {
#if 1
  enable_interrupts();
  StartScreen();
  SaveProfile_t prof;
  MainMenu(&prof);
  fast_memcpy32(&TILE_MEM[4][0], WordleLettersTiles, WordleLettersTilesLen/4);
  fast_memcpy32(PAL_OBJ_MEM, WordleLettersPal, WordleLettersPalLen/4);
  
  init_board(obj_buf);
  Keyboard_Ctx_t kbd = {0};
  kbd.keys = &obj_buf[60];
  kbd.cur_letter = 'q';

  init_kbd(&kbd);
  oam_init_ofs(&obj_buf[86], 128-86, 86);
  REG_DISPLAY_CNT = 0x1000;
 
  while (1) {
    if (!(REG_KEY_STAT&KEY_START)) {
      do vsync(); while (!(REG_KEY_STAT&KEY_START));
      pause_game();
    }
    vsync();
  }

#elif 0
  enable_interrupts();
  StartScreen();
  u16 kst8;
  while (1) {
    vsync();
    kst8 = Poll_Keys();
    if (((~kst8)&0x3FF)) {
      do vsync(); while (~Poll_Keys()&0x3FF);
      if (!(kst8&KEY_SEL)) {
        mode3_printf(0,8,0,"Rand yield: %08X", LFSR_Rand());
      }
      mode3_printf(0,0,0,"LFSR State: %08X", LFSR_DEBUGFUNC_GetState());
    }
  }
  return 0;
#else
  enable_interrupts();
  StartScreen();
  SaveProfile_t prof;
  MainMenu(&prof);
  
  while (1);
#endif

}

#endif
