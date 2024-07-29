#ifndef _ANSWERS_H_
#define _ANSWERS_H_



#ifdef __cplusplus
extern "C" {
#endif  /* Name mangler guard */

#define ANSWER_LIST_LEN 2309

extern __attribute__((section(".rodata"))) const char *ANSWERS[ANSWER_LIST_LEN];


#ifdef __cplusplus
}
#endif  /* Name mangler guard */



#endif
