#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API uniform final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using sampler = typename texture const *;

		using allowed_types = typename meta::list<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, color,
			mat2, mat3, mat4,
			sampler
		>;

		using variable_type = typename meta::rename<std::variant, allowed_types>;

		using function_type = typename std::function<variable_type()>;

		using info_type = typename typeof<>;

		using name_type = typename pmr::string;

		using data_type = typename std::variant<variable_type, function_type>;

		enum id_ : size_t { id_variable, id_function };

		template <class T> static constexpr bool is_sampler_ish
		{
			std::is_convertible_v<T, sampler> ||
			std::is_convertible_v<std::add_pointer_t<std::decay_t<T>>, sampler>
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform(allocator_type const & alloc = {}) noexcept
			: m_type{}
			, m_name{ alloc }
			, m_data{}
		{
		}

		uniform(info_type const & info, name_type const & name, data_type const & data, allocator_type const & alloc = {})
			: m_type{ info }
			, m_name{ name, alloc }
			, m_data{ data }
		{
		}

		uniform(info_type && info, name_type && name, data_type && data, allocator_type const & alloc = {}) noexcept
			: m_type{ ML_forward(info) }
			, m_name{ ML_forward(name), alloc }
			, m_data{ ML_forward(data) }
		{
		}

		uniform(uniform const & value, allocator_type const & alloc = {})
			: m_type{ value.m_type }
			, m_name{ value.m_name, alloc }
			, m_data{ value.m_data }
		{
		}

		uniform(uniform && value, allocator_type const & alloc = {}) noexcept
			: uniform{ alloc }
		{
			swap(std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform & operator=(uniform const & value)
		{
			uniform temp{ value };
			swap(temp);
			return (*this);
		}

		uniform & operator=(uniform && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		void swap(uniform & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_type, value.m_type);
				std::swap(m_name, value.m_name);
				std::swap(m_data, value.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD variable_type var() const noexcept
		{
			switch (m_data.index())
			{
			case id_variable:
				return std::get<variable_type>(m_data);

			case id_function:
				if (auto const & fn{ std::get<function_type>(m_data) })
					return std::invoke(fn);
			}
			return variable_type{};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD bool holds() const noexcept
		{
			if constexpr (is_sampler_ish<T> && !std::is_same_v<T, sampler>)
			{
				return this->holds<sampler>();
			}
			else
			{
				return std::holds_alternative<T>(this->var());
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD auto get() const noexcept
		{
			if constexpr (is_sampler_ish<T> && !std::is_same_v<T, sampler>)
			{
				return this->get<sampler>();
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
			if constexpr (is_sampler_ish<T> && !std::is_same_v<T, sampler>)
			{
				return this->set<sampler>(ML_forward(args)...);
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

		ML_NODISCARD auto compare(uniform const & value) const noexcept
		{
			if (m_type != value.m_type)
			{
				return m_type.compare(value.m_type);
			}
			else
			{
				return m_name.compare(value.m_name);
			}
		}

		ML_NODISCARD bool operator==(uniform const & value) const noexcept { return compare(value) == 0; }

		ML_NODISCARD bool operator!=(uniform const & value) const noexcept { return compare(value) != 0; }

		ML_NODISCARD bool operator<(uniform const & value) const noexcept { return compare(value) < 0; }

		ML_NODISCARD bool operator>(uniform const & value) const noexcept { return compare(value) > 0; }

		ML_NODISCARD bool operator<=(uniform const & value) const noexcept { return compare(value) <= 0; }

		ML_NODISCARD bool operator>=(uniform const & value) const noexcept { return compare(value) >= 0; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool const() const noexcept { return !m_name.empty(); }

		ML_NODISCARD info_type const & type() const noexcept { return m_type; }

		ML_NODISCARD name_type const & name() const noexcept { return m_name; }

		ML_NODISCARD data_type const & data() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		info_type m_type; // info
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