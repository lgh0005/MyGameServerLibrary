#pragma once

namespace MGSL::Server
{
	class VirtualScene
	{
		MGSL_DISABLE_COPY(VirtualScene)
		MGSL_DISABLE_MOVE(VirtualScene)

	public:
		VirtualScene();
		virtual ~VirtualScene();

	protected:
		virtual void Build() = 0;

	public:
		void Init();
		void Update(float deltaTime);
		void Destroy();

	/*=========================================//
	//   default game object control members   //
	//=========================================*/
	public:
		GameObject* AddGameObject(GameObjectUPtr&& gameObject);
		void RemoveGameObject(GameObject* gameObject);
		void ClearGameObjects();

	private:
		Shared::List<GameObjectUPtr> m_gameObjects;
	};
}

