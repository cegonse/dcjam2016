#ifndef _ENGINE_H_
#define _ENGINE_H


#define DREAM_BUILD
#include <kos.h>
#include <stdint.h>

#include "renderer.h"
#include "scene.h"
#include "debug.h"
#include "resources.h"
#include "gui.h"

/*
 * Engine structure: engine_t
 *
 * - Holds the context por the whole game engine.
 * - Public attributes:
 *   � (PC Only) window: OpenGL window context.
 *   � renderer: graphics renderer context.
 *   � scene: scene graph manager context.
 *   � internalResolutionX/Y: real render buffer width and height.
 *         This will be used to be able to render to a constant render
 *         resolution independently of the window size, using a FBO.
 *   � deltaTime: time in seconds needed to process the last frame.
 *
 * - Public methods:
 *   � engine_init: initializes the engine and instances the subsystems owned
 *                  by itself (renderer, scene manager).
 *   � engine_destroy: gracefully clears all data allocated by the engine and
 *                     closes the app.
 *   � engine_doFrame: commands all subsystems to start a new game frame.
 *   � engine_endFrame: commits all changes to all subsystems in order to
 *                      end the game frame.
 *
*/
typedef struct engine
{
	renderer_t renderer;
	scene_t scene;

	int internalResolutionX;
	int internalResolutionY;
	double lastTime;
	double deltaTime;
} engine_t;

// Public functions
int engine_init(engine_t* hndl);
void engine_destroy();
int engine_doFrame();
void engine_endFrame();

#endif // _ENGINE_H_