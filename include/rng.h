#ifndef _RNG_H_
#define _RNG_H_

#include "gba_types.h"
#ifdef __cplusplus
extern "C" {
#endif

#define FEEDBACK_POLY_DEGREE 4

typedef u32 (*LFSR_State_Adjust_Callback_t)(u32, u32);

void LFSR_Init(u32 seed, u32 feedback_poly_ini[FEEDBACK_POLY_DEGREE]);
void LFSR_Shift(void);
u32 LFSR_Rand(void);
u32 LFSR_Adjust_State(u32 adjustor);
void LFSR_Set_State_Adjustor_Callback(LFSR_State_Adjust_Callback_t adjustor_callback);
void LFSR_SRand(u32 seed);
u32 LFSR_GetState(void);

#ifdef __cplusplus
}
#endif


#endif  /* _RNG_H_ */
