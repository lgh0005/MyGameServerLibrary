#include "2DPch.h"
#include "Sandbox2DScene.h"
#include "MyGameFramework/Camera.h"
#include "MyGameFramework/GameObject.h"
#include "MyGameFramework/Font.h"
#include "MyGameFramework/Shader.h"
#include "MyGameFramework/Texture2D.h"
#include "MyGameFramework/Transform.h"
#include "MyGameFramework/SpriteRenderer.h"
#include "MyGameFramework/FlipbookClip.h"
#include "MyGameFramework/FlipbookController.h"
#include "MyGameFramework/FlipbookPlayer.h"
#include "MyGameFramework/UICanvas.h"
#include "MyGameFramework/UIImage.h"
#include "MyGameFramework/UIText.h"
#include "MyGameFramework/CharacterBody2D.h"
#include "MyGameFramework/BoxCollider.h"

#include "CameraController.h"
#include "MyPlayerController.h"

namespace MGSL::Sandbox2D
{
	Sandbox2DScene::Sandbox2DScene() = default;
	Sandbox2DScene::~Sandbox2DScene() = default;

	bool Sandbox2DScene::Load()
	{
		/* 리소스를 로드합니다. */

		/*=======================//
		//   shader loading      //
		//=======================*/
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Shader>("Sandbox2D.SpriteShader", "Resource/Shaders/Sprite2D.vert", "Resource/Shaders/Sprite2D.frag")) return false;
		AddRequiredResourceName("Sandbox2D.SpriteShader");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Shader>("Sandbox2D.PostProcessingShader", "Resource/Shaders/PostProcessing2D.vert", "Resource/Shaders/PostProcessing2D.frag")) return false;
		AddRequiredResourceName("Sandbox2D.PostProcessingShader");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Shader>("Sandbox2D.UIImageShader", "Resource/Shaders/UIImage.vert", "Resource/Shaders/UIImage.frag")) return false;
		AddRequiredResourceName("Sandbox2D.UIImageShader");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Shader>("Sandbox2D.UITextShader", "Resource/Shaders/UIText.vert", "Resource/Shaders/UIText.frag")) return false;
		AddRequiredResourceName("Sandbox2D.UITextShader");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Shader>("Sandbox2D.DebugGizmoShader", "Resource/Shaders/DebugGizmo.vert", "Resource/Shaders/DebugGizmo.frag")) return false;
		AddRequiredResourceName("Sandbox2D.DebugGizmoShader");

		/*=======================//
		//   texture loading     //
		//=======================*/
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Background.Background1", "Resource/Sprites/Background/background1.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Background.Background1");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Background.Background2", "Resource/Sprites/Background/background2.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Background.Background2");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Background.Map", "Resource/Sprites/Background/background_map.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Background.Map");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Props.Background3", "Resource/Sprites/Props/background3.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Props.Background3");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Props.Bullet", "Resource/Sprites/Props/bullet.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Props.Bullet");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Props.Ladder", "Resource/Sprites/Props/ladder.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Props.Ladder");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Props.UIFace", "Resource/Sprites/Props/ui_face.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Props.UIFace");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Props.UIHeart", "Resource/Sprites/Props/ui_heart.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Props.UIHeart");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Effect.HitEffect", "Resource/Sprites/Effect/hit_effect.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Effect.HitEffect");

		/*========================//
		//   flipbook loading     //
		//========================*/
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.AirAttack", "Resource/Sprites/Player/fighter/fighter_air_attack.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.AirAttack");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Climb", "Resource/Sprites/Player/fighter/fighter_climb.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Climb");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Combo1", "Resource/Sprites/Player/fighter/fighter_combo_1.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Combo1");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Combo2", "Resource/Sprites/Player/fighter/fighter_combo_2.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Combo2");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Combo3", "Resource/Sprites/Player/fighter/fighter_combo_3.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Combo3");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Dash", "Resource/Sprites/Player/fighter/fighter_dash.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Dash");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Death", "Resource/Sprites/Player/fighter/fighter_death.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Death");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Hit", "Resource/Sprites/Player/fighter/fighter_hit.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Hit");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Idle", "Resource/Sprites/Player/fighter/fighter_idle.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Idle");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Jump", "Resource/Sprites/Player/fighter/fighter_jump.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Jump");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Run", "Resource/Sprites/Player/fighter/fighter_run.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Run");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Slide", "Resource/Sprites/Player/fighter/fighter_slide.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Slide");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Walk", "Resource/Sprites/Player/fighter/fighter_walk.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Fighter.Walk");

		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.AirAttack", "Resource/Sprites/Player/pistol/pistol_air_attack.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.AirAttack");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Climb", "Resource/Sprites/Player/pistol/pistol_climb.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Climb");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Dash", "Resource/Sprites/Player/pistol/pistol_dash.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Dash");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Death", "Resource/Sprites/Player/pistol/pistol_death.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Death");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Hit", "Resource/Sprites/Player/pistol/pistol_hit.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Hit");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Idle", "Resource/Sprites/Player/pistol/pistol_idle.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Idle");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Jump", "Resource/Sprites/Player/pistol/pistol_jump.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Jump");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Run", "Resource/Sprites/Player/pistol/pistol_run.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Run");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Shot", "Resource/Sprites/Player/pistol/pistol_shot.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Shot");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Slide", "Resource/Sprites/Player/pistol/pistol_slide.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Slide");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Walk", "Resource/Sprites/Player/pistol/pistol_walk.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Pistol.Walk");

		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.AirAttack", "Resource/Sprites/Player/sword/sword_air_attack.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.AirAttack");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Climb", "Resource/Sprites/Player/sword/sword_climb.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Climb");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Combo1", "Resource/Sprites/Player/sword/sword_combo_1.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Combo1");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Combo2", "Resource/Sprites/Player/sword/sword_combo_2.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Combo2");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Combo3", "Resource/Sprites/Player/sword/sword_combo_3.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Combo3");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Dash", "Resource/Sprites/Player/sword/sword_dash.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Dash");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Death", "Resource/Sprites/Player/sword/sword_death.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Death");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Hit", "Resource/Sprites/Player/sword/sword_hit.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Hit");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Idle", "Resource/Sprites/Player/sword/sword_idle.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Idle");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Jump", "Resource/Sprites/Player/sword/sword_jump.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Jump");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Run", "Resource/Sprites/Player/sword/sword_run.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Run");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Slide", "Resource/Sprites/Player/sword/sword_slide.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Slide");
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Walk", "Resource/Sprites/Player/sword/sword_walk.ktx2")) return false;
		AddRequiredResourceName("Sandbox2D.Player.Sword.Walk");

		/*=======================//
		//   font loading        //
		//=======================*/
		if (!MGSL_RESOURCE_MGR.RegisterResource<Framework::Font>("Sandbox2D.Font", "Resource/Fonts/ArchitectsDaughter-Regular.ttf", 48)) return false;
		AddRequiredResourceName("Sandbox2D.Font");

		return true;
	}

	void Sandbox2DScene::Build()
	{
		/* 게임 오브젝트와 컴포넌트를 생성하고 씬에 넣습니다. */

		/*===============================//
		//   loaded resource retrieval   //
		//===============================*/
		Framework::ShaderPtr spriteShader = MGSL_RESOURCE_MGR.GetResource<Framework::Shader>("Sandbox2D.SpriteShader"); if (!spriteShader) return;
		Framework::ShaderPtr postProcessingShader = MGSL_RESOURCE_MGR.GetResource<Framework::Shader>("Sandbox2D.PostProcessingShader"); if (!postProcessingShader) return;
		Framework::ShaderPtr uiImageShader = MGSL_RESOURCE_MGR.GetResource<Framework::Shader>("Sandbox2D.UIImageShader"); if (!uiImageShader) return;
		Framework::ShaderPtr uiTextShader = MGSL_RESOURCE_MGR.GetResource<Framework::Shader>("Sandbox2D.UITextShader");	 if (!uiTextShader) return;
		Framework::ShaderPtr debugGizmoShader = MGSL_RESOURCE_MGR.GetResource<Framework::Shader>("Sandbox2D.DebugGizmoShader"); if (!debugGizmoShader) return;
		Framework::FontPtr font = MGSL_RESOURCE_MGR.GetResource<Framework::Font>("Sandbox2D.Font");	 if (!font) return;
		Framework::Texture2DPtr background1 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Background.Background1"); if (!background1) return;
		Framework::Texture2DPtr background2 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Background.Background2"); if (!background2) return;
		Framework::Texture2DPtr backgroundMap = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Background.Map"); if (!backgroundMap) return;
		Framework::Texture2DPtr background3 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Props.Background3"); if (!background3) return;
		Framework::Texture2DPtr ladder = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Props.Ladder"); if (!ladder) return;
		Framework::Texture2DPtr bullet = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Props.Bullet"); if (!bullet) return;
		
		Framework::Texture2DPtr fighterIdle = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Idle"); if (!fighterIdle) return;
		Framework::Texture2DPtr fighterAirAttack = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.AirAttack"); if (!fighterAirAttack) return;
		Framework::Texture2DPtr fighterClimb = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Climb"); if (!fighterClimb) return;
		Framework::Texture2DPtr fighterCombo1 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Combo1"); if (!fighterCombo1) return;
		Framework::Texture2DPtr fighterCombo2 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Combo2"); if (!fighterCombo2) return;
		Framework::Texture2DPtr fighterCombo3 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Combo3"); if (!fighterCombo3) return;
		Framework::Texture2DPtr fighterDash = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Dash"); if (!fighterDash) return;
		Framework::Texture2DPtr fighterDeath = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Death"); if (!fighterDeath) return;
		Framework::Texture2DPtr fighterHit = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Hit"); if (!fighterHit) return;
		Framework::Texture2DPtr fighterJump = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Jump"); if (!fighterJump) return;
		Framework::Texture2DPtr fighterRun = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Run"); if (!fighterRun) return;
		Framework::Texture2DPtr fighterSlide = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Slide"); if (!fighterSlide) return;
		Framework::Texture2DPtr fighterWalk = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Walk"); if (!fighterWalk) return;

		Framework::Texture2DPtr pistolAirAttack = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.AirAttack"); if (!pistolAirAttack) return;
		Framework::Texture2DPtr pistolClimb = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Climb"); if (!pistolClimb) return;
		Framework::Texture2DPtr pistolDash = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Dash"); if (!pistolDash) return;
		Framework::Texture2DPtr pistolDeath = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Death"); if (!pistolDeath) return;
		Framework::Texture2DPtr pistolHit = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Hit"); if (!pistolHit) return;
		Framework::Texture2DPtr pistolIdle = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Idle"); if (!pistolIdle) return;
		Framework::Texture2DPtr pistolJump = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Jump"); if (!pistolJump) return;
		Framework::Texture2DPtr pistolRun = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Run"); if (!pistolRun) return;
		Framework::Texture2DPtr pistolShot = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Shot"); if (!pistolShot) return;
		Framework::Texture2DPtr pistolSlide = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Slide"); if (!pistolSlide) return;
		Framework::Texture2DPtr pistolWalk = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol.Walk"); if (!pistolWalk) return;

		Framework::Texture2DPtr swordAirAttack = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.AirAttack"); if (!swordAirAttack) return;
		Framework::Texture2DPtr swordClimb = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Climb"); if (!swordClimb) return;
		Framework::Texture2DPtr swordCombo1 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Combo1"); if (!swordCombo1) return;
		Framework::Texture2DPtr swordCombo2 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Combo2"); if (!swordCombo2) return;
		Framework::Texture2DPtr swordCombo3 = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Combo3"); if (!swordCombo3) return;
		Framework::Texture2DPtr swordDash = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Dash"); if (!swordDash) return;
		Framework::Texture2DPtr swordDeath = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Death"); if (!swordDeath) return;
		Framework::Texture2DPtr swordHit = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Hit"); if (!swordHit) return;
		Framework::Texture2DPtr swordIdle = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Idle"); if (!swordIdle) return;
		Framework::Texture2DPtr swordJump = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Jump"); if (!swordJump) return;
		Framework::Texture2DPtr swordRun = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Run"); if (!swordRun) return;
		Framework::Texture2DPtr swordSlide = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Slide"); if (!swordSlide) return;
		Framework::Texture2DPtr swordWalk = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword.Walk"); if (!swordWalk) return;

		/*===============================//
		//   render pipeline settings    //
		//===============================*/
		SetSpriteShader(spriteShader);
		SetPostProcessing2DShader(postProcessingShader);
		SetUIImageShader(uiImageShader);
		SetUITextShader(uiTextShader);
		SetDebugShader(debugGizmoShader);

		if (!FinalizeRenderSetting()) return;

		/*================================//
		//   Scene environment creation   //
		//================================*/
		// Background2
		Framework::GameObject* background2Object = MGSL_OBJECT_MGR.CreateGameObject(this); if (!background2Object) return;
		background2Object->GetTransform().SetPosition(Shared::vec3(7.5f, -0.2f, 0.7f));
		background2Object->GetTransform().SetScale(Shared::vec3(0.002f));
		Framework::SpriteRenderer* background2Renderer = MGSL_OBJECT_MGR.AddComponent<Framework::SpriteRenderer>(background2Object); if (!background2Renderer) return;
		background2Renderer->SetAtlasTexture(background2);
		background2Renderer->SetSize(Shared::vec2(9558.0f, 1090.0f));

		// Background1
		Framework::GameObject* background1Object = MGSL_OBJECT_MGR.CreateGameObject(this); if (!background1Object) return;
		background1Object->GetTransform().SetPosition(Shared::vec3(7.5f, -0.45f, 0.6f));
		background1Object->GetTransform().SetScale(Shared::vec3(0.002f));
		Framework::SpriteRenderer* background1Renderer = MGSL_OBJECT_MGR.AddComponent<Framework::SpriteRenderer>(background1Object); if (!background1Renderer) return;
		background1Renderer->SetAtlasTexture(background1);
		background1Renderer->SetSize(Shared::vec2(10320.0f, 1061.0f));

		// BackgroundMap
		Framework::GameObject* backgroundMapObject = MGSL_OBJECT_MGR.CreateGameObject(this); if (!backgroundMapObject) return;
		backgroundMapObject->GetTransform().SetPosition(Shared::vec3(7.5f, 1.5f, 0.5f));
		backgroundMapObject->GetTransform().SetScale(Shared::vec3(0.002f));
		Framework::SpriteRenderer* backgroundMapRenderer = MGSL_OBJECT_MGR.AddComponent<Framework::SpriteRenderer>(backgroundMapObject); if (!backgroundMapRenderer) return;
		backgroundMapRenderer->SetAtlasTexture(backgroundMap);
		backgroundMapRenderer->SetSize(Shared::vec2(7679.0f, 1817.0f));

		// Ground Collider
		Framework::GameObject* groundColliderObject = MGSL_OBJECT_MGR.CreateGameObject(this);
		if (!groundColliderObject) return;
		groundColliderObject->GetTransform().SetPosition(Shared::vec3(7.5f, -1.45f, 0.0f));
		Framework::BoxCollider* groundCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(groundColliderObject);
		groundCollider->SetDebugSize(Shared::vec2(30.0f, 0.35f));
		MGSL_COLLIDE_MGR.Register(groundCollider);

		// Left-wall Collider
		Framework::GameObject* leftWallObject = MGSL_OBJECT_MGR.CreateGameObject(this);
		if (!leftWallObject) return;
		leftWallObject->GetTransform().SetPosition(Shared::vec3(-6.25f, 0.5f, 0.0f));
		Framework::BoxCollider* leftWallCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(leftWallObject);
		if (!leftWallCollider) return;
		leftWallCollider->SetDebugSize(Shared::vec2(0.3f, 4.0f));
		MGSL_COLLIDE_MGR.Register(leftWallCollider);

		// Right-wall Collider
		Framework::GameObject* rightWallObject = MGSL_OBJECT_MGR.CreateGameObject(this);
		if (!rightWallObject) return;
		rightWallObject->GetTransform().SetPosition(Shared::vec3(21.5f, 0.5f, 0.0f));
		Framework::BoxCollider* rightWallCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(rightWallObject);
		if (!rightWallCollider) return;
		rightWallCollider->SetDebugSize(Shared::vec2(0.3f, 4.0f));
		MGSL_COLLIDE_MGR.Register(rightWallCollider);

		// Platform : building1
		Framework::GameObject* building1RoofObject = MGSL_OBJECT_MGR.CreateGameObject(this);
		if (!building1RoofObject) return;
		building1RoofObject->GetTransform().SetPosition(Shared::vec3(-2.05f, -0.08f, 0.0f));
		Framework::BoxCollider* building1RoofCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(building1RoofObject); if (!building1RoofCollider) return;
		building1RoofCollider->SetDebugSize(Shared::vec2(2.0f, 0.125f));
		MGSL_COLLIDE_MGR.Register(building1RoofCollider);

		// Platform : building2
		Framework::GameObject* building2RoofObject = MGSL_OBJECT_MGR.CreateGameObject(this);
		if (!building2RoofObject) return;
		building2RoofObject->GetTransform().SetPosition(Shared::vec3(1.45f, 0.35f, 0.0f));
		Framework::BoxCollider* building2RoofCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(building2RoofObject); if (!building2RoofCollider) return;
		building2RoofCollider->SetDebugSize(Shared::vec2(2.65f, 0.2f));
		MGSL_COLLIDE_MGR.Register(building2RoofCollider);

		// Ladder #1
		Framework::GameObject* ladderObject = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ladderObject) return;
		ladderObject->GetTransform().SetPosition(Shared::vec3(0.45f, -0.4f, 0.0f));
		ladderObject->GetTransform().SetScale(Shared::vec3(0.55f, 0.425f, 0.65f));
		Framework::SpriteRenderer* ladderRenderer = MGSL_OBJECT_MGR.AddComponent<Framework::SpriteRenderer>(ladderObject); if (!ladderRenderer) return;
		ladderRenderer->SetAtlasTexture(ladder);
		ladderRenderer->SetSize(Shared::vec2(0.55f, 1.8f));
		Framework::BoxCollider* ladderCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(ladderObject); if (!ladderCollider) return;
		ladderCollider->SetTrigger(true);
		ladderCollider->SetDebugSize(Shared::vec2(0.65f, 3.95f));
		ladderCollider->SetDebugOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_COLLIDE_MGR.Register(ladderCollider);

		// Platform : building3
		Framework::GameObject* building3RoofObject = MGSL_OBJECT_MGR.CreateGameObject(this);
		if (!building3RoofObject) return;
		building3RoofObject->GetTransform().SetPosition(Shared::vec3(6.0f, 0.2f, 0.0f));
		Framework::BoxCollider* building3RoofCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(building3RoofObject); if (!building3RoofCollider) return;
		building3RoofCollider->SetDebugSize(Shared::vec2(1.85f, 0.185f));
		MGSL_COLLIDE_MGR.Register(building3RoofCollider);

		// Ladder #2
		Framework::GameObject* ladderObject2 = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ladderObject2) return;
		ladderObject2->GetTransform().SetPosition(Shared::vec3(6.5f, -0.5f, 0.0f));
		ladderObject2->GetTransform().SetScale(Shared::vec3(0.45f, 0.4f, 0.65f));
		Framework::SpriteRenderer* ladderRenderer2 = MGSL_OBJECT_MGR.AddComponent<Framework::SpriteRenderer>(ladderObject2); if (!ladderRenderer2) return;
		ladderRenderer2->SetAtlasTexture(ladder);
		ladderRenderer2->SetSize(Shared::vec2(0.55f, 1.8f));
		Framework::BoxCollider* ladderCollider2 = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(ladderObject2); if (!ladderCollider2) return;
		ladderCollider2->SetTrigger(true);
		ladderCollider2->SetDebugSize(Shared::vec2(0.65f, 3.95f));
		ladderCollider2->SetDebugOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_COLLIDE_MGR.Register(ladderCollider2);

		// Platform : building5
		Framework::GameObject* building5RoofObject = MGSL_OBJECT_MGR.CreateGameObject(this);
		if (!building5RoofObject) return;
		building5RoofObject->GetTransform().SetPosition(Shared::vec3(12.75f, 0.5f, 0.0f));
		Framework::BoxCollider* building5RoofCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(building5RoofObject); if (!building5RoofCollider) return;
		building5RoofCollider->SetDebugSize(Shared::vec2(4.2f, 0.185f));
		MGSL_COLLIDE_MGR.Register(building5RoofCollider);

		// Ladder #3
		Framework::GameObject* ladderObject3 = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ladderObject3) return;
		ladderObject3->GetTransform().SetPosition(Shared::vec3(11.0f, -0.35f, 0.0f));
		ladderObject3->GetTransform().SetScale(Shared::vec3(0.65f, 0.5f, 0.65f));
		Framework::SpriteRenderer* ladderRenderer3 = MGSL_OBJECT_MGR.AddComponent<Framework::SpriteRenderer>(ladderObject3); if (!ladderRenderer3) return;
		ladderRenderer3->SetAtlasTexture(ladder);
		ladderRenderer3->SetSize(Shared::vec2(0.55f, 1.8f));
		Framework::BoxCollider* ladderCollider3 = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(ladderObject3); if (!ladderCollider3) return;
		ladderCollider3->SetTrigger(true);
		ladderCollider3->SetDebugSize(Shared::vec2(0.65f, 3.8f));
		ladderCollider3->SetDebugOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_COLLIDE_MGR.Register(ladderCollider3);

		// Platform : building6
		Framework::GameObject* building6RoofObject = MGSL_OBJECT_MGR.CreateGameObject(this);
		if (!building6RoofObject) return;
		building6RoofObject->GetTransform().SetPosition(Shared::vec3(17.9f, 0.28f, 0.0f));
		Framework::BoxCollider* building6RoofCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(building6RoofObject); if (!building6RoofCollider) return;
		building6RoofCollider->SetDebugSize(Shared::vec2(2.80f, 0.185f));
		MGSL_COLLIDE_MGR.Register(building6RoofCollider);

		// Ladder #3
		Framework::GameObject* ladderObject4 = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ladderObject4) return;
		ladderObject4->GetTransform().SetPosition(Shared::vec3(17.0f, -0.45f, 0.0f));
		ladderObject4->GetTransform().SetScale(Shared::vec3(0.45f, 0.4f, 0.45f));
		Framework::SpriteRenderer* ladderRenderer4 = MGSL_OBJECT_MGR.AddComponent<Framework::SpriteRenderer>(ladderObject4); if (!ladderRenderer4) return;
		ladderRenderer4->SetAtlasTexture(ladder);
		ladderRenderer4->SetSize(Shared::vec2(0.55f, 1.8f));
		Framework::BoxCollider* ladderCollider4 = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(ladderObject4); if (!ladderCollider4) return;
		ladderCollider4->SetTrigger(true);
		ladderCollider4->SetDebugSize(Shared::vec2(0.65f, 3.95f));
		ladderCollider4->SetDebugOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_COLLIDE_MGR.Register(ladderCollider4);

		/*=============================//
		//   Scene MyPlayer creation   //
		//=============================*/
		Framework::GameObject* player = MGSL_OBJECT_MGR.CreateGameObject(this); if (!player) return;
		player->GetTransform().SetPosition(Shared::vec3(0.0f, 0.0f, 0.0f));
		player->GetTransform().SetScale(Shared::vec3(1.25f));
		Framework::Texture2DPtr fighterIdleTex = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Idle"); if (!fighterIdleTex) return;
		Framework::FlipbookControllerPtr flipbookController = Framework::FlipbookController::Create(fighterIdleTex); if (!flipbookController) return;
		Shared::List<Shared::vec4> idleFrames =
		{
			{ 0.000f, 0.0f, 0.125f, 1.0f },
			{ 0.125f, 0.0f, 0.125f, 1.0f },
			{ 0.250f, 0.0f, 0.125f, 1.0f },
			{ 0.375f, 0.0f, 0.125f, 1.0f },
			{ 0.500f, 0.0f, 0.125f, 1.0f },
			{ 0.625f, 0.0f, 0.125f, 1.0f },
			{ 0.750f, 0.0f, 0.125f, 1.0f },
			{ 0.875f, 0.0f, 0.125f, 1.0f }
		};
		Framework::FlipbookClipPtr idleClip = Framework::FlipbookClip::Create(idleFrames, 16.0f); if (!idleClip) return;
		if (!flipbookController->SetClip(0, idleClip)) return;
		Framework::FlipbookPlayer* flipbookPlayer = MGSL_OBJECT_MGR.AddComponent<Framework::FlipbookPlayer>(player, flipbookController); if (!flipbookPlayer) return;
		if (!flipbookPlayer->SetState(0)) return;
		flipbookPlayer->SetSize(Shared::vec2(1.0f, 1.0f));
		flipbookPlayer->Play();

		// BoxCollider
		Framework::BoxCollider* playerCollider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(player);
		if (!playerCollider) return;
		playerCollider->SetDebugSize(Shared::vec2(0.3f, 0.6f));
		playerCollider->SetDebugOffset(Shared::vec2(0.0f, -0.2f));
		MGSL_COLLIDE_MGR.Register(playerCollider);

		// PlayerController
		MyPlayerController* playerController = MGSL_OBJECT_MGR.AddComponent<MyPlayerController>(player);

		// CharacterBody2D
		Framework::CharacterBody2D* characterBody = MGSL_OBJECT_MGR.AddComponent<Framework::CharacterBody2D>(player);
		if (!characterBody) return;
		characterBody->SetGravity(-11.0f);

		/*===========================//
		//   Scene camera creation   //
		//===========================*/
		Framework::GameObject* cameraObject = MGSL_OBJECT_MGR.CreateGameObject(this); if (!cameraObject) return;
		Framework::Camera* camera = MGSL_OBJECT_MGR.AddComponent<Framework::Camera>(cameraObject); if (!camera) return;
		cameraObject->GetTransform().SetPosition(Shared::vec3(0.0f, 0.0f, 0.0f));
		camera->SetOrthographic(4.0f, 1280.0f / 720.0f, -100.0f, 100.0f);
		SetMainCamera(camera);
		CameraController* cameraController = MGSL_OBJECT_MGR.AddComponent<CameraController>(cameraObject); if (!cameraController) return;
		cameraController->SetTarget(player);

		Framework::GameObject* uiCameraObject = MGSL_OBJECT_MGR.CreateGameObject(this); if (!uiCameraObject) return;
		Framework::Camera* uiCamera = MGSL_OBJECT_MGR.AddComponent<Framework::Camera>(uiCameraObject); if (!uiCamera) return;
		uiCameraObject->GetTransform().SetPosition(Shared::vec3(0.0f, 0.0f, 0.0f));
		const Shared::vec2& referenceResolution = MGSL_UI_MGR.GetReferenceResolution();
		const float aspect = referenceResolution.x / referenceResolution.y;
		uiCamera->SetOrthographic(referenceResolution.y, aspect, -100.0f, 100.0f);
		SetUICamera(uiCamera);
	}
}
