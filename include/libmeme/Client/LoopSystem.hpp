#ifndef _ML_LOOP_SYSTEM_HPP_
#define _ML_LOOP_SYSTEM_HPP_

#include <libmeme/Client/Export.hpp>
#include <libmeme/Core/Events.hpp>
#include <libmeme/Core/Duration.hpp>

// WIP

namespace ml
{
	struct ML_CLIENT_API player_loop : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		std::function<bool()>		condition	{};
		std::function<void()>		update		{};
		pmr::vector<player_loop>	subsystems	{};

		player_loop() noexcept {}

		virtual ~player_loop() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	struct ML_CLIENT_API loop_system final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		loop_system(allocator_type alloc = {}) noexcept;

		ML_NODISCARD player_loop * get_main_loop() const { return m_main_loop; }

		player_loop * set_main_loop(player_loop * value) { return (m_main_loop = value); }

	private:
		pmr::vector<player_loop *>	m_loops		; // 
		player_loop *				m_main_loop	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_LOOP_SYSTEM_HPP_