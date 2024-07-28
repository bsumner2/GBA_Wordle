#ifndef _SUBPIXEL_H_
#define _SUBPIXEL_H_

#include "gba_util_macros.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SubPixel_Glyph_Count 96
#define SubPixel_Glyph_Cell_Size 16
#define SubPixel_Glyph_Width 4
#define SubPixel_Glyph_Height 8


typedef struct s_subpixel_pair {
  unsigned char r:4, l:4;
  
} PACKED SubPixel_Pair_t; 
typedef struct s_subpixel_pair SubPixel_Glyph_t[SubPixel_Glyph_Cell_Size];



extern const unsigned short SubPixel_Glyph_Data[768];

extern const unsigned short SubPixel_Pal[15];


#ifdef __cplusplus
}
#endif

#endif  /* _SUBPIXEL_H_ */
