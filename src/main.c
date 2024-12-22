#include "gba_def.h"
#include "input.h"
#include "mode3_io.h"
#include "gba_funcs.h"
#include "gba_types.h"
#include "rng.h"
#include "save_interface.h"
#include "wordle_letters.h"
#include "words.h"
#include "answers.h"
#include "wordlist.h"
#include "answerlist.h"
#include <string.h>



#define TILE_OFS 512



static const char KBD_LAYOUT[] = "qwertyuiopasdfghjklzxcvbnm";
static u8 KBD_LOCATIONS[26];
static const int KBD_ROW_LENS[3] = {10,9,7};

static Obj_Attrs_t obj_buf[128];
static Obj_Attrs_t *attempt_buf[6];


typedef struct {
  Obj_Attrs_t *keys;
  const char *letters[3];
  i32 cur_row, cur_col;
  i16 cur_letter, buf_cursor, cur_letter_prev_palbank;
  char buf[6];
} Keyboard_Ctx_t;

typedef struct {
  const char *word;
  u32 won;
} ALIGN(4) GameOutcome_t;


void enable_interrupts(void) {
  REG_IME = 0;
  REG_IE = 1;
  REG_DISPLAY_STAT = 8;
  REG_ISR_MAIN = isr_handler_cb;
  REG_IME = 1;
}




void init_board(Obj_Attrs_t *objs) {
  int cur=0;
  static const int x_ofs_init = 98; /* (240 - ((4 margin px) + (5 tiles * 8 px/tile)))/2=(240-44)/2=  */

  int x, 
      y=16;  /* Arbitrarily-chosen top margin ofs. 
              * Set aligned to top so as to leave room for keyboard */ 
  for (int i = 0; i < 6; ++i) {
    x = x_ofs_init;
    attempt_buf[i] = objs+cur;
    for (int j = 0; j < 5; ++j) {
      objs[cur].attr0.attrs.y=y;
      objs[cur].attr1.attrs_reg.x=x;

      objs[cur++].attr2.raw = TILE_OFS;

      x+=9;
    }
    y+=9;
  }
  oam_cpy(OAM_MEM, objs, 30);
}


#define KEY_HOVER_PALBANK 4
#define KEY_NEUTRAL_PALBANK 0


void init_kbd(Keyboard_Ctx_t *kbd) {
  Obj_Attrs_t *objs = kbd->keys;
  int row_len, cur=0, c;
  int x, y = 80;
  const char *row_layout = KBD_LAYOUT;
  memset(kbd->buf, 0, sizeof(kbd->buf));
  kbd->buf_cursor=0;
  for (int row=0; row < 3; ++row) {
    row_len = KBD_ROW_LENS[row];
    kbd->letters[row] = row_layout;
    x = (240 - ((row_len-1) + (row_len*8)))/2;
    for (int i = 0; i < row_len; ++i) {
      objs[cur].attr0.attrs.y=y;
      objs[cur].attr1.attrs_reg.x=x;
      objs[cur].attr2.attr.tile_idx = TILE_OFS + 1 + (c=row_layout[i] - 'a');
      KBD_LOCATIONS[c] = cur++;

      x+=9;
    }
    y+=9;
    row_layout += row_len;
  }

  objs[0].attr2.attr.palbank = 4;

  oam_cpy(&OAM_MEM[30], objs, 26);

}

