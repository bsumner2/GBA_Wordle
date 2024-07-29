#ifndef _SAVE_INTERFACE_H_
#define _SAVE_INTERFACE_H_

#include "gba_types.h"
#ifdef __cplusplus
extern "C" {
#endif

#define PROF_NAME_MAX_LEN 15

#define MAX_SAVE_PROFILES 32

typedef struct s_save_profile {
  char name[PROF_NAME_MAX_LEN+1];
  u32 acct_idx;
  u16 win_ct, attempt_ct;
  u16 win_freqs[6];
} SaveProfile_t;

/**
 * @brief Populates buff provided in param 0 with save profiles
 *
 * */
int Get_Save_Profiles(SaveProfile_t *profiles);

void Add_Save_Profile(SaveProfile_t *profile);
bool Update_Save_Profile(const SaveProfile_t *profile);


#ifdef __cplusplus
}
#endif

#endif  /* _SAVE_INTERFACE_H_ */
