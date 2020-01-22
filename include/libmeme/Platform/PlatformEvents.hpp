#ifndef _ML_PLATFORM_EVENTS_HPP_
#define _ML_PLATFORM_EVENTS_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Event.hpp>
#include <libmeme/Platform/KeyCode.hpp>
#include <libmeme/Platform/MouseButton.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Input States
#define ML_KEY_RELEASE	0 // High	-> Low
#define ML_KEY_PRESS	1 // Low	-> High
#define ML_KEY_REPEAT	2 // High	-> High

// Keyboard Input Modifiers
#define ML_MOD_SHIFT	(0 << 1)
#define ML_MOD_CTRL		(1 << 1)
#define ML_MOD_ALT		(1 << 2)
#define ML_MOD_SUPER	(1 << 3)
#define ML_MOD_CAPSLOCK	(1 << 4)
#define ML_MOD_NUMLOCK	(1 << 5)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct char_event final : I_event<char_event>
	{
		uint32_t const value;

		constexpr char_event(uint32_t value) noexcept
			: value{ value }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct cursor_enter_event final : I_event<cursor_enter_event>
	{
		int32_t const entered;

		constexpr cursor_enter_event(int32_t entered) noexcept
			: entered{ entered }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct cursor_pos_event final : I_event<cursor_pos_event>
	{
		float64_t const x;
		float64_t const y;

		constexpr cursor_pos_event(float64_t x, float64_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct key_event final : I_event<key_event>
	{
		key_code const	key;
		int32_t const	scan;
		int32_t const	action;
		int32_t const	mods;

		constexpr key_event(int32_t key, int32_t scan, int32_t action, int32_t mods) noexcept
			: key	{ (key_code)key }
			, scan	{ scan }
			, action{ action }
			, mods	{ mods }
		{
		}

		constexpr bool getKeyAction(key_code k, int32_t a) const
		{
			return (key == k) && (action == a);
		}

		constexpr bool getPress(key_code k) const { return getKeyAction(k, ML_KEY_PRESS); }
		constexpr bool getDown(key_code k) const { return getKeyAction(k, ML_KEY_REPEAT); }
		constexpr bool getUp(key_code k) const { return getKeyAction(k, ML_KEY_RELEASE); }

		constexpr bool getPress(key_code k, int32_t m) const { return getPress(k) && (mods == m); }
		constexpr bool getDown(key_code k, int32_t m) const { return getDown(k) && (mods == m); }
		constexpr bool getUp(key_code k, int32_t m) const { return getUp(k) && (mods == m); }

		constexpr bool isShift(key_code k) const { return getPress(k, KeyMods_Shift); }
		constexpr bool isCtrl(key_code k) const { return getPress(k, KeyMods_Ctrl); }
		constexpr bool isAlt(key_code k) const { return getPress(k, KeyMods_Alt); }
		constexpr bool isSuper(key_code k) const { return getPress(k, KeyMods_Super); }

		constexpr bool isNew() const { return isCtrl(key_code::N); }
		constexpr bool isOpen() const { return isCtrl(key_code::O); }
		constexpr bool isSave() const { return isCtrl(key_code::S) || getPress(key_code::S, KeyMods_Ctrl | KeyMods_Shift); }
		constexpr bool isUndo() const { return isCtrl(key_code::Z); }
		constexpr bool isRedo() const { return isCtrl(key_code::Y) || getPress(key_code::Z, KeyMods_Ctrl | KeyMods_Shift); }
		constexpr bool isCut() const { return isCtrl(key_code::X) || isShift(key_code::Delete); }
		constexpr bool isCopy() const { return isCtrl(key_code::C) || isCtrl(key_code::Insert); }
		constexpr bool isPaste() const { return isCtrl(key_code::V) || isShift(key_code::Insert); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct mouse_event final : I_event<mouse_event>
	{
		int32_t const key;
		int32_t const action;
		int32_t const mods;

		constexpr mouse_event(int32_t key, int32_t action, int32_t mods) noexcept
			: key{ key }
			, action{ action }
			, mods{ mods }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct scroll_event final : I_event<scroll_event>
	{
		float64_t const x;
		float64_t const y;

		constexpr scroll_event(float64_t x, float64_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct frame_size_event final : I_event<frame_size_event>
	{
		int32_t const width;
		int32_t const height;

		constexpr frame_size_event(int32_t width, int32_t height) noexcept
			: width{ width }
			, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_close_event final : I_event<window_close_event>
	{
		window_close_event() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_error_event final : I_event<window_error_event>
	{
		int32_t const code;
		C_string const desc;

		constexpr window_error_event(int32_t code, C_string desc) noexcept
			: code{ code }
			, desc{ desc }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_focus_event final : I_event<window_focus_event>
	{
		int32_t const focused;
		
		constexpr window_focus_event(int32_t focused) noexcept
			: focused{ focused }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_pos_event final : I_event<window_pos_event>
	{
		int32_t const x;
		int32_t const y;

		constexpr window_pos_event(int32_t x, int32_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_size_event final : I_event<window_size_event>
	{
		int32_t const width;
		int32_t const height;

		constexpr window_size_event(int32_t width, int32_t height) noexcept
			: width{ width }
			, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLATFORM_EVENTS_HPP_