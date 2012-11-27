#include "glsl.h"
#include <iostream>


namespace cg2
{

	Program::Program()   //Konstruktor , create program here
	{
		/**
		*	create your program here:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glCreateProgram.xml
		*	as a consequence, you can only create objects of the class after an opengl context exists!
		**/
		mHandle = glCreateProgram();

		
		

	}
	

	Program::~Program()  //destructor, delete program and shaders
	{
		/**
		*	delete your program here:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glDeleteProgram.xml
		*	also delete all your attached shaders:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glDeleteShader.xml
		**/
		glDeleteProgram(mHandle);
		for(std::list<GLuint>::iterator s = mShaders.begin(); s != mShaders.end(); ++s)
		{
			glDeleteShader(*s);
		}
		/*for(std::map<std::string, VariableInfo>::iterator u = mUniforms.begin(); u != mUniforms.end(); ++u)
		{
			delete &u->first;
		}*/
	}

	bool Program::addShader( const GLenum shaderType, std::string const& shaderSource )
	{
		GLuint shaderReference = glCreateShader(shaderType);

		if(shaderReference != 0)
		{
			mShaders.push_back(glCreateShader(shaderType));
			
		}
		else 
		{
			return false;
		}

		//add source to shader

		GLint length = shaderSource.length();

		//GLint numOfStrings = 1;


		const char* sourceCode =  shaderSource.c_str();
		//glShaderSource(shaderReference, 1, (const GLchar**)&shaderSource, NULL);
		glShaderSource(shaderReference,1,(const GLchar**)&sourceCode, &length); 
		
		
		glCompileShader(shaderReference);

		GLint isCompiled;

		glGetShaderiv(shaderReference, GL_COMPILE_STATUS, &isCompiled);

		if(isCompiled != GL_TRUE)
		{
			return false;
		}

		glAttachShader(mHandle, shaderReference);

		GLint infoLogLength;
		
		glGetShaderiv(shaderReference, GL_INFO_LOG_LENGTH, &infoLogLength);

		glGetShaderInfoLog(shaderReference, mLogString.length(), &infoLogLength, (GLchar*)mLogString.c_str());
		
		return true;
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
	}

	bool Program::link()
	{
		glLinkProgram(mHandle);
		GLsizei infoLogLength;
		GLsizei linkStatus;
		glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
		glGetProgramiv(mHandle, GL_LINK_STATUS, &linkStatus);
		if(!linkStatus)
		{
			return false;
		}
		glGetProgramInfoLog(mHandle, mLogString.length(), &infoLogLength,(GLchar*) mLogString.c_str());

		GLsizei activeUniforms;
		GLsizei maxActiveUniformLength;

		glGetProgramiv(mHandle,GL_ACTIVE_UNIFORMS, &activeUniforms);
		glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxActiveUniformLength);

		for(int i = 0; i < activeUniforms; i++)
		{
			GLchar* name = new GLchar[maxActiveUniformLength];
			GLsizei nameLength;
			GLint size;
			GLenum type;

			

			glGetActiveUniform(mHandle, i, maxActiveUniformLength, &nameLength, &size, &type, name);

			name[nameLength] = NULL; // terminate string

			GLint location = glGetUniformLocation(mHandle, (const GLchar*)name);
			
			struct VariableInfo var = {location, type, size};
			std::string nameString(name);
			mUniforms.insert(std::make_pair<std::string, VariableInfo>(nameString, var));

			delete name;


		}


		//now the same for attributes

		GLsizei activeAttributes;
		GLsizei maxActiveAttributeLength;

