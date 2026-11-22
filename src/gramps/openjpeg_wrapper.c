#ifdef openjpeg
#include <OpenGL/gl.h>
#include <openjpeg.h>
#include <stdlib.h>
#include <string.h>

// Optional error/warning callbacks for debugging
static void jp2_error_callback(const char *msg, void *client_data) {
(void)client_data;
fprintf(stderr, "[OpenJPEG ERROR] %s", msg);
}

static void jp2_warning_callback(const char *msg, void *client_data) {
(void)client_data;
fprintf(stderr, "[OpenJPEG WARNING] %s", msg);
}

int save_framebuffer_to_jp2(const char *filename, int width, int height) {
// 1. Ensure 1-byte row alignment so width doesn't need padding
glReadBuffer(GL_FRONT);
glPixelStorei(GL_PACK_ALIGNMENT, 1);

// 2. Read interleaved RGB pixels from the OpenGL framebuffer
unsigned char *gl_pixels = (unsigned char *)malloc(width * height * 3);
if (!gl_pixels) return 0;
glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, gl_pixels);

// 3. Configure 3 image components (R, G, B) at 8 bits per channel
opj_image_cmptparm_t cmptparm[3];
memset(&cmptparm, 0, 3 * sizeof(opj_image_cmptparm_t));
for (int i = 0; i < 3; i++) {
        cmptparm[i].dx = 1;
        cmptparm[i].dy = 1;
        cmptparm[i].w = width;
        cmptparm[i].h = height;
        cmptparm[i].prec = 8; // 8-bit color
        cmptparm[i].sgnd = 0; // Unsigned
}

// 4. Create the OpenJPEG image structure
opj_image_t *image = opj_image_create(3, cmptparm, OPJ_CLRSPC_SRGB);
if (!image) {
        free(gl_pixels);
        return 0;
}
image->x0 = 0;
image->y0 = 0;
image->x1 = width;
image->y1 = height;

// 5. De-interleave RGB and flip vertically (OpenGL bottom-up -> JP2 top-down)
for (int y = 0; y < height; y++) {
        int gl_row = (height - 1 - y) * width * 3;
        int jp2_row = y * width;
        for (int x = 0; x < width; x++) {
            image->comps[0].data[jp2_row + x] = gl_pixels[gl_row + x * 3 + 0]; // R
            image->comps[1].data[jp2_row + x] = gl_pixels[gl_row + x * 3 + 1]; // G
            image->comps[2].data[jp2_row + x] = gl_pixels[gl_row + x * 3 + 2]; // B
        }
}
free(gl_pixels);

// 6. Set up compression parameters
opj_cparameters_t parameters;
opj_set_default_encoder_parameters(&parameters);

// Enable MCT (Multiple Component Transform) for RGB color decorrelation
parameters.tcp_mct = 1;

// Optional: For lossy compression, set rates and irreversible wavelet transform:
// parameters.tcp_numlayers = 1;
// parameters.tcp_rates[0] = 20.0f; // 20:1 compression ratio
// parameters.cp_disto_alloc = 1;
// parameters.irreversible = 1;

// 7. Create the JP2 codec (use OPJ_CODEC_J2K for raw codestreams)
opj_codec_t *codec = opj_create_compress(OPJ_CODEC_JP2);
opj_set_error_handler(codec, jp2_error_callback, NULL);
opj_set_warning_handler(codec, jp2_warning_callback, NULL);

if (!opj_setup_encoder(codec, &parameters, image)) {
        opj_destroy_codec(codec);
        opj_image_destroy(image);
        return 0;
}

// 8. Create file output stream and encode
opj_stream_t *stream = opj_stream_create_default_file_stream(filename, OPJ_FALSE);
if (!stream) {
        opj_destroy_codec(codec);
        opj_image_destroy(image);
        return 0;
}

int success = opj_start_compress(codec, image, stream) &&
              opj_encode(codec, stream) &&
              opj_end_compress(codec, stream);

// 9. Clean up resources
opj_stream_destroy(stream);
opj_destroy_codec(codec);
opj_image_destroy(image);

return success;
}
#endif
