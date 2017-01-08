#ifndef _VECTOR_H_
#define _VECTOR_H_

#define PI 3.14159265359f

typedef struct mat44
{
	float m[16];
} mat44_t;

/*
0 1 2 3
4 5 6 7
8 9 A B
C D E F
*/

typedef struct vec4
{
	float m[4];
} vec4_t;

static const vec4_t X_AXIS = { { 1, 0, 0, 0 } };
static const vec4_t Y_AXIS = { { 0, 1, 0, 0 } };
static const vec4_t Z_AXIS = { { 0, 0, 1, 0 } };
static const vec4_t INV_X_AXIS = { { -1, 0, 0, 0 } };
static const vec4_t INV_Y_AXIS = { { 0, -1, 0, 0 } };
static const vec4_t INV_Z_AXIS = { { 0, 0, -1, 0 } };
static const vec4_t VEC4_ZERO = { { 0, 0, 0, 0 } };
static const vec4_t VEC4_ONE = { { 1, 1, 1, 1 } };

static const mat44_t IDENTITY_MATRIX = 
{ {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
} };

void mat44_perspective(mat44_t* m, float fovy, float aspect_ratio, float near_plane, float far_plane);
float vec4_distance(vec4_t* p1, vec4_t* p2);
float smoothDamp(float current, float target, float* currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

#endif // _VECTOR_H_
