#include "resources.h"

void resources_init()
{
}

void resources_load_texture(const char *filename, GLint filtering, texture_t *tex)
{
	FILE* hndl = fopen(filename, "rb");
	unsigned char textureType;
	uint16_t textureWidth, textureHeight;
	uint32_t textureSize;
	uint8_t *binaryData;

	if (hndl != NULL)
	{
		// Get the texture type from the header
		fread(&textureType, sizeof(unsigned char), 1, hndl);

		// Load the texture using the appropriate format
		if (textureType == TEXTURE_TYPE_RAW)
		{
			// Get the complete texture width and height
			fread(&textureWidth, sizeof(uint16_t), 1, hndl);
			fread(&textureHeight, sizeof(uint16_t), 1, hndl);

			tex->height = textureHeight;
			tex->width = textureWidth;

			// Get the file size and load the binary data
			fseek(hndl, 0, SEEK_END);
			textureSize = ftell(hndl) - TEXTURE_RAW_HEADER_LENGTH;
			rewind(hndl);
			fseek(hndl, TEXTURE_RAW_HEADER_LENGTH, SEEK_SET);

			// Load the binary data
			binaryData = malloc(textureSize);
			fread(binaryData, sizeof(uint8_t), textureSize, hndl);

			// Create the native texture and upload the binary data
			// to the GPU
			glGenTextures(1, &tex->nativePtr);
			glBindTexture(GL_TEXTURE_2D, tex->nativePtr);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, binaryData);

			// Set the appropriate filtering for the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Unload the binary data from RAM
			free(binaryData);
		}
		else if (textureType == TEXTURE_TYPE_PVR)
		{
			debug_warn("PVR texture loading not implemented. Ignoring");
			debug_warn(filename);
		}
	}
	else
	{
		debug_error("Texture file couldn't be loaded!");
		debug_error(filename);
	}

	fclose(hndl);
}

void resources_unload_texture(texture_t *tex)
{
	// Free the GPU data
	glBindTexture(GL_TEXTURE_2D, tex->nativePtr);
	glDeleteTextures(1, &tex->nativePtr);
}

void resources_load_mesh(const char* filename, mesh_t* mesh)
{
	FILE* hndl = fopen(filename, "rb");
	uint16_t vertexCount;

	if (hndl != NULL)
	{
		// Get the vertex count from the header
		fread(&vertexCount, sizeof(uint16_t), 1, hndl);
		mesh->count = vertexCount;

		// Allocate memory for vertices and UV coordinates
		mesh->vertices = malloc(sizeof(float) * vertexCount * 3);
		mesh->uv = malloc(sizeof(float) * vertexCount * 2);
		mesh->normals = malloc(sizeof(float) * vertexCount * 3);
		mesh->indices = malloc(sizeof(unsigned short) * vertexCount);

		// Load the mesh data
		fread(mesh->vertices, sizeof(float), vertexCount * 3, hndl);
		fread(mesh->uv, sizeof(float), vertexCount * 2, hndl);
		fread(mesh->normals, sizeof(float), vertexCount * 3, hndl);
		fread(mesh->indices, sizeof(unsigned short), vertexCount, hndl);
		
		mesh->texture = NULL;
		mesh->colorTint = VEC4_ONE;

		// Close the file
		fclose(hndl);
	}
	else
	{
		debug_error("Mesh file couldn't be loaded!");
		debug_error(filename);
	}
}

void resources_unload_mesh(mesh_t* mesh)
{
	if (mesh->vertices != NULL)
	{
		free(mesh->vertices);
	}

	if (mesh->uv != NULL)
	{
		free(mesh->uv);
	}

	if (mesh->normals != NULL)
	{
		free(mesh->normals);
	}
	
	if (mesh->indices != NULL)
	{
		free(mesh->indices);
	}
}

void resources_load_font(const char* filename, font_t* font)
{
	FILE* hndl = fopen(filename, "rb");
	char fontTexFilename[256], *fontFn;
	
	uint8_t glyphCount;
	uint8_t* buffer;
	uint8_t i, k;
	
	char dbg[256];

	if (hndl != NULL)
	{
		fread(&glyphCount, sizeof(uint8_t), 1, hndl);
		font->count = glyphCount;
		
		// Load the texture used by the font
		fontFn = strrchr(filename, '.');
		fontFn[0] = '\0';
		sprintf(fontTexFilename, "%s.raw", filename);
		
		resources_load_texture(fontTexFilename, GL_LINEAR, &font->texture);
		
		// Allocate font data
		font->glyphs = calloc(glyphCount, sizeof(glyph_t));
		
		// Load all glyphs, caching all the data into
		// a buffer
		buffer = malloc(5 * glyphCount * sizeof(uint8_t));
		fread(buffer, 5 * sizeof(uint8_t), glyphCount, hndl);
		
		for (i = 0, k = 0; i < glyphCount; ++i)
		{
			font->glyphs[i].id = buffer[k++];
			font->glyphs[i].x = (float)buffer[k++] / (float)font->texture.width;
			font->glyphs[i].y = (float)buffer[k++] / (float)font->texture.height;
			font->glyphs[i].h = (float)buffer[k++] / (float)font->texture.height;
			font->glyphs[i].w = (float)buffer[k++] / (float)font->texture.width;
		}
		
		free(buffer);
		fclose(hndl);
	}
	else
	{
		debug_error("Font file couldn't be loaded!");
		debug_error(filename);
	}
}

void resources_unload_font(font_t* font)
{
	resources_unload_texture(&font->texture);
	
	if (font->glyphs)
	{
		free(font->glyphs);
	}
}
