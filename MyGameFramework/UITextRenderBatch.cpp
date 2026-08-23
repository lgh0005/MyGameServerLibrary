#include "GameFrameworkPch.h"
#include "UITextRenderBatch.h"
#include "QuadMesh.h"
#include "Font.h"
#include "GLSampler.h"
#include "GLVertexArray.h"
#include "GLBuffer.h"

namespace MGSL::Framework
{
	UITextRenderBatch::UITextRenderBatch(const QuadMeshPtr& quadMesh, const FontPtr& font, Shared::uint32 maxInstanceCount)
		: Super(maxInstanceCount), m_quadMesh(quadMesh), m_font(font) { }
	UITextRenderBatch::~UITextRenderBatch() = default;

	UITextRenderBatchUPtr UITextRenderBatch::Create
	(
		const QuadMeshPtr& quadMesh,
		const FontPtr& font,
		Shared::uint32 maxInstanceCount
	)
	{
		if (!quadMesh->IsValid() || font->GetAtlasTexture() == 0 || maxInstanceCount == 0) 
			return nullptr;

		auto renderBatch = UITextRenderBatchUPtr(new UITextRenderBatch(quadMesh, font, maxInstanceCount));
		if (!renderBatch->SetInstanceBuffer()) return nullptr;

		return renderBatch;
	}

	void UITextRenderBatch::Clear()
	{
		m_instances.clear();
		ResetItemCount();
	}

	void UITextRenderBatch::Upload()
	{
		if (IsEmpty()) return;
		m_instanceBuffer->SetData
		(
			0,
			sizeof(UIInstanceData) * m_instances.size(),
			m_instances.data()
		);
	}

	void UITextRenderBatch::Render()
	{
		if (IsEmpty() || !m_instanceVertexArray || !m_quadMesh || !m_font)
			return;

		const GLuint atlasTexture = m_font->GetAtlasTexture();
		if (atlasTexture == 0) return;

		// 폰트 텍스처 바인딩
		::glBindTextureUnit(0, atlasTexture);
		const GLSampler* sampler = m_font->GetSampler();
		if (sampler) sampler->Bind(0);
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
		if (sampler)GLSampler::Unbind(0);
		::glBindTextureUnit(0, 0);
	}

	bool UITextRenderBatch::AddInstance(const UIInstanceData& instanceData)
	{
		if (IsFull()) return false;

		m_instances.push_back(instanceData);
		IncreaseItemCount();

		return true;
	}

	bool UITextRenderBatch::AddInstances(const Shared::List<UIInstanceData>& instances)
	{
		for (const auto& instance : instances) { if (!AddInstance(instance)) return false; }
		return true;
	}

	const FontPtr& UITextRenderBatch::GetFont() const
	{
		return m_font;
	}

	bool UITextRenderBatch::SetInstanceBuffer()
	{
		m_instances.reserve(GetMaxItemCount());

		// UI Text 인스턴스 전용 VAO 생성
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

	bool UITextRenderBatch::SetInstanceVertexArrayAttrib()
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
		//   UIText instance attributes       //
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
