#version 460 core

/*===============================//
//   fragment shader inputs      //
//===============================*/
in VS_OUT
{
	vec2 texCoord;
} fsIn;

const float PI = 3.141592;

/*===============================//
//   scene color texture         //
//===============================*/
layout(binding = 0) uniform sampler2D uSceneColorTexture;

/*===============================//
//   post processing uniforms    //
//===============================*/
layout(std140, binding = 2) uniform PostProcessingData
{
	float uTime;
	float uVignetteIntensity;
	float uChromaticAberrationStrength;
	float uPad0;
};

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

/*===============================//
//        color methods          //
//===============================*/
vec3 LinearToSRGB(vec3 color)
{
	vec3 lower = color * 12.92;
	vec3 upper = 1.055 * pow(max(color, vec3(0.0)), vec3(1.0 / 2.4)) - 0.055;
	return mix(lower, upper, step(vec3(0.0031308), color));
}

/*===============================//
//     chromatic aberration      //
//===============================*/
vec3 ApplyChromaticAberration(vec2 uv)
{
	vec2 centerOffset = uv - vec2(0.5);

	float distanceFromCenter = length(centerOffset);
	vec2 offset = centerOffset * distanceFromCenter * uChromaticAberrationStrength;

	float r = texture(uSceneColorTexture, uv + offset).r;
	float g = texture(uSceneColorTexture, uv).g;
	float b = texture(uSceneColorTexture, uv - offset).b;

	return vec3(r, g, b);
}

/*===============================//
//          CRT effect           //
//===============================*/
vec3 ApplyCRT(vec3 color, vec2 uv)
{
	float scanline = sin(uv.y * 720.0 * PI) * 0.5 + 0.5;
	color *= mix(0.97, 1.0, scanline);

	float rollingPosition = fract(uTime * 0.25);
	float rollingLine = 1.0 - smoothstep(0.0, 0.08, abs(uv.y - rollingPosition));
	color += rollingLine * 0.08;

	return color;
}

/*===============================//
//        hit vignette           //
//===============================*/
vec3 ApplyHitVignette(vec3 color, vec2 uv)
{
	vec2 centered = uv - vec2(0.5);

	float distanceFromCenter = length(centered);
	float vignette = smoothstep(0.30, 0.72, distanceFromCenter);

	vec3 hitColor = vec3(0.65, 0.02, 0.02);
	color = mix(color, hitColor, vignette * uVignetteIntensity * 0.7);

	return color;
}

void main()
{
	vec2 uv = fsIn.texCoord;

	// Chromatic Aberration
	vec3 color = ApplyChromaticAberration(uv);

	// CRT
	color = ApplyCRT(color, uv);

	// Hit Vignette
	color = ApplyHitVignette(color, uv);

	// Linear -> sRGB
	color = LinearToSRGB(color);

	outColor = vec4(color, 1.0);
}