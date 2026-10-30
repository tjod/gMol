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
        
        float xpos = cursorX + ch.BearingX;
        float ypos = -(ch.SizeY - ch.BearingY);
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
