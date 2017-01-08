#include "gui.h"

static int sw = 640, sh = 480;

void gui_init(int screenWidth, int screenHeight)
{
    sw = screenWidth;
    sh = screenHeight;
}

void gui_doFrame()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    glEnable(GL_BLEND); 
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set the GUI projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, sw, 0, sh, 0, 1);
}

void gui_endFrame()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void gui_drawRect(int x, int y, int w, int h, vec4_t* color, float alpha)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor4f(color->m[0], color->m[1], color->m[2], alpha);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_TRIANGLES);
        glVertex3f(x + w, y, 0.f);
        glVertex3f(x, y + h, 0.f);
        glVertex3f(x, y, 0.f);
        
        glVertex3f(x + w, y, 0.f);
        glVertex3f(x + w, y + h, 0.f);
        glVertex3f(x, y + h, 0.f);
    glEnd();
}

void gui_drawTexture(int x, int y, vec4_t* color, texture_t* texture, float alpha)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor4f(color->m[0], color->m[1], color->m[2], alpha);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set the texture atlas for drawing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture->nativePtr);
    
    glBegin(GL_QUADS); 
        glTexCoord2f(0.f, 0.f);
        glVertex3f(x, y, 0.f);
        
        glTexCoord2f(1.f, 0.f);
        glVertex3f(x + texture->width, y, 0.f);
        
        glTexCoord2f(1.f, 1.f);
        glVertex3f(x + texture->width, y + texture->height, 0.f);
        
        glTexCoord2f(0.f, 1.f);
        glVertex3f(x, y + texture->height, 0.f);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void gui_drawText(const char* text, int x, int y, vec4_t* color, float alpha, font_t* font, float scale)
{
    int i, j, len = strlen(text);
    float cummulatedWidth = 0.f, xf = 0.f, yf = 0.f, hf = 0.f, wf = 0.f;
    glyph_t* currGlyph = &font->glyphs[0];
    
    // Set the texture atlas for drawing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font->texture.nativePtr);
    
    // For each character, find its glyph and draw  
    for (i = 0; i < len; ++i)
    {
        // Search the glyph
        for (j = 0; j < font->count; ++j)
        {
            if (font->glyphs[j].id == text[i])
            {
                currGlyph = &font->glyphs[j];
                break;
            }
        }
        
        // Draw it
        xf = x + cummulatedWidth;
        yf = y;
        hf = currGlyph->h * font->texture.height * scale;
        wf = currGlyph->w * font->texture.height * scale;
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glColor4f(color->m[0], color->m[1], color->m[2], alpha);
        glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);
        
        glBegin(GL_QUADS); 
            glTexCoord2f(currGlyph->x, 1.f - currGlyph->y - currGlyph->h);
            glVertex3f(xf, yf, 0.f);
            
            glTexCoord2f(currGlyph->x + currGlyph->w,  1.f - currGlyph->y - currGlyph->h);
            glVertex3f(xf + wf, yf, 0.f);
            
            glTexCoord2f(currGlyph->x + currGlyph->w, 1.f - currGlyph->y);
            glVertex3f(xf + wf, yf + hf, 0.f);
            
            glTexCoord2f(currGlyph->x, 1.f - currGlyph->y);
            glVertex3f(xf, yf + hf, 0.f);
        glEnd();
        
        cummulatedWidth += hf;
    }
    
    glDisable(GL_TEXTURE_2D);
}
