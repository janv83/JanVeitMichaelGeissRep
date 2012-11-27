#pragma once

#include "GLEW/glew.h"

#include <GL/GL.h>
#include <GL/GLU.h>


#include <string>

namespace cg2
{
	void checkGlState();
	std::string getErrorString( const GLenum state );
	std::string loadShaderSource( std::string const& filePath );
}