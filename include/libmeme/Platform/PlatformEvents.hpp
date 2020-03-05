#ifndef _ML_PLATFORM_EVENTS_HPP_
#define _ML_PLATFORM_EVENTS_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Core/Event.hpp>
#include <libmeme/Platform/KeyCode.hpp>
#include <libmeme/Platform/MouseButton.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Input States
#define ML_KEY_RELEASE	0 // High -> Low
#define ML_KEY_PRESS	1 // Low  -> High
#define ML_KEY_REPEAT	2 // High -> High

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

	struct char_event final : T_event<char_event>
	{
		uint32_t const value;

		constexpr char_event(uint32_t value) noexcept
			: value{ value }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct cursor_enter_event final : T_event<cursor_enter_event>
	{
		int32_t const entered;

		constexpr cursor_enter_event(int32_t entered) noexcept
			: entered{ entered }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct cursor_pos_event final : T_event<cursor_pos_event>
	{
		float64_t const x, y;

		constexpr cursor_pos_event(float64_t x, float64_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct key_event final : T_event<key_event>
	{
		int32_t const key, scan, action, mods;

		constexpr key_event(int32_t key, int32_t scan, int32_t action, int32_t mods) noexcept
			: key	{ key }
			, scan	{ scan }
			, action{ action }
			, mods	{ mods }
		{
		}

		constexpr bool is_press(int32_t k) const { return (key == k) && (action == ML_KEY_PRESS); }
		constexpr bool is_repeat(int32_t k) const { return (key == k) && (action == ML_KEY_REPEAT); }
		constexpr bool is_release(int32_t k) const { return (key == k) && (action == ML_KEY_RELEASE); }

		constexpr bool is_press(int32_t k, int32_t m) const { return is_press(k) && (mods == m); }
		constexpr bool is_repeat(int32_t k, int32_t m) const { return is_repeat(k) && (mods == m); }
		constexpr bool is_release(int32_t k, int32_t m) const { return is_release(k) && (mods == m); }

		constexpr bool is_shift	(int32_t k) const { return is_press(k, KeyMods_Shift); }
		constexpr bool is_ctrl	(int32_t k) const { return is_press(k, KeyMods_Ctrl); }
		constexpr bool is_alt	(int32_t k) const { return is_press(k, KeyMods_Alt); }
		constexpr bool is_super	(int32_t k) const { return is_press(k, KeyMods_Super); }

		constexpr bool is_new	() const { return is_ctrl(key_code::N); }
		constexpr bool is_open	() const { return is_ctrl(key_code::O); }
		constexpr bool is_save	() const { return is_ctrl(key_code::S) || is_press(key_code::S, KeyMods_Ctrl | KeyMods_Shift); }
		constexpr bool is_undo	() const { return is_ctrl(key_code::Z); }
		constexpr bool is_redo	() const { return is_ctrl(key_code::Y) || is_press(key_code::Z, KeyMods_Ctrl | KeyMods_Shift); }
		constexpr bool is_cut	() const { return is_ctrl(key_code::X) || is_shift(key_code::Delete); }
		constexpr bool is_copy	() const { return is_ctrl(key_code::C) || is_ctrl(key_code::Insert); }
		constexpr bool is_paste	() const { return is_ctrl(key_code::V) || is_shift(key_code::Insert); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct mouse_event final : T_event<mouse_event>
	{
		int32_t const key, action, mods;

		constexpr mouse_event(int32_t key, int32_t action, int32_t mods) noexcept
			: key{ key }
			, action{ action }
			, mods{ mods }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct scroll_event final : T_event<scroll_event>
	{
		float64_t const x, y;

		constexpr scroll_event(float64_t x, float64_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct frame_size_event final : T_event<frame_size_event>
	{
		int32_t const width, height;

		constexpr frame_size_event(int32_t width, int32_t height) noexcept
			: width{ width }
			, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_close_event final : T_event<window_close_event>
	{
		window_close_event() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_error_event final : T_event<window_error_event>
	{
		int32_t const code;
		cstring const desc;

		constexpr window_error_event(int32_t code, cstring desc) noexcept
			: code{ code }
			, desc{ desc }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_focus_event final : T_event<window_focus_event>
	{
		int32_t const focused;
		
		constexpr window_focus_event(int32_t focused) noexcept
			: focused{ focused }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_pos_event final : T_event<window_pos_event>
	{
		int32_t const x, y;

		constexpr window_pos_event(int32_t x, int32_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct window_size_event final : T_event<window_size_event>
	{
		int32_t const width, height;

		constexpr window_size_event(int32_t width, int32_t height) noexcept
			: width{ width }
			, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLATFORM_EVENTS_HPP_