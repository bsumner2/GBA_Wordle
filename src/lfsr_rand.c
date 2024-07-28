
#include "gba_types.h"
#include "rng.h"
#include <stddef.h>

static struct s_lfsr_ctx {
  u32 state;
  u32 feedback_polynomial[FEEDBACK_POLY_DEGREE];
} LFSR_Main_Context = {0};

static u32 xor_and_swap_hword_sigs(u32 a, u32 b) {
    return ((0xFFFF&(a>>16))|((0xFFFF&a)<<16)) ^ ((0xFFFF&(b>>16))|((0xFFFF&b)<<16));
    
}

static LFSR_State_Adjust_Callback_t adj_cb;

u32 LFSR_DEBUGFUNC_GetState(void) {return LFSR_Main_Context.state;}

void LFSR_Init(u32 seed, u32 feedback_poly_ini[FEEDBACK_POLY_DEGREE]) {
  LFSR_Main_Context.state = seed;
  for (int i = 0; i < FEEDBACK_POLY_DEGREE; ++i)
    LFSR_Main_Context.feedback_polynomial[i] = feedback_poly_ini[i];
  LFSR_Set_State_Adjustor_Callback(NULL);
}

void LFSR_Shift(void) {
  u32 fbb = 0;
  for (int i = 0; i < FEEDBACK_POLY_DEGREE; ++i) {
    fbb ^= (LFSR_Main_Context.state >> LFSR_Main_Context.feedback_polynomial[i]);
  }
  fbb&=1;
  LFSR_Main_Context.state >>= 1;
  LFSR_Main_Context.state |= fbb<<31;
}
void LFSR_Set_State_Adjustor_Callback(LFSR_State_Adjust_Callback_t adjustor_callback) {
  if (!adjustor_callback)
    adj_cb = xor_and_swap_hword_sigs;
  else
    adj_cb = adjustor_callback;
}

u32 LFSR_Adjust_State(u32 adjustor) {
  return (LFSR_Main_Context.state = adj_cb(adjustor, LFSR_Main_Context.state));
}


u32 LFSR_Rand(void) {
  u32 ret = LFSR_Main_Context.state;

  LFSR_Shift();
  return ret;
}