		glGetProgramiv(mHandle,GL_ACTIVE_ATTRIBUTES, &activeAttributes);
		glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxActiveAttributeLength);

		for(int i = 0; i < activeAttributes; i++)
		{
			GLchar* name = new GLchar[maxActiveAttributeLength];
			GLsizei nameLength;
			GLint size;
			GLenum type;

			glGetActiveAttrib(mHandle, i, maxActiveAttributeLength, &nameLength, &size, &type, name);

			name[nameLength] = NULL; // terminate string

			GLint location = glGetAttribLocation(mHandle, name);

			std::string nameString(name);  //now make string out of it
			struct VariableInfo var = {location, type, size};
			mAttributes.insert(std::make_pair<std::string, VariableInfo>(nameString, var));


			delete name;

		}

		printActiveUniforms();
		printActiveAttributes();

		return linkStatus;
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
	}

	void Program::setActive( const bool active )
	{
		
		/**
		*	activate / deactive the program:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glUseProgram.xml
		*	also store in the variable mIsActive if the program is currently used or not
		*/

		if(active)
		{
			glUseProgram(mHandle);
			mIsActive = true;
		}
		else
		{
			glUseProgram(0);
			mIsActive = false;
		}

	}

	GLint Program::getFragDataLocation( std::string const& name )
	{
		/**
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetFragDataLocation.xml
		*
		*	@return: the location where the fragment output is bound
		**/

		return glGetFragDataLocation(mHandle, (GLchar*)name.c_str());
	}
	GLint Program::getAttributeLocation( std::string const& name )
	{
		/**
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetAttribLocation.xml
		*
		*	@return: the location where the vertex attribute is bound
		**/
		return glGetAttribLocation(mHandle, (GLchar*) name.c_str());
	}
	/**
		*	lots of uniform functions - add functionality as you need it
		*
		*	use the lookup table to find out if the uniform exists, and if it has the correct type
		*	then, use the cached location to set the value
		*
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glUniform.xml
		**/
	void Program::setUniformValue( std::string const& name, const float value )
	{
		LookupTable::iterator u = mUniforms.find(name);
		if(u != mUniforms.end())
		{
			if(u->second.type == GL_FLOAT)
			{
				glUniform1f(u->second.location, value);
			}
		}
	}
	void Program::setUniformVec2( std::string const& name, const float x, const float y )
	{
		
	
		LookupTable::iterator u = mUniforms.find(name);
		if(u != mUniforms.end() &&u->second.type == GL_FLOAT_VEC2)
		{
			
				glUniform2f(u->second.location, x,y);
			
		}
		else
		{

		}

	}
	void Program::setUniformMat4( std::string const& name, float const* const mat, const bool transpose )
	{
		LookupTable::iterator u = mUniforms.find(name);
		if(u != mUniforms.end())
		{
			if(u->second.type == GL_FLOAT_MAT4)
			{
				glUniformMatrix4fv(u->second.location, 1, transpose, mat);
			}
		}
	}
	bool Program::isValid()
	{
		
		/**
		*	check if the program may be executed with the current gl state:
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glValidateProgram.xml
		*	http://www.opengl.org/sdk/docs/man3/xhtml/glGetProgram.xml + GL_VALIDATE_STATUS
		*	update the variable mLogString with the program info log
		*
		*	@return: whether or not the program can be executed
		**/

		glValidateProgram(mHandle);
			GLsizei infoLogLength;
			GLsizei status;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
			glGetProgramiv(mHandle, GL_VALIDATE_STATUS, &status);
			
			glGetProgramInfoLog(mHandle, mLogString.length(), &infoLogLength,(GLchar*) mLogString.c_str());

			return status;
		

	}
	void Program::printActiveUniforms()
	{
		/**
		*	print all active uniforms to std::out, with name, type, size and location
		*/
		

		for(LookupTable::iterator u = mUniforms.begin(); u != mUniforms.end(); ++u)
		{
			std::cout << "name: " << u->first << std::endl;
			std::cout << "type: " << u->second.type << std::endl;
			std::cout << "size: " << u->second.size << std::endl;
			std::cout << "location: " << u->second.location << std::endl;
			std::cout << std::endl;
		
		}
	}
	void Program::printActiveAttributes()
	{
		/**
		*	print all active attributes to std::out, with name, type, size and location
		*/
		for(LookupTable::iterator u = mAttributes.begin(); u != mAttributes.end(); ++u)
		{
			std::cout << "name: " << u->first << std::endl;
			std::cout << "type: " << u->second.type << std::endl;
			std::cout << "size: " << u->second.size << std::endl;
			std::cout << "location: " << u->second.location << std::endl;
			std::cout << std::endl;
		
		}
	}




/**
*	implement the program class here. yay.
**/

}