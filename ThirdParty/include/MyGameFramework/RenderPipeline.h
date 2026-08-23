#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(RenderPass)

	class RenderPipeline
	{
		MGSL_DISABLE_COPY(RenderPipeline)
		MGSL_DISABLE_MOVE(RenderPipeline)

	public:
		virtual ~RenderPipeline();

	public:
		virtual bool Init() = 0;
		virtual void Build() = 0;
		virtual void Render() = 0;
		virtual void Clear() = 0;

	protected:
		RenderPipeline();
	};
}
