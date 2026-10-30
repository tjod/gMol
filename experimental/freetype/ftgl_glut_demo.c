#define GL_SILENCE_DEPRECATION 
#include <GLUT/glut.h>         
#include <FTGL/ftgl.h>

const char* FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf";
FTFont* font = nullptr;
float rotationAngle = 0.0f;

// Setup basic OpenGL lighting so we can see the 3D depth/thickness
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); // Allows glColor3f to still change text colors

    // Position light slightly above, to the right, and in front of the scene
    GLfloat lightPos[] = { 5.0f, 5.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Subtle ambient light so shadowed sides aren't pitch black
    GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_position[] = { 1.0f, 2.0f, 3.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void display() {
    glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Position camera
    gluLookAt(0.0, 0.0, 15.0,  
              0.0, 0.0, 0.0,   
              0.0, 1.0, 0.0);  

    // Render Extruded Text in 3D Space
    if (font) {
        glPushMatrix();
            // Center the text roughly in the middle of the screen
            glTranslatef(-3.5f, -0.5f, 0.0f); 
            
            // Rotate the text to reveal its thick 3D edges
            glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
            glRotatef(-25.0f, 1.0f, 0.0f, 0.0f);
            glRotatef(-25.0f, 0.0f, 0.0f, 1.0f);
            
            // Scale text down to fit 3D viewport coordinates
            glScalef(0.04f, 0.04f, 0.04f); 
            
            // Text color (Bright cyan)
            glColor3f(0.0f, 0.6f, 0.6f);
            
            // Render the thick 3D mesh
            font->Render("Extruded!");
        glPopMatrix();
    }

    glutSwapBuffers();
}

void timer(int value) {
    rotationAngle += 1.0f;
    if (rotationAngle > 360.0f) rotationAngle -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); 
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
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
    glutCreateWindow("FTGL Thick Extruded Text");

    glEnable(GL_DEPTH_TEST);
    setupLighting(); // Initialize the lights

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    // 1. Instantiate as FTExtrudeFont instead of FTPolygonFont
    font = new FTExtrudeFont(FONT_PATH); 
    if (font->Error()) {
        //std::cerr << "Could not load font: " << FONT_PATH << std::endl;
        printf("Could not load font");
        delete font;
        return -1;
    }
    
    // 2. Set Font Properties
    font->FaceSize(48); 
    
    // 3. CRITICAL STEP: Define the physical depth/thickness of the extrusion.
    // The value represents OpenGL coordinate units relative to the font's FaceSize.
    font->Depth(20.0f); 

    glutMainLoop();
    return 0;
}
