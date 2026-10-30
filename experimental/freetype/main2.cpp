#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#define GL_SILENCE_DEPRECATION // Silences macOS legacy OpenGL warnings
#include <GLUT/glut.h>

// FreeType Headers
#include <ft2build.h>
#include FT_FREETYPE_H

// Structure to hold metrics and texture data for each character glyph
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    int SizeX, SizeY;   // Size of glyph (width, height)
    int BearingX, BearingY; // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next character
};
/*
struct Character {
    GLuint textureId;  // ID handle of the glyph texture
    int width;         // Glyph width in pixels
    int height;        // Glyph height in pixels
    int bearingX;      // Offset from baseline to left of glyph
    int bearingY;      // Offset from baseline to top of glyph
    long advance;      // Offset to advance to next glyph
};
*/

// Storage map for the ASCII characters (32 to 126)
std::map<char, Character> Characters;

// Initializes FreeType, loads a font, and populates the character map textures
bool initFreeTypeFont(const char* fontPath, unsigned int fontSize) {
    FT_Library ft;
    // Initialize the FreeType library
    if (FT_Init_FreeType(&ft)) {
        printf("Error: Could not init FreeType Library\n");
        return false;
    }

    FT_Face face;
    // Load the font face
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        printf("Error: Failed to load font at %s\n", fontPath);
        FT_Done_FreeType(ft);
        return false;
    }

    // Set font size (width set to 0 dynamically calculates height aspect ratio)
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable pixel alignment restrictions so textures don't skew
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (unsigned char c = 32; c < 128; c++) {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            printf("Error: Failed to load Glyph for ASCII %d\n", c);
            continue;
        }

        // Generate an individual texture ID for this character
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // Target GL_LUMINANCE_ALPHA so glColor3f can dynamically shade the text
        // FreeType rasterizes to an 8-bit grayscale bitmap (`face->glyph->bitmap.buffer`)
        int width = face->glyph->bitmap.width;
        int height = face->glyph->bitmap.rows;
        unsigned char* srcBuffer = face->glyph->bitmap.buffer;
        
        unsigned char* rgbaData = (unsigned char*)malloc(width * height * 2);
        for (int i = 0; i < width * height; ++i) {
            rgbaData[i * 2 + 0] = 255;          // RGB Channel (White)
            rgbaData[i * 2 + 1] = srcBuffer[i]; // Alpha Channel from FreeType bitmap
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, rgbaData);
        free(rgbaData);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character data for rendering later
        Character character = {
            texture,
            width,
            height,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    // Destroy FreeType objects once textures are securely on the GPU memory pipeline
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return true;
}

