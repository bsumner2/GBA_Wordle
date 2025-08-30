#include "gba_types.h"
#include "gba_def.h"
#include "subpixel.h"
#include "gba_funcs.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


 
static int ipow(int base, u32 power) {
  if (power&1) return base*ipow(base*base, power>>1);
  else return power ? ipow(base*base, power>>1) : 1;
}


/*
 * @brief Use after encountering escape char, ASCII(27). Advance the pointer to
 * so that it's pointing to the char immediately following the esc char.
 * */
static u16 parse_color(char **buf_ptr, bool *return_errflag) {
  char *str = *buf_ptr;
  u16 ret = 0;
  char tmp;
  int len = 0, i = 0;
  *return_errflag = 1;
  if (*str++ != '[')
    return 0;
  if (strncmp(str++, "0x", 2))
    return 0;
  while ((tmp = *++str) && tmp != ']') {
    if (!isxdigit(tmp) || ++len > 4)
      return 0;
  }
  if (!tmp || !len)
    return 0;
  if (!str[1]) {
    *buf_ptr = ++str;
    return 0;  // Color won't matter anyway if EOS reached.
  }
  for (tmp = tolower(*(str - 1 - (i = 0))); i < len;
      tmp = tolower(*(str - 1 - ++i))) {
    tmp = (tmp > '9' ? 10 + tmp - 'a' : tmp - '0');
    ret += tmp*ipow(16, i);
  }
  *buf_ptr = ++str;
  *return_errflag = 0;
  return ret;
}


__attribute__ ((__format__ (__printf__, 4, 5))) int mode3_printf(int x, int y, u16 bg_clr, const char *restrict fmt, ...) {
  BMP_Rect_t tabrect = {
    .x=0,.y=0,.width=SubPixel_Glyph_Width*4, .height=SubPixel_Glyph_Height
  };
  va_list args;
  const SubPixel_Pair_t *g_row;
  u16 *vbuf, clr_idx;
  va_start(args, fmt);
  int len = vsnprintf(NULL, 0, fmt, args), idx;
  va_end(args);
  va_start(args, fmt);

  char buf[len+1];
  va_start(args, fmt);
  vsnprintf(buf, len+1, fmt, args);
  if (buf[len]) buf[len] = '\0';
  va_end(args);

  char *pstr=buf, cur;
  int x_pos=x, y_pos=y;
  bool err_flag;

  while ((cur=*pstr++)) {
    if (cur < ' ') {
      if (cur == '\n') {
        x_pos = x;
        y_pos += SubPixel_Glyph_Height;
        if ((y_pos + SubPixel_Glyph_Height) > SCREEN_HEIGHT)
          return pstr-&buf[0] - 1;
      } else if (cur == '\t') {
        if (!(bg_clr&0x8000)) {
          tabrect.x=x_pos;
          tabrect.y=y_pos;
          tabrect.color = bg_clr;
          mode3_draw_rect(&tabrect);
        }

        x_pos += SubPixel_Glyph_Width*4;
      } else if (cur == '\x1b') {
        bg_clr = parse_color(&pstr, &err_flag);
        if (err_flag)
          return pstr-&buf[0]-1;
      }
      continue;
    }

    idx = cur - ' ';
    if (idx >= SubPixel_Glyph_Count) {
      continue;
    }

    if ((x_pos + SubPixel_Glyph_Width) > SCREEN_WIDTH) {
      x_pos = x;
      y_pos += SubPixel_Glyph_Height;
      if ((y+SubPixel_Glyph_Height) > SCREEN_HEIGHT)
        return pstr - &(buf[0]) - 1;
      
    }

    g_row = (SubPixel_Pair_t*) (&SubPixel_Glyph_Data[idx*8]);
    vbuf = (y_pos*SCREEN_WIDTH) + x_pos + VRAM_BUF;
    if (bg_clr&0x8000) {
      for (int i = 0; i < SubPixel_Glyph_Height; ++i) {
        for (int j = 0; j < 2; ++j) {
          if ((clr_idx=g_row[j].l)) {
            vbuf[j*2] = SubPixel_Pal[clr_idx];
          }

          if ((clr_idx=g_row[j].r)) {
            vbuf[j*2 + 1] = SubPixel_Pal[clr_idx];
          }

        }
        g_row += 2;
        vbuf += SCREEN_WIDTH;
      }
    } else {

      for (int i = 0; i < SubPixel_Glyph_Height; ++i) {
        for (int j = 0; j < 2; ++j) {

          if ((clr_idx=g_row[j].l)) {
            vbuf[j*2] = SubPixel_Pal[clr_idx];
          } else {
            vbuf[j*2] = bg_clr;
          }
 
          if ((clr_idx=g_row[j].r)) {
            vbuf[j*2 + 1] = SubPixel_Pal[clr_idx];
          } else {
            vbuf[j*2+1] = bg_clr;
          }
        }
        g_row += 2;
        vbuf += SCREEN_WIDTH;
      }
    }

    x_pos += SubPixel_Glyph_Width;

  }

  return pstr - &buf[0] - 1;

}

void mode3_putchar(int c, int x, int y, u16 bg_color) {
  u16 *vrbuf, clr_idx;
  c -= ' ';
  if ( c < 0 || c >= SubPixel_Glyph_Count)
    return;
  if (x < 0 || y < 0)
    return;
  if (x+4>SCREEN_WIDTH || y+8>SCREEN_HEIGHT)
    return;
  
  SubPixel_Pair_t *g_row = (SubPixel_Pair_t*)(&SubPixel_Glyph_Data[c*8]);
  vrbuf = x+y*SCREEN_WIDTH+VRAM_BUF;
  if (bg_color&0x8000) {
    for (int i = 0; i < SubPixel_Glyph_Height; ++i) {
      for (int j = 0; j < 2; ++j) {
        if ((clr_idx=g_row[j].l)) {
          vrbuf[j*2] = SubPixel_Pal[clr_idx];
        }

        if ((clr_idx=g_row[j].r)) {
          vrbuf[j*2 + 1] = SubPixel_Pal[clr_idx];
        }
      }
      vrbuf+=SCREEN_WIDTH;
      g_row+=2;
    }
  
  }
  
  for (int i = 0; i < SubPixel_Glyph_Height; ++i) {
    for (int j = 0; j < 2; ++j) {
      if ((clr_idx=g_row[j].l)) {
        vrbuf[j*2] = SubPixel_Pal[clr_idx];
      } else {
        vrbuf[j*2] = bg_color;
      }
 
      if ((clr_idx=g_row[j].r)) {
        vrbuf[j*2 + 1] = SubPixel_Pal[clr_idx];
      } else {
        vrbuf[j*2+1] = bg_color;
      }
    }
    vrbuf+=SCREEN_WIDTH;
    g_row+=2;
  }
}
