#ifndef __ModuleScene_h__
#define __ModuleScene_h__

#include "Module.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
#include "SDL.h"

class ModuleScene : public Module
{
public:
    ModuleScene();
    ~ModuleScene();

	bool            Init    ();
	update_status   Update  ();
	bool            CleanUp ();
	void			DrawReferenceGround();
	void			DrawReferenceAxis();

public:
	math::float3 target;
	math::float3 eye;
	math::float3 up;
	GLuint texture0		= 0;

private:
    unsigned vbo        = 0;
	GLuint progDefault  = GL_FALSE;
	GLuint progTexture  = GL_FALSE;

};

#endif /* __ModuleScene_h__ */
