#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API uniform final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using sampler_type = typename texture const *;

		using allowed_types = typename meta::list<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, color,
			mat2, mat3, mat4,
			sampler_type
		>;

		using variable_type = typename meta::rename<std::variant, allowed_types>;

		using function_type = typename std::function<variable_type()>;

		using info_type = typename typeof<>;

		using name_type = typename pmr::string;

		using data_type = typename std::variant<variable_type, function_type>;

		enum : size_t { ID_Variable, ID_Function };

		template <class T> static constexpr bool is_sampler_v
		{
			std::is_convertible_v<T, sampler_type> ||
			std::is_convertible_v<std::add_pointer_t<std::decay_t<T>>, sampler_type>
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform(allocator_type const & alloc = {}) noexcept
			: m_info{}
			, m_name{ alloc }
			, m_data{}
		{
		}

		uniform(info_type const & info, name_type const & name, data_type const & data, allocator_type const & alloc = {})
			: m_info{ info }
			, m_name{ name, alloc }
			, m_data{ data }
		{
		}

		uniform(info_type && info, name_type && name, data_type && data, allocator_type const & alloc = {}) noexcept
			: m_info{ ML_forward(info) }
			, m_name{ ML_forward(name), alloc }
			, m_data{ ML_forward(data) }
		{
		}

		uniform(uniform const & other, allocator_type const & alloc = {})
			: m_info{ other.m_info }
			, m_name{ other.m_name, alloc }
			, m_data{ other.m_data }
		{
		}

		uniform(uniform && other, allocator_type const & alloc = {}) noexcept
			: uniform{ alloc }
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform & operator=(uniform const & other)
		{
			uniform temp{ other };
			swap(temp);
			return (*this);
		}

		uniform & operator=(uniform && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		void swap(uniform & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_info, other.m_info);
				std::swap(m_name, other.m_name);
				std::swap(m_data, other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD info_type const & info() const noexcept { return m_info; }
		
		ML_NODISCARD name_type const & name() const noexcept { return m_name; }
		
		ML_NODISCARD data_type const & data() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD variable_type var() const noexcept
		{
			switch (m_data.index())
			{
			case ID_Variable:
				return std::get<variable_type>(m_data);

			case ID_Function:
				if (auto const & fn{ std::get<function_type>(m_data) })
					return std::invoke(fn);
			}
			return variable_type{};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD bool holds() const noexcept
		{
			if constexpr (is_sampler_v<T> && !std::is_same_v<T, sampler_type>)
			{
				return this->holds<sampler_type>();
			}
			else
			{
				return std::holds_alternative<T>(this->var());
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD decltype(auto) get() const noexcept
		{
			if constexpr (is_sampler_v<T> && !std::is_same_v<T, sampler_type>)
			{
				return this->get<sampler_type>();
			}
			else if (auto const v{ this->var() }; std::holds_alternative<T>(v))
			{
				return std::make_optional<T>(std::get<T>(v));
			}
			else
			{
				return (std::optional<T>)std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... Args
		> bool set(Args && ... args) noexcept
		{
			if constexpr (is_sampler_v<T> && !std::is_same_v<T, sampler_type>)
			{
				return this->set<sampler_type>(ML_forward(args)...);
			}
			else if (this->holds<T>())
			{
				m_data = data_type{ ML_forward(args)... };
				return true;
			}
			else
			{
				return false;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool const() const noexcept { return !m_name.empty(); }

		ML_NODISCARD auto compare(uniform const & other) const noexcept
		{
			if (m_info != other.m_info)
			{
				return m_info.compare(other.m_info);
			}
			else
			{
				return m_name.compare(other.m_name);
			}
		}

		ML_NODISCARD bool operator==(uniform const & other) const noexcept
		{
			return this->compare(other) == 0;
		}

		ML_NODISCARD bool operator!=(uniform const & other) const noexcept
		{
			return this->compare(other) != 0;
		}

		ML_NODISCARD bool operator<(uniform const & other) const noexcept
		{
			return this->compare(other) < 0;
		}

		ML_NODISCARD bool operator>(uniform const & other) const noexcept
		{
			return this->compare(other) > 0;
		}

		ML_NODISCARD bool operator<=(uniform const & other) const noexcept
		{
			return this->compare(other) <= 0;
		}

		ML_NODISCARD bool operator>=(uniform const & other) const noexcept
		{
			return this->compare(other) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		info_type m_info; // info
		name_type m_name; // name
		data_type m_data; // data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Type, class Name, class ... Args
	> ML_NODISCARD inline auto make_uniform(Name && name, Args && ... args) noexcept
	{
		return uniform{ typeof_v<Type>, ML_forward(name), ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_