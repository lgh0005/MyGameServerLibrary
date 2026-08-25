#pragma once
#include "ObjectRegistry.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GameObject)

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

	/*==============================//
	//   default scene interfaces   //
	//==============================*/
	public:
		virtual bool Load()	 = 0;

	protected:
		virtual void Build() = 0;

	public:
		void Init();
		void Update(float deltaTime);
		void Destroy();

	/*====================================//
	//   rendering setting interfaces     //
	//====================================*/
	protected:
		void SetMainCamera(Camera* camera);
		void SetUICamera(Camera* camera);

	protected:
		bool FinalizeRenderSetting();
		void SetSpriteShader(const ShaderPtr& shader);
		void SetPostProcessing2DShader(const ShaderPtr& shader);
		void SetUIImageShader(const ShaderPtr& shader);
		void SetUITextShader(const ShaderPtr& shader);
		void SetDebugShader(const ShaderPtr& shader);

	/*================================================//
	//   default scene members for resource loading   //
	//================================================*/
	public:
		bool IsLoadCompleted() const;
		bool IsLoadFailed() const;

	protected:
		void AddRequiredResourceName(Shared::String&& name);
		void ClearRequiredResourceNames();

	private:
		Shared::List<Shared::String> m_requiredResourceNames;

	/*=========================================//
	//   default game object control members   //
	//=========================================*/
	public:
		template<typename T>
		GameObject*  FindGameObjectWithComponent();
		GameObject* AddGameObject(GameObjectUPtr&& gameObject);
		void RemoveGameObject(GameObject* gameObject);
		void ClearGameObjects();

		bool RegisterNetworkObject(Shared::uint64 objectID, GameObject* gameObject);
		GameObject* FindNetworkObject(Shared::uint64 objectID);
		bool UnregisterNetworkObject(Shared::uint64 objectID);

	private:
		void FlushGameObjects();

	private:
		ObjectRegistry m_objectRegistry;
	};
}

#include "Scene.inl"