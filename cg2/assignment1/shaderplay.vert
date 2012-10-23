#version 330 core

uniform mat2 matRotation;
uniform mat4 matScene;
uniform mat4 matView;
uniform mat4 matProjection;
uniform float time;
in vec3 position;
in vec2 texcoord;
in float circleId;
out vec4 distanceVector;
out vec2 fTexcoord;  // output zu fragment shader (invar)
flat out int skip;
out float circleId2;

float angle()  //returns angle in degrees of the current vertex
{
	if(position.x >= 0 && position.y >= 0)
	{
		return degrees(atan(position.y, position.x));  //quadrant I
	}
	else if(position.x < 0)
	{
		return 180.0 + degrees(atan(position.y, position.x));  // quadrant II und III
	}
	else
	{
		return 360.0 + degrees(atan(position.y, position.x)); //quadrant IV
	}
	
	
}
float displacement()
{
	float amplitude = 0.2;
	float frequenzy_factor = 50;
	
	return  length(position) * length(position)* amplitude * cos(frequenzy_factor*angle()+time);
}

void main(void)
{
	float vectorlength = length(position);
	distanceVector = vec4(vectorlength, vectorlength, vectorlength, vectorlength);
	
	
	fTexcoord = texcoord;
	
	fTexcoord.x = fTexcoord.x-0.5;  // rotate to origin , rotate in texcoords (0,0 to 1,1)
	fTexcoord.y = fTexcoord.y-0.5;
	
	fTexcoord = matRotation * fTexcoord;  //rotate 
	
	fTexcoord.x = fTexcoord.x+0.5;  //rotate back
	fTexcoord.y = fTexcoord.y+0.5;
	
	circleId2 = circleId;
	
	if(int(mod(circleId, 3)) == 0)   
		skip = 1;
	else
		skip = 0;
		
	vec4 posTmp = vec4(position.xyz, 1.0);
	
	posTmp.z = displacement();
	
	posTmp = matScene * posTmp;   // Scenerotation around Y
	posTmp = matView * posTmp;    //set the new Viewpoint
	posTmp = matProjection * posTmp;  //new Projection
	
	
	
	gl_Position = posTmp;
	
	
	
	
	
}
 
