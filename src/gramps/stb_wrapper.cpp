#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_wrapper.h"
#include <OpenGL/gl.h>
#include <vector>
void saveScreenshotJPEG(const char* filename, int width, int height, int quality = 90) {
    // 0. Read what user acutally sees
    glReadBuffer(GL_FRONT);

    // 1. Set pack alignment to 1 byte so rows aren't padded to 4-byte boundaries
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // 2. Allocate buffer (3 bytes per pixel for RGB)
    std::vector<unsigned char> pixels(width * height * 3);

    // 3. Read pixels from the current framebuffer
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // 4. Flip vertically (OpenGL origin is bottom-left; JPEG is top-left)
    stbi_flip_vertically_on_write(1);

    // 5. Write to JPEG
    stbi_write_jpg(filename, width, height, 3, pixels.data(), quality);
}

void saveScreenshotPNG(const char* filename, int width, int height) {
#include <stdio.h>
#include <stdlib.h>

        // Use 4 channels (RGBA). Apple's Metal-backed OpenGL driver handles
        // 4-byte aligned RGBA readbacks significantly faster than 3-byte RGB.
        int channels = 4;
        size_t buffer_size = (size_t)width * height * channels;
        unsigned char* pixels = (unsigned char*)malloc(buffer_size);
        if (!pixels) {
            fprintf(stderr, "Failed to allocate screenshot buffer.\n");
            return;
        }

        // 3. Ensure default framebuffer is bound for reading and specify front buffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glReadBuffer(GL_FRONT);

        // 4. Set pack alignment to 1 (safe practice even with 4-channel RGBA)
        GLint prev_alignment;
        glGetIntegerv(GL_PACK_ALIGNMENT, &prev_alignment);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        // 5. Read pixels from the Retina-scaled framebuffer
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        // 6. Restore alignment state
        glPixelStorei(GL_PACK_ALIGNMENT, prev_alignment);

        // 7. Flip vertically and write PNG
        stbi_flip_vertically_on_write(1);
        int stride = width * channels;
        if (!stbi_write_png(filename, width, height, channels, pixels, stride)) {
            fprintf(stderr, "Failed to write PNG: %s\n", filename);
        }

        free(pixels);
    }
