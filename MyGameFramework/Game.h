#pragma once

namespace MGSL::Framework
{
	struct WindowDesc;
	MGSL_CLASS_PTR(Window)
	MGSL_CLASS_PTR(GLContext)
	MGSL_CLASS_PTR(Scene)

	class Game
	{
	public:
		Game();
		virtual ~Game();
		Scene* GetScene() const { return m_singleScene.get(); }

	public:
		bool InitFramework(const WindowDesc& desc);
		void BeginFrame();
		void UpdateScene();
		void RenderFrame();
		void ClearFramework();

	protected:
		bool ShouldClose() const;
		void SetSceneState(ESceneState state);
		ESceneState GetSceneState() const;
		virtual void OnSceneReady() = 0;

	private:
		void UpdateSceneState();

	protected:
		WindowUPtr m_window;
		GLContextUPtr m_renderContext;
		SceneUPtr m_singleScene;
		ESceneState m_sceneState = ESceneState::NONE;
	};
}

