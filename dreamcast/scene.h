#ifndef _SCENE_H_
#define _SCENE_H_

#ifdef _WIN64
#pragma once
#endif

#include "vector.h"

#include <stdlib.h>
#include <string.h>

#define MAX_SCENE_NODES 128

typedef struct transform transform_t;

struct transform
{
	vec4_t localPosition;
	vec4_t localScale;
	vec4_t localRotation;
	
	transform_t* parent;
};

typedef struct scene
{
	transform_t* nodes[MAX_SCENE_NODES];
	unsigned char nodeCount;

	vec4_t backgroundColor;
	vec4_t cameraPosition;
	vec4_t cameraDirection;
	float cameraFov;
	mat44_t projectionMatrix;

	unsigned char drawGizmos;
} scene_t;

void scene_init(scene_t* sc);
void scene_doFrame();

transform_t* transform_instantiate();
void transform_destroy(transform_t* go);
void transform_getWorldPosition(transform_t* go, vec4_t* p);

#endif // _SCENE_H_