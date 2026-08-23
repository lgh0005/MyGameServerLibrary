#pragma once
#include "MyGameShared/BlockingQueue.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Resource)
	MGSL_CLASS_PTR(GPUResource)

	class ResourceManager
	{
		MGSL_DECLARE_SINGLE(ResourceManager)

	private:
		ResourceManager();
		~ResourceManager();

	public:
		void Init(Shared::int32 workerCount = 2);
		void Update();
		void Clear();

	/*====================================//
	//   Main ResourceManager Intefaces   //
	//====================================*/
	public:
		template<typename T>
		Shared::SharedPtr<T> GetResource(Shared::String name);

		template<typename T, typename... Args>
		bool RegisterResource
		(
			Shared::String name,
			Args&&... args
		);

		// TODO : 이후에 리소스 상태를 얻어오는 식으로 통합 필요
		bool IsResourceReady(const Shared::String& name);
		bool IsResourceFailed(const Shared::String& name);

	/*===================================//
	//   Resource async loading members  //
	//===================================*/
	private:
		bool RequestLoad(ResourcePtr resource);
		void ResourceWorkerMain();

	private:
		Shared::int32 m_workerCount = 0;
		Shared::BlockingQueue<ResourcePtr>    m_resourceRequestQueue;
		Shared::BlockingQueue<GPUResourcePtr> m_gpuUploadQueue;
		Shared::Dictionary<Shared::String, ResourcePtr> m_resources;
	};
}

#include "ResourceManager.inl"