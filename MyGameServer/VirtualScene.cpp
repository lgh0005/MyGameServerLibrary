#include "ServerPch.h"
#include "VirtualScene.h"
#include "GameObject.h"

namespace MGSL::Server
{
	VirtualScene::VirtualScene() = default;
	VirtualScene::~VirtualScene() = default;

	void VirtualScene::Init()
	{
		Build();
	}

	void VirtualScene::Update(float deltaTime)
	{
		/*========================//
		//      Update Copy       //
		//========================*/
		Shared::List<GameObject*> updateObjects;
		updateObjects.reserve(m_gameObjects.size());

		/*========================//
		//         Update         //
		//========================*/
		for (const GameObjectUPtr& gameObject : m_gameObjects)
		{
			if (!gameObject) continue;
			updateObjects.push_back(gameObject.get());
		}

		for (GameObject* gameObject : updateObjects)
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

		/*========================//
		//         Remove         //
		//========================*/
		FlushRemoveObjects();
	}

	void VirtualScene::Destroy()
	{
		m_pendingRemoveObjects.clear();

		while (!m_gameObjects.empty())
		{
			GameObject* gameObject = m_gameObjects.back().get();
			if (!gameObject)
			{
				m_gameObjects.pop_back();
				continue;
			}

			gameObject->OnDestroy();
			m_gameObjects.pop_back();
		}
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
		const auto it = std::find
		(m_pendingRemoveObjects.begin(), m_pendingRemoveObjects.end(), gameObject);
		if (it != m_pendingRemoveObjects.end()) return;
		m_pendingRemoveObjects.push_back(gameObject);
	}

	void VirtualScene::FlushRemoveObjects()
	{
		if (m_pendingRemoveObjects.empty())
			return;

		m_gameObjects.erase
		(
			std::remove_if
			(
				m_gameObjects.begin(),
				m_gameObjects.end(),

				[this](const GameObjectUPtr& gameObject)
				{
					if (!gameObject) return false;
					const auto it = std::find
					(
						m_pendingRemoveObjects.begin(),
						m_pendingRemoveObjects.end(),
						gameObject.get()
					);

					if (it == m_pendingRemoveObjects.end()) return false;
					gameObject->OnDestroy();
					return true;
				}
			),
			m_gameObjects.end()
		);

		m_pendingRemoveObjects.clear();
	}
}