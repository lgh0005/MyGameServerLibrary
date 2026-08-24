#include "2DPch.h"
#include "UIController.h"
#include "MyGameFramework/GameObject.h"
#include "MyGameFramework/UIImage.h"
#include "MyGameFramework/UIText.h"

namespace MGSL::Sandbox2D
{
	UIController::UIController(Framework::GameObject* owner) : Super(owner) { }
	UIController::~UIController() = default;

	UIControllerUPtr UIController::Create(Framework::GameObject* owner)
	{
		return UIControllerUPtr(new UIController(owner));
	}

	/*===========================//
	//    UI reference setters   //
	//===========================*/
	void UIController::SetFaceImage(Framework::UIImage* image)
	{
		m_faceImage = image;
	}

	void UIController::AddHeartImage(Framework::UIImage* image)
	{
		if (!image) return;
		m_heartImages.push_back(image);
	}

	void UIController::SetKillCountText(Framework::UIText* text)
	{
		m_killCountText = text;
	}

	void UIController::SetWeaponTypeText(Framework::UIText* text)
	{
		m_weaponTypeText = text;
	}

	/*===========================//
	//      HUD state setters    //
	//===========================*/
	void UIController::SetPlayerColor(const Shared::vec4& color)
	{
		m_playerColor = color;
		if (!m_faceImage) return;
		m_faceImage->SetColor(m_playerColor);
	}

	void UIController::SetHeartCount(Shared::uint32 heartCount)
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

	void UIController::SetKillCount(Shared::uint32 killCount)
	{
		m_killCount = killCount;
		if (!m_killCountText) return;
		m_killCountText->SetText("KILL : " + std::to_string(m_killCount));
	}

	void UIController::SetWeaponType(EWeaponType weaponType)
	{
		m_weaponType = weaponType;

		if (!m_weaponTypeText) return;

		switch (m_weaponType)
		{
		case EWeaponType::FIGHTER:
			m_weaponTypeText->SetText("FIGHTER");
			break;

		case EWeaponType::PISTOL:
			m_weaponTypeText->SetText("PISTOL");
			break;

		case EWeaponType::SWORD:
			m_weaponTypeText->SetText("SWORD");
			break;

		default:
			break;
		}
	}
}