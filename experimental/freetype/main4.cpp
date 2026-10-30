#define GL_SILENCE_DEPRECATION 
#include <GLUT/glut.h>         
#include <FTGL/ftgl.h>
#include <iostream>

// Font configuration
const char* FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf";
FTFont* font = nullptr;

// Rotation angle for our 3D animation loop
float rotationAngle = 0.0f;

void display() {
    glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Position the camera in 3D space
    gluLookAt(0.0, 0.0, 15.0,  // Camera position
              0.0, 0.0, 0.0,   // Look at target
              0.0, 1.0, 0.0);  // Up vector

    // --- RENDER A REFERENCE 3D OBJECT ---
    glPushMatrix();
        glColor3f(0.2f, 0.6f, 0.3f);
        glTranslatef(-4.0f, 0.0f, 0.0f); // Put a cube on the left
        glRotatef(rotationAngle, 1.0f, 1.0f, 0.0f);
        glutWireCube(2.0f);
    glPopMatrix();

    // --- RENDER TEXT IN 3D SPACE ---
    if (font) {
        glPushMatrix();
            // 1. Position the text globally in the 3D scene
            glTranslatef(1.0f, 0.0f, 0.0f); 
            
            // 2. Rotate the text along with the scene
            glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
            
            // 3. CRITICAL: Scale it down. 
            // Geometry-based FTGL fonts render natively in "font unit" sizes 
            // (e.g. 72 points = roughly 72 units wide), which is huge for a 3D scene.
            glScalef(0.05f, 0.05f, 0.05f); 
            
            // Set 3D Mesh Color (Yellow)
            glColor3f(1.0f, 0.8f, 0.2f);
            
            // 4. Render without using glRasterPos!
            font->Render("3D Text");
        glPopMatrix();
    }

    glutSwapBuffers();
}

// Fixed interval timer callback to animate the text and cube rotation
void timer(int value) {
    rotationAngle += 1.0f;
    if (rotationAngle > 360.0f) rotationAngle -= 360.0f;
    
    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    
    // Use a Perspective Projection for realistic 3D depth
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { 
        if (font) delete font;
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D FTGL Space Example");

    // Enable Depth Testing so text clips correctly behind/in-front of 3D objects
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0); // Start animation loop

    // Initialize an Outline, Polygon, or Extruded font mesh
    // FTPolygonFont = Flat 2D shapes floating in 3D space
    // FTExtrudeFont = Deep, 3D extruded block lettering
    font = new FTPolygonFont(FONT_PATH); 
    if (font->Error()) {
        std::cerr << "Could not load font: " << FONT_PATH << std::endl;
        delete font;
        return -1;
    }
    font->FaceSize(48); 

    glutMainLoop();
    return 0;
}
