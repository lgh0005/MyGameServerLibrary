#include "2DPch.h"
#include "MainUIController.h"
#include "MyPlayerController.h"
#include "MyPlayerNetworkState.h"
#include "MyGameFramework/GameObject.h"
#include "MyGameFramework/UIImage.h"
#include "MyGameFramework/UIText.h"

namespace MGSL::Sandbox2D
{
	MainUIController::MainUIController(Framework::GameObject* owner) : Super(owner) { }
	MainUIController::~MainUIController() = default;

	MainUIControllerUPtr MainUIController::Create(Framework::GameObject* owner)
	{
		return MainUIControllerUPtr(new MainUIController(owner));
	}

	void MainUIController::Awake()
	{
		Framework::GameObject* player = GetOwner()->GetOwner()->FindGameObjectWithComponent<MyPlayerController>();
		if (!player) return;
		m_playerNetworkState = player->GetComponent<MyPlayerNetworkState>();
	}

	void MainUIController::Update(float /*deltaTime*/)
	{
		if (!m_playerNetworkState) return;

		if (m_prevWeapon != m_playerNetworkState->GetWeapon())
			ApplyWeaponType();

		if (m_prevPlayerColor != m_playerNetworkState->GetColor())
			ApplyPlayerColor();
	}

	/*===========================//
	//    UI reference setters   //
	//===========================*/
	void MainUIController::SetFaceImage(Framework::UIImage* image) { m_faceImage = image; }

	void MainUIController::AddHeartImage(Framework::UIImage* image) { if (!image) return; m_heartImages.push_back(image); }

	void MainUIController::SetKillCountText(Framework::UIText* text) { m_killCountText = text; }

	void MainUIController::SetWeaponTypeText(Framework::UIText* text) { m_weaponTypeText = text; }

	/*===========================//
	//      HUD state setters    //
	//===========================*/
	void MainUIController::SetHeartCount(Shared::uint32 heartCount)
	{
		m_heartCount = heartCount;

		if (m_heartCount > m_heartImages.size())
			m_heartCount = static_cast<Shared::uint32>(m_heartImages.size());

		for (Shared::usize i = 0; i < m_heartImages.size(); ++i)
		{
			Framework::UIImage* heartImage = m_heartImages[i];
			if (!heartImage) continue;

			if (i < m_heartCount) heartImage->SetColor(Shared::vec4(1.0f));
			else heartImage->SetColor(Shared::vec4(0.35f, 0.35f, 0.35f, 1.0f));
		}
	}

	void MainUIController::SetKillCount(Shared::uint32 killCount)
	{
		m_killCount = killCount;
		if (!m_killCountText) return;
		m_killCountText->SetText("KILL : " + std::to_string(m_killCount));
	}

	void MainUIController::ApplyPlayerColor()
	{
		const Shared::vec4 color = m_playerNetworkState->GetColor();
		m_prevPlayerColor = color;
		if (!m_faceImage) return;
		m_faceImage->SetColor(color);
	}

	void MainUIController::ApplyWeaponType()
	{
		const Protobuf::WEAPON_TYPE weapon = m_playerNetworkState->GetWeapon();

		if (!m_weaponTypeText) return;
		switch (weapon)
		{
			case Protobuf::WEAPON_TYPE_NONE: m_weaponTypeText->SetText("FIGHTER"); break;
			case Protobuf::WEAPON_TYPE_PISTOL: m_weaponTypeText->SetText("PISTOL"); break;
			case Protobuf::WEAPON_TYPE_SWORD: m_weaponTypeText->SetText("SWORD"); break;
			default: break;
		}

		m_prevWeapon = weapon;
	}
}