#pragma once
#include "IInputDevice.h"

namespace MGSL::Framework
{
	class KeyboardDevice : public IInputDevice
	{
		using Super = IInputDevice;

	public:
		explicit KeyboardDevice(HWND hwnd);
		virtual ~KeyboardDevice();
		static constexpr Shared::usize KEY_COUNT = 256;

	public:
		virtual void Update(const Shared::uint8* keyStates) override;
		virtual void Clear() override;

	public:
		bool GetKeyDown(Shared::int32 vk) const { return m_states[vk] == EInputState::DOWN; }
		bool GetKeyPress(Shared::int32 vk) const { return m_states[vk] == EInputState::PRESS; }
		bool GetKeyUp(Shared::int32 vk) const { return m_states[vk] == EInputState::UP; }

	private:
		Shared::Array<EInputState, KEY_COUNT> m_states { };
		Shared::Array<bool, KEY_COUNT> m_prevStates { };
	};
}
