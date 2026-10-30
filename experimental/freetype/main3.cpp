#define GL_SILENCE_DEPRECATION // Silences macOS OpenGL deprecation warnings
#include <GLUT/glut.h>         // Native macOS GLUT header
#include <FTGL/ftgl.h>
#include <iostream>

// Font configuration - standard macOS font path
const char* FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf";
FTFont* font = nullptr;

// Display callback function (handles rendering)
void display() {
    // Clear screen
    glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set font color (White text)
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Position text in 2D space
    glRasterPos2f(50.0f, 300.0f);
    
    // Render text using FTGL
    if (font) {
        font->Render("Hello from FTGL + GLUT!");
    }

    // Flush and swap buffers
    glutSwapBuffers();
}

// Reshape callback function (handles window resizing)
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard callback function (handles key presses)
void keyboard(unsigned char key, int x, int y) {
    // Exit if ESC is pressed
    if (key == 27) { 
        // Cleanup font before exiting
        if (font) {
            delete font;
        }
        exit(0);
    }
}

int main(int argc, char** argv) {
    // 1. Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("FTGL + GLUT Example");

    // Register GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // 2. Initialize FTGL Fonts (Must happen AFTER window/context creation)
    font = new FTPixmapFont(FONT_PATH); // Creates rasterized screen-aligned glyphs
    if (font->Error()) {
        std::cerr << "Could not load font from path: " << FONT_PATH << std::endl;
        delete font;
        return -1;
    }
    font->FaceSize(48); // Set font point size

    // 3. Enter the permanent GLUT event loop
    glutMainLoop();
    
    return 0;
}
