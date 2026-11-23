#include <cstdio>
#include <vector>
#include <OpenGL/gl.h> // Or your platform's OpenGL loader (GLAD, GLEW, etc.)
#include "jo_mpeg_wrapper.h"

// Include Jon Olick's single-file MPEG writer
#include "jo_mpeg.h"

void save_framebuffer_to_mpeg(FILE* fp, int width, int height, int fps) {
    // 1. Open output MPEG file in binary write mode
    //FILE* fp = std::fopen("output.mpg", "wb");
    //if (!fp) return;

    // 2. Allocate a 4-byte-per-pixel buffer (RGBX/RGBA)
    std::vector<unsigned char> pixels(width * height * 4);
    std::vector<unsigned char> flipped(width * height * 4);

    // 3. Set pack alignment to 1 byte so rows have no extra padding
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    //for (int frame = 0; frame < total_frames; ++frame) {
        // --- Render your OpenGL scene here ---
        // render_scene();

        // 4. Read the framebuffer into CPU memory
        glReadBuffer(GL_FRONT); // Use GL_FRONT if single-buffered, or GL_COLOR_ATTACHMENT0 for an FBO
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        // 5. Flip vertically (OpenGL origin is bottom-left; MPEG expects top-left)
        const int row_bytes = width * 4;
        for (int y = 0; y < height; ++y) {
            std::memcpy(
                flipped.data() + y * row_bytes,
                pixels.data() + (height - 1 - y) * row_bytes,
                row_bytes
            );
        }

        // 6. Write the frame (jo_write_mpeg supports 24, 25, 30, 50, or 60 fps)
        jo_write_mpeg(fp, flipped.data(), width, height, fps);
    //}

    //std::fclose(fp);
}
