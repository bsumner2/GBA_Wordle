#ifndef _MODE3_IO_H_
#define _MODE3_IO_H_


#ifdef __cplusplus
extern "C" {
#define restrict
#endif

int mode3_printf(int x, int y, unsigned short bg_clr, const char *restrict fmt, ...);

void mode3_putchar(int c, int x, int y, unsigned short bg_color);



#ifdef __cplusplus
}
#udef restrict
#endif


#endif  /* _MODE3_IO_H_ */
