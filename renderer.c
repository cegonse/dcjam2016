#include "renderer.h"
#include "engine.h"
#include "builtin_meshes.h"

#include <malloc.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

renderer_t* rendererInstance;
mesh_t primitiveCube;

void renderer_init(renderer_t* rend)
{
	rend->renderResolutionX = 640;
	rend->renderResolutionY = 480;

	rend->cameraMatrix = IDENTITY_MATRIX;
	rend->projectionMatrix = IDENTITY_MATRIX;

	primitiveCube.vertices = builtin_cube_vertex_data;
	primitiveCube.uv = builtin_cube_uv_data;
	primitiveCube.texture = NULL;
	primitiveCube.count = BUILTIN_CUBE_VERTEX_COUNT;
	primitiveCube.colorTint = VEC4_ONE;

	// Set the video mode. Use always 640x480x24
	vid_set_mode(DM_640x480, PM_RGB565);
	glKosInit();

	rendererInstance = rend;
}

void renderer_doFrame(scene_t* scene)
{
	glViewport(0, 0, rendererInstance->renderResolutionX, rendererInstance->renderResolutionY);

	glShadeModel(GL_FLAT);
	
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	// Set projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);

	glClearColor(scene->backgroundColor.m[0], scene->backgroundColor.m[1], scene->backgroundColor.m[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_setCamera(scene_t* scene)
{
	gluLookAt(scene->cameraPosition.m[0], scene->cameraPosition.m[1], scene->cameraPosition.m[2],
		scene->cameraDirection.m[0], scene->cameraDirection.m[1], scene->cameraDirection.m[2],
		0.0f, 1.0f, 0.0f);
}

void renderer_drawAxes()
{
	glBegin(GL_LINES);

	// X axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);

	// Z axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);

	glEnd();
}

void renderer_drawMesh(const transform_t* model, const mesh_t* mesh, const scene_t* scene)
{
	vec4_t worldPosition = VEC4_ZERO;
	transform_getWorldPosition(model, &worldPosition);
	
	glEnable(GL_KOS_NEARZ_CLIPPING);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(worldPosition.m[0], worldPosition.m[1], worldPosition.m[2]);
	glScalef(model->localScale.m[0], model->localScale.m[1], model->localScale.m[2]);
	glRotatef(model->localRotation.m[1], 0.f, 1.f, 0.f);
	
	renderer_setCamera(scene);
	
	glColor3f(mesh->colorTint.m[0], mesh->colorTint.m[1], mesh->colorTint.m[2]);

	if (mesh->texture != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mesh->texture->nativePtr);
	}

	if (mesh->normals != NULL)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, mesh->normals);
	}

	if (mesh->uv != NULL)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, mesh->uv);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mesh->vertices);
	
	glDrawElements(GL_TRIANGLES, mesh->count, GL_UNSIGNED_SHORT, mesh->indices);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	
	if (mesh->normals != NULL)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	
	if (mesh->uv != NULL)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if (mesh->texture != NULL)
	{
		glDisable(GL_TEXTURE_2D);
	}

	glDisable(GL_KOS_NEARZ_CLIPPING);
}

mesh_t* renderer_getPrimitiveCube()
{
	return &primitiveCube;
}

void renderer_endFrame()
{
	glutSwapBuffers();
}

void renderer_destroy()
{
}