// Renders a string at a specified (x, y, z) baseline pixel coordinate
void drawFreeTypeText(float x, float y, float z, const char* text) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (const char* c = text; *c != '\0'; c++) {
        Character ch = Characters[*c];

        // Calculate positions relative to font baseline metrics
        float xpos = x + ch.bearingX;
        float ypos = y - ch.bearingY; // Negative because of top-left window coordinates
        float zpos = z;

        float w = ch.width;
        float h = ch.height;

        // Bind the specific texture allocated for this character
        glBindTexture(GL_TEXTURE_2D, ch.textureId);

        // Render textured quad
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos,     ypos,     zpos);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(xpos + w, ypos,     zpos);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(xpos + w, ypos + h, zpos);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos,     ypos + h, zpos);
        glEnd();

        // Advance cursors for next glyph (advance is number of 1/64 pixels)
        x += (ch.advance >> 6); 
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void renderCustomBillboardText3D(
    std::string text, 
    float worldX, float worldY, float worldZ, // 3D World Anchor
    float localX, float localY,                 // Local Translation (Shift text on screen plane)
    float localRotationAngle,                 // Local Rotation (Spin text like a clock hand)
    float localScaleFactor,                   // Local Scale (Grow/Shrink text)
    float retinaScale) 
{
    glPushMatrix();
    
    // =========================================================================
    // PHASE 1: POSITION IN THE 3D WORLD
    // =========================================================================
    glTranslatef(worldX, worldY, worldZ);
    
    // =========================================================================
    // PHASE 2: THE BILLBOARD SNAP (Strip Camera Rotation)
    // =========================================================================
    float modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    
    // Extract the scaling elements from the current modelview matrix 
    // to preserve any global/camera scaling if it exists.
    float scaleX = sqrt(modelview[0]*modelview[0] + modelview[1]*modelview[1] + modelview[2]*modelview[2]);
    float scaleY = sqrt(modelview[4]*modelview[4] + modelview[5]*modelview[5] + modelview[6]*modelview[6]);
    float scaleZ = sqrt(modelview[8]*modelview[8] + modelview[9]*modelview[9] + modelview[10]*modelview[10]);

    // Reset the rotation components to Identity, but keep global scale
    modelview[0] = scaleX; modelview[1] = 0.0f;   modelview[2] = 0.0f;
    modelview[4] = 0.0f;   modelview[5] = scaleY; modelview[6] = 0.0f;
    modelview[8] = 0.0f;   modelview[9] = 0.0f;   modelview[10] = scaleZ;
    
    glLoadMatrixf(modelview);
    
    // =========================================================================
    // PHASE 3: LOCAL USER TRANSFORMS (Relative to the Billboard Plane)
    // =========================================================================
    // 1. Move the text locally on the 2D billboard plane
    glTranslatef(localX, localY, 0.0f);
    
    // 2. Rotate the text around its local anchor point (Z-axis points out of screen)
    glRotatef(localRotationAngle, 0.0f, 0.0f, 1.0f);
    
    // 3. Combine your custom scale factor with the mandatory Retina resolution correction
    float finalScale = (0.01f * localScaleFactor) / retinaScale; 
    glScalef(finalScale, finalScale, 1.0f);
    
    // =========================================================================
    // PHASE 4: RENDER THE FREETYPE QUADS
    // =========================================================================
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    
    float cursorX = 0.0f;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];
        
        float xpos = cursorX + ch.bearingX;
        float ypos = -(ch.SizeY - ch.bearingY);
        float w = ch.SizeX;
        float h = ch.SizeY;
        
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos,     ypos + h);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos,     ypos);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos + h);
        glEnd();
        
        cursorX += (ch.Advance >> 6); 
    }
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 1. Draw your 3D assets/scene objects here...
    glLoadIdentity();
    glTranslatef(100.0f, 100.0f, 0.0f);
    glRotatef(20.0f, 0.0f, 0.0f, 1.0f);

    // 2. Switch matrix over to flat Ortho Overlay Mode
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    // Maps 0,0 to the Top-Left of your display window
    glOrtho(0, width, height, 0, -1, 1); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST); 

    // 3. Render the Text
    glColor3f(0.2f, 0.8f, 1.0f); // Bright blue text color cyan
    //drawFreeTypeText(30.0f, 60.0f, 1.0f, "Hello FreeType text on macOS!"); 
    renderCustomBillboardText3D(
     "Hello new render test",
     30.0f, 60.0f, 0.0f, // 3D World Anchor
     0.0f, 0.0f,                 // Local Translation (Shift text on screen plane)
     0.0f,                 // Local Rotation (Spin text like a clock hand)
     1.0f,                    // Local Scale (Grow/Shrink text)
     2.0f)  // retina scale


    // 4. Restore matrix state pipeline back to normal
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("FreeType legacy OpenGL Text");

    // Initialize FreeType.
    // Unlike stb_truetype, FreeType parses TrueType Collection (.ttc) sub-files perfectly!
    // We can directly ask it for a high-res 48pt font size to look stunning on Retina.
    if (!initFreeTypeFont("/System/Library/Fonts/Helvetica.ttc", 48)) {
        return -1;
    }

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
