#include "ServerPch.h"
#include "VirtualSandbox2DScene.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "BoxCollider.h"

namespace MGSL::Server
{
	VirtualSandbox2DScene::VirtualSandbox2DScene() = default;
	VirtualSandbox2DScene::~VirtualSandbox2DScene() = default;

	void VirtualSandbox2DScene::Build()
	{
		/*==========================================//
		//   game map's logical collider positions  //
		//==========================================*/
		// ground
		GameObject* ground = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ground) return;
		ground->GetTransform().SetPosition(Shared::vec3(7.5f, -1.45f, 0.0f));
		BoxCollider* groundCollider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(ground); if (!groundCollider) return;
		groundCollider->SetMobility(EColliderMobility::STATIC);
		groundCollider->SetCollisionLayer(ECollisionLayer::WORLD);
		groundCollider->SetSize(Shared::vec2(30.0f, 0.35f));
		MGSL_SERVER_COLLISION_MGR.Register(groundCollider);

		// left-wall
		GameObject* leftWall = MGSL_OBJECT_MGR.CreateGameObject(this); if (!leftWall) return;
		leftWall->GetTransform().SetPosition(Shared::vec3(-6.25f, 0.5f, 0.0f));
		BoxCollider* leftWallCollider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(leftWall); if (!leftWallCollider) return;
		leftWallCollider->SetMobility(EColliderMobility::STATIC);
		leftWallCollider->SetCollisionLayer(ECollisionLayer::WORLD);
		leftWallCollider->SetSize(Shared::vec2(0.3f, 4.0f));
		MGSL_SERVER_COLLISION_MGR.Register(leftWallCollider);

		// right-wall
		GameObject* rightWall = MGSL_OBJECT_MGR.CreateGameObject(this); if (!rightWall) return;
		rightWall->GetTransform().SetPosition(Shared::vec3(21.5f, 0.5f, 0.0f));
		BoxCollider* rightWallCollider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(rightWall); if (!rightWallCollider) return;
		rightWallCollider->SetMobility(EColliderMobility::STATIC);
		rightWallCollider->SetCollisionLayer(ECollisionLayer::WORLD);
		rightWallCollider->SetSize(Shared::vec2(0.3f, 4.0f));
		MGSL_SERVER_COLLISION_MGR.Register(rightWallCollider);

		// Platform : building1
		GameObject* building1Roof = MGSL_OBJECT_MGR.CreateGameObject(this); if (!building1Roof) return;
		building1Roof->GetTransform().SetPosition(Shared::vec3(-2.05f, -0.08f, 0.0f));
		BoxCollider* building1RoofCollider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(building1Roof); if (!building1RoofCollider) return;
		building1RoofCollider->SetMobility(EColliderMobility::STATIC);
		building1RoofCollider->SetCollisionType(ECollisionType::PLATFORM);
		building1RoofCollider->SetCollisionLayer(ECollisionLayer::WORLD);
		building1RoofCollider->SetSize(Shared::vec2(2.0f, 0.125f));
		MGSL_SERVER_COLLISION_MGR.Register(building1RoofCollider);

		// Platform : building2
		GameObject* building2Roof = MGSL_OBJECT_MGR.CreateGameObject(this); if (!building2Roof) return;
		building2Roof->GetTransform().SetPosition(Shared::vec3(1.45f, 0.35f, 0.0f));
		BoxCollider* building2RoofCollider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(building2Roof); if (!building2RoofCollider) return;
		building2RoofCollider->SetMobility(EColliderMobility::STATIC);
		building2RoofCollider->SetCollisionType(ECollisionType::PLATFORM);
		building2RoofCollider->SetCollisionLayer(ECollisionLayer::WORLD);
		building2RoofCollider->SetSize(Shared::vec2(2.65f, 0.2f));
		MGSL_SERVER_COLLISION_MGR.Register(building2RoofCollider);

		// Ladder #1
		GameObject* ladder1 = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ladder1) return;
		ladder1->GetTransform().SetPosition(Shared::vec3(0.45f, -0.4f, 0.0f));
		BoxCollider* ladder1Collider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(ladder1); if (!ladder1Collider) return;
		ladder1Collider->SetMobility(EColliderMobility::STATIC);
		ladder1Collider->SetCollisionLayer(ECollisionLayer::LADDER);
		ladder1Collider->SetTrigger(true);
		ladder1Collider->SetSize(Shared::vec2(0.65f, 3.95f));
		ladder1Collider->SetOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_SERVER_COLLISION_MGR.Register(ladder1Collider);

		// Platform : building3
		GameObject* building3Roof = MGSL_OBJECT_MGR.CreateGameObject(this); if (!building3Roof) return;
		building3Roof->GetTransform().SetPosition(Shared::vec3(6.0f, 0.2f, 0.0f));
		BoxCollider* building3RoofCollider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(building3Roof); if (!building3RoofCollider) return;
		building3RoofCollider->SetMobility(EColliderMobility::STATIC);
		building3RoofCollider->SetCollisionType(ECollisionType::PLATFORM);
		building3RoofCollider->SetCollisionLayer(ECollisionLayer::WORLD);
		building3RoofCollider->SetSize(Shared::vec2(1.85f, 0.185f));
		MGSL_SERVER_COLLISION_MGR.Register(building3RoofCollider);

		// Ladder #2
		GameObject* ladder2 = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ladder2) return;
		ladder2->GetTransform().SetPosition(Shared::vec3(6.5f, -0.5f, 0.0f));
		BoxCollider* ladder2Collider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(ladder2); if (!ladder2Collider) return;
		ladder2Collider->SetMobility(EColliderMobility::STATIC);
		ladder2Collider->SetCollisionLayer(ECollisionLayer::LADDER);
		ladder2Collider->SetTrigger(true);
		ladder2Collider->SetSize(Shared::vec2(0.65f, 3.95f));
		ladder2Collider->SetOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_SERVER_COLLISION_MGR.Register(ladder2Collider);

		// Platform : building5
		GameObject* building5Roof = MGSL_OBJECT_MGR.CreateGameObject(this); if (!building5Roof) return;
		building5Roof->GetTransform().SetPosition(Shared::vec3(12.75f, 0.5f, 0.0f));
		BoxCollider* building5RoofCollider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(building5Roof); if (!building5RoofCollider) return;
		building5RoofCollider->SetMobility(EColliderMobility::STATIC);
		building5RoofCollider->SetCollisionType(ECollisionType::PLATFORM);
		building5RoofCollider->SetCollisionLayer(ECollisionLayer::WORLD);
		building5RoofCollider->SetSize(Shared::vec2(4.2f, 0.185f));
		MGSL_SERVER_COLLISION_MGR.Register(building5RoofCollider);

		// Ladder #3
		GameObject* ladder3 = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ladder3) return;
		ladder3->GetTransform().SetPosition(Shared::vec3(11.0f, -0.35f, 0.0f));
		BoxCollider* ladder3Collider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(ladder3); if (!ladder3Collider) return;
		ladder3Collider->SetMobility(EColliderMobility::STATIC);
		ladder3Collider->SetCollisionLayer(ECollisionLayer::LADDER);
		ladder3Collider->SetTrigger(true);
		ladder3Collider->SetSize(Shared::vec2(0.65f, 3.8f));
		ladder3Collider->SetOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_SERVER_COLLISION_MGR.Register(ladder3Collider);

		// Platform : building6
		GameObject* building6Roof = MGSL_OBJECT_MGR.CreateGameObject(this); if (!building6Roof) return;
		building6Roof->GetTransform().SetPosition(Shared::vec3(17.9f, 0.28f, 0.0f));
		BoxCollider* building6RoofCollider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(building6Roof); if (!building6RoofCollider) return;
		building6RoofCollider->SetMobility(EColliderMobility::STATIC);
		building6RoofCollider->SetCollisionType(ECollisionType::PLATFORM);
		building6RoofCollider->SetCollisionLayer(ECollisionLayer::WORLD);
		building6RoofCollider->SetSize(Shared::vec2(2.8f, 0.185f));
		MGSL_SERVER_COLLISION_MGR.Register(building6RoofCollider);

		// Ladder #4
		GameObject* ladder4 = MGSL_OBJECT_MGR.CreateGameObject(this); if (!ladder4) return;
		ladder4->GetTransform().SetPosition(Shared::vec3(17.0f, -0.45f, 0.0f));
		BoxCollider* ladder4Collider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(ladder4); if (!ladder4Collider) return;
		ladder4Collider->SetMobility(EColliderMobility::STATIC);
		ladder4Collider->SetCollisionLayer(ECollisionLayer::LADDER);
		ladder4Collider->SetTrigger(true);
		ladder4Collider->SetSize(Shared::vec2(0.65f, 3.95f));
		ladder4Collider->SetOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_SERVER_COLLISION_MGR.Register(ladder4Collider);
	}
}
