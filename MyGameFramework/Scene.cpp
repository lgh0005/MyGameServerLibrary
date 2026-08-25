#include "GameFrameworkPch.h"
#include "Scene.h"
#include "GameObject.h"

namespace MGSL::Framework
{
	Scene::Scene() = default;
	Scene::~Scene() = default;

	/*==============================//
	//   default scene interfaces   //
	//==============================*/
	void Scene::Init()
	{
		// 0. Build 페이즈
		// Build 과정에서 생성한 객체를 활성 객체로 옮김
		Build();

		// 1. Awake와 Start 호출
		FlushGameObjects();
	}

	void Scene::Update(float deltaTime)
	{
		// 새로 추가된 객체는 이 시점에 Awake와 Start가 실행
		FlushGameObjects();

		// 3. Update 페이즈
		m_objectRegistry.Update(deltaTime);

		// 4. LateUpdate 페이즈
		m_objectRegistry.LateUpdate(deltaTime);

		// 5. Submit 페이즈
		// RenderManager에 렌더링 작업을 제출
		m_objectRegistry.Submit();
	}

	void Scene::Destroy()
	{
		ClearGameObjects();
		ClearRequiredResourceNames();
	}

	/*====================================//
	//   rendering setting interfaces     //
	//====================================*/
	bool Scene::FinalizeRenderSetting() { return MGSL_RENDER_MGR.Finalize(); }
	void Scene::SetMainCamera(Camera* camera) { MGSL_RENDER_MGR.SetMainCamera(camera); }
	void Scene::SetUICamera(Camera* camera) { MGSL_UI_MGR.SetUICamera(camera); }
	void Scene::SetSpriteShader(const ShaderPtr& shader) { MGSL_RENDER_MGR.SetSpriteShader(shader); }
	void Scene::SetPostProcessing2DShader(const ShaderPtr& shader) { MGSL_RENDER_MGR.SetPostProcessing2DShader(shader); }
	void Scene::SetUIImageShader(const ShaderPtr& shader) { MGSL_RENDER_MGR.SetUIImageShader(shader); }
	void Scene::SetUITextShader(const ShaderPtr& shader) { MGSL_RENDER_MGR.SetUITextShader(shader); }
	void Scene::SetDebugShader(const ShaderPtr& shader) { MGSL_RENDER_MGR.SetDebugShader(shader); }

	/*================================================//
	//   default scene methods for resource loading   //
	//================================================*/
	bool Scene::IsLoadCompleted() const
	{
		for (const Shared::String& name : m_requiredResourceNames)
		{
			if (!MGSL_RESOURCE_MGR.IsResourceReady(name))
				return false;
		}

		return true;
	}

	bool Scene::IsLoadFailed() const
	{
		for (const Shared::String& name: m_requiredResourceNames)
		{
			if (!MGSL_RESOURCE_MGR.IsResourceFailed(name)) continue;
			MGSL_LOG_ERROR
			(
				"Failed to load required resource: {}",
				name
			);
			return true;
		}

		return false;
	}

	void Scene::AddRequiredResourceName(Shared::String&& name)
	{
		m_requiredResourceNames.push_back(std::move(name));
	}

	void Scene::ClearRequiredResourceNames()
	{
		m_requiredResourceNames.clear();
	}

	/*=========================================//
	//   default game object control members   //
	//=========================================*/
	GameObject* Scene::AddGameObject(GameObjectUPtr&& gameObject)
	{
		return m_objectRegistry.Add(std::move(gameObject));
	}

	void Scene::RemoveGameObject(GameObject* gameObject)
	{
		m_objectRegistry.Remove(gameObject);
	}

	void Scene::ClearGameObjects()
	{
		m_objectRegistry.Clear();
	}

	bool Scene::RegisterNetworkObject(Shared::uint64 objectID, GameObject* gameObject)
	{
		return m_objectRegistry.RegisterNetworkObject(objectID, gameObject);
	}

	GameObject* Scene::FindNetworkObject(Shared::uint64 objectID)
	{
		return m_objectRegistry.FindNetworkObject(objectID);
	}

	bool Scene::UnregisterNetworkObject(Shared::uint64 objectID)
	{
		return m_objectRegistry.UnregisterNetworkObject(objectID);
	}

	void Scene::FlushGameObjects()
	{
		// ObjectRegistry가 내부 멤버로 재사용하는 이번 Flush의 신규 객체 목록을 참조로 가져오기
		const Shared::List<GameObject*>& addedObjects = m_objectRegistry.Flush();

		// 이번에 새로 활성화된 모든 객체의 Awake를 먼저 실행
		for (GameObject* gameObject : addedObjects)
		{
			if (gameObject)
				gameObject->Awake();
		}

		// 모든 객체의 Awake가 끝난 뒤 Start를 실행
		for (GameObject* gameObject : addedObjects)
		{
			if (gameObject)
				gameObject->Start();
		}
	}
}
