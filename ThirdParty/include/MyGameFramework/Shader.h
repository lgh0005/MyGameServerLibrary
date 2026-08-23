#pragma once
#include "GPUResource.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLProgram)

	// 이후에 컴퓨트 셰이더를 받도록
	// Shader -> GraphicsShader, ComputeShader로 상속 구조 생성 필요

	class Shader : public GPUResource
	{
		using Super = GPUResource;

	public:
		explicit Shader
		(
			fs::path vertexShaderPath,
			fs::path fragmentShaderPath
		);
		virtual ~Shader() override;

	/*============================//
	//   default shader methods   //
	//============================*/
	public:
		void Bind() const;
		static void Unbind();
		bool IsValid() const;

	/*==========================//
	//   Interface implements   //
	//==========================*/
	public:
		virtual bool Load() override;
		virtual bool UploadToGPU() override;

	private:
		static bool ReadShaderSource
		(
			const fs::path& path,
			Shared::String& source
		);

	private:
		fs::path m_vertexShaderPath;
		fs::path m_fragmentShaderPath;

		Shared::String m_vertexSource;
		Shared::String m_fragmentSource;

		GLProgramUPtr m_program;
	};
}

