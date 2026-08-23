#include "GameFrameworkPch.h"
#include "QuadMesh.h"
#include "GLVertexArray.h"
#include "Vertex.h"

namespace MGSL::Framework
{
	QuadMesh::QuadMesh() = default;
	QuadMesh::~QuadMesh() = default;

	QuadMeshPtr QuadMesh::Create()
	{
		// 사각형 메시 생성에 필요한 정점 데이터 정의
		const Shared::List<QuadMeshVertex> vertices
		{
			// position					// uv			   // padding
			{ Shared::vec3(-1.0f, -1.0f, 0.0f), Shared::vec2(0.0f,  1.0f), Shared::vec3(0.0f) },
			{ Shared::vec3(1.0f, -1.0f, 0.0f),  Shared::vec2(1.0f,  1.0f), Shared::vec3(0.0f) },
			{ Shared::vec3(1.0f,  1.0f, 0.0f),  Shared::vec2(1.0f,  0.0f), Shared::vec3(0.0f) },
			{ Shared::vec3(-1.0f,  1.0f, 0.0f), Shared::vec2(0.0f,  0.0f), Shared::vec3(0.0f) }
		};

		// Screen 메시 정점에 대한 인덱스
		const Shared::List<Shared::uint32> indices { 0, 1, 2, 2, 3, 0 };

		// QuadMesh 인스턴스 생성
		QuadMeshPtr quadMesh(new QuadMesh());

		// 1. OpenGL 객체 생성
		if (!quadMesh->CreateOpenGLObjects<QuadMeshVertex>(vertices, indices))
			return nullptr;

		// 2. 정점 속성 설정 (QuadMeshVertex)
		quadMesh->SetVertexAttributes();

		// 3. Mesh의 멤버 변수 채우기
		quadMesh->SetMeshMetadata(vertices.size(), indices.size());

		// 최종 메시 반환
		if (!quadMesh->IsValid()) return nullptr;
		return quadMesh;
	}

	void QuadMesh::SetVertexAttributes()
	{
		m_vertexArray->SetIndexBuffer(*m_indexBuffer);
		m_vertexArray->SetAttrib(*m_vertexBuffer, 0, 0, 3, GL_FLOAT, false, sizeof(QuadMeshVertex), offsetof(QuadMeshVertex, position));
		m_vertexArray->SetAttrib(*m_vertexBuffer, 1, 0, 2, GL_FLOAT, false, sizeof(QuadMeshVertex), offsetof(QuadMeshVertex, texCoord));
	}
}

