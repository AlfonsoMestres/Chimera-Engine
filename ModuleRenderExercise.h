#ifndef __ModuleRenderExercise_h__
#define __ModuleRenderExercise_h__

#include "Module.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
#include "SDL.h"

class ModuleRenderExercise : public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();

	bool            Init    ();
	update_status   Update  ();
	bool            CleanUp ();
	math::float4x4	LookAt(math::float3& target, math::float3& eye, math::float3& up);
	math::float4x4	ProjectionMatrix();
	void			InitFrustum();
	char*			LoadShader(const char* shaderName);

public:
	math::float3 target;
	math::float3 eye;
	math::float3 up;
	float cameraSpeed = 0.5f;
	Frustum frustum;

private:
    unsigned vbo        = 0;
	GLuint program		= GL_FALSE;

};

#endif /* __ModuleRenderExercise_h__ */
