#include "GameFrameworkPch.h"
#include "UIImageRenderBatch.h"
#include "QuadMesh.h"
#include "Texture2D.h"
#include "GLVertexArray.h"
#include "GLBuffer.h"

namespace MGSL::Framework
{
	UIImageRenderBatch::UIImageRenderBatch(const QuadMeshPtr& quadMesh, const Texture2DPtr& texture, Shared::uint32 maxInstanceCount)
		: Super(maxInstanceCount), m_quadMesh(quadMesh), m_texture(texture) { }
	UIImageRenderBatch::~UIImageRenderBatch() = default;

	UIImageRenderBatchUPtr UIImageRenderBatch::Create
	(
		const QuadMeshPtr& quadMesh, 
		const Texture2DPtr& texture, 
		Shared::uint32 maxInstanceCount
	)
	{
		if (!quadMesh->IsValid() || !texture->IsValid() || maxInstanceCount == 0)
			return nullptr;

		auto renderBatch = UIImageRenderBatchUPtr(new UIImageRenderBatch(quadMesh, texture, maxInstanceCount));
		if (!renderBatch->SetInstanceBuffer()) return nullptr;

		return renderBatch;
	}

	/*====================================//
	//   default RenderBatch interfaces   //
	//====================================*/
	void UIImageRenderBatch::Clear()
	{
		m_instances.clear();
		ResetItemCount();
	}

	void UIImageRenderBatch::Upload()
	{
		if (IsEmpty()) return;
		m_instanceBuffer->SetData
		(
			0, 
			sizeof(UIInstanceData) * m_instances.size(), 
			m_instances.data()
		);
	}

	void UIImageRenderBatch::Render()
	{
		if (IsEmpty() || !m_instanceVertexArray || !m_quadMesh || !m_texture)
			return;

		// 이미지 텍스처 바인딩
		m_texture->Bind(0);
		m_instanceVertexArray->Bind();

		// 인스턴싱 드로우 콜
		::glDrawElementsInstanced
		(
			m_quadMesh->GetPrimitiveType(),
			static_cast<GLsizei>(m_quadMesh->GetIndexCount()),
			m_quadMesh->GetIndexType(),
			nullptr,
			static_cast<GLsizei>(GetItemCount())
		);

		// 언바인딩
		GLVertexArray::Unbind();
	}

	bool UIImageRenderBatch::AddInstance(const UIInstanceData& instanceData)
	{
		if (IsFull()) return false;

		m_instances.push_back(instanceData);
		IncreaseItemCount();

		return true;
	}

	const Texture2DPtr& UIImageRenderBatch::GetTexture() const
	{
		return m_texture;
	}

	bool UIImageRenderBatch::SetInstanceBuffer()
	{
		m_instances.reserve(GetMaxItemCount());

		// UI Image 인스턴스 전용 VAO 생성
		m_instanceVertexArray = GLVertexArray::Create();
		if (!m_instanceVertexArray)
			return false;

		// UI 인스턴스 버퍼 생성
		m_instanceBuffer = GLBuffer::Create
		(
			sizeof(UIInstanceData) * GetMaxItemCount(),
			nullptr,
			GL_DYNAMIC_STORAGE_BIT
		);

		if (!m_instanceBuffer) return false;

		return SetInstanceVertexArrayAttrib();
	}

	bool UIImageRenderBatch::SetInstanceVertexArrayAttrib()
	{
		if (!m_instanceVertexArray ||
			!m_instanceBuffer ||
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
		const Shared::uint32 instanceStride = sizeof(UIInstanceData);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 2, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(UIInstanceData, worldMatrix) + sizeof(Shared::vec4) * 0);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 3, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(UIInstanceData, worldMatrix) + sizeof(Shared::vec4) * 1);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 4, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(UIInstanceData, worldMatrix) + sizeof(Shared::vec4) * 2);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 5, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(UIInstanceData, worldMatrix) + sizeof(Shared::vec4) * 3);
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 6, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(UIInstanceData, uvRect));
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 7, instanceBindingIndex, 4, GL_FLOAT, false, instanceStride, offsetof(UIInstanceData, color));
		m_instanceVertexArray->SetAttrib(*m_instanceBuffer, 8, instanceBindingIndex, 2, GL_FLOAT, false, instanceStride, offsetof(UIInstanceData, size));
		m_instanceVertexArray->SetAttribDivisor(instanceBindingIndex, 1);

		return true;
	}
}

