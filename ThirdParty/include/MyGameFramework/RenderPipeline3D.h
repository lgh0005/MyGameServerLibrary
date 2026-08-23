#pragma once
#include "RenderPipeline.h"

namespace MGSL::Framework
{
	class RenderPipeline3D : public RenderPipeline
	{
		MGSL_DISABLE_COPY(RenderPipeline3D)
		MGSL_DISABLE_MOVE(RenderPipeline3D)
		using Super = RenderPipeline;

	public:
		virtual ~RenderPipeline3D() override;

	/*============================================//
	//   default RenderPipeline interface impls   //
	//============================================*/
	public:
		virtual bool Init() override;
		virtual void Build() override;
		virtual void Render() override;
		virtual void Clear() override;

	private:
		RenderPipeline3D();
	};
}
