#include "2DPch.h"
#include "AttackEffect.h"
#include "MyGameFramework/GameObject.h"
#include "MyGameFramework/FlipbookPlayer.h"

namespace MGSL::Sandbox2D
{
	AttackEffect::AttackEffect(Framework::GameObject* owner) : Super(owner) { }
	AttackEffect::~AttackEffect() = default;

	AttackEffectUPtr AttackEffect::Create(Framework::GameObject* owner)
	{
		return AttackEffectUPtr(new AttackEffect(owner));
	}

	void AttackEffect::Update(float deltaTime)
	{
		Framework::GameObject* owner = GetOwner();
		if (!owner) return;

		Framework::FlipbookPlayer* flipbook =
			owner->GetComponent<Framework::FlipbookPlayer>();

		if (!flipbook) return;

		// Effect 애니메이션이 끝나면 GameObject 삭제
		if (!flipbook->IsPlaying())
			MGSL_OBJECT_MGR.RemoveGameObject(owner);
	}
}