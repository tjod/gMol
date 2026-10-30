#define GL_SILENCE_DEPRECATION // Silences macOS legacy OpenGL deprecation warnings
#include <GLUT/glut.h>         // Correct framework header path for macOS
#include <OpenGL/gl.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

// --- Global font variables ---
GLuint fontTexture;
stbtt_bakedchar bakedChars[96]; // Handles ASCII 32..126
int windowWidth = 800;
int windowHeight = 600;

// --- Initialize and Bake Font into Texture Atlas ---
void initFont(const char* filename, float fontSize) {
    // 1. Open the TTF font file safely on macOS
    FILE* f = fopen(filename, "rb");
    if (!f) {
        printf("Error: Could not open font file at %s\n", filename);
        printf("Please ensure the path is valid on your macOS system.\n");
        exit(1);
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char* fontBuffer = (unsigned char*)malloc(size);
    fread(fontBuffer, 1, size, f);
    fclose(f);

    // 2. Allocate memory for a single-channel alpha bitmap atlas
    int atlasWidth = 512;
    int atlasHeight = 512;
    unsigned char* alphaBitmap = (unsigned char*)calloc(atlasWidth * atlasHeight, 1);

    // 3. Bake font outlines directly into the bitmap atlas grid
    int result = stbtt_BakeFontBitmap(fontBuffer, 0, fontSize, alphaBitmap, 
                                      atlasWidth, atlasHeight, 32, 96, bakedChars);
    if (result <= 0) {
        printf("Error: Failed to bake font bitmap. Atlas canvas size is too small.\n");
        exit(1);
    }
    free(fontBuffer);

    // 4. Generate and configure OpenGL Texture mapping
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // CRITICAL FOR MAC: Using GL_LUMINANCE ensures the single color channel maps 
    // cleanly to RGB vectors while dynamically preserving pure alpha opacity blending.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, atlasWidth, atlasHeight, 0, 
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, alphaBitmap);
                 
    free(alphaBitmap);
}

// --- Draw Text String Loop ---
void drawText(float x, float y, const char* text) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    while (*text) {
        char c = *text;
        if (c >= 32 && c < 128) { // <-- FIXED HERE: Proper ASCII range check
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(bakedChars, 512, 512, c - 32, &x, &y, &q, 1);
            
            glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
            glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
            glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
            glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
        }
        text++;
    }
    glEnd();
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}
void reshape(int w, int h) {
    windowWidth = w > 0 ? w : 1;  // <-- FIXED HERE: Standard safety evaluation
    windowHeight = h > 0 ? h : 1;
    glViewport(0, 0, windowWidth, windowHeight);
}
void display() {
    // Clear the screen (color buffer) to the background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Reset any coordinate transformations
    glLoadIdentity();

    // Draw a basic triangle
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red vertex
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0.0f, 1.0f, 0.0f); // Green vertex
        glVertex2f(0.5f, -0.5f);

        glColor3f(0.0f, 0.0f, 1.0f); // Blue vertex
        glVertex2f(0.0f, 0.5f);
    glEnd();
    drawText(100.0, 100.0, "Hello, World!");

    // Force execution of OpenGL commands (renders immediately)
    glFlush();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("stb_truetype macOS Overlay");

    // Standard high-quality text file path bundled natively with macOS Monterey
    initFont("/System/Library/Fonts/Supplemental/Arial.ttf", 36.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
