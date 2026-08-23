#include "GameFrameworkPch.h"
#include "SpriteRenderBatch.h"
#include "QuadMesh.h"
#include "Texture2D.h"
#include "GLVertexArray.h"
#include "GLBuffer.h"

namespace MGSL::Framework
{
	SpriteRenderBatch::SpriteRenderBatch
	(
		const QuadMeshPtr& quadMesh,
		const Texture2DPtr& atlasTexture,
		Shared::uint32 maxInstanceCount)
		: Super(maxInstanceCount),
		  m_quadMesh(quadMesh),
		  m_atlasTexture(atlasTexture) { }
	SpriteRenderBatch::~SpriteRenderBatch() = default;

	SpriteRenderBatchUPtr SpriteRenderBatch::Create
	(
		const QuadMeshPtr& quadMesh,
		const Texture2DPtr& atlasTexture,
		Shared::uint32 maxInstanceCount)
	{
		if (!quadMesh->IsValid() || !atlasTexture->IsValid() || maxInstanceCount == 0)
			return nullptr;

		SpriteRenderBatchUPtr renderBatch(new SpriteRenderBatch(quadMesh, atlasTexture, maxInstanceCount));
		if (!renderBatch->SetInstanceBuffer()) return nullptr;

		return renderBatch;
	}

	void SpriteRenderBatch::Clear()
	{
		m_instances.clear();
		ResetItemCount();
	}

	void SpriteRenderBatch::Upload()
	{
		if (IsEmpty()) return;
		m_instanceBuffer->SetData
		(
			0,
			sizeof(SpriteInstanceData) * m_instances.size(),
			m_instances.data()
		);
	}

	void SpriteRenderBatch::Render()
	{
		if (IsEmpty() || !m_instanceVertexArray || !m_quadMesh || !m_atlasTexture)
			return;

		m_atlasTexture->Bind(0);
		m_instanceVertexArray->Bind();

		::glDrawElementsInstanced
		(
			m_quadMesh->GetPrimitiveType(),
			static_cast<GLsizei>(m_quadMesh->GetIndexCount()),
			m_quadMesh->GetIndexType(),
			nullptr,
			static_cast<GLsizei>(GetItemCount())
		);

		GLVertexArray::Unbind();
	}

	bool SpriteRenderBatch::AddInstance(const SpriteInstanceData& instanceData)
	{
		if (IsFull()) return false;

		m_instances.push_back(instanceData);
		IncreaseItemCount();

		return true;
	}

	const Texture2DPtr& SpriteRenderBatch::GetAtlasTexture() const
	{
		return m_atlasTexture;
	}

	bool SpriteRenderBatch::SetInstanceBuffer()
	{
		m_instances.reserve(GetMaxItemCount());

		// 인스턴스 전용 VAO 생성
		m_instanceVertexArray = GLVertexArray::Create();
		if (!m_instanceVertexArray) return false;

		// Sprite 인스턴스 버퍼 생성
		m_instanceBuffer = GLBuffer::Create
		(
			sizeof(SpriteInstanceData) * GetMaxItemCount(),
			nullptr,
			GL_DYNAMIC_STORAGE_BIT
		);
		if (!m_instanceBuffer) return false;

		// 인스턴스 속성 설정
		return SetInstanceVertexArrayAttrib();
	}

	bool SpriteRenderBatch::SetInstanceVertexArrayAttrib()
	{
		if (!m_instanceVertexArray ||
			!m_instanceBuffer	   ||
			!m_quadMesh) return false;

		GLBuffer* vertexBuffer = m_quadMesh->GetVertexBuffer();
		GLBuffer* indexBuffer = m_quadMesh->GetIndexBuffer();
		if (!vertexBuffer || !indexBuffer) return false;

		/*====================================//
		//   quad mesh vertex attributes      //
		//====================================*/
		constexpr Shared::uint32 vertexBindingIndex = 0;
		const Shared::uint32 vertexStride = sizeof(QuadMeshVertex);
		m_instanceVertexArray->SetIndexBuffer(*indexBuffer);
		m_instanceVertexArray->SetAttrib(*vertexBuffer, 0, vertexBindingIndex, 3, GL_FLOAT, false, vertexStride, offsetof(QuadMeshVertex, position));
		m_instanceVertexArray->SetAttrib(*vertexBuffer, 1, vertexBindingIndex, 2, GL_FLOAT, false, vertexStride, offsetof(QuadMeshVertex, texCoord));

		/*====================================//
		//   sprite instance attributes       //
		//====================================*/
		constexpr Shared::uint32 instanceBindingIndex = 1;
		const Shared::uint32 instanceStride = sizeof(SpriteInstanceData);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 2, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(SpriteInstanceData, worldMatrix) + sizeof(Shared::vec4) * 0);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 3, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(SpriteInstanceData, worldMatrix) + sizeof(Shared::vec4) * 1);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 4, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(SpriteInstanceData, worldMatrix) + sizeof(Shared::vec4) * 2);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 5, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(SpriteInstanceData, worldMatrix) + sizeof(Shared::vec4) * 3);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 6, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(SpriteInstanceData, uvRect));
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 7, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(SpriteInstanceData, color));
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 8, instanceBindingIndex, 2, GL_FLOAT, false, instanceStride, offsetof(SpriteInstanceData, size));
		m_instanceVertexArray->SetAttribDivisor(instanceBindingIndex, 1);

		return true;
	}
}


