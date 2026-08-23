#version 460 core

/*===============================//
//   default vertex attributes   //
//===============================*/
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

/*===============================//
//   sprite instance attributes  //
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
layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 uView;
	mat4 uProjection;
	mat4 uViewProjection;
	vec4 uCameraWorldPosition;
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
	
	vec3 localPosition = vec3
	(
		aPosition.x * aSize.x,
		aPosition.y * aSize.y,
		aPosition.z
	);
	
	vec4 worldPosition = worldMatrix * vec4(localPosition, 1.0);
	gl_Position = uViewProjection * worldPosition;
	
	vsOut.texCoord = aUVRect.xy + aTexCoord * aUVRect.zw;
	vsOut.color = aColor;
}