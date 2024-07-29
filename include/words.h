#ifndef _WORDS_ASSET_DECL_H_
#define _WORDS_ASSET_DECL_H_

#ifdef __cplusplus
extern "C" {
#endif  /* Name mangler guard */

#define WORD_LIST_LEN 12947
extern __attribute__((section(".rodata"))) const char *WORDS[WORD_LIST_LEN];


#ifdef __cplusplus
}
#endif  /* Name mangler guard */



#endif  /* _WORDS_ASSET_DECL_H_ */
