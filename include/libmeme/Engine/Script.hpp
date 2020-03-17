#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_ENGINE_API script final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using callback_type = std::function<void(void *)>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script(allocator_type const & alloc = {}) noexcept;

		script(script const & other, allocator_type const & alloc = {});

		script(script && other, allocator_type const & alloc = {}) noexcept;

		~script();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline script & operator=(script const & other)
		{
			script temp{ other };
			swap(temp);
			return (*this);
		}

		inline script & operator=(script && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(script & other) noexcept
		{
			if (this != std::addressof(other))
			{
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_HPP_