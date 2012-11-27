#include "glutil.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace cg2
{
	std::string getErrorString( const GLenum state )
	{
		switch( state )
		{
		case GL_NO_ERROR:
			return "no error";
		case GL_INVALID_ENUM:
			return "invalid enum: an unacceptable value is specified for an enumerated argument";
		case GL_INVALID_VALUE:
			return "invalid value: a numeric argument is out of range";
		case GL_INVALID_OPERATION:
			return "invalid operation: the specified operation is not allowed in the current state";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "invalid framebuffer operation: the framebuffer object is not complete";
		case GL_OUT_OF_MEMORY:
			return "out of memory: there is not enough memory left to execute the command";
		case GL_STACK_UNDERFLOW:
			return "stack underflow: an attempt has been made to perform an operation that would cause an internal stack to underflow";
		case GL_STACK_OVERFLOW:
			return "stack overflow: an attempt has been made to perform an operation that would cause an internal stack to overflow";
		default:
			return "unknown error code";
		}
	}

	void checkGlState()
	{
		GLenum state = glGetError();
		if ( state != GL_NO_ERROR ) cout << getErrorString( state ) << endl;
	}

	std::string loadShaderSource( std::string const& filePath )
	{
		std::ifstream file( filePath.c_str() );
		if ( !file.is_open() )
		{
			std::ostringstream msg;
			cout << "could not open file: " << filePath << endl;
			return "";
		}

		std::string source;
		while ( !file.eof() )
		{
			std::string line;
			getline( file, line );
			source.append( line );
			source.append( "\n" );
		}
		file.close();
		return source;
	}
}