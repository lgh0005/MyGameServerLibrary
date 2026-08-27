#include "ServerPch.h"
#include "PrefabUtils.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "VirtualScene.h"

#include "BulletController.h"

namespace MGSL::Server
{
	GameObject* PrefabUtils::CreateBullet(VirtualScene* scene, const ::Protobuf::BulletInfo& info)
	{
		if (!scene) return nullptr;

		/*========================//
		//      Game Object       //
		//========================*/
		GameObject* bulletObject = MGSL_OBJECT_MGR.CreateGameObject(scene); if (!bulletObject) return nullptr;
		bulletObject->GetTransform().SetPosition(Shared::vec3(info.position().x(), info.position().y(), 0.0f));

		/*========================//
		//        Collider        //
		//========================*/
		BoxCollider* collider = MGSL_OBJECT_MGR.AddComponent<BoxCollider>(bulletObject);
		if (!collider) return nullptr;
		collider->SetMobility(EColliderMobility::DYNAMIC);
		collider->SetCollisionLayer(ECollisionLayer::BULLET);
		collider->SetTrigger(true);
		collider->SetSize(Shared::vec2(0.20f, 0.10f));
		MGSL_SERVER_COLLISION_MGR.Register(collider);

		/*========================//
		//    Bullet Controller   //
		//========================*/
		BulletController* bulletController = MGSL_OBJECT_MGR.AddComponent<BulletController>(bulletObject); if (!bulletController) return nullptr;
		bulletController->SetInfo(info);

		return bulletObject;
	}
}