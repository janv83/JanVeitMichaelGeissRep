#version 330 core

in float pos;

uniform vec2 P0;
uniform vec2 P1;
uniform vec2 P2;
uniform vec2 P3;

vec2 evalBezierCurve(in float t)
{
	vec2 bezierPos =	pow( ( 1.0 - t ), 3 ) * P0 +
				3*t * pow( ( 1 - t ), 2 ) * P1 +
				3*t * t * ( 1 - t ) * P2 +
				pow ( t, 3 ) * P3;
						
	return bezierPos;
}

void main()
{
	vec2 bezierPos = evalBezierCurve( pos );
	gl_Position = vec4( bezierPos, 0.0, 1.0 );
}