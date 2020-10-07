#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <libmeme/Core/NonCopyable.hpp>

namespace ml
{
	template <class Derived
	> struct singleton : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename Derived;

		template <class ... Args
		> ML_NODISCARD static self_type * get_singleton(Args && ... args) noexcept
		{
			static auto buf{ get_buffer() };

			if constexpr (0 < sizeof...(args))
			{
				::new (buf) self_type{ ML_forward(args)... };
			}
			else
			{
				static ML_scope() { ::new (buf) self_type{}; };
			}

			return buf;
		}

	private:
		static self_type * get_buffer() noexcept
		{
			static byte_t buf[sizeof(self_type)]{};
			return reinterpret_cast<self_type *>(buf);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SINGLETON_HPP_