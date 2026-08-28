#pragma once
#include "RenderPass.h"
#include "Uniform.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Shader)
	MGSL_CLASS_PTR(GLSampler)
	MGSL_CLASS_PTR(GLFramebuffer)
	MGSL_CLASS_PTR(QuadMesh)
	MGSL_CLASS_PTR(PostProcessingRenderPass2D)

	class PostProcessingRenderPass2D : public RenderPass
	{
		MGSL_DISABLE_COPY(PostProcessingRenderPass2D)
		MGSL_DISABLE_MOVE(PostProcessingRenderPass2D)
		using Super = RenderPass;

	public:
		static PostProcessingRenderPass2DUPtr Create();
		virtual ~PostProcessingRenderPass2D() override;

	/*============================================//
	//   default RenderPass interface impls       //
	//============================================*/
	public:
		virtual bool Init() override;
		virtual void Render() override;

	/*============================================//
	//   scene render-target control methods      //
	//============================================*/
	public:
		void BeginRenderTarget() const;
		void EndRenderTarget() const;

	/*==============================================//
	//   PostProcessingRenderPass2D shader setters  //
	//==============================================*/
	public:
		void SetPostProcessingShader(ShaderPtr shader);
		void SetRenderSize(Shared::int32 width, Shared::int32 height);

	/*===============================================//
	//   PostProcessingRenderPass2D uniform setters  //
	//===============================================*/
	public:
		void SetTime(float time);
		void SetVignetteIntensity(float intensity);
		void SetChromaticAberrationStrength(float strength);

	private:
		bool CreateUniformBuffer();
		void BindUniformBuffer() const;
		void UpdateUniformBuffer();

	private:
		PostProcessingRenderPass2D();

		ShaderPtr m_postProcessingShader;
		GLFramebufferUPtr m_framebuffer;
		QuadMeshPtr m_screenQuad;

		GLSamplerUPtr m_postProcessingSampler;
		Shared::uint32 m_colorTextureID = 0;
		Shared::uint32 m_depthStencilTextureID = 0;

		Shared::int32 m_width = 1280;
		Shared::int32 m_height = 720;

		PostProcessingData m_postProcessingData{};
		GLBufferUPtr m_uniformBuffer;
	};
}

