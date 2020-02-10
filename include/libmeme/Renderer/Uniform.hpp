#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/TypeOf.hpp>

namespace ml
{
	struct ML_RENDERER_API uniform final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using variable_type = typename std::variant<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, color,
			mat2, mat3, mat4,
			struct texture const *
		>;

		using function_type = typename std::function<
			variable_type()
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		uniform(type_t && type, name_t && name, data_t && data, allocator_type const & alloc = {})
			: m_type{ ML_FWD(type) }
			, m_name{ ML_FWD(name), alloc }
			, m_data{ ML_FWD(data) }
		{
		}

		uniform(uniform const & other, allocator_type const & alloc = {})
			: m_type{ other.m_type }
			, m_name{ other.m_name, alloc }
			, m_data{ other.m_data }
		{
		}

		uniform(uniform && other, allocator_type const & alloc = {}) noexcept
			: m_type{ std::move(other.m_type) }
			, m_name{ std::move(other.m_name), alloc }
			, m_data{ std::move(other.m_data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline uniform & operator=(uniform const & other)
		{
			uniform temp{ other };
			swap(temp);
			return (*this);
		}

		inline uniform & operator=(uniform && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(uniform & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_type.swap(other.m_type);
				m_name.swap(other.m_name);
				m_data.swap(other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline type_t const & type() const noexcept { return m_type; }
		
		ML_NODISCARD inline name_t const & name() const noexcept { return m_name; }
		
		ML_NODISCARD inline data_t const & data() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto compare(uniform const & other) const noexcept
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

		ML_NODISCARD inline bool is_variable() const noexcept
		{
			return (m_data.index() == ID_Variable);
		}

		ML_NODISCARD inline bool is_function() const noexcept
		{
			return (m_data.index() == ID_Function);
		}

		ML_NODISCARD inline variable_type var() const
		{
			switch (m_data.index())
			{
			case ID_Variable: return std::get<variable_type>(m_data);
			case ID_Function: return std::invoke(std::get<function_type>(m_data));
			default			: return variable_type{};
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline bool holds() const noexcept
		{
			return std::holds_alternative<T>(this->var());
		}

		template <class T
		> ML_NODISCARD inline std::optional<T> get() const
		{
			if (auto const v{ this->var() }; std::holds_alternative<T>(v))
			{
				return std::make_optional<T>(std::get<T>(v));
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Data
		> inline uniform & set(Data && value)
		{
			m_data = ML_FWD(value);
			return (*this);
		}

		template <class T, class Data
		> inline uniform & set(Data && value)
		{
			return this->holds<T>() ? this->set<T>(ML_FWD(value)) : (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool const() const noexcept
		{
			return !m_name.empty();
		}

		ML_NODISCARD inline bool operator==(uniform const & other) const noexcept
		{
			return compare(other) == 0;
		}

		ML_NODISCARD inline bool operator!=(uniform const & other) const noexcept
		{
			return compare(other) != 0;
		}

		ML_NODISCARD inline bool operator<(uniform const & other) const noexcept
		{
			return compare(other) < 0;
		}

		ML_NODISCARD inline bool operator>(uniform const & other) const noexcept
		{
			return compare(other) > 0;
		}

		ML_NODISCARD inline bool operator<=(uniform const & other) const noexcept
		{
			return compare(other) <= 0;
		}

		ML_NODISCARD inline bool operator>=(uniform const & other) const noexcept
		{
			return compare(other) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		type_t m_type;
		name_t m_name;
		data_t m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Type, class Name, class ... Args
	> ML_NODISCARD static inline auto make_uniform(Name && name, Args && ... args) noexcept
	{
		return uniform{ typeof_v<Type>, ML_FWD(name), ML_FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_