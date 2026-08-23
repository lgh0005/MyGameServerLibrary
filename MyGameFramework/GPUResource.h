#pragma once
#include "Resource.h"

namespace MGSL::Framework
{
	class GPUResource : public Resource
	{
		using Super = Resource;

	public:
		explicit GPUResource();
		virtual ~GPUResource() override;

	public:
		virtual bool UploadToGPU() = 0;
	};
}