void StartScreen(void) {
  Obj_Affine_t aff = {0};
  mode3_printf((240-24*4)/2, 16, 0x8000, "A Burt Sumner Production");
  fast_memcpy32(&TILE_MEM[5][0], WordleLettersTiles, WordleLettersTilesLen/4);
  fast_memcpy32(PAL_OBJ_MEM, WordleLettersPal, WordleLettersPalLen/4);
  
  {
    Obj_Attrs_t *obj = obj_buf, *oam = OAM_MEM;
    int l2_xofs = (SCREEN_WIDTH-7*8-6)/2, l1_xofs = (SCREEN_WIDTH-6*8-5)/2;
    for (int i = 0; i < 6; ++i) {
      obj->attr0.raw = 36;
      obj->attr1.raw = l1_xofs+i*9;
      obj->attr2.attr.tile_idx = 513+"wordle"[i]-'a';
      oam_cpy(oam++, obj++, 1);
    }
    for (int i = 0; i < 7; ++i) {
      obj->attr0.raw = 52;
      obj->attr1.raw = l2_xofs+i*9;
      obj->attr2.attr.tile_idx = 513+"advance"[i]-'a';
      oam_cpy(oam++, obj++, 1);
    }
  }
  
  oam_init_ofs(obj_buf+13, 128-13, 13);
 
  REG_DISPLAY_CNT = 0x1403;
//  mode3_printf((240-18*4)/2, 48, 0x8000, "Wordle Boy Advance");
  vsync();

  u32 rand_seed = 0;
  u16 key_stat_snapshot;
  _Bool start_prompted = 0;
  for ( ; ; ) {
    vsync();
    if ((key_stat_snapshot = (~REG_KEY_STAT)&0x03FF)) {
      rand_seed ^= (rand_seed<<10) ^ key_stat_snapshot;
      if (!start_prompted) {
        for (int i = 0; i < 13; ++i) {
          for (key_stat_snapshot=0; ++key_stat_snapshot < 3; )  // Just recycling unused (@ least in this branch) var instead of inst'ing a new one
            vsync();
          aff.pa = 256;
          aff.pd = 256;
          obj_buf[i].attr0.attrs.obj_mode = 1;  // switch it to affine mode
          oam_cpy(&OAM_MEM[i], &obj_buf[i], 1);
          obj_affine_cpy(AFFINE_MEM, &aff, 1);
          while (aff.pa != 2048) {
            vsync();
            aff.pa += 256;
            obj_affine_cpy(AFFINE_MEM, &aff, 1);
          }
          obj_buf[i].attr2.attr.palbank = 1;
          oam_cpy(&OAM_MEM[i], &obj_buf[i], 1);
          while (aff.pa != 256) {
            aff.pa -= 256;
            obj_affine_cpy(AFFINE_MEM, &aff, 1);
            vsync();
          }
          obj_buf[i].attr0.attrs.obj_mode = 0;  // back to regular mode
          oam_cpy(OAM_MEM+i, obj_buf+i, 1);
        }

        mode3_printf((SCREEN_WIDTH-39*4)/2, 68, 0x8000, 
            "A \x1b[0x1CA2]Wordle\x1b[0x8000] \x1b[0x014F]Clone\x1b[0x8000] for the Gameboy Advance!");
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
  while (!(Poll_Keys()&KEY_START)) vsync();
}

#define K_EV_ENTER_LETTER_FAIL   512
#define K_EV_DELETE_LETTER_FAIL  256
#define K_EV_ENTER_WORD_FAIL     128 
#define K_EV_ENTER_LETTER        64
#define K_EV_DELETE_LETTER       32
#define K_EV_MOVE_CURSOR_DOWN    16
#define K_EV_MOVE_CURSOR_UP      8
#define K_EV_MOVE_CURSOR_RIGHT   4
#define K_EV_MOVE_CURSOR_LEFT    2
#define K_EV_ENTER_WORD          1
#define K_EV_NO_EVENTS           0


int Wordle_wordcmp(const char *word_a, const char *word_b);


// Iterative binary search implementation
bool search_wordlist(const char *word, const char list[][6], int len) {
  int midpt, diff;
  do {
    diff = Wordle_wordcmp(word, list[(midpt=(len>>1))]);
    if (!diff) {
      return true;
    } else if (diff > 0) {
      list = &list[midpt+1];
      len -= midpt+1;
    } else {
      len = midpt;
    }
  } while (len > 1);
  return len ? !Wordle_wordcmp(word, list[0]) : false;
} 

#define SEARCH_WORDLIST(word) search_wordlist(word, WORDS, WORD_LIST_LEN)




int handle_kbd_events(Keyboard_Ctx_t *kbd) {
  static char state_modifier[4];
  static int state_mod_cur=0;
  Obj_Attrs_t *keys = kbd->keys;
  int cur_col, cur_row, new_col, new_row, tmp, ret=0, buf_cursor=kbd->buf_cursor;
  cur_col = new_col = kbd->cur_col, cur_row = new_row = kbd->cur_row;
  if (K_STROKE(START)) {
    if (buf_cursor!=5) {
      return K_EV_ENTER_WORD_FAIL;
    } else if (!SEARCH_WORDLIST(kbd->buf)) {
      LFSR_Adjust_State(*((int*)(kbd->buf)));
      return K_EV_ENTER_WORD_FAIL;
    }
//    LFSR_Adjust_State(*((int*)(kbd->buf)));
    return K_EV_ENTER_WORD;
  }
  if (K_STROKE(UP)) {
    if (0 > --new_row) {
      new_row = 2;
    }

    ret = K_EV_MOVE_CURSOR_UP;

  } else if (K_STROKE(DOWN)) {
    if (2 < ++new_row) {
      new_row = 0;
    }
    ret=K_EV_MOVE_CURSOR_DOWN;
  }
  
  if (ret) {
    if ((tmp=KBD_ROW_LENS[new_row]-1) < new_col) {
      new_col=tmp;
    }
  } else {   
    if (K_STROKE(LEFT)) {
      if (0 > --new_col) {
        new_col = KBD_ROW_LENS[new_row]-1;
      }
      ret = K_EV_MOVE_CURSOR_LEFT;
    } else if (K_STROKE(RIGHT)) {
      if ((KBD_ROW_LENS[new_row]-1) < ++new_col) {
        new_col = 0;
      }
      ret = K_EV_MOVE_CURSOR_RIGHT;
    }
  }
  if (ret) {
    if (!((K_EV_MOVE_CURSOR_LEFT|K_EV_MOVE_CURSOR_RIGHT)&ret)) {
      if (new_row&2) {
        if (new_col==(KBD_ROW_LENS[2]-1)) {
          if (cur_col==6) {
            --new_col;
          }
        } else {
          if (0 > --new_col) {
            new_col = 0;
          }
        }
      } else if (ret&K_EV_MOVE_CURSOR_UP) {
        if (new_row&1) {
          ++new_col;
        }
      } else {
        if (!new_row) {
          ++new_col;
        }
      }
    }
    tmp=0;
    for (int i = 0; i < cur_row; ++i)
      tmp += KBD_ROW_LENS[i];
    tmp+=cur_col;
    keys[tmp].attr2.attr.palbank = kbd->cur_letter_prev_palbank;
    oam_cpy(OAM_MEM + 30 + tmp, keys + tmp, 1);
    tmp = 0;
    for (int i = 0; i < new_row; ++i)
      tmp+=KBD_ROW_LENS[i];
    tmp+=new_col;
    kbd->cur_letter_prev_palbank = keys[tmp].attr2.attr.palbank;
    keys[tmp].attr2.attr.palbank = KEY_HOVER_PALBANK;
    oam_cpy(OAM_MEM + 30 + tmp, keys + tmp, 1);
    kbd->cur_row = new_row;
    kbd->cur_col = new_col;
    kbd->cur_letter = tmp = kbd->letters[new_row][new_col];
    tmp |= tmp<<8;
    tmp ^= tmp<<12;
    tmp ^= tmp<<16;
    LFSR_Adjust_State(tmp);
    return ret;
  }
  
  if (K_STROKE(A)) {
    if (5 <= buf_cursor) {
      ret = K_EV_ENTER_LETTER_FAIL;
    } else {
      kbd->buf[kbd->buf_cursor++] = kbd->cur_letter;
      ret = K_EV_ENTER_LETTER;
    }

  } else if (K_STROKE(B)) {
    if (0 > --buf_cursor) {
      ret = K_EV_DELETE_LETTER_FAIL;
    } else {
      kbd->buf[kbd->buf_cursor=buf_cursor] = 0;
      ret = K_EV_DELETE_LETTER;
    }
  }

    if (ret) {
      if (state_mod_cur >= 4) {
        LFSR_Adjust_State(*((int*)state_modifier));
        state_mod_cur = 0;
        *((int*)state_modifier) = 0;
      } else {
        state_modifier[state_mod_cur++] = kbd->cur_letter;
      }
      return ret;
    }


  

  return K_EV_NO_EVENTS;


  
}


void MainMenu(SaveProfile_t *profile);

GameOutcome_t PlayGame(SaveProfile_t *profile) {
  i8 alpha_hashtable[28];
  const char *word = NULL;
  Obj_Attrs_t *obj_ofs, *curr_tile, *key;
  Obj_Affine_t spin_matrix;
  u16 ev_fields, attempt=0;
  bool won = 0;
  char c;
  fast_memcpy32(&TILE_MEM[5][0], WordleLettersTiles, WordleLettersTilesLen/4);
  fast_memcpy32(PAL_OBJ_MEM, WordleLettersPal, WordleLettersPalLen/4);
  
  init_board(obj_buf);
  Keyboard_Ctx_t kbd = {0};
  kbd.keys = &obj_buf[30];
  kbd.cur_letter = 'q';

  init_kbd(&kbd);
  oam_init_ofs(&obj_buf[56], 128-56, 56);

  REG_DISPLAY_CNT = 0x1403;
  BMP_Rect_t erase_rect = {.x = (SCREEN_WIDTH-30*4)/2, .y=4, .width = 30*4, .height = 8, .color = 0 };
  int message_erase_timer;
  while (!won && attempt < 6) {
    vsync();
    Poll_Keys();
    if ((ev_fields = handle_kbd_events(&kbd))!=K_EV_NO_EVENTS) {
      do {
        // poor man's try catch lol
        if (ev_fields&(K_EV_DELETE_LETTER_FAIL|K_EV_ENTER_LETTER_FAIL|K_EV_ENTER_WORD_FAIL)) {
          if (ev_fields&K_EV_ENTER_WORD_FAIL) {
            message_erase_timer = 60;
            if (kbd.buf_cursor!=5) {
              mode3_printf((SCREEN_WIDTH-28*4)/2, 4, 0x10A5, "Word must be 5 letters long!");
            } else {
              mode3_printf(erase_rect.x, 4, 0x1085, "\"%s\" not found in database!", kbd.buf);
            }
            do {
              vsync();
              Poll_Keys();
            } while (--message_erase_timer && !K_STROKE(A));
            mode3_draw_rect(&erase_rect);
          }

          break;
        }
        if (ev_fields&K_EV_ENTER_LETTER) {
          (*(obj_ofs = &attempt_buf[attempt][kbd.buf_cursor-1])).attr2.attr.tile_idx = TILE_OFS + 1 + kbd.cur_letter - 'a';
          oam_cpy(OAM_MEM + (obj_ofs-obj_buf), obj_ofs, 1);
          break;
        }
        if (ev_fields&K_EV_DELETE_LETTER) {
          (*(obj_ofs = &attempt_buf[attempt][kbd.buf_cursor])).attr2.attr.tile_idx = TILE_OFS + 0;
          oam_cpy(OAM_MEM+(obj_ofs-obj_buf), obj_ofs, 1);
          break;
        } 
        if (ev_fields&K_EV_ENTER_WORD) {
#ifdef DEBUG_FALSE_POSITIVES
          do vsync(); while (!(REG_KEY_STAT&KEY_START));
#endif
          obj_ofs = curr_tile = attempt_buf[attempt];
          won=1;
          if (NULL==word) {
#ifdef DEBUG_FALSE_POSITIVES
            word = "perky";
#else
            word = ANSWERS[LFSR_Rand()%ANSWER_LIST_LEN];
#endif
          }

            fast_memset32(alpha_hashtable, 0, (sizeof(alpha_hashtable)/sizeof(i32)));
            for (int i = 0; i < 5; ++i) {
              ++alpha_hashtable[word[i]-'a'];
           }


          for (int i = 0; i < 5; ++i) {
            if ((c=kbd.buf[i]) == word[i])
              --alpha_hashtable[c-'a'];
          }

          for (int i = 0; i < 5; ++i, ++curr_tile) {
            c=kbd.buf[i];
            curr_tile->attr2.attr.tile_idx = TILE_OFS + 1+c-'a';
            key = &kbd.keys[KBD_LOCATIONS[c-'a']];
            curr_tile->attr0.attrs.obj_mode = 0;
            if (c!=word[i]) {
              won = 0;
              if (0 < (int)alpha_hashtable[c-'a']) {
                --alpha_hashtable[c-'a'];
                curr_tile->attr2.attr.palbank = 2;
                if (c == kbd.cur_letter) {
                  if (kbd.cur_letter_prev_palbank != 1)
                    kbd.cur_letter_prev_palbank = 2;
                } else {
                  if (key->attr2.attr.palbank!=1)
                    key->attr2.attr.palbank=2;
                }
                continue;
              }
              curr_tile->attr2.attr.palbank = 3;
              if (c == kbd.cur_letter) {
                if (!(kbd.cur_letter_prev_palbank&3))
                  kbd.cur_letter_prev_palbank = 3;
              } else {
                if (!(key->attr2.attr.palbank&3)) {
                  key->attr2.attr.palbank = 3;
                }
              }
              continue;
            }
#ifdef DEBUG_FALSE_POSITIVES
            mode3_printf(0,0,0, "alpha_hashtable[c-'a'] = %d", alpha_hashtable[c-'a']);
            do vsync(); while (REG_KEY_STAT&KEY_START);
            mode3_clear_screen();
#endif
            curr_tile->attr2.attr.palbank = 1;
            if (c == kbd.cur_letter) {
              kbd.cur_letter_prev_palbank = 1;
            } else {
              key->attr2.attr.palbank = 1;
            }
          }
          curr_tile = OAM_MEM + (obj_ofs-obj_buf);
          for (int i = 0; i < 5; ++i) {
            c=10;
            do vsync(); while (--c);
            spin_matrix = (Obj_Affine_t){ .pa = 256, .pb = 0, .pc = 0, .pd = 256 };
            obj_affine_cpy(AFFINE_MEM, &spin_matrix, 1);
            obj_ofs->attr0.attrs.obj_mode = 1;  // switch sprite to affine mode
            c = obj_ofs->attr2.attr.palbank;
            obj_ofs->attr2.attr.palbank = 0;
            oam_cpy(curr_tile, obj_ofs, 1);
            while (spin_matrix.pa != 2048) {
              vsync();
              spin_matrix.pa += 256;
              obj_affine_cpy(AFFINE_MEM, &spin_matrix, 1);
            }
            obj_ofs->attr2.attr.palbank = c;
            oam_cpy(curr_tile, obj_ofs, 1);
            while (spin_matrix.pa != 256) {
              spin_matrix.pa -= 256;
              obj_affine_cpy(AFFINE_MEM, &spin_matrix, 1);
              vsync();
            }
            obj_ofs->attr0.attrs.obj_mode = 0;  // switch it back to regular mode
            oam_cpy(curr_tile++, obj_ofs++, 1);
          }
          oam_cpy(OAM_MEM+30, kbd.keys, 26);
//          memset(kbd.buf, 0, sizeof(kbd.buf));
          *((u32*) kbd.buf) = 0;
          *((u16*) (&kbd.buf[4])) = 0;
          kbd.buf_cursor = 0;
          if (won) {
            ++profile->win_ct;
            ++profile->win_freqs[attempt];
            switch (attempt) {
              case 0:
                mode3_printf(erase_rect.x=(SCREEN_WIDTH-6*4)/2, 4, 0x10A5, "Genius");
                erase_rect.width = 24;
                break;
              case 1:
                mode3_printf(erase_rect.x = (SCREEN_WIDTH-11*4)/2, 4, 0x10A5, "Magnificent");
                erase_rect.width = 44;
                break;
              case 2:
                mode3_printf(erase_rect.x = (SCREEN_WIDTH-10*4)/2, 4, 0x10A5, "Impressive");
                erase_rect.width = 40;
                break;
              case 3:
                mode3_printf(erase_rect.x = (SCREEN_WIDTH-8*4)/2, 4, 0x10A5, "Splendid");
                erase_rect.width = 32;
                break;
              case 4:
                mode3_printf(erase_rect.x = (SCREEN_WIDTH-5*4)/2, 4, 0x10A5, "Great");
                erase_rect.width = 20;
                break;
              case 5:
                mode3_printf(erase_rect.x = (SCREEN_WIDTH-4*4)/2, 4, 0x10A5, "Phew");
                erase_rect.width = 16;
                break;
            }
          }
          ++attempt;
        }
      } while (0);
    }
  }
  if (!won) {
    mode3_printf(erase_rect.x = (SCREEN_WIDTH-44*4)/2, 4, 0x10A5, "Better luck next time! The word was: \"%s\"", word);
    erase_rect.width = 44*4;
  }
  do { 
    vsync();
    Poll_Keys();
  }while (!K_STROKE(A));
  ++profile->attempt_ct;
  mode3_draw_rect(&erase_rect);
  return (GameOutcome_t){.won=won, .word = word};
}

void PostGame_Synopsis(const GameOutcome_t *outcome, const SaveProfile_t *profile) {
  REG_DISPLAY_CNT = 0x0403;
  mode3_printf((SCREEN_WIDTH-8*4)/2, 16, 0x10A5, "You %s", outcome->won?"Won!":"Lost");
  mode3_printf((SCREEN_WIDTH-29*4)/2, 28, 0x10A5, "The correct word was: \"%s\"", outcome->word);
  mode3_printf((SCREEN_WIDTH-14*4)/2, 40, 0x10A5, "Profile Stats:");

  mode3_printf(4, 56, 0x8000, "%d Wins / %d Attempts", profile->win_ct, profile->attempt_ct);

  mode3_printf(2, 66, 0x10A5, "Wins by Guess Count:");

  for (int i = 0; i < 6; ++i) {
    mode3_printf(4, 76+i*10, 0x8000, "%d-Guess Wins: %d", i+1, profile->win_freqs[i]);
  }

  mode3_printf((SCREEN_WIDTH - 33*4)/2, SCREEN_HEIGHT - 16, 0x10A5, "Press \x1b[0x2861][START]\x1b[0x10A5] to Play a New Game!");
  do {
    vsync();
    Poll_Keys();
  } while (!K_STROKE(START));
}

__attribute__ ((naked)) STAT_INLN void Huffman_Decompress(const void *data_with_header, void *dest_decompression_buffer) {
  __asm volatile ( "SVC 0x13\n\t"
      "BX lr" );
}

int main(void) {
  enable_interrupts();
  StartScreen();
  SaveProfile_t prof;
  GameOutcome_t outcome;
  MainMenu(&prof);
  Huffman_Decompress(answerlist_Huffman_Compression_Data, (void*)ANSWERS);
  Huffman_Decompress(wordlist_Huffman_Compression_Data, (void*)WORDS);
  while (1) {
    mode3_clear_screen();
    fast_memset32(obj_buf, 0, sizeof(obj_buf)/4);
    oam_cpy(OAM_MEM, obj_buf, 128);
    outcome = PlayGame(&prof);
    mode3_clear_screen();
    REG_DISPLAY_CNT=0x0403;
    mode3_printf((SCREEN_WIDTH-49*4)/2, 76, 0x0030, "Saving Data to Cartridge. Don't Power System Off.");
    Update_Save_Profile(&prof);
    mode3_clear_screen();
    PostGame_Synopsis(&outcome, &prof);
  }
  return 0;
}
