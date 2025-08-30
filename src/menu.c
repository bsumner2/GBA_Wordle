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

    if (K_STROKE(START)) {
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

    if (K_STROKE(UP)) {
      if (0 > --row) {
        row = 2;
      } else if (row==1 && col==8) {
        col = 7;
      }

      kbd_cursor_moved = true;
      continue;
    } else if (K_STROKE(DOWN)) {
       if (2 < ++row) {
         row = 0;
       } else if (row==1 && col==8) {
          col = 7;
       }

       kbd_cursor_moved = true;
       continue;
    }
    
    if (K_STROKE(LEFT)) {
      if (0 > --col) {
        col = row&1?7:8;
      }
      kbd_cursor_moved = true;
      continue;
    } else if (K_STROKE(RIGHT)) {
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


    
    if (K_STROKE(A)) {
      if (cursor+1 > 15) {
        continue;
      }
      rect.x = name_dsp_x_off + 7*cursor;
      mode3_draw_rect(&rect);
      tmp = ROW_START_CHARS[row] + col;
      mode3_putchar(tmp, rect.x+1, rect.y+1, 0x10A5);
      return_buf[cursor++] = tmp;
    } else if (K_STROKE(B)) {
      if (0 > --cursor) {
        cursor=0;
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

#define PFS_PER_SECT 4

#define SEL_STATE_CHOSE_EXISTING_PF 1
#define SEL_STATE_CREATE_NEW_PF 2
#define SEL_STATE_UNDECIDED 0

static void PrintAccount(const SaveProfile_t *pf, int idx, bool is_highlighted) {
  static BMP_Rect_t rect = {
    .width = (6+PROF_NAME_MAX_LEN)*4+4,
    .height = 12,
    .x = (SCREEN_WIDTH-(6+PROF_NAME_MAX_LEN)*4-4)/2,
  };
  rect.y = 22 + idx*14;
  if (NULL==pf)
    rect.color = 0;
  else if (is_highlighted) rect.color = 0x0883;
  else rect.color = 0x2485;
  mode3_draw_rect(&rect);
  if (NULL!=pf)
    mode3_printf(rect.x+2, rect.y+2, 0x8000, "(%02d): %s", pf->acct_idx+1, pf->name);
}


void MainMenu(SaveProfile_t *return_loaded_pf) {
  BMP_Rect_t rect = {0};
  int ct = Get_Save_Profiles(profiles), len, state, sel, scroll_sect, i, chosen_idx;
  const int SCROLL_SECTS=(ct+(PFS_PER_SECT-1))/PFS_PER_SECT;
  bool reset_gui;
  REG_DISPLAY_CNT = 0x0403;
  mode3_clear_screen();
  if (ct) {
    for (reset_gui = true, state=SEL_STATE_UNDECIDED, scroll_sect=0, sel = 0, Poll_Keys();
        SEL_STATE_UNDECIDED == state; 
        vsync(), Poll_Keys()) {
      if (reset_gui) {
        reset_gui = false;
        rect.width = 14*4+4;
        rect.height = 12;
        rect.x = (SCREEN_WIDTH-rect.width)/2;
        rect.y = 8;
        rect.color = 0x0883;
        mode3_draw_rect(&rect);
        mode3_printf(rect.x+2, rect.y+2, 0x10A5, "Select Account");
 
        rect.width = 55*4+4;
        rect.x = (SCREEN_WIDTH-rect.width)/2;
        rect.y = SCREEN_HEIGHT-2-rect.height;
        mode3_draw_rect(&rect);
        mode3_printf(rect.x+2, rect.y+2, 0x10A5, "\x1b[0x2485][^/v]\x1b[0x10A5]: Scroll \x1b[0x2485][A]\x1b[0x10A5]: Select account \x1b[0x2485][B]\x1b[0x10A5]: Make new account");
        for (i = scroll_sect = 0; i < PFS_PER_SECT; ++i) {
          PrintAccount(i < ct ? &profiles[i] : NULL, i, (const int)i==sel);
        }
      }
      state = K_STROKE(UP)?KEY_UP:(K_STROKE(DOWN)?KEY_DOWN:0);
      if (KEY_UP==state) {
        state = SEL_STATE_UNDECIDED;
        if (!sel) {
          if (1==SCROLL_SECTS) {
            sel = ct-1;
            PrintAccount(&profiles[0], 0, false);
            PrintAccount(&profiles[sel], sel, true);
            continue;
          }
          if (0 < scroll_sect) {
            --scroll_sect;
            sel = PFS_PER_SECT - 1;
          } else {
            scroll_sect = SCROLL_SECTS-1;
            sel = ct - scroll_sect*PFS_PER_SECT;
          }
          for (i = 0; i < PFS_PER_SECT; ++i) {
            PrintAccount(i+scroll_sect*PFS_PER_SECT < ct ? &profiles[i+scroll_sect*PFS_PER_SECT] : NULL, i, (const int)i==sel);
          }
          continue;
        }
        PrintAccount(&profiles[sel+scroll_sect*PFS_PER_SECT], sel, false);
        --sel;
        PrintAccount(&profiles[sel+scroll_sect*PFS_PER_SECT], sel, true);
        continue;
      } else if (KEY_DOWN==state) {
        state = SEL_STATE_UNDECIDED;
        if ((PFS_PER_SECT-1 == sel) || 
            (SCROLL_SECTS-1 == scroll_sect 
                && ct - scroll_sect*PFS_PER_SECT - 1 == sel)) {
          if (1 == SCROLL_SECTS) {
            sel = 0;
            PrintAccount(&profiles[ct-1], ct-1, false);
            PrintAccount(&profiles[sel], sel, true);
            continue;
          }
          if (++scroll_sect >= SCROLL_SECTS) {
            scroll_sect = 0;
          }
          sel = 0;
          for (i=0; i<PFS_PER_SECT; ++i) {
            PrintAccount(i+scroll_sect*PFS_PER_SECT < ct ? &profiles[i+scroll_sect*PFS_PER_SECT] : NULL, i, (const int)i==sel);
          }
          continue;
        }
        PrintAccount(&profiles[sel+scroll_sect*PFS_PER_SECT], sel, false);
        ++sel;
        PrintAccount(&profiles[sel+scroll_sect*PFS_PER_SECT], sel, true);
        continue;
      }
      state |= (K_STROKE(A) ? SEL_STATE_CHOSE_EXISTING_PF : 0)
            | (K_STROKE(B) ? SEL_STATE_CREATE_NEW_PF : 0);

      if (SEL_STATE_UNDECIDED == state)
        continue;
      if (0!=(SEL_STATE_CREATE_NEW_PF&state)) {
        if (0!=(SEL_STATE_CHOSE_EXISTING_PF&state)) {
          state = SEL_STATE_UNDECIDED;
          continue;
        }
        mode3_clear_screen();
        continue;
      }

      chosen_idx = sel+scroll_sect*PFS_PER_SECT;
      mode3_clear_screen();
      rect.width = (21+strlen(profiles[chosen_idx].name))*4+4;
      rect.height = 12;
      rect.x = (SCREEN_WIDTH-rect.width)/2;
      rect.y = 8;
      rect.color= 0x0883;
      mode3_draw_rect(&rect);
      mode3_printf(rect.x+2, rect.y+2, 0x10A5, "Choose profile \x1b[0x5180]#%02d\x1b[0x10A5]: \x1b[0x1C18]%s\x1b[0x10A5]?", chosen_idx, profiles[chosen_idx].name);
      rect.width = 18*4+4;
      rect.x = (SCREEN_WIDTH-rect.width)/2;
      rect.y = SCREEN_HEIGHT - 2 - rect.height;
      mode3_draw_rect(&rect);
      mode3_printf(rect.x+2, rect.y+2, 0x10A5, "\x1b[0x2485][A]\x1b[0x10A5]: Yes | \x1b[0x2485][B]\x1b[0x10A5]: No");
      for (vsync(), Poll_Keys(); 
          !(K_STROKE(A) ^ K_STROKE(B));
          vsync(), Poll_Keys())
        continue;
      mode3_clear_screen();
      if (K_STROKE(A)) {
        continue;
      }
      state = SEL_STATE_UNDECIDED;
      sel = 0;
      scroll_sect = 0;
      reset_gui = true;
      continue;
    }
  }
  if (!ct || 0!=(SEL_STATE_CREATE_NEW_PF&state)) {
    rect.width = 17*4+4;
    rect.height = 12;
    rect.x = (SCREEN_WIDTH-rect.width)/2;
    rect.y = 8;
    rect.color = 0x0883;
    mode3_draw_rect(&rect);
    mode3_printf(rect.x+2, rect.y+2, 0x10A5, !ct?"No Accounts Found":"No Account Chosen");
    rect.x = (SCREEN_WIDTH-38*4)/2 + 6*4;
    rect.y = 120-1;
    rect.width = 4*7;
    rect.height = 9;
    rect.color = 0x2861;
    mode3_draw_rect(&rect);
    mode3_printf(rect.x - 6*4, rect.y+1, 0x8000, "Press [START] to create a new account!");
    do vsync(); while (KEY_START&Poll_Keys());
    do vsync(); while (!(KEY_START&Poll_Keys()));
    fast_memset32(return_loaded_pf, 0, sizeof(SaveProfile_t)/4);
    len = Prompt_Profile_Name(return_loaded_pf->name);
    mode3_clear_screen();
    return_loaded_pf->rand_state = LFSR_GetState();
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
  *return_loaded_pf = *(profiles+chosen_idx);
  len = strlen(return_loaded_pf->name);
  mode3_printf((SCREEN_WIDTH-(10+len)*4)/2, 76, 0x2485, "Welcome, %s!", return_loaded_pf->name);
  len=60;
  do vsync(); while (--len);
  return;
}
