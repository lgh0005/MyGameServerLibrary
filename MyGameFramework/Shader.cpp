#include "GameFrameworkPch.h"
#include "Shader.h"
#include "GLProgram.h"

namespace MGSL::Framework
{
	Shader::Shader
	(
		fs::path vertexShaderPath,
		fs::path fragmentShaderPath
	) : m_vertexShaderPath(std::move(vertexShaderPath)),
		m_fragmentShaderPath(std::move(fragmentShaderPath)) { }
	Shader::~Shader() = default;

	/*=====================//
	//   default methods   //
	//=====================*/
	void Shader::Bind() const
	{
		if (!m_program) return;
		m_program->Bind();
	}

	void Shader::Unbind()
	{
		GLProgram::Unbind();
	}

	bool Shader::IsValid() const
	{
		return m_program != nullptr && m_program->IsValid();
	}

	/*==========================//
	//   Interface implements   //
	//==========================*/
	bool Shader::Load()
	{
		// 1. Vertex Shader 파일 입출력
		if (!ReadShaderSource(m_vertexShaderPath, m_vertexSource))
		{
			MGSL_LOG_ERROR("Failed to load vertex shader: {}", m_vertexShaderPath.string());
			return false;
		}

		// 2. Fragment Shader 파일 입출력
		if (!ReadShaderSource(m_fragmentShaderPath, m_fragmentSource))
		{
			MGSL_LOG_ERROR("Failed to load fragment shader: {}", m_fragmentShaderPath.string());
			return false;
		}

		return true;
	}

	bool Shader::UploadToGPU()
	{
		m_program = GLProgram::Create
		(
			m_vertexSource,
			m_fragmentSource
		);

		if (!m_program)return false;

		m_vertexSource.clear();
		m_fragmentSource.clear();

		return true;
	}

	bool Shader::ReadShaderSource
	(
		const fs::path& path,
		Shared::String& source
	)
	{
		// 파일 인풋 스트림 생성
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file.is_open())
			return false;

		// 파일 끝으로 커서를 이동
		file.seekg(0, std::ios::end);

		// 현재 커서 위치(파일 크기, 바이트 단위) 획득
		const std::streamoff fileSize = file.tellg();
		if (fileSize <= 0) return false;

		// 커서를 다시 파일 맨 앞으로 이동
		file.seekg(0, std::ios::beg);
		source.resize((Shared::usize)fileSize);

		// 파일 내용을 source 버퍼에 통째로 읽어들이기
		file.read(source.data(), (std::streamsize)fileSize);
		if (!file)
		{
			source.clear();
			return false;
		}

		return true;
	}
}
