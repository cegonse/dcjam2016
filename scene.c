#include "scene.h"

scene_t* sceneInstance;

void scene_init(scene_t* sc)
{
	unsigned char i = MAX_SCENE_NODES;

	sceneInstance = sc;

	sc->nodeCount = 0;
	sc->backgroundColor = VEC4_ONE;
	sc->cameraFov = 60.0f;
	sc->cameraPosition = VEC4_ZERO;
	sc->cameraDirection = VEC4_ONE;

	while (i --> 0)
	{
		sc->nodes[i] = NULL;
	}
}

void scene_doFrame()
{
}

transform_t* transform_instantiate()
{
	unsigned char i = sceneInstance->nodeCount;
	transform_t* tr = NULL, *next = sceneInstance->nodes[i];

	if (sceneInstance->nodeCount < MAX_SCENE_NODES)
	{
		tr = malloc(sizeof(transform_t));

		tr->parent = NULL;
		tr->localPosition = VEC4_ZERO;
		tr->localRotation = VEC4_ZERO;
		tr->localScale = VEC4_ONE;

		if (i != 0)
		{
			for (; i < MAX_SCENE_NODES; ++i)
			{
				if (next == NULL)
				{
					sceneInstance->nodes[i] = tr;
					break;
				}
				else
				{
					++next;
				}
			}
		}
		else
		{
			sceneInstance->nodes[0] = tr;
		}

		++sceneInstance->nodeCount;
	}

	return tr;
}

void transform_destroy(transform_t* go)
{
	unsigned char i = sceneInstance->nodeCount;
	transform_t* next = sceneInstance->nodes[0];

	while (i --> 0)
	{
		if (go == next)
		{
			free(go);
			sceneInstance->nodes[i] = NULL;
			break;
		}

		++next;
	}
}

void transform_getWorldPosition(transform_t* go, vec4_t* p)
{
	transform_t* next = go->parent;

	p->m[0] = go->localPosition.m[0];
	p->m[1] = go->localPosition.m[1];
	p->m[2] = go->localPosition.m[2];

	while (next != NULL)
	{
		p->m[0] += next->localPosition.m[0];
		p->m[1] += next->localPosition.m[1];
		p->m[2] += next->localPosition.m[2];

		next = next->parent;
	}
}