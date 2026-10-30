
clang++ -std=c++11 ftgl_glut_demo.cpp -o ftgl_glut_demo \
  -I/opt/homebrew/include \
  -I/usr/local/opt/freetype/include/freetype2 \
  -lftgl \
  -framework GLUT -framework OpenGL
