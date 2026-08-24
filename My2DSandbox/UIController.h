#pragma once
#include "MyGameFramework\MonoBehaviour.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(UIImage)
	MGSL_CLASS_PTR(UIText)
}

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(UIController)

	class UIController : public Framework::MonoBehaviour
	{
		MGSL_DISABLE_COPY(UIController)
		MGSL_DISABLE_MOVE(UIController)
		using Super = Framework::MonoBehaviour;

	public:
		virtual ~UIController() override;
		static UIControllerUPtr Create(Framework::GameObject* owner);

	/*===========================//
	//    UI reference setters   //
	//===========================*/
	public:
		void SetFaceImage(Framework::UIImage* image);
		void AddHeartImage(Framework::UIImage* image);
		void SetKillCountText(Framework::UIText* text);
		void SetWeaponTypeText(Framework::UIText* text);

	/*===========================//
	//      HUD state setters    //
	//===========================*/
	public:
		void SetPlayerColor(const Shared::vec4& color);
		void SetHeartCount(Shared::uint32 heartCount);
		void SetKillCount(Shared::uint32 killCount);
		void SetWeaponType(EWeaponType weaponType);

	private:
		explicit UIController(Framework::GameObject* owner);

	/*===========================//
	//       UI references       //
	//===========================*/
	private:
		Framework::UIImage* m_faceImage = nullptr;
		Shared::List<Framework::UIImage*> m_heartImages;
		Framework::UIText* m_killCountText = nullptr;
		Framework::UIText* m_weaponTypeText = nullptr;
	
	/*===========================//
	//        HUD states         //
	//===========================*/
	private:
		Shared::vec4 m_playerColor{ 1.0f };
		Shared::uint32 m_heartCount = 3;
		Shared::uint32 m_killCount = 0;
		EWeaponType m_weaponType = EWeaponType::FIGHTER;
	};
}
