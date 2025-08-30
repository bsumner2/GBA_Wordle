#ifndef _INPUT_H_
#define _INPUT_H_

#include "gba_def.h"
#include "gba_types.h"
#include "rng.h"
#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

extern u16 KEY_CURR, KEY_PREV;

static inline u16 Poll_Keys(void) {
  KEY_PREV=KEY_CURR;
  KEY_CURR=0x03FF&REG_KEY_STAT;
  LFSR_Adjust_State(((KEY_PREV==KEY_CURR)<<31)|((~KEY_CURR<<15)|(~KEY_PREV)));
  return KEY_CURR;
}

#define K_STROKE(key) Key_Pressed(KEY_##key)

static inline bool Key_Pressed(u16 key) {
  return (!(KEY_CURR&key) && (KEY_PREV&key));
}





#ifdef __cplusplus
}
#endif

#endif  /* _INPUT_H_ */
