#version 330 core

uniform sampler2D texColor;
in vec2 fTexcoord;
flat in int skip;
in float circleId2;
in vec4 distanceVector;
out vec4 fragColor;


void main(void)
{
	 
	
	/*if(int(mod(circleId2, 3)) == 0)
		discard;*/
	if(skip == 1)
		discard;
	if(int(mod(gl_FragCoord.x,4.0)) ==0)
		discard;
		
	//if(gl_FragCoord.x > 255)
	//	discard;
	
	
	vec4 color = texture(texColor, fTexcoord);
	
		//color = vec4(1.0f,1.0f,1.0f,1.0f);
	fragColor = color;
	fragColor.x = fragColor.x + (distanceVector.x/4);   //modulate color with distance
	fragColor.y = fragColor.y + (distanceVector.y/4);
	fragColor.z = fragColor.z + (distanceVector.z/4);
	
	if(gl_FrontFacing)
	{
		
		float r = fragColor.x;
		float g = fragColor.y;
		float b = fragColor.z;
		
		fragColor.x = b;
		fragColor.y = r;
		fragColor.z = g;
	}
	ivec2 circleVec = ivec2(int(circleId2), int(circleId2));
	vec4 alphaVec = texelFetch(texColor, circleVec, 0);
	
	fragColor.a = alphaVec.a;
	/*fragColor.x = fragColor.x + (distanceVector.x /8);
	fragColor.y = fragColor.y + (distanceVector.y /8);
	fragColor.z= fragColor.z + (distanceVector.z /8);*/
	
}
