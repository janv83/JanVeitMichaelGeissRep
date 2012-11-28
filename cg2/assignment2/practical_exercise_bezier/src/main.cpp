#include "GLSL.h"

#include <SFML\OpenGL.hpp>
#include <SFML\Window.hpp>

#include "glutil.h"		// glew is included in there
#include "bezier.h"
#include "glm.hpp"
#include "assimp/importer.hpp"
#include "assimp/types.h"
#include "assimp\mesh.h"
#include "assimp/postprocess.h"


#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cg2;

const aiScene* loadModel(std::string modelPath)
{
	modelPath = "../data/bunny.ply";
	std::string filepath = modelPath;

	Assimp::Importer imp;
	//this tells assimp to remove point & line primitives, so that
	//triangles remain as the only primitive type
	imp.SetPropertyInteger( AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE );
	//this tells assimp to scale the model so that all vertices lie in
	//the [-1, 1] range
	imp.SetPropertyInteger( AI_CONFIG_PP_PTV_NORMALIZE , 1 );
	//for an overview of all postprocessing flags, see
	//http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	const aiScene* scene = imp.ReadFile( filepath, aiProcess_PreTransformVertices | aiProcess_JoinIdenticalVertices |
	aiProcess_FindDegenerates | aiProcess_SortByPType |
	aiProcess_Triangulate | aiProcess_RemoveComponent |
	aiProcess_ValidateDataStructure );

	return scene;
}

int main( int argc, char** argv )
{
	/**
	*	window & opengl context creation
	*	http://www.sfml-dev.org/documentation/2.0/classsf_1_1Window.php
	**/
	sf::Window window(	sf::VideoMode( 800, 800, 32 ),					// window size & number of bits per pixel
						"gaudiamus igitur",
						sf::Style::Default,
						sf::ContextSettings( 16, 0, 8, 3, 3 )	);		// desired opengl context settings:
																		// depth bits, stencil bits, samples per pixel ( for antialiasing )
																		// opengl major version, minor version
																		// the actual settings may or may not match the desired settings

	/**
	*	now an opengl context exists -> initialize glew
	*	if glewInit fails ( unlikely ), there's basically nothing that can be done other than quit the application
	**/
	GLenum status = glewInit();
	if ( status != GLEW_OK )
	{
		cout << "GLEW initialization failure: " << glewGetErrorString( status ) << endl;
		return -1;
	}

	/**
	*	initializations go here -> create shader / program, load textures, create vertex attributes etc.
	**/
	loadModel("../data/bunny.ply");

	/* program creation */
	Program program;
	if ( !program.addShader( GL_VERTEX_SHADER, loadShaderSource( "..\\bezier.vert" ) ) )
		cout << "shader compilation failed " << program.getProgramLog() << endl;
	if ( !program.addShader( GL_FRAGMENT_SHADER, loadShaderSource( "..\\bezier.frag" ) ) )
		cout << "shader compilation failed " << program.getProgramLog() << endl;
	if ( !program.link() )
		cout << "program linking failed " << program.getProgramLog() << endl;

	/* there's only one program, so it can be activated here and stay active until the end */
	program.setActive( true );

	/* get the location of the single vertex attrib */
	const GLuint attributeLocation = program.getAttributeLocation( "pos" );

	/* defines the vertex attrib & makes sure it goes to the correct location */
	float distance = 0.f;
	vector< float > distances( 101 );
	for ( vector< float >::iterator it = distances.begin(); it != distances.end(); ++it ) { *it = distance; distance += 0.01f; }
	GLuint distanceBuffer;
	glGenBuffers( 1, &distanceBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, distanceBuffer );
	glBufferData( GL_ARRAY_BUFFER, distances.size() * sizeof( float ), &distances[ 0 ], GL_STATIC_DRAW );
	glVertexAttribPointer( attributeLocation, 1, GL_FLOAT, false, 0, 0 );
	glEnableVertexAttribArray( attributeLocation );

	/* animation related stuff */
	BezierAnimation bezier( 3000, 20 );
	sf::Clock timer;

	/**
	*	main loop - is carried out until the window is closed
	**/
	while( 1 )
	{

		/**
		*	window events are processed
		**/
		sf::Event ev;
		bool exit = false;
		while ( window.pollEvent( ev ) )
		{
			if ( ev.type == sf::Event::Closed )
			{
				exit = true;
				break;
			}
			else if ( ev.type == sf::Event::Resized )
			{
				glViewport( 0, 0, ev.size.width, ev.size.height );
			}
			/**
			*	other possible events:
			*	KeyPressed, MouseMoved, MouseWheelMoved, MouseButtonPressed etc.
			*	http://www.sfml-dev.org/documentation/2.0/classsf_1_1Event.php
			**/
		}
		if ( exit ) break;

		/**
		*	opengl related commands go here
		**/

		glClear( GL_COLOR_BUFFER_BIT );

		/* set some uniforms */
		program.setUniformVec2( "P0", bezier.ctrl0().x, bezier.ctrl0().y );
		program.setUniformVec2( "P1", bezier.ctrl1().x, bezier.ctrl1().y );
		program.setUniformVec2( "P2", bezier.ctrl2().x, bezier.ctrl2().y );
		program.setUniformVec2( "P3", bezier.ctrl3().x, bezier.ctrl3().y );

		/* the program should be valid, since we don't really do complicated stuff */
		if ( !program.isValid() )
			cout << "program can not be executed: " << program.getProgramLog() << endl;

		/* draw stuff */
		glDrawArrays( GL_LINE_STRIP, 0, 101 );

		/**
		*	after everything opengl related has been done, you can do other stuff while the GPU is busy
		*/
		bezier.update( timer.getElapsedTime().asMilliseconds() );

		/**
		*	display() basically is a call to SwapBuffers ( windows api )
		*	which first makes sure that all opengl commands are executed, and then switches the front and back buffers
		*/
		window.display();
	}

	glDeleteBuffers( 1, &distanceBuffer );

	return 0;
}