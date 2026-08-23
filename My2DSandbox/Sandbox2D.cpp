#include "2DPch.h"
#include "Sandbox2D.h"
#include "Sandbox2DScene.h"
#include "ClientPacketHandler.h"

namespace MGSL::Sandbox2D
{
    Sandbox2D::Sandbox2D() = default;
    Sandbox2D::~Sandbox2D() = default;

	bool Sandbox2D::Init(const Framework::WindowDesc& desc)
	{
		// 프레임워크 초기화
		if (!InitFramework(desc)) return false;

		// 네트워크 초기화
		if (!MGSL_NETWORK_MGR.Init(L"127.0.0.1", 7777)) return false;

		// 씬 초기화
		m_singleScene = std::make_unique<Sandbox2DScene>();
		if (!m_singleScene) return false;
		if (!m_singleScene->Load()) return false;
		SetSceneState(Framework::ESceneState::LOADING);

		return true;
	}

	void Sandbox2D::Run()
	{
		while (!ShouldClose())
		{
			// Window / Time / Input / Resource
			BeginFrame();

			// Client Content
			MGSL_NETWORK_MGR.Update();

			// Scene
			UpdateScene();

			// Rendering
			RenderFrame();
		}
	}

	void Sandbox2D::Clear()
	{
		// Network worker 종료 요청이 먼저
		MGSL_NETWORK_MGR.Clear();

		// 이후 Framework thread join 및 자원 정리
		ClearFramework();
	}

	void Sandbox2D::OnSceneReady()
	{
		MGSL_NETWORK_MGR.SendPacket(Net::ClientPacketHandler::Make_C_EnterGame());
	}
}