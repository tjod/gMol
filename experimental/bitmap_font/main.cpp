#include <GLUT/glut.h> // Standard GLUT/FreeGLUT header
#include <string.h>
#include "glut_18x30_font.h" // Include your auto-generated header file

// Custom string rendering loop utilizing the 18x30 font data
void RenderBitmapString18x30(float x, float y, const char* str) {
    // 1. Position the raster cursor using pixel coordinates
    glRasterPos2f(x, y);

    // 2. Ensure OpenGL processes byte data sequential tightly (1-byte boundary)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // 3. Cycle through the string characters
    while (*str) {
        char c = *str;
        if (c >= 32 && c <= 126) {
            int index = c - 32;
            
            // Render the specific character bitmap
            glBitmap(
                18,    // Width of bitmap
                30,    // Height of bitmap
                0.0,   // X-hotspot origin
                0.0,   // Y-hotspot origin
                18.0,  // Move raster pointer right exactly 18 pixels for next char
                0.0,   // Y-advance
                Font18x30[index].bitmap
            );
        }
        str++;
    }
}

// Main rendering callback
void display() {
    // Clear screen with a dark background for sharp contrast
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set text color to bright green/cyan
    glColor3f(0.0f, 1.0f, 0.8f);

    // Render strings at absolute pixel coordinates
    RenderBitmapString18x30(50.0f, 250.0f, "RETINA SCREEN HIGH-DENSITY FONT");
    RenderBitmapString18x30(50.0f, 200.0f, "Font Footprint: 18x30 Pixels");
    RenderBitmapString18x30(50.0f, 150.0f, "ASCII Range Test: !@#$%^&*()_+12345");

    // Swap buffers to push content to screen
    glutSwapBuffers();
}

// Window resize adjustment loop
void reshape(int w, int h) {
    // Map Viewport directly to window pixels
    glViewport(0, 0, w, h);

    // Initialize 2D Orthogonal Camera space mapping pixels 1:1
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    // Set typical fallback size. 
    // Note: If running on a true Retina display OS layer, 
    // your OS may scale window buffers automatically.
    glutInitWindowSize(800, 450);
    glutCreateWindow("Custom glBitmap Retina Font Window");

    // Register routines
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Hand execution to loop
    glutMainLoop();
    return 0;
}

