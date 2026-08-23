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
//   font atlas texture          //
//===============================*/
layout(binding = 0) uniform sampler2D uFontAtlas;

/*===============================//
//   fragment shader output      //
//===============================*/
layout(location = 0) out vec4 outColor;

void main()
{
	float coverage = texture(uFontAtlas, fsIn.texCoord).r;

	vec4 finalColor = vec4
	(
		fsIn.color.rgb,
		fsIn.color.a * coverage
	);

	if (finalColor.a <= 0.001)
		discard;

	outColor = finalColor;
}