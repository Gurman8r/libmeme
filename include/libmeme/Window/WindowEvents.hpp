#ifndef _ML_WINDOW_EVENTS_HPP_
#define _ML_WINDOW_EVENTS_HPP_

#include <libmeme/System/Event.hpp>
#include <libmeme/Window/WindowAPI.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(char_event)
	{
		void * const user;
		uint32_t const value;

		constexpr char_event(void * user, uint32_t value) noexcept
			: user{ user }, value{ value }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(char_mods_event)
	{
		void * const user;
		uint32_t const value;
		int32_t const mods;

		constexpr char_mods_event(void * user, uint32_t value, int32_t mods) noexcept
			: user{ user }, value{ value }, mods{ mods }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(close_event)
	{
		void * const user;

		constexpr close_event(void * user) noexcept
			: user{ user }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(content_scale_event)
	{
		void * const user;
		float_t const x, y;

		constexpr content_scale_event(void * user, float_t x, float_t y) noexcept
			: user{ user }, x{ x }, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(drop_event)
	{
		void * const user;
		int32_t const count;
		cstring const * const paths;

		constexpr drop_event(void * user, int32_t count, cstring const * paths) noexcept
			: user{ user }, count{ count }, paths{ paths }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(cursor_enter_event)
	{
		void * const user;
		int32_t const entered;

		constexpr cursor_enter_event(void * user, int32_t entered) noexcept
			: user{ user }, entered{ entered }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(cursor_position_event)
	{
		void * const user;
		float64_t const x, y;

		constexpr cursor_position_event(void * user, float64_t x, float64_t y) noexcept
			: user{ user }, x{ x }, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(error_event)
	{
		int32_t const code;
		cstring const desc;

		constexpr error_event(int32_t code, cstring desc) noexcept
			: code{ code }, desc{ desc }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(focus_event)
	{
		void * const user;
		int32_t const focused;

		constexpr focus_event(void * user, int32_t focused) noexcept
			: user{ user }, focused{ focused }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(framebuffer_size_event)
	{
		void * const user;
		int32_t const width, height;

		constexpr framebuffer_size_event(void * user, int32_t width, int32_t height) noexcept
			: user{ user }, width{ width }, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(iconify_event)
	{
		void * const user;
		int32_t const iconified;

		constexpr iconify_event(void * user, int32_t iconified) noexcept
			: user{ user }, iconified{ iconified }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(key_event)
	{
		void * const user;
		int32_t const key, scan, action, mods;

		constexpr key_event(void * user, int32_t key, int32_t scan, int32_t action, int32_t mods) noexcept
			: user{ user }, key{ key }, scan{ scan }, action{ action }, mods{ mods }
		{
		}

		constexpr bool is_action	(int32_t a) const noexcept { return (action == a); }
		constexpr bool is_key		(int32_t k) const noexcept { return (key == k); }
		constexpr bool is_mods		(int32_t m) const noexcept { return (mods == m); }
		constexpr bool has_mods		(int32_t m) const noexcept { return (m & mods) == m; }
		
		constexpr bool is_release	(int32_t k) const noexcept { return is_key(k) && is_action(input_state_release); }
		constexpr bool is_press		(int32_t k) const noexcept { return is_key(k) && is_action(input_state_press); }
		constexpr bool is_repeat	(int32_t k) const noexcept { return is_key(k) && is_action(input_state_repeat); }

		constexpr bool is_release	(int32_t k, int32_t m) const noexcept { return is_release(k) && is_mods(m); }
		constexpr bool is_press		(int32_t k, int32_t m) const noexcept { return is_press(k) && is_mods(m); }
		constexpr bool is_repeat	(int32_t k, int32_t m) const noexcept { return is_repeat(k) && is_mods(m); }

		constexpr bool is_shift		() const noexcept { return has_mods(key_mods_shift);  }
		constexpr bool is_ctrl		() const noexcept { return has_mods(key_mods_ctrl); }
		constexpr bool is_alt		() const noexcept { return has_mods(key_mods_alt); }
		constexpr bool is_super		() const noexcept { return has_mods(key_mods_super); }
		constexpr bool is_caps		() const noexcept { return has_mods(key_mods_caps_lock); }
		constexpr bool is_numlk		() const noexcept { return has_mods(key_mods_num_lock); }

		constexpr bool is_shift		(int32_t k) const noexcept { return is_press(k) && is_shift();  }
		constexpr bool is_ctrl		(int32_t k) const noexcept { return is_press(k) && is_ctrl(); }
		constexpr bool is_alt		(int32_t k) const noexcept { return is_press(k) && is_alt(); }
		constexpr bool is_super		(int32_t k) const noexcept { return is_press(k) && is_super(); }
		constexpr bool is_caps		(int32_t k) const noexcept { return is_press(k) && is_caps(); }
		constexpr bool is_numlk		(int32_t k) const noexcept { return is_press(k) && is_numlk(); }

		constexpr bool is_new		() const noexcept { return is_ctrl(key_code_n); }
		constexpr bool is_open		() const noexcept { return is_ctrl(key_code_o); }
		constexpr bool is_save		() const noexcept { return is_ctrl(key_code_s) || is_press(key_code_s, key_mods_ctrl | key_mods_shift); }
		constexpr bool is_undo		() const noexcept { return is_ctrl(key_code_z); }
		constexpr bool is_redo		() const noexcept { return is_ctrl(key_code_y) || is_press(key_code_z, key_mods_ctrl | key_mods_shift); }
		constexpr bool is_cut		() const noexcept { return is_ctrl(key_code_x) || is_shift(key_code_del); }
		constexpr bool is_copy		() const noexcept { return is_ctrl(key_code_c) || is_ctrl(key_code_insert); }
		constexpr bool is_paste		() const noexcept { return is_ctrl(key_code_v) || is_shift(key_code_insert); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(maximize_event)
	{
		void * const user;
		int32_t const maximized;

		constexpr maximize_event(void * user, int32_t maximized) noexcept
			: user{ user }, maximized{ maximized }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(mouse_event)
	{
		void * const user;
		int32_t const button, action, mods;

		constexpr mouse_event(void * user, int32_t button, int32_t action, int32_t mods) noexcept
			: user{ user }, button{ button }, action{ action }, mods{ mods }
		{
		}

		constexpr bool is_action	(int32_t a) const noexcept { return (action == a); }
		constexpr bool is_button	(int32_t b) const noexcept { return (button == b); }
		constexpr bool is_mods		(int32_t m) const noexcept { return (mods == m); }
		constexpr bool has_mods		(int32_t m) const noexcept { return (mods & m); }
		
		constexpr bool is_release	(int32_t b) const noexcept { return is_button(b) && is_action(input_state_release); }
		constexpr bool is_press		(int32_t b) const noexcept { return is_button(b) && is_action(input_state_press); }
		constexpr bool is_repeat	(int32_t b) const noexcept { return is_button(b) && is_action(input_state_repeat); }

		constexpr bool is_release	(int32_t b, int32_t m) const noexcept { return is_release(b) && is_mods(m); }
		constexpr bool is_press		(int32_t b, int32_t m) const noexcept { return is_press(b) && is_mods(m); }
		constexpr bool is_repeat	(int32_t b, int32_t m) const noexcept { return is_repeat(b) && is_mods(m); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(position_event)
	{
		void * const user;
		int32_t const x, y;

		constexpr position_event(void * user, int32_t x, int32_t y) noexcept
			: user{ user }, x{ x }, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(refresh_event)
	{
		void * const user;

		constexpr refresh_event(void * user) noexcept
			: user{ user }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(scroll_event)
	{
		void * const user;
		float64_t const x, y;

		constexpr scroll_event(void * user, float64_t x, float64_t y) noexcept
			: user{ user }, x{ x }, y{ y }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_decl_event(size_event)
	{
		void * const user;
		int32_t const width, height;

		constexpr size_event(void * user, int32_t width, int32_t height) noexcept
			: user{ user }, width{ width }, height{ height }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_EVENTS_HPP_