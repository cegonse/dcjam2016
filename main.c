#include "engine.h"

KOS_INIT_FLAGS(INIT_DEFAULT);

typedef struct room_data
{
	texture_t *tex;
	mesh_t *mesh;
	transform_t *transform;
} room_data_t;

int main(int argc, char **argv)
{
	// Start the game engine
	engine_t engine;
	
	maple_device_t* cont;
    cont_state_t* state;

	if (engine_init(&engine) < 0)
	{
		debug_error("[main.c] Couldn't start game engine");
	}
	else
	{
		engine.renderer.wireframe = 0;

		engine.scene.cameraFov = 45.0f;
		engine.scene.drawGizmos = 1;

		engine.scene.backgroundColor.m[0] = 0.f;
		engine.scene.backgroundColor.m[1] = 0.f;
		engine.scene.backgroundColor.m[2] = 0.f;
		
		engine.scene.cameraPosition.m[0] = 30.0f;
		engine.scene.cameraPosition.m[1] = 45.0f;
		engine.scene.cameraPosition.m[2] = 30.0f;

		engine.scene.cameraDirection.m[0] = 0.0f;
		engine.scene.cameraDirection.m[1] = 0.0f;
		engine.scene.cameraDirection.m[2] = 0.0f;

		// Load the test block
		transform_t* block = transform_instantiate();
		texture_t blockTexture;
		mesh_t blockMesh;

		resources_load_texture("/cd/data/block.raw", GL_LINEAR, &blockTexture);
		resources_load_mesh("/cd/data/box.mesh", &blockMesh);
		blockMesh.texture = &blockTexture;
		
		block->localScale.m[0] = 0.5f;
		block->localScale.m[1] = 0.5f;
		block->localScale.m[2] = 0.5f;
		
		block->localPosition.m[0] = -15.f;
		block->localPosition.m[1] = 3.f;
		block->localPosition.m[2] = -15.f;
		
		// Load the room 1
		transform_t* room = transform_instantiate();
		texture_t roomTexture;
		mesh_t roomMesh;
		
		resources_load_texture("/cd/data/Room1.raw", GL_LINEAR, &roomTexture);
		resources_load_mesh("/cd/data/Room1.mesh", &roomMesh);
		roomMesh.texture = &roomTexture;
		
		room->localScale.m[0] = 4.f;
		room->localScale.m[1] = 4.f;
		room->localScale.m[2] = 4.f;
		
		// Load the corridor 1
		transform_t* corridor = transform_instantiate();
		texture_t corridorTexture;
		mesh_t corridorMesh;
		
		resources_load_texture("/cd/data/Corridor1.raw", GL_LINEAR, &corridorTexture);
		resources_load_mesh("/cd/data/Corridor1.mesh", &corridorMesh);
		corridorMesh.texture = &corridorTexture;
		
		corridor->localScale.m[0] = 0.8f;
		corridor->localScale.m[1] = 0.8f;
		corridor->localScale.m[2] = 0.8f;
		
		corridor->localPosition.m[0] = 96.f;
		corridor->localPosition.m[1] = 1.1f;
		corridor->localPosition.m[2] = 0.2f;
		
		// Load the room 2
		transform_t* room2 = transform_instantiate();
		texture_t room2Texture;
		mesh_t room2Mesh;
		
		resources_load_texture("/cd/data/Room2.raw", GL_LINEAR, &room2Texture);
		resources_load_mesh("/cd/data/Room2.mesh", &room2Mesh);
		room2Mesh.texture = &room2Texture;
		
		room2->localScale.m[0] = 0.5f;
		room2->localScale.m[1] = 0.5f;
		room2->localScale.m[2] = 0.5f;
		
		room2->localPosition.m[0] = 191.5f;
		room2->localPosition.m[1] = 1.f;
		room2->localPosition.m[2] = 1.f;
		
		room2->localRotation.m[1] = -90.f;
		
		// Load the turret
		transform_t* turret = transform_instantiate();
		transform_t* turretDock = transform_instantiate();
		turret->parent = turretDock;
		
		texture_t turretTexture, turretDockTexture;
		mesh_t turretMesh, turretDockMesh;
		
		resources_load_texture("/cd/data/Turret.raw", GL_LINEAR, &turretTexture);
		resources_load_texture("/cd/data/TurretDock.raw", GL_LINEAR, &turretDockTexture);
		
		resources_load_mesh("/cd/data/Turret.mesh", &turretMesh);
		resources_load_mesh("/cd/data/TurretDock.mesh", &turretDockMesh);
		
		turretMesh.texture = &turretTexture;
		turretDockMesh.texture = &turretDockTexture;
		
		turret->localScale.m[0] = 1.f;
		turret->localScale.m[1] = 1.f;
		turret->localScale.m[2] = 1.f;
		
		turretDock->localScale.m[0] = 0.5f;
		turretDock->localScale.m[1] = 0.5f;
		turretDock->localScale.m[2] = 0.5f;
		
		turretDock->localPosition.m[0] = 191.5f;
		turretDock->localPosition.m[1] = 0.f;
		turretDock->localPosition.m[2] = 0.f;
		
		// Create the room list
		room_data_t *roomList = calloc(32, sizeof(room_data_t));
		
		roomList[0].transform = room;
		roomList[0].mesh = &roomMesh;
		roomList[0].tex = &roomTexture;
		
		roomList[1].transform = corridor;
		roomList[1].mesh = &corridorMesh;
		roomList[1].tex = &corridorTexture;
		
		roomList[2].transform = room2;
		roomList[2].mesh = &room2Mesh;
		roomList[2].tex = &room2Texture;
		
		// Load the player
		transform_t* player = transform_instantiate();
		texture_t playerTexture;
		mesh_t playerMesh;
		
		resources_load_texture("/cd/data/player.raw", GL_LINEAR, &playerTexture);
		resources_load_mesh("/cd/data/player.mesh", &playerMesh);
		playerMesh.texture = &playerTexture;
		
		player->localScale.m[0] = 0.08f;
		player->localScale.m[1] = 0.08f;
		player->localScale.m[2] = 0.08f;
		
		player->localPosition.m[1] = 1.3f;
		
		// Load the bullet
		texture_t bulletTexture;
		mesh_t bulletMesh;
		
		resources_load_texture("/cd/data/bullet.raw", GL_LINEAR, &bulletTexture);
		resources_load_mesh("/cd/data/bullet.mesh", &bulletMesh);
		bulletMesh.texture = &bulletTexture;
		
		// Create a bullet pool
		transform_t **bulletPool = calloc(32, sizeof(transform_t*));
		int i = 0;
		
		for (i = 0; i < 32; ++i)
		{
			bulletPool[i] = transform_instantiate();
			
			bulletPool[i]->localPosition.m[0] = 1000.f;
			bulletPool[i]->localPosition.m[1] = 10.f;
			bulletPool[i]->localPosition.m[2] = 1000.f;
			
			bulletPool[i]->localScale.m[0] = 0.3f;
			bulletPool[i]->localScale.m[1] = 0.3f;
			bulletPool[i]->localScale.m[2] = 0.3f;
		}
		
		float shootCadence = 0.1f, shootTimer = 0.f;
		uint8_t bulletsInUse[32] = { 0 };
		vec4_t bulletDirections[32] = { VEC4_ZERO };
		float bulletTimers[32] = { 0.f };
		float shootSpeed = 4.f;
		
		// Load the main font
		font_t font;
		resources_load_font("/cd/data/Arial.binfnt", &font);
		
		float playerRot = 180.f, playerSpeed = 0.7f;
		float cameraAngle = 0.f, cam_len = 1.f;
		float cam_x = 30.f, cam_y = 30.f;
		float pl_fwd_x = 0.f, pl_fwd_y = 1.f;
		char dbgVram[128], dbg[128];
		
		float enemyDistance = 25.f;
		
		// Enemy list
		transform_t **enemyList = calloc(64, sizeof(transform_t*));
		enemyList[0] = block;
		enemyList[1] = turretDock;
		transform_t *enemyIterator = enemyList[0];
		int enemyCounter = 0;
		
		float turret_fwd_x = 1.f, turret_fwd_y = 0.f;
		
		while (!engine_doFrame())
		{
			// Draw scene
			for (i = 0; i < 32; ++i)
			{
				if (roomList[i].transform != NULL)
				{
					renderer_drawMesh(roomList[i].transform, roomList[i].mesh, &engine.scene);
				}
			}
			
			renderer_drawMesh(player, &playerMesh, &engine.scene);
			renderer_drawMesh(block, &blockMesh, &engine.scene);
			
			renderer_drawMesh(turret, &turretMesh, &engine.scene);
			renderer_drawMesh(turretDock, &turretDockMesh, &engine.scene);
			
			// Draw bullet pool
			for (i = 0; i < 32; ++i)
			{
				if (bulletsInUse[i])
				{
					renderer_drawMesh(bulletPool[i], &bulletMesh, &engine.scene);
				}
			}
			
			// Draw GUI
			gui_doFrame();
			
			// VRAM Usage
			float vramUsage = pvr_mem_available() / 1000000.f;
			sprintf(dbgVram, "VRAM:%.03f/%.03f-%.02f%%", vramUsage, 16.f, 100.f * (vramUsage / 16.f));
			gui_drawText(dbgVram, 10, 10, &VEC4_ONE, 1.0f, &font, 0.5f);
			
			// Handle input
			cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
			
			// The controller handle could return null if there isn't
			// a controller connected into the slot 1
			if (cont)
			{
				state = (cont_state_t*)maple_dev_status(cont);
				
				float joy_xf = (float)state->joyx / 128.f;
				float joy_yf = (float)state->joyy / 128.f;
				
				// Handle the camera rotation
				if (state->ltrig != 0.f)
				{
					cameraAngle += 0.05f * (float)state->ltrig / 255.f;
				}
				else if (state->rtrig != 0.f)
				{
					cameraAngle -= 0.05f * (float)state->rtrig / 255.f;
				}
				
				if (joy_xf != 0.f)
				{
					cameraAngle += 0.05f * joy_xf;
				}
				
				engine.scene.cameraPosition.m[0] = 25.f * fcos(cameraAngle) + engine.scene.cameraDirection.m[0];
				engine.scene.cameraPosition.m[2] = 25.f * fsin(cameraAngle) + engine.scene.cameraDirection.m[2];
				
				// Get the normalized camera vector
				cam_x = (engine.scene.cameraDirection.m[0] - engine.scene.cameraPosition.m[0]);
				cam_y = (engine.scene.cameraDirection.m[2] - engine.scene.cameraPosition.m[2]);
				cam_len = fsqrt(fipr_magnitude_sqr(cam_x, cam_y, 0.f, 0.f));
				
				if (cam_len != 0.f)
				{
					cam_x /= cam_len;
					cam_y /= cam_len;
				}
				
				// Back the camera position a bit from the player
				engine.scene.cameraPosition.m[0] -= cam_x * 20.f;
				engine.scene.cameraPosition.m[2] -= cam_y * 20.f;
				
				// Rotate the player around the camera vector
				pl_fwd_x = -cam_x;
				pl_fwd_y = -cam_y;
				
				enemyIterator = &enemyList[0];
				enemyCounter = 0;
				
				// Search for enemies. If there is a close enemy, point towards it
				while (enemyCounter < 64)
				{
					enemyIterator = enemyList[enemyCounter];
					
					if (enemyIterator != NULL)
					{
						pl_fwd_x = player->localPosition.m[0] - enemyIterator->localPosition.m[0];
						pl_fwd_y = player->localPosition.m[2] - enemyIterator->localPosition.m[2];
						
						float pl_fwd_len = fsqrt(abs(fipr_magnitude_sqr(pl_fwd_x, pl_fwd_y, 0.f, 0.f)));
						
						if (pl_fwd_len <= enemyDistance)
						{
							if (pl_fwd_len != 0.f)
							{
								pl_fwd_x /= pl_fwd_len;
								pl_fwd_y /= pl_fwd_len;
							}
							
							break;
						}
						else
						{
							pl_fwd_x = -cam_x;
							pl_fwd_y = -cam_y;
						}
					}
					
					++enemyCounter;
				}
				
				playerRot = atan2(pl_fwd_x, pl_fwd_y) * 180.f / M_PI;
				player->localRotation.m[1] = playerRot;
				
				// Make the turret follow the player
				turret_fwd_x = player->localPosition.m[0] - turretDock->localPosition.m[0];
				turret_fwd_y = player->localPosition.m[2] - turretDock->localPosition.m[2];
				
				float turret_fwd_len = fsqrt(abs(fipr_magnitude_sqr(turret_fwd_x, turret_fwd_y, 0.f, 0.f)));
				
				if (turret_fwd_len != 0.f)
				{
					turret_fwd_x /= turret_fwd_len;
					turret_fwd_y /= turret_fwd_len;
				}
				
				turret->localRotation.m[1] = (atan2(turret_fwd_x, turret_fwd_y) * 180.f / M_PI) + 90.f;
				
				// If the A button is pressed, shoot
				if (state->buttons & CONT_A)
				{
					// Find an idle bullet and shoot it
					if (shootTimer <= 0.f)
					{
						for (i = 0; i < 32; ++i)
						{
							if (!bulletsInUse[i])
							{
								bulletsInUse[i] = 1;
								bulletPool[i]->localPosition.m[0] = player->localPosition.m[0];
								bulletPool[i]->localPosition.m[2] = player->localPosition.m[2];
								
								bulletPool[i]->localRotation.m[1] = playerRot - 90.f;
								
								bulletDirections[i].m[0] = -pl_fwd_x;
								bulletDirections[i].m[2] = -pl_fwd_y;
								
								bulletTimers[i] = 0.f;
								
								break;
							}
						}
						
						shootTimer = shootCadence;
					}
					
					shootTimer -= engine.deltaTime;
				}
				
				// Move all bullets in use
				for (i = 0; i < 32; ++i)
				{
					if (bulletsInUse[i])
					{
						bulletPool[i]->localPosition.m[0] += bulletDirections[i].m[0] * shootSpeed;
						bulletPool[i]->localPosition.m[2] += bulletDirections[i].m[2] * shootSpeed;
						bulletTimers[i] += engine.deltaTime;
						
						// Despawn the bullet if too much time has passed
						if (bulletTimers[i] > 3.f)
						{
							bulletsInUse[i] = 0;
						}
					}
				}
				
				// The player movement is the camera vector modulated by the input vector
				player->localPosition.m[0] += cam_x * -joy_yf * playerSpeed;
				player->localPosition.m[2] += cam_y * -joy_yf * playerSpeed;
				
				engine.scene.cameraPosition.m[0] += cam_x * -joy_yf * playerSpeed;
				engine.scene.cameraPosition.m[2] += cam_y * -joy_yf * playerSpeed;

				engine.scene.cameraDirection.m[0] += cam_x * -joy_yf * playerSpeed;
				engine.scene.cameraDirection.m[2] += cam_y * -joy_yf * playerSpeed;
			}
			
			gui_endFrame();
			engine_endFrame();
		}
		
		transform_destroy(block);
		transform_destroy(room);
		transform_destroy(player);
		transform_destroy(corridor);
		
		resources_unload_texture(&blockTexture);
		resources_unload_texture(&roomTexture);
		resources_unload_texture(&playerTexture);
		resources_unload_texture(&corridorTexture);
		resources_unload_font(&font);
		
		free(enemyList);

		engine_destroy();
	}

	return 0;
}