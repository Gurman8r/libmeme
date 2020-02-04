#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Color.hpp>
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

		~uniform() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit uniform(allocator_type const & alloc)
			: m_type{}
			, m_name{ alloc }
			, m_data{}
		{
		}

		explicit uniform(type_t const & type, name_t const & name, data_t const & data, allocator_type const & alloc = {})
			: m_type{ type }
			, m_name{ name, alloc }
			, m_data{ data }
		{
		}

		explicit uniform(type_t const & type, name_t && name, data_t const & data, allocator_type const & alloc = {})
			: m_type{ type }
			, m_name{ std::move(name), alloc }
			, m_data{ data }
		{
		}

		explicit uniform(type_t const & type, name_t const & name, data_t && data, allocator_type const & alloc = {})
			: m_type{ type }
			, m_name{ name, alloc }
			, m_data{ std::move(data) }
		{
		}

		explicit uniform(type_t const & type, name_t && name, data_t && data, allocator_type const & alloc = {})
			: m_type{ type }
			, m_name{ std::move(name), alloc }
			, m_data{ std::move(data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		ML_NODISCARD inline auto type() const noexcept -> type_t const & { return m_type; }
		
		ML_NODISCARD inline auto name() const noexcept -> name_t const & { return m_name; }
		
		ML_NODISCARD inline auto data() const noexcept -> data_t const & { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool is_variable() const noexcept
		{
			return (m_data.index() == ID_Variable);
		}

		ML_NODISCARD inline bool is_function() const noexcept
		{
			return (m_data.index() == ID_Function);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline variable_type load() const
		{
			switch (m_data.index())
			{
			case ID_Variable:
				return std::get<variable_type>(m_data);

			case ID_Function:
				return std::invoke(std::get<function_type>(m_data));
			}
			return variable_type{};
		}

		template <class T
		> ML_NODISCARD inline bool holds() const noexcept
		{
			return std::holds_alternative<T>(load());
		}

		template <class T
		> ML_NODISCARD inline std::optional<T> get() const
		{
			if (auto const v{ load() }; std::holds_alternative<T>(v))
			{
				return std::make_optional<T>(std::get<T>(v));
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline uniform & set(data_t const & value)
		{
			m_type = typeof<T>{};
			m_data = value;
			return (*this);
		}

		template <class T> inline uniform & set(data_t && value)
		{
			m_type = typeof<T>{};
			m_data = std::move(value);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool good() const noexcept
		{
			return !m_name.empty();
		}

		ML_NODISCARD inline int32_t compare(uniform const & other) const noexcept
		{
			return m_name.compare(other.m_name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool const() const noexcept
		{
			return good();
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

	inline ML_SERIALIZE(std::ostream & out, uniform const & value)
	{
		return out
			<< value.type().name() << " "
			<< value.name() << " ";
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Type, class Name, class ... Args
	> ML_NODISCARD static inline auto make_uniform(Name && name, Args && ... args) noexcept
	{
		return uniform{ typeof<Type>{}, std::move(name), std::forward<Args>(args)... };
	}

	ML_NODISCARD static inline auto make_uniform(uniform const & value)
	{
		return uniform{ value };
	}

	ML_NODISCARD static inline auto make_uniform(uniform && value) noexcept
	{
		return uniform{ std::move(value) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_