#include "ffmpeg_wrapper.h"
#include <OpenGL/gl.h>
#include <cstdio>
#include <vector>
    
FILE* open_framebuffer_to_ffmpeg(const char *filename, int width, int height, int framerate = 60) {
  // 1. Open the FFmpeg pipe before your render loop
  // Note: Use _popen / _pclose on Windows, popen / pclose on Linux/macOS
  char cmd[512];
//  std::snprintf(cmd, sizeof(cmd), "/usr/local/bin/ffmpeg -r %d -f rawvideo -pix_fmt rgba -s %dx%d -i - "
//                    "-c:v libx264 -preset fast -crf 21 -pix_fmt yuv420p -vf vflip -y %s", framerate, width, height, filename);
  std::snprintf(cmd, sizeof(cmd), "/usr/local/bin/ffmpeg -r %d -f rawvideo -pix_fmt rgba -s %dx%d -i - "
  "-c:v libx264 -preset fast -crf 21 -vf \"vflip,crop=trunc(iw/2)*2:trunc(ih/2)*2\" -pix_fmt yuv420p -y %s "
  ">/tmp/ffmpeg.log 2>&1", framerate, width, height, filename);

  FILE* ffmpeg = popen(cmd, "w");
  if (ffmpeg) {
     save_framebuffer_to_ffmpeg(ffmpeg, width, height);
    //Close the pipe when finished to finalize the MP4 header
    //pclose(ffmpeg);
  } else {
    fprintf(stderr, "can't popen command %s\n", cmd);
    fprintf(stderr, "popen failed: %s (errno=%d)\n", strerror(errno), errno);

  }
  return (ffmpeg);
}

int save_framebuffer_to_ffmpeg(FILE *ffmpeg, int width, int height) {

 std::vector<unsigned char> buffer(width * height * 4);

 glPixelStorei(GL_PACK_ALIGNMENT, 1); // Prevents row-padding skew on odd widths
 glReadBuffer(GL_FRONT);
 glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
 return fwrite(buffer.data(), 1, buffer.size(), ffmpeg);
}
