#pragma once

namespace MGSL::Framework
{
	enum class EInputState : Shared::uint8
	{
		NONE,
		DOWN,
		PRESS,
		UP
	};

	enum class EMouseButton : Shared::uint8
	{
		LEFT = 0,
		RIGHT,
		MIDDLE,
		COUNT
	};

	enum class EResourceType : Shared::uint8
	{
		RESOURCE,
		GPU_RESOURCE
	};

	enum class EResourceState : Shared::uint8
	{
		UNLOADED,
		LOADING,
		READY,
		FAILED
	};

	enum class ESceneState : Shared::uint8
	{
		NONE,
		LOADING,
		RUNNING,
		FAILED
	};

	enum class EUniformBinding : Shared::uint8
	{
		CAMERA,
		UI,
		POST_PROCESSING
	};

	enum class EUIButtonState : Shared::uint8
	{
		NORMAL,
		HOVERED,
		PRESSED,
		DISABLED
	};

	enum class ECollisionEventState
	{
		ENTER,
		STAY,
		EXIT
	};

	enum class EColliderMobility
	{
		STATIC,
		DYNAMIC
	};

	enum class ECollisionType
	{
		SOLID,
		PLATFORM
	};
}