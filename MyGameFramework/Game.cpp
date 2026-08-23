#include "GameFrameworkPch.h"
#include "Game.h"
#include "Window.h"
#include "GLContext.h"
#include "Scene.h"

namespace MGSL::Framework
{
	Game::Game() = default;
	Game::~Game() = default;

	bool Game::InitFramework(const WindowDesc& desc)
	{
		// 1. 윈도우 생성
		m_window = std::make_unique<Window>();
		if (!m_window->Create(desc)) return false;
		HWND hwnd = m_window->GetHWND();

		// 2. 렌더 컨텍스트 생성 및 연결
		m_renderContext = std::make_unique<GLContext>();
		if (!m_renderContext->Init(hwnd)) return false;

		// 3. 매니저 초기화 (순서 보장)
		if (!MGSL_INPUT_MGR.Init(hwnd)) return false;
		MGSL_RESOURCE_MGR.Init();
		MGSL_TIME_MGR.Init();
		MGSL_UI_MGR.Init();
		MGSL_COLLIDE_MGR.Init();
		if (!MGSL_RENDER_MGR.Init()) return false;

		return true;
	}

	void Game::BeginFrame()
	{
		m_window->ProcessMessages();
		MGSL_TIME_MGR.Update();
		MGSL_INPUT_MGR.Update();
		MGSL_RESOURCE_MGR.Update();
		MGSL_UI_MGR.Update();
	}

	void Game::UpdateScene()
	{
		UpdateSceneState();
	}

	void Game::RenderFrame()
	{
		if (m_sceneState != ESceneState::RUNNING)
			return;

		// RenderItem을 Batch로 빌드
		MGSL_RENDER_MGR.Build();

		// 렌더 초기화
		m_renderContext->SetClearColor(0.52f, 0.72f, 0.74f);
		m_renderContext->ClearBuffer();

		// 씬 내용 렌더링
		MGSL_RENDER_MGR.Render();

		// 버퍼 스왑
		m_renderContext->SwapBuffers();

		// 이번 프레임 렌더 데이터 초기화
		MGSL_RENDER_MGR.Clear();
	}

	void Game::ClearFramework()
	{
		// 싼 리소스 해제
		m_singleScene->Destroy();

		// 매니저 리소스 해제
		MGSL_COLLIDE_MGR.Clear();
		MGSL_TIME_MGR.Clear();
		MGSL_UI_MGR.Clear();
		MGSL_RESOURCE_MGR.Clear();
		MGSL_INPUT_MGR.Clear();
		MGSL_THREAD_MGR.Join();

		// 렌더 컨텍스트 비우기
		m_renderContext->Clear(m_window->GetHWND());
	}

	bool Game::ShouldClose() const
	{
		if (!m_window) return true;
		return m_window->ShouldClose();
	}

	void Game::SetSceneState(ESceneState state)
	{
		m_sceneState = state;
	}

	ESceneState Game::GetSceneState() const
	{
		return m_sceneState;
	}

	void Game::UpdateSceneState()
	{
		switch (m_sceneState)
		{
			case ESceneState::NONE: break;
			case ESceneState::LOADING:
			{
				// 씬 로드 실패 시
				if (m_singleScene->IsLoadFailed())
					break;

				// 씬 로드 완료 시
				if (m_singleScene->IsLoadCompleted())
				{
					m_singleScene->Init();
					SetSceneState(ESceneState::RUNNING);
					OnSceneReady();
				}
				break;
			}
			case ESceneState::RUNNING:
			{
				float deltaTime = MGSL_TIME_MGR.GetDeltaTime();
				m_singleScene->Update(deltaTime);
				MGSL_COLLIDE_MGR.Update(deltaTime);
				break;
			}
		}
	}
}