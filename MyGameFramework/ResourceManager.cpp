#include "GameFrameworkPch.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "GPUResource.h"

namespace MGSL::Framework
{
	ResourceManager::ResourceManager() = default;
	ResourceManager::~ResourceManager() = default;

	void ResourceManager::Init(Shared::int32 workCount)
	{
		m_workerCount = workCount;
		for (Shared::int32 i = 0; i < workCount; i++)
			MGSL_THREAD_MGR.Launch([this]() { { ResourceWorkerMain(); } });
	}

	void ResourceManager::Update()
	{
		// 메인 스레드로부터 GPU 업로드를 시켜야 함 (OpenGL 컨텍스트가 메인 스레드에 생성)
		GPUResourcePtr resource;
		while (m_gpuUploadQueue.TryPop(resource))
		{
			bool result = resource->UploadToGPU();
			if (result) resource->SetResourceState(EResourceState::READY);
			else resource->SetResourceState(EResourceState::FAILED);
		}
	}

	void ResourceManager::Clear()
	{
		m_resourceRequestQueue.Close();
	}

	/*====================================//
	//   Main ResourceManager Intefaces   //
	//====================================*/
	bool ResourceManager::IsResourceReady(const Shared::String& name)
	{
		const auto iter = m_resources.find(name);
		if (iter == m_resources.end()) return false;

		const EResourceState state = iter->second->GetResourceState();
		return state == EResourceState::READY;
	}

	bool ResourceManager::IsResourceFailed(const Shared::String& name)
	{
		const auto iter = m_resources.find(name);
		if (iter == m_resources.end()) return false;
		const EResourceState state = iter->second->GetResourceState();
		return state == EResourceState::FAILED;
	}

	/*===================================//
	//   Resource async loading members  //
	//===================================*/
	bool ResourceManager::RequestLoad(ResourcePtr resource)
	{
		if (resource->GetResourceState() != EResourceState::UNLOADED)
			return false;

		resource->SetResourceState(EResourceState::LOADING);

		if (!m_resourceRequestQueue.Push(resource))
		{
			resource->SetResourceState(EResourceState::UNLOADED);
			return false;
		}

		return true;
	}

	void ResourceManager::ResourceWorkerMain()
	{
		ResourcePtr resource;
		while (m_resourceRequestQueue.WaitPop(resource))
		{
			if (!resource->Load())
			{
				resource->SetResourceState(EResourceState::FAILED);
				continue;
			}

			if (resource->GetResourceType() == EResourceType::GPU_RESOURCE)
			{
				auto gpuResource = static_pointer_cast<GPUResource>(resource);
				if (!m_gpuUploadQueue.Push(std::move(gpuResource)))
					resource->SetResourceState(EResourceState::FAILED);
				continue;
			}

			resource->SetResourceState(EResourceState::READY);
		}
	}
}