#pragma once
#include "MyGameFramework\MonoBehaviour.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(UIImage)
	MGSL_CLASS_PTR(UIText)
}

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(MyPlayerNetworkState)
	MGSL_CLASS_PTR(MainUIController)

	class MainUIController : public Framework::MonoBehaviour
	{
		MGSL_DISABLE_COPY(MainUIController)
		MGSL_DISABLE_MOVE(MainUIController)
		using Super = Framework::MonoBehaviour;

	public:
		virtual ~MainUIController() override;
		static MainUIControllerUPtr Create(Framework::GameObject* owner);
		virtual void Awake() override;
		virtual void Update(float deltaTime) override;

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
		void SetHeartCount(Shared::uint32 heartCount);
		void SetKillCount(Shared::uint32 killCount);

	private:
		explicit MainUIController(Framework::GameObject* owner);
		void ApplyPlayerColor();
		void ApplyWeaponType();

	/*===========================//
	//       UI references       //
	//===========================*/
	private:
		Framework::UIImage* m_faceImage = nullptr;
		Shared::List<Framework::UIImage*> m_heartImages;
		Framework::UIText* m_killCountText = nullptr;
		Framework::UIText* m_weaponTypeText = nullptr;
	
	/*===========================//
	//       Network state       //
	//===========================*/
	private:
		MyPlayerNetworkState* m_playerNetworkState = nullptr;
		Shared::vec4 m_prevPlayerColor{};
		Protobuf::WEAPON_TYPE m_prevWeapon{};

	/*===========================//
	//        HUD states         //
	//===========================*/

	// TODO : 내가 봤을 땐, 이 부분도 PlayerInfo로 넘어가야
	// 할 대상으로 보임
	private:
		Shared::uint32 m_heartCount = 5;
		Shared::uint32 m_killCount = 0;
	};
}
