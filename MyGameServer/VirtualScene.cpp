#include "ServerPch.h"
#include "VirtualScene.h"
#include "GameObject.h"
#include "BoxCollider.h"

namespace MGSL::Server
{
	VirtualScene::VirtualScene() = default;
	VirtualScene::~VirtualScene() { ClearGameObjects(); }

	void VirtualScene::Init()
	{
		Build();
	}

	void VirtualScene::Update(float deltaTime)
	{
		/*========================//
		//         Update         //
		//========================*/
		for (const GameObjectUPtr& gameObject : m_gameObjects)
		{
			if (!gameObject) continue;
			gameObject->Update(deltaTime);
		}

		/*========================//
		//       Collision        //
		//========================*/
		MGSL_SERVER_COLLISION_MGR.Update();

		/*========================//
		//       LateUpdate       //
		//========================*/
		for (const GameObjectUPtr& gameObject : m_gameObjects)
		{
			if (!gameObject) continue;
			gameObject->LateUpdate(deltaTime);
		}
	}

	void VirtualScene::Destroy()
	{
		ClearGameObjects();
	}

	GameObject* VirtualScene::AddGameObject(GameObjectUPtr&& gameObject)
	{
		if (!gameObject) return nullptr;
		GameObject* ptr = gameObject.get();
		m_gameObjects.push_back(std::move(gameObject));
		return ptr;
	}

	void VirtualScene::RemoveGameObject(GameObject* gameObject)
	{
		if (!gameObject) return;
		for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
		{
			if (it->get() != gameObject) continue;
			
			// 등록된 컴포넌트를 해제
			// TODO : 이후에 이건 클라이언트도 똑같은데,
			// 컴포넌트를 정리하기 위해 자신이 들고 있는 컴포넌트 배열을 반환, 그걸 순회해서
			// 해제하는 로직이 필요할 수 있음.
			// 아마, Component 클래스 단에서 등록 해제와 관련된 메서드를 두고 그걸 오버라이딩 시켜야 할 수도.
			BoxCollider* collider = gameObject->GetComponent<BoxCollider>();
			if (collider) MGSL_SERVER_COLLISION_MGR.Unregister(collider);

			m_gameObjects.erase(it);
			return;
		}
	}

	void VirtualScene::ClearGameObjects()
	{
		// 등록된 충돌체 정리
		for (const GameObjectUPtr& gameObject : m_gameObjects)
		{
			if (!gameObject) continue;

			BoxCollider* collider = gameObject->GetComponent<BoxCollider>();
			if (collider) MGSL_SERVER_COLLISION_MGR.Unregister(collider);
		}

		// 게임 오브젝트 정리
		m_gameObjects.clear();
	}
}