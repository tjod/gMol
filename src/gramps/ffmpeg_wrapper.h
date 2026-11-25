#ifndef FFMPEG_WRAPPER_H
#define FFMPEG_WRAPPER_H

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

// Declare functions using only C-compatible types (primitives, pointers, structs)
FILE* open_framebuffer_to_ffmpeg(const char* filename, int width, int height, int framerate);
int   save_framebuffer_to_ffmpeg(FILE *ffmpeg, int width, int height);
int   close_ffmepg_();
#ifdef __cplusplus
}
#endif

#endif // FFMPEG_WRAPPER_H
