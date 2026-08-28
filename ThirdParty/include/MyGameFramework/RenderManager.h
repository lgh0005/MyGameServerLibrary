#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Shader)
	MGSL_CLASS_PTR(Camera)
	MGSL_CLASS_PTR(GLBuffer)
	MGSL_CLASS_PTR(RenderPipeline2D)

	class RenderManager
	{
		MGSL_DECLARE_SINGLE(RenderManager)

	private:
		RenderManager();
		~RenderManager();

	public:
		bool Init();
		bool Finalize();

		void Build();
		void Render();
		void Clear();

	/*=================================//
	//   main camera setting methods   //
	//=================================*/
	public:
		void SetMainCamera(Camera* camera);
		Camera* GetMainCamera() const;
		void ClearMainCamera();
		bool HasMainCamera() const;

	/*===============================//
	//   render submission methods   //
	//===============================*/
	public:
		void Submit(const SpriteRenderItem& renderItem);
		void Submit(const UIImageRenderItem& renderItem);
		void Submit(const UITextRenderItem& renderItem);

	/*====================//
	//   shader setters   //
	//====================*/
	public:
		void SetSpriteShader(ShaderPtr spriteShader);
		void SetPostProcessing2DShader(ShaderPtr postProcessingShader);
		void SetUIImageShader(ShaderPtr uiImageShader);
		void SetUITextShader(ShaderPtr uiTextShader);

		// TEMP
		void SetDebugShader(ShaderPtr debugShader);

	/*=================//
	//   main camera   //
	//=================*/
	private:
		bool CreateCameraBuffer();
		void UpdateCameraBuffer();
		void BindCameraBuffer() const;

	/*==================================//
	//   post-processing value setters  //
	//==================================*/
	public:
		void SetPostProcessingTime(float time);
		void SetVignetteIntensity(float intensity);
		void SetChromaticAberrationStrength(float strength);

	private:
		Camera* m_mainCamera		= nullptr;
		GLBufferUPtr m_cameraBuffer = nullptr;
		RenderPipeline2DUPtr m_renderPipeline2D;
	};
}
