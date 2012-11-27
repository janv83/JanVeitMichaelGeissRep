#pragma once

#include "glutil.h"

#include <string>
#include <map>
#include <list>

namespace cg2
{
	class Program
	{

	public:

		/**
		*	create your program here:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glCreateProgram.xml
		*	as a consequence, you can only create objects of the class after an opengl context exists!
		**/
		Program();

		/**
		*	delete your program here:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glDeleteProgram.xml
		*	also delete all your attached shaders:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glDeleteShader.xml
		**/
		~Program();

		/**
		*	create a shader with the desired type:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glCreateShader.xml
		*	compile the source code:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glShaderSource.xml
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glCompileShader.xml
		*	and attach it to the program:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glAttachShader.xml
		*	also get the shader's info log and store it in the variable mLogString:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetShader.xml + GL_INFO_LOG_LENGTH, GL_COMPILE_STATUS
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetShaderInfoLog.xml
		*
		*	store your shader handles, so that you can delete them when the program is deleted!
		*
		*	@return: whether or not the shader compiled
		**/
		bool addShader( const GLenum shaderType, std::string const& shaderSource );

		/**
		*	link the program object:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glLinkProgram.xml
		*	get the program's info log and store it in the variable mLogString:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetProgram.xml + GL_INFO_LOG_LENGTH, GL_LINK_STATUS
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetProgramInfoLog.xml
		*	after linking, query all active uniforms and attributes and cache them:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetProgram.xml	+ GL_MAX_ACTIVE_UNIFORMS, GL_MAX_ACTIVE_UNIFORM_LENGTH
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetActiveUniform.xml
		*	do the same for active attributes
		*
		*	@return: wheter or not the program was linked
		**/
		bool link();

		/**
		*	activate / deactive the program:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glUseProgram.xml
		*	also store in the variable mIsActive if the program is currently used or not
		*/
		void setActive( const bool active );

		/**
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetFragDataLocation.xml
		*
		*	@return: the location where the fragment output is bound
		**/
		GLint getFragDataLocation( std::string const& name );

		/**
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetAttribLocation.xml
		*
		*	@return: the location where the vertex attribute is bound
		**/
		GLint getAttributeLocation( std::string const& name );

		/**
		*	lots of uniform functions - add functionality as you need it
		*
		*	use the lookup table to find out if the uniform exists, and if it has the correct type
		*	then, use the cached location to set the value
		*
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glUniform.xml
		**/
		void setUniformValue( std::string const& name, const float value );
		void setUniformVec2( std::string const& name, const float x, const float y );
		void setUniformMat4( std::string const& name, float const* const mat, const bool transpose );

		/**
		*	the log string should always contain the results of the last operation
		*/
		std::string const& getProgramLog() const { return mLogString; }

		/**
		*	check if the program may be executed with the current gl state:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glValidateProgram.xml
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetProgram.xml + GL_VALIDATE_STATUS
		*	update the variable mLogString with the program info log
		*
		*	@return: whether or not the program can be executed
		**/
		bool isValid();

		/**
		*	print all active uniforms to std::out, with name, type, size and location
		*/
		void printActiveUniforms();

		/**
		*	print all active attributes to std::out, with name, type, size and location
		*/
		void printActiveAttributes();

	private:

		/**
		*	the copy ctor & assignment operator are inaccessible
		*	this means that if you want to create a container of programs, you need to use pointers
		*	i.e.:
		*	std::vector< cg2::Program * > instead of std::vector< cg2::Program >
		**/
		Program( Program const& src ) : mHandle( 0 ), mLogString( "invalid: copied program" ) {}
		Program& operator=( Program const& src ) { return *this; }

		GLuint					mHandle;			// program handle as created by opengl
		std::string				mLogString;			// current log string
		std::list< GLuint >		mShaders;			// shader handles - needed for deletion

		/**
		*	cached info - program usage & uniforms
		**/
		struct VariableInfo
		{
			GLint	location;
			GLenum	type;
			GLint	size;
		};
		typedef std::map< std::string, VariableInfo >	LookupTable;

		bool					mIsActive;			// stores whether or not program is active
		LookupTable				mUniforms;			// lookup table for active uniforms
		LookupTable				mAttributes;		// lookup table for active attributes
	};
}