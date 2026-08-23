#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLProgram)

	class GLProgram
	{
		MGSL_DISABLE_COPY(GLProgram)
		MGSL_DISABLE_MOVE(GLProgram)

	public:
		~GLProgram();
		static GLProgramUPtr Create
		(
			const Shared::String& vertexSource,
			const Shared::String& fragmentSource
		);

		void Bind() const;
		static void Unbind();

	public:
		Shared::uint32 GetProgramID() const;
		bool IsValid() const;

	private:
		GLProgram();
		static Shared::uint32 CompileShader
		(
			GLenum shaderType,
			const Shared::String& shaderSource
		);

		bool Link
		(
			Shared::uint32 vertexShaderID,
			Shared::uint32 fragmentShaderID
		);

		static Shared::String GetShaderErrorLog(Shared::uint32 shaderID);
		static Shared::String GetProgramErrorLog(Shared::uint32 programID);

	private:
		Shared::uint32 m_programID = 0;
	};
}

