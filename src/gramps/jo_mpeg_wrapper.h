#ifndef JO_WRAPPER_H
#define JO_WRAPPER_H

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

// Declare functions using only C-compatible types (primitives, pointers, structs)
void save_framebuffer_to_mpeg(FILE* fp, int width, int height, int fps);
#ifdef __cplusplus
}
#endif

#endif // JO_WRAPPER_H
