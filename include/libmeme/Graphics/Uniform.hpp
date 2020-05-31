#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml
{
	struct uniform final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using sampler2d = typename shared<gl::texture2d>;

		template <class T> static constexpr bool is_sampler2d_ish
		{
			!std::is_same_v<T, sampler2d> &&
			std::is_convertible_v<std::add_pointer_t<T>, std::add_pointer_t<sampler2d::element_type>>
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allowed_types = meta::list<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, color,
			mat2, mat3, mat4,
			sampler2d
		>;

		using variable_type = typename meta::rename<std::variant, allowed_types>;

		using function_type = typename std::function<variable_type()>;

		enum : size_t { id_variable, id_function };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using info_type		= typename typeof<>;
		using name_type		= typename pmr::string;
		using data_type		= typename std::variant<variable_type, function_type>;
		using storage_type	= typename std::tuple<info_type, name_type, data_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform() noexcept
			: m_storage{}
		{
		}

		explicit uniform(storage_type const & value)
			: m_storage{ value }
		{
		}

		explicit uniform(storage_type && value) noexcept
			: m_storage{ std::move(value) }
		{
		}

		uniform(uniform const & other)
			: m_storage{ other.m_storage }
		{
		}

		uniform(uniform && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		template <class I, class N, class D
		> uniform(I && type, N && name, D && data)
			: m_storage{ ML_forward(type), ML_forward(name), ML_forward(data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform & operator=(uniform const & other)
		{
			uniform temp{ other };
			this->swap(temp);
			return (*this);
		}

		uniform & operator=(uniform && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(uniform & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto const & type() const & noexcept { return std::get<info_type>(m_storage); }

		ML_NODISCARD auto const & name() const & noexcept { return std::get<name_type>(m_storage); }

		ML_NODISCARD auto const & data() const & noexcept { return std::get<data_type>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD variable_type var() const noexcept
		{
			switch (auto const & d{ std::get<data_type>(m_storage) }; d.index())
			{
			default			: return variable_type{};
			case id_variable: return std::get<variable_type>(d);
			case id_function: return std::invoke(std::get<function_type>(d));
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD bool holds() const noexcept
		{
			if constexpr (is_sampler2d_ish<T>)
			{
				return this->holds<sampler2d>();
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
			if constexpr (is_sampler2d_ish<T>)
			{
				return this->get<sampler2d>();
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
			if constexpr (is_sampler2d_ish<T>)
			{
				return this->set<sampler2d>(ML_forward(args)...);
			}
			else if (this->holds<T>())
			{
				std::get<data_type>(m_storage) = data_type{ ML_forward(args)... };
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
			if (type() != value.type())
			{
				return type().compare(value.type());
			}
			else
			{
				return name().compare(value.name());
			}
		}

		ML_NODISCARD bool operator==(uniform const & value) const noexcept { return compare(value) == 0; }

		ML_NODISCARD bool operator!=(uniform const & value) const noexcept { return compare(value) != 0; }

		ML_NODISCARD bool operator<(uniform const & value) const noexcept { return compare(value) < 0; }

		ML_NODISCARD bool operator>(uniform const & value) const noexcept { return compare(value) > 0; }

		ML_NODISCARD bool operator<=(uniform const & value) const noexcept { return compare(value) <= 0; }

		ML_NODISCARD bool operator>=(uniform const & value) const noexcept { return compare(value) >= 0; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Type, class Name, class ... Args
	> ML_NODISCARD auto make_uniform(Name && name, Args && ... args) noexcept
	{
		return uniform{ typeof_v<Type>, ML_forward(name), uniform::data_type{ ML_forward(args)... } };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_