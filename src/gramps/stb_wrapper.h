#ifndef STB_WRAPPER_H
#define STB_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

// Declare functions using only C-compatible types (primitives, pointers, structs)
void  saveScreenshotPNG(const char* filename, int width, int height);
void saveScreenshotJPEG(const char* filename, int width, int height, int quality);
#ifdef __cplusplus
}
#endif

#endif // STB_WRAPPER_H
