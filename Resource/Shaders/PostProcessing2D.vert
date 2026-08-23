#version 460 core

/*===============================//
//   default vertex attributes   //
//===============================*/
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

/*===============================//
//   vertex shader outputs       //
//===============================*/
out VS_OUT
{
	vec2 texCoord;
} vsOut;

void main()
{
	gl_Position = vec4(aPosition, 1.0);
	vsOut.texCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}