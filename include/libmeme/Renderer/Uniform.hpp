#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/TypeOf.hpp>

namespace ml
{
	struct ML_RENDERER_API uniform final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allowed_types = typename meta::list<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, color,
			mat2, mat3, mat4,
			struct texture const *
		>;

		using variable_type = typename meta::rename<
			std::variant, allowed_types
		>;

		using function_type = typename std::function<
			variable_type()
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using type_t = typename typeof<>;

		using name_t = typename pmr::string;

		using data_t = typename std::variant<variable_type, function_type>;

		enum : size_t { ID_Variable, ID_Function };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uniform() noexcept
			: m_type{}
			, m_name{}
			, m_data{}
		{
		}

		explicit uniform(allocator_type const & alloc)
			: m_type{}
			, m_name{ alloc }
			, m_data{}
		{
		}

		uniform(type_t const & type, name_t const & name, data_t const & data, allocator_type const & alloc = {})
			: m_type{ type }
			, m_name{ name, alloc }
			, m_data{ data }
		{
		}

		uniform(type_t && type, name_t && name, data_t && data, allocator_type const & alloc = {}) noexcept
			: m_type{ ML_forward(type) }
			, m_name{ ML_forward(name), alloc }
			, m_data{ ML_forward(data) }
		{
		}

		uniform(uniform const & other, allocator_type const & alloc = {})
			: m_type{ other.m_type }
			, m_name{ other.m_name, alloc }
			, m_data{ other.m_data }
		{
		}

		uniform(uniform && other, allocator_type const & alloc = {}) noexcept
			: m_type{ ML_forward(other.m_type) }
			, m_name{ ML_forward(other.m_name), alloc }
			, m_data{ ML_forward(other.m_data) }
		{
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
				std::swap(m_type, other.m_type);
				std::swap(m_name, other.m_name);
				std::swap(m_data, other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD type_t const & type() const noexcept { return m_type; }
		
		ML_NODISCARD name_t const & name() const noexcept { return m_name; }
		
		ML_NODISCARD data_t const & data() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto compare(uniform const & other) const noexcept
		{
			if (m_type != other.m_type)
			{
				return m_type.compare(other.m_type);
			}
			else
			{
				return m_name.compare(other.m_name);
			}
		}

		ML_NODISCARD bool is_variable() const noexcept
		{
			return (m_data.index() == ID_Variable);
		}

		ML_NODISCARD bool is_function() const noexcept
		{
			return (m_data.index() == ID_Function);
		}

		ML_NODISCARD variable_type get_var() const
		{
			switch (m_data.index())
			{
			case ID_Variable: return std::get<variable_type>(m_data);
			case ID_Function: return std::invoke(std::get<function_type>(m_data));
			default: return variable_type{};
			}
		}

		ML_NODISCARD size_t index() const noexcept
		{
			return get_var().index();
		}

		template <class T
		> ML_NODISCARD bool holds() const noexcept
		{
			return std::holds_alternative<T>(get_var());
		}

		template <class T
		> ML_NODISCARD std::optional<T> get() const
		{
			if (auto const v{ get_var() }; std::holds_alternative<T>(v))
			{
				return std::make_optional<T>(std::get<T>(v));
			}
			else
			{
				return std::nullopt;
			}
		}

		template <class T, class Data
		> uniform & set(Data const & value) noexcept
		{
			if (this->holds<T>())
			{
				m_data = value;
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool const() const noexcept
		{
			return !m_name.empty();
		}

		ML_NODISCARD bool operator==(uniform const & other) const noexcept
		{
			return compare(other) == 0;
		}

		ML_NODISCARD bool operator!=(uniform const & other) const noexcept
		{
			return compare(other) != 0;
		}

		ML_NODISCARD bool operator<(uniform const & other) const noexcept
		{
			return compare(other) < 0;
		}

		ML_NODISCARD bool operator>(uniform const & other) const noexcept
		{
			return compare(other) > 0;
		}

		ML_NODISCARD bool operator<=(uniform const & other) const noexcept
		{
			return compare(other) <= 0;
		}

		ML_NODISCARD bool operator>=(uniform const & other) const noexcept
		{
			return compare(other) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		type_t m_type; // type info
		name_t m_name; // uniform name
		data_t m_data; // variant data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Type, class Name, class ... Args
	> ML_NODISCARD static inline auto make_uniform(Name && name, Args && ... args) noexcept
	{
		return uniform{ typeof_v<Type>, ML_forward(name), ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_