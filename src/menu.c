#include "gba_types.h"
#include "gba_util_macros.h"
#include "gba_def.h"
#include "gba_funcs.h"
#include "input.h"
#include "mode3_io.h"
#include "save_interface.h"
#include <string.h>


EWRAM_DATA SaveProfile_t profiles[MAX_SAVE_PROFILES];


int Prompt_Profile_Name(char *return_buf) {
  static const int ROW_STARTS[3][2] = {
    {(SCREEN_WIDTH-((9+8)*4))/2 - 3, 24+3},
    {(SCREEN_WIDTH-((8+7)*4))/2 - 3, 34+3},
    {(SCREEN_WIDTH-((9+8)*4))/2 - 3, 44+3}
  };
  static const char ROW_START_CHARS[] = "AJR";
  static const int name_dsp_x_off = (SCREEN_WIDTH-((6*PROF_NAME_MAX_LEN) + PROF_NAME_MAX_LEN - 1))/2;


  BMP_Rect_t rect = {
    .x = name_dsp_x_off,
    .y = 8,
    .width = 6,
    .height = 10,
    .color = 0x0883
  }, kbd_cursor = {
    .x = ROW_STARTS[0][0],
    .y = ROW_STARTS[0][1],
    .width = 2,
    .height = 2,
    .color = 0x6BFE
  }, error_label = {
    .x = (SCREEN_WIDTH-(30*4))/2,
    .y = 64,
    .width = 30*4,
    .height = 8,
    .color = 0
  };
  int row=0, col=0, cursor=0;
  u16 key_stat=0;
  char tmp;
  bool kbd_cursor_moved = true;
  mode3_clear_screen();

  for (int i = 0; i < 15; ++i, rect.x +=7) {
    mode3_draw_rect(&rect);
    
    mode3_putchar((tmp=return_buf[i])?tmp:' ', rect.x+1, rect.y+1, 0x10A5);
  }
  cursor = strlen(return_buf);

  mode3_printf(ROW_STARTS[0][0]+3, 24, 0x8000, "A B C D E F G H I");
  mode3_printf(ROW_STARTS[1][0]+3, 34, 0x8000, "J K L M N O P Q");
  mode3_printf(ROW_STARTS[2][0]+3, 44, 0x8000, "R S T U V W X Y Z");
  
  mode3_printf(0, SCREEN_HEIGHT-16, 0x8000, "Press \x1b[0x18CE][B]\x1b[0x8000] for Backspace\nPress \x1b[0x10A5][START]\x1b[0x8000] to Confirm!");
  while (1) {
    vsync();
    if (kbd_cursor_moved) {
      kbd_cursor.color = 0;
      mode3_draw_rect(&kbd_cursor);
      kbd_cursor.x += 8;
      mode3_draw_rect(&kbd_cursor);
      kbd_cursor.x = ROW_STARTS[row][0] + 8*col + 8;
      kbd_cursor.y = ROW_STARTS[row][1];
      kbd_cursor.color = 0x6BFE;
      mode3_draw_rect(&kbd_cursor);
      kbd_cursor.x -= 8;
      mode3_draw_rect(&kbd_cursor);
      kbd_cursor_moved = false;
    }
    Poll_Keys();

    if (!(KEY_START&KEY_CURR) && (KEY_PREV&KEY_START)) {
      if (!cursor) {
        mode3_printf((SCREEN_WIDTH-(30*4))/2, 64, 0x1CB1, "Cannot use empty profile name!");
        tmp = 60;
        while (0 < tmp--)
          vsync();
        mode3_draw_rect(&error_label);
        continue;
      }
      break; 
    }

    if (!(KEY_CURR&KEY_UP) && (KEY_PREV&KEY_UP)) {
      if (0 > --row) {
        row = 2;
      } else if (row==1 && col==8) {
        col = 7;
      }

      kbd_cursor_moved = true;
      continue;
    } else if (!(KEY_CURR&KEY_DOWN) && (KEY_PREV&KEY_DOWN)) {
       if (2 < ++row) {
         row = 0;
       } else if (row==1 && col==8) {
          col = 7;
       }

       kbd_cursor_moved = true;
       continue;
    }
    
    if (!(KEY_CURR&KEY_LEFT) && (KEY_PREV&KEY_LEFT)) {
      if (0 > --col) {
        col = row&1?7:8;
      }
      kbd_cursor_moved = true;
      continue;
    } else if (!(KEY_CURR&KEY_RIGHT) && (KEY_PREV&KEY_RIGHT)) {
      if (row&1) {
        if (7 < ++col) {
          col = 0;
        }
      } else if (8 < ++col) {
        col = 0;
      }
      kbd_cursor_moved = true;
      continue;
    }


    
    if (!(KEY_CURR&KEY_A) && (KEY_PREV&KEY_A)) {
      if (cursor+1 > 15) {
        continue;
      }
      rect.x = name_dsp_x_off + 7*cursor;
      mode3_draw_rect(&rect);
      tmp = ROW_START_CHARS[row] + col;
      mode3_putchar(tmp, rect.x+1, rect.y+1, 0x10A5);
      return_buf[cursor++] = tmp;
    } else if (!(KEY_CURR&KEY_B) && (KEY_PREV&KEY_B)) {
      if (0 > --cursor) {
        continue;
      }
      rect.x = name_dsp_x_off + 7*cursor;
      mode3_draw_rect(&rect);
      mode3_putchar(' ', rect.x+1, rect.y+1, 0x10A5);
      return_buf[cursor] = '\0';
    }


    

  }
  return cursor;

}


void MainMenu(SaveProfile_t *return_loaded_pf) {
  BMP_Rect_t rect = {0};
  int ct = Get_Save_Profiles(profiles), len;
  REG_DISPLAY_CNT = 0x0403;
  mode3_clear_screen();
  if (!ct) {
    rect.width = 17*4+4;
    rect.height = 12;
    rect.x = (SCREEN_WIDTH-rect.width)/2;
    rect.y = 8;
    rect.color = 0x0883;
    mode3_draw_rect(&rect);
    mode3_printf(rect.x+2, 10, 0x10A5, "No Accounts Found");
    rect.x = (SCREEN_WIDTH-38*4)/2 + 6*4;
    rect.y = 120-1;
    rect.width = 4*7;
    rect.height = 9;
    rect.color = 0x2861;
    mode3_draw_rect(&rect);
    mode3_printf(rect.x - 6*4, 120, 0x8000, "Press [START] to create a new account!");
    do vsync(); while (KEY_START&Poll_Keys());
    do vsync(); while (!(KEY_START&Poll_Keys()));
    fast_memset32(return_loaded_pf, 0, sizeof(SaveProfile_t)/4);
    len = Prompt_Profile_Name(return_loaded_pf->name);
    mode3_clear_screen();
    mode3_printf((SCREEN_WIDTH-28*4)/2, 72, 0x1CB1, "Saving profile to cartridge!");
    mode3_printf((SCREEN_WIDTH-44*4)/2, 80, 0x1CB1, "Please do not power off or remove cartridge!");
    Add_Save_Profile(return_loaded_pf);
    mode3_clear_screen();
    mode3_printf((SCREEN_WIDTH-(10+len)*4)/2, 76, 0x2485, "Welcome, %s!", return_loaded_pf->name);
    len=60;
    do vsync(); while (--len);
    mode3_clear_screen();
    return;
  }
  mode3_clear_screen();
  *return_loaded_pf = *profiles;
  len = strlen(return_loaded_pf->name);
  mode3_printf((SCREEN_WIDTH-(10+len)*4)/2, 76, 0x2485, "Welcome, %s!", return_loaded_pf->name);
  len=60;
  do vsync(); while (--len);
  mode3_clear_screen();
  return;
  




}
