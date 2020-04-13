#ifndef _ML_ASSET_HPP_
#define _ML_ASSET_HPP_

// WIP

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct asset final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		std::optional<std::any> m_data{};

		ML_NODISCARD bool has_value() const noexcept
		{
			return m_data.has_value();
		}

		template <class T
		> ML_NODISCARD bool holds() const noexcept
		{
			if (!has_value()) { return false; }
			
			return false;
		}

		template <class T
		> ML_NODISCARD auto get() noexcept
		{
		}

		template <class T
		> ML_NODISCARD auto get() const noexcept
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_ASSET_HPP_