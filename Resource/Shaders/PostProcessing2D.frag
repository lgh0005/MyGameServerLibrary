#version 460 core

/*===============================//
//   fragment shader inputs      //
//===============================*/
in VS_OUT
{
	vec2 texCoord;
} fsIn;

/*===============================//
//   scene color texture         //
//===============================*/
layout(binding = 0) uniform sampler2D uSceneColorTexture;

/*===============================//
//   fragment shader output      //
//===============================*/
layout(location = 0) out vec4 outColor;

/*===============================//
//   tone-mapping methods        //
//===============================*/
vec3 ReinhardToneMapping(vec3 color)
{
	return color / (color + vec3(1.0));
}

vec3 LinearToSRGB(vec3 color)
{
	vec3 lower = color * 12.92;
	vec3 upper = 1.055 * pow(max(color, vec3(0.0)), vec3(1.0 / 2.4)) - 0.055;
	return mix(lower, upper, step(vec3(0.0031308), color));
}

void main()
{
	vec4 sceneColor = texture(uSceneColorTexture, fsIn.texCoord);
	vec3 color = sceneColor.rgb;

	// Linear to sRGB
	color = LinearToSRGB(color);

	outColor = vec4(color, sceneColor.a);
}