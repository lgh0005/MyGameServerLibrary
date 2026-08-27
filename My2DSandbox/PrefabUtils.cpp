#include "2DPch.h"
#include "PrefabUtils.h"
#include "MyGameFramework/GameObject.h"
#include "MyGameFramework/Scene.h"
#include "MyGameFramework/Transform.h"
#include "MyGameFramework/SpriteRenderer.h"
#include "MyGameFramework/FlipbookPlayer.h"
#include "MyGameFramework/Texture2D.h"
#include "MyGameFramework/FlipbookController.h"
#include "MyGameFramework/BoxCollider.h"

#include "BulletNetworkState.h"
#include "AttackEffect.h"

namespace MGSL::Sandbox2D
{
	Framework::GameObject* PrefabUtils::CreateBullet(Framework::Scene* scene, const ::Protobuf::BulletInfo& info)
	{
		if (!scene) return nullptr;

		/*========================//
		//      Game Object       //
		//========================*/
		Framework::GameObject* bulletObject = MGSL_OBJECT_MGR.CreateGameObject(scene); if (!bulletObject) return nullptr;
		bulletObject->GetTransform().SetPosition(Shared::vec3(info.position().x(), info.position().y(), 0.0f));

		/*========================//
		//     Sprite Renderer    //
		//========================*/
		Framework::SpriteRenderer* renderer = MGSL_OBJECT_MGR.AddComponent<Framework::SpriteRenderer>(bulletObject); if (!renderer) return nullptr;
		Framework::Texture2DPtr bulletTexture =MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Props.Bullet"); if (!bulletTexture) return nullptr;
		renderer->SetAtlasTexture(bulletTexture);
		renderer->SetSize(Shared::vec2(0.10f, 0.05f));
		Shared::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		if (info.facing() == ::Protobuf::FACING_TYPE_LEFT) { uvRect.x += uvRect.z; uvRect.z *= -1.0f; }
		renderer->SetUVRect(uvRect);

		/*========================//
		//        Collider        //
		//========================*/
		Framework::BoxCollider* collider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(bulletObject);  if (!collider) return nullptr;
		collider->SetTrigger(true);
		collider->SetDebugSize(Shared::vec2(0.20f, 0.10f));
		MGSL_COLLIDE_MGR.Register(collider);

		/*========================//
		//         Bullet         //
		//========================*/
		BulletNetworkState* bullet = MGSL_OBJECT_MGR.AddComponent<BulletNetworkState>(bulletObject); if (!bullet) return nullptr;
		bullet->SetInfo(info);

		return bulletObject;
	}

	Framework::GameObject* PrefabUtils::CreateAttackEffect
	(
		Framework::Scene* scene,
		const Shared::vec3& position,
		::Protobuf::FACING_TYPE facing
	)
	{
		if (!scene) return nullptr;

		/*========================//
		//      Game Object       //
		//========================*/
		Framework::GameObject* effectObject = MGSL_OBJECT_MGR.CreateGameObject(scene); if (!effectObject) return nullptr;
		effectObject->GetTransform().SetPosition(position);

		/*========================//
		//      Effect Atlas      //
		//========================*/
		Framework::Texture2DPtr effectAtlas = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Effect.HitEffect"); if (!effectAtlas) return nullptr;
		Framework::FlipbookControllerPtr effectController = Framework::FlipbookController::Create(effectAtlas); if (!effectController) return nullptr;
		Framework::FlipbookPlayer* flipbook = MGSL_OBJECT_MGR.AddComponent<Framework::FlipbookPlayer>(effectObject); if (!flipbook) return nullptr;

		constexpr Shared::uint32 controllerIndex = 0;
		if (!flipbook->SetController(controllerIndex, effectController)) return nullptr;
		if (!flipbook->ChangeController(controllerIndex)) return nullptr;
		flipbook->SetFlipX(facing == ::Protobuf::FACING_TYPE_LEFT);

		/*========================//
		//		   Effect         //
		//========================*/
		// 이후 EffectLifeTime 같은 컴포넌트를 만들면:
		//
		// EffectLifeTime* lifeTime =
		//     MGSL_OBJECT_MGR.AddComponent<EffectLifeTime>(
		//         effectObject);
		//
		// if (!lifeTime)
		//     return nullptr;
		//
		// lifeTime->SetDuration(0.25f);

		return effectObject;
	}
}
