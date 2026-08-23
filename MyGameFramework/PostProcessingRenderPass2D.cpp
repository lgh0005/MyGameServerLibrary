#include "GameFrameworkPch.h"
#include "PostProcessingRenderPass2D.h"
#include "GLFramebuffer.h"
#include "GLSampler.h"
#include "QuadMesh.h"
#include "Shader.h"

namespace MGSL::Framework
{
	PostProcessingRenderPass2D::PostProcessingRenderPass2D() = default;
	PostProcessingRenderPass2D::~PostProcessingRenderPass2D()
	{
		if (m_depthStencilTextureID != 0)
		{
			::glDeleteTextures(1, &m_depthStencilTextureID);
			m_depthStencilTextureID = 0;
		}

		if (m_colorTextureID != 0)
		{
			::glDeleteTextures(1, &m_colorTextureID);
			m_colorTextureID = 0;
		}
	}

	PostProcessingRenderPass2DUPtr PostProcessingRenderPass2D::Create()
	{
		return PostProcessingRenderPass2DUPtr(new PostProcessingRenderPass2D());
	}

	/*============================================//
	//   default RenderPass interface impls       //
	//============================================*/
	bool PostProcessingRenderPass2D::Init()
	{
		if (!m_postProcessingShader) return false;
		if (!m_postProcessingShader->IsValid()) return false;
		if (m_width <= 0 || m_height <= 0) return false;

		// 컬러 및 깊이-스텐실 텍스처 생성
		::glCreateTextures(GL_TEXTURE_2D, 1, &m_colorTextureID);
		if (m_colorTextureID == 0) return false;
		::glTextureStorage2D(m_colorTextureID, 1, GL_RGBA16F, m_width, m_height);

		::glCreateTextures(GL_TEXTURE_2D, 1, &m_depthStencilTextureID);
		if (m_depthStencilTextureID == 0) return false;
		::glTextureStorage2D(m_depthStencilTextureID, 1, GL_DEPTH24_STENCIL8, m_width, m_height);

		// 포스트 프로세싱을 위한 프레임버퍼 생성
		m_framebuffer = GLFramebuffer::Create();
		if (!m_framebuffer) return false;

		m_framebuffer->AttachColorTexture(m_colorTextureID, 0);
		 m_framebuffer->AttachDepthStencilTexture(m_depthStencilTextureID);
		m_framebuffer->SetDrawBuffer(0);
		m_framebuffer->SetReadBuffer(0);
		if (!m_framebuffer->IsAttachmentComplete()) return false;

		// 포스트 프로세싱 샘플러 생성
		m_postProcessingSampler = GLSampler::Create();
		if (!m_postProcessingSampler) return false;
		m_postProcessingSampler->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		m_postProcessingSampler->SetFilter(GL_LINEAR, GL_LINEAR);

		// 화면 전용 QuadMesh 생성
		m_screenQuad = QuadMesh::Create();
		if (!m_screenQuad) return false;

		return true;
	}

	void PostProcessingRenderPass2D::Render()
	{
		if (!m_postProcessingShader  || !m_screenQuad || 
			!m_postProcessingSampler || m_colorTextureID == 0) return;

		// 셰이더 바인딩
		::glDisable(GL_DEPTH_TEST);
		::glDisable(GL_BLEND);
		m_postProcessingShader->Bind();

		// 컬러 텍스처 바인드
		::glBindTextureUnit(0, m_colorTextureID);

		// 스크린 메시 Draw call
		m_postProcessingSampler->Bind(0);
		m_screenQuad->Bind();
		::glDrawElements(GL_TRIANGLES, (GLsizei)m_screenQuad->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

		// 상태 초기화
		GLSampler::Unbind(0);
		::glBindTextureUnit(0, 0);
		m_postProcessingShader->Unbind();

		::glEnable(GL_BLEND);
	}

	/*============================================//
	//   scene render-target control methods      //
	//============================================*/
	void PostProcessingRenderPass2D::BeginRenderTarget() const
	{
		if (!m_framebuffer) return;

		m_framebuffer->Bind();
		::glViewport(0, 0, m_width, m_height);
		::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void PostProcessingRenderPass2D::EndRenderTarget() const
	{
		GLFramebuffer::Unbind();
		::glViewport(0, 0, m_width, m_height);
	}

	/*============================================//
	//   PostProcessingRenderPass2D shader setters  //
	//============================================*/
	void PostProcessingRenderPass2D::SetPostProcessingShader(ShaderPtr shader)
	{
		m_postProcessingShader = std::move(shader);
	}

	void PostProcessingRenderPass2D::SetRenderSize(Shared::int32 width, Shared::int32 height)
	{
		m_width = width;
		m_height = height;
	}
}