#pragma once

namespace MGSL::Framework
{
	class RenderPass
	{
		MGSL_DISABLE_COPY(RenderPass)
		MGSL_DISABLE_MOVE(RenderPass)

	public:
		virtual ~RenderPass();

	public:
		virtual bool Init() = 0;
		virtual void Build();
		virtual void Render() = 0;
		virtual void Clear();

	protected:
		RenderPass();
	};
}

