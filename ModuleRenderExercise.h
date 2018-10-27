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
	void			DrawReferenceGround();
	void			DrawReferenceAxis();

public:
	math::float3 target;
	math::float3 eye;
	math::float3 up;


private:
    unsigned vbo        = 0;
	GLuint program		= GL_FALSE;

};

#endif /* __ModuleRenderExercise_h__ */
