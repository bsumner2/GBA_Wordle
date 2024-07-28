#ifndef _UTIL_MACROS_H_
#define _UTIL_MACROS_H_


#ifdef __cplusplus
extern "C" {
#endif  /* extern "C" name mangler guard */

// Have variable data stored in IWRAM (default behavior)
#define IWRAM_DATA __attribute__((section(".iwram")))

// Have variable data stored in EWRAM
#define EWRAM_DATA __attribute__((section(".ewram")))


// Have uninitialized variable data stored in EWRAM ((WITHOUT USING UP ROM SPACE))
#define EWRAM_BSS __attribute__((section(".sbss")))

// Store function in IWRAM
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

// Store function in EWRAM
#define EWRAM_CODE __attribute__((section(".ewram"), long_call))

#define ALIGN(byte_ct) __attribute__((aligned(byte_ct)))

#define PACKED __attribute__((packed))

#define STAT_INLN static inline


#ifdef __cplusplus
}
#endif  /* extern "C" name mangler guard */



#endif  /* _UTIL_MACROS_H_ */
