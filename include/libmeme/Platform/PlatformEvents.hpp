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

	ML_EVENT(char_event)
	{
		uint32_t const value;

		constexpr char_event(uint32_t value) noexcept
			: value{ value }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(cursor_enter_event)
	{
		int32_t const entered;

		constexpr cursor_enter_event(int32_t entered) noexcept
			: entered{ entered }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(cursor_pos_event)
	{
		float64_t const x, y;

		constexpr cursor_pos_event(float64_t x, float64_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(key_event)
	{
		int32_t const key, scan, action, mods;

		constexpr key_event(int32_t key, int32_t scan, int32_t action, int32_t mods) noexcept
			: key	{ key }
			, scan	{ scan }
			, action{ action }
			, mods	{ mods }
		{
		}

		constexpr bool is_press(int32_t k) const noexcept { return (key == k) && (action == ML_KEY_PRESS); }
		constexpr bool is_repeat(int32_t k) const noexcept { return (key == k) && (action == ML_KEY_REPEAT); }
		constexpr bool is_release(int32_t k) const noexcept { return (key == k) && (action == ML_KEY_RELEASE); }

		constexpr bool is_shift	(int32_t k) const noexcept { return is_press(k) && (mods == key_mods_shift);  }
		constexpr bool is_ctrl	(int32_t k) const noexcept { return is_press(k) && (mods == key_mods_ctrl); }
		constexpr bool is_alt	(int32_t k) const noexcept { return is_press(k) && (mods == key_mods_alt); }
		constexpr bool is_super	(int32_t k) const noexcept { return is_press(k) && (mods == key_mods_super); }
		constexpr bool is_caps	(int32_t k) const noexcept { return is_press(k) && (mods & key_mods_caps_lock); }
		constexpr bool is_numlk	(int32_t k) const noexcept { return is_press(k) && (mods & key_mods_num_lock); }

		constexpr bool is_new	() const noexcept { return is_ctrl(key_code::N); }
		constexpr bool is_open	() const noexcept { return is_ctrl(key_code::O); }
		constexpr bool is_save	() const noexcept { return is_ctrl(key_code::S) || is_press(key_code::S) && (mods & (key_mods_ctrl | key_mods_shift)); }
		constexpr bool is_undo	() const noexcept { return is_ctrl(key_code::Z); }
		constexpr bool is_redo	() const noexcept { return is_ctrl(key_code::Y) || is_press(key_code::Z) && (mods & (key_mods_ctrl | key_mods_shift)); }
		constexpr bool is_cut	() const noexcept { return is_ctrl(key_code::X) || is_shift(key_code::Delete); }
		constexpr bool is_copy	() const noexcept { return is_ctrl(key_code::C) || is_ctrl(key_code::Insert); }
		constexpr bool is_paste	() const noexcept { return is_ctrl(key_code::V) || is_shift(key_code::Insert); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(mouse_event)
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

	ML_EVENT(scroll_event)
	{
		float64_t const x, y;

		constexpr scroll_event(float64_t x, float64_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(frame_size_event)
	{
		int32_t const width, height;

		constexpr frame_size_event(int32_t width, int32_t height) noexcept
			: width{ width }
			, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(window_close_event)
	{
		window_close_event() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(window_error_event)
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

	ML_EVENT(window_focus_event)
	{
		int32_t const focused;
		
		constexpr window_focus_event(int32_t focused) noexcept
			: focused{ focused }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(window_pos_event)
	{
		int32_t const x, y;

		constexpr window_pos_event(int32_t x, int32_t y) noexcept
			: x{ x }
			, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_EVENT(window_size_event)
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