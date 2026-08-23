#version 460 core

/*===============================//
//   fragment shader inputs      //
//===============================*/
in VS_OUT
{
	vec2 texCoord;
	vec4 color;
} fsIn;

/*===============================//
//   sprite texture              //
//===============================*/
layout(binding = 0) uniform sampler2D uAtlasTexture;

/*===============================//
//   fragment shader output      //
//===============================*/
layout(location = 0) out vec4 outColor;

void main()
{
	vec4 textureColor = texture(uAtlasTexture, fsIn.texCoord);
	vec4 finalColor = textureColor * fsIn.color;

	if (finalColor.a <= 0.001)
		discard;

	outColor = finalColor;
}