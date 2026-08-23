#include "GameFrameworkPch.h"
#include "GLProgram.h"

namespace MGSL::Framework
{
	GLProgram::GLProgram() = default;
	GLProgram::~GLProgram()
	{
		if (!IsValid()) return;
		::glDeleteProgram(m_programID);
		m_programID = 0;
	}

	GLProgramUPtr GLProgram::Create
	(
		const Shared::String& vertexSource,
		const Shared::String& fragmentSource
	)
	{
		if (vertexSource.empty() || fragmentSource.empty())
			return nullptr;

		// Vertex Shader 컴파일
		const Shared::uint32 vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexSource);
		if (vertexShaderID == 0)
			return nullptr;

		// Fragment Shader 컴파일
		const Shared::uint32 fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
		if (fragmentShaderID == 0)
		{
			::glDeleteShader(vertexShaderID);
			return nullptr;
		}

		// Program 생성
		GLProgramUPtr program(new GLProgram());
		program->m_programID = ::glCreateProgram();
		if (!program->IsValid())
		{
			::glDeleteShader(vertexShaderID);
			::glDeleteShader(fragmentShaderID);
			return nullptr;
		}

		// Program 링크
		const bool linkResult = program->Link(vertexShaderID, fragmentShaderID);

		::glDeleteShader(vertexShaderID);
		::glDeleteShader(fragmentShaderID);
		if (!linkResult) return nullptr;

		return program;
	}

	void GLProgram::Bind() const
	{
		if (!IsValid()) return;
		::glUseProgram(m_programID);
	}

	void GLProgram::Unbind()
	{
		::glUseProgram(0);
	}

	Shared::uint32 GLProgram::GetProgramID() const
	{
		return m_programID;
	}

	bool GLProgram::IsValid() const
	{
		return m_programID != 0;
	}

	Shared::uint32 GLProgram::CompileShader
	(
		GLenum shaderType,
		const Shared::String& shaderSource
	)
	{
		// 셰이더 생성
		const Shared::uint32 shaderID = ::glCreateShader(shaderType);
		if (shaderID == 0) return 0;

		// 셰이더 소스 추출
		Shared::cstr sourcePointer = shaderSource.c_str();
		::glShaderSource(shaderID, 1, &sourcePointer, nullptr);

		// 셰이더 컴파일
		::glCompileShader(shaderID);

		// 셰이더 컴파일 결과 반환
		GLint compileResult = GL_FALSE;
		::glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_TRUE)
			return shaderID;

		// 셰이더 컴파일 오류 로그 반환
		const Shared::String errorLog = GetShaderErrorLog(shaderID);

		// 자원 정리
		::glDeleteShader(shaderID);

		return 0;
	}

	bool GLProgram::Link
	(
		Shared::uint32 vertexShaderID,
		Shared::uint32 fragmentShaderID
	)
	{
		// 셰이더 부착 및 링크
		::glAttachShader(m_programID, vertexShaderID);
		::glAttachShader(m_programID, fragmentShaderID);
		::glLinkProgram(m_programID);

		// 셰이더 링크 결과
		GLint linkResult = GL_FALSE;
		::glGetProgramiv(m_programID, GL_LINK_STATUS, &linkResult);

		// 자원 정리
		::glDetachShader(m_programID, vertexShaderID);
		::glDetachShader(m_programID, fragmentShaderID);
		if (linkResult == GL_TRUE)
			return true;

		// 셰이더 링크 오류 로그 반환
		const Shared::String errorLog = GetProgramErrorLog(m_programID);
		return false;
	}

	Shared::String GLProgram::GetShaderErrorLog(Shared::uint32 shaderID)
	{
		GLint logLength = 0;
		::glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength <= 0)
			return {};

		Shared::String errorLog((Shared::usize)logLength, '\0');

		GLsizei writtenLength = 0;
		::glGetShaderInfoLog(shaderID, logLength, &writtenLength, errorLog.data());
		if (writtenLength <= 0)
			return {};

		errorLog.resize((Shared::usize)writtenLength);
		return errorLog;
	}

	Shared::String GLProgram::GetProgramErrorLog(Shared::uint32 programID)
	{
		GLint logLength = 0;
		::glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength <= 0)
			return {};

		Shared::String errorLog((Shared::usize)logLength, '\0');

		GLsizei writtenLength = 0;
		::glGetProgramInfoLog(programID, logLength, &writtenLength, errorLog.data());
		if (writtenLength <= 0)
			return {};

		errorLog.resize((Shared::usize)writtenLength);
		return errorLog;
	}
}
