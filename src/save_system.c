#include "gba_funcs.h"
#include "save_interface.h"

#include <stddef.h>
#include <string.h>


static const char HEADER_IDENTIFIER[] = "WRDL";


typedef struct s_save_header {
  const char ID[sizeof(HEADER_IDENTIFIER)];
  u32 profile_count;
} SaveHeaderFields_t;

#define SAVE_PROFILES_OFS sizeof(SaveProfile_t)




static inline void Get_Save_Header(SaveHeaderFields_t *fields) {
  SRAM_Read(fields, sizeof(SaveHeaderFields_t), 0);
}

static inline void Set_Save_Header(SaveHeaderFields_t *fields) {
  SRAM_Write(fields, sizeof(SaveHeaderFields_t), 0);
}

int Get_Save_Profiles(SaveProfile_t *profiles) {
  SaveHeaderFields_t fields;
  Get_Save_Header(&fields);
  
  if (strcmp(fields.ID, HEADER_IDENTIFIER)) {
    memcpy((void*)(fields.ID), HEADER_IDENTIFIER, sizeof(HEADER_IDENTIFIER));
    fields.profile_count = 0;
    Set_Save_Header(&fields);
    return 0;
  }
  SRAM_Read(profiles, fields.profile_count*sizeof(SaveProfile_t), SAVE_PROFILES_OFS);

  return fields.profile_count;
}

void Add_Save_Profile(SaveProfile_t *profile) {
  SaveHeaderFields_t hdr;
  Get_Save_Header(&hdr);
  if (strcmp(hdr.ID, HEADER_IDENTIFIER)) {
    memcpy((void*)(hdr.ID), HEADER_IDENTIFIER, sizeof(HEADER_IDENTIFIER));
    hdr.profile_count = 1;
    profile->acct_idx = 0;
    SRAM_Write(profile, sizeof(SaveProfile_t), SAVE_PROFILES_OFS);
    Set_Save_Header(&hdr);
    return;
  }

  SRAM_Write(profile, sizeof(SaveProfile_t), SAVE_PROFILES_OFS+sizeof(SaveProfile_t)*(profile->acct_idx=hdr.profile_count++));
  Set_Save_Header(&hdr);
}

bool Update_Save_Profile(const SaveProfile_t *profile) {
  SaveHeaderFields_t fields;
  Get_Save_Header(&fields);
  if (strcmp(fields.ID, HEADER_IDENTIFIER))
    return false;
  if (profile->acct_idx >= fields.profile_count)
    return false;
  
  SRAM_Write(profile, sizeof(SaveProfile_t), SAVE_PROFILES_OFS+sizeof(SaveProfile_t)*(profile->acct_idx));
  return true;

}
