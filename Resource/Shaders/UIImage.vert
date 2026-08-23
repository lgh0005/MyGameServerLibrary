#version 460 core

/*===============================//
//   default vertex attributes   //
//===============================*/
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

/*===============================//
//   UI instance attributes      //
//===============================*/
layout(location = 2) in vec4 aWorldMatrixColumn0;
layout(location = 3) in vec4 aWorldMatrixColumn1;
layout(location = 4) in vec4 aWorldMatrixColumn2;
layout(location = 5) in vec4 aWorldMatrixColumn3;
layout(location = 6) in vec4 aUVRect;
layout(location = 7) in vec4 aColor;
layout(location = 8) in vec2 aSize;

/*===============================//
//   camera uniform buffer       //
//===============================*/
layout(std140, binding = 1) uniform UICameraBuffer
{
	mat4 uView;
	mat4 uProjection;
	mat4 uViewProjection;
	vec4 uWorldPosition;
};

/*===============================//
//   vertex shader outputs       //
//===============================*/
out VS_OUT
{
	vec2 texCoord;
	vec4 color;
} vsOut;

void main()
{
	mat4 worldMatrix = mat4
	(
		aWorldMatrixColumn0,
		aWorldMatrixColumn1,
		aWorldMatrixColumn2,
		aWorldMatrixColumn3
	);

	vec2 halfSize = aSize * 0.5;

	vec3 localPosition = vec3
	(
		aPosition.x * halfSize.x,
		aPosition.y * halfSize.y,
		aPosition.z
	);

	vec4 worldPosition = worldMatrix * vec4(localPosition, 1.0);
	gl_Position = uProjection * worldPosition;

	vsOut.texCoord = aUVRect.xy + aTexCoord * aUVRect.zw;
	vsOut.color = aColor;
}