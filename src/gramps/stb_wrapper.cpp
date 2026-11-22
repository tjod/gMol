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
