#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>

#include "renderer.h"
#include "debug.h"

#define TEXTURE_TYPE_RAW 0x00
#define TEXTURE_TYPE_PVR 0x01

#define TEXTURE_RAW_HEADER_LENGTH 0x05

// Public methods
void resources_init();

void resources_load_texture(const char *filename, GLint filtering, texture_t *tex);
void resources_unload_texture(texture_t *tex);

void resources_load_mesh(const char* filename, mesh_t* mesh);
void resources_unload_mesh(mesh_t* mesh);

void resources_load_font(const char* filename, font_t* font);
void resources_unload_font(font_t* font);

#endif // _RESOURCES_H_