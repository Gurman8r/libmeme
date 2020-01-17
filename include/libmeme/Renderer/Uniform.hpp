#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/TypeOf.hpp>

namespace ml
{
	struct ML_RENDERER_API Uniform final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using variable_t = typename std::variant<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, Color,
			mat2, mat3, mat4,
			struct Texture const *
		>;

		using function_t = typename std::function<
			variable_t()
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type_t = typename typeof<>;

		using name_t = typename std::string;

		using data_t = typename std::variant<variable_t, function_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Type, ID_Name, ID_Data };

		using storage_type = typename std::tuple<type_t, name_t, data_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Uniform() noexcept : m_storage{} {}

		~Uniform() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Uniform(storage_type const & storage)
			: m_storage{ storage }
		{
		}

		explicit Uniform(storage_type && storage) noexcept
			: m_storage{ std::move(storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Uniform(Uniform const & other)
			: m_storage{ other.m_storage }
		{
		}

		Uniform(Uniform && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform & operator=(Uniform const & other)
		{
			Uniform temp{ other };
			swap(temp);
			return (*this);
		}

		inline Uniform & operator=(Uniform && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(Uniform & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline type_t const & type() const noexcept
		{
			return std::get<ID_Type>(m_storage);
		}

		ML_NODISCARD inline name_t const & name() const noexcept
		{
			return std::get<ID_Name>(m_storage);
		}

		ML_NODISCARD inline data_t const & data() const noexcept
		{
			return std::get<ID_Data>(m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool is_variable() const noexcept
		{
			return std::holds_alternative<variable_t>(data());
		}

		ML_NODISCARD inline bool is_function() const noexcept
		{
			return std::holds_alternative<function_t>(data());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline std::optional<variable_t> load() const
		{
			if (is_function())
			{
				if (auto const & fn{ std::get<function_t>(data()) })
				{
					return std::make_optional(std::invoke(fn));
				}
			}
			else if (is_variable())
			{
				return std::make_optional(std::get<variable_t>(data()));
			}
			return std::nullopt;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD inline std::optional<T> get() const
		{
			if (std::optional<variable_t> v{ load() }; v && std::holds_alternative<T>(*v))
			{
				return std::make_optional<T>(std::get<T>(*v));
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool operator==(Uniform const & other)
		{
			return (type() == other.type())
				&& (name() == other.name());
		}

		ML_NODISCARD inline bool operator!=(Uniform const & other)
		{
			return !(*this == other);
		}

		ML_NODISCARD inline bool operator<(Uniform const & other)
		{
			return (type() < other.type())
				&& (name() < other.name());
		}

		ML_NODISCARD inline bool operator>(Uniform const & other)
		{
			return !(*this < other);
		}

		ML_NODISCARD inline bool operator<=(Uniform const & other)
		{
			return (*this < other) || (*this == other);
		}

		ML_NODISCARD inline bool operator>=(Uniform const & other)
		{
			return (*this > other) || (*this == other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, Uniform const & value)
	{
		return out
			<< value.type().name() << " "
			<< value.name() << " ";
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Type, class Name, class ... Args
	> ML_NODISCARD static inline auto make_uniform(Name && name, Args && ... args) noexcept
	{
		return Uniform{ std::make_tuple(
			typeof<Type>{}, std::move(name), std::forward<Args>(args)...
		) };
	}

	ML_NODISCARD static inline auto make_uniform(Uniform const & value)
	{
		return Uniform{ value };
	}

	ML_NODISCARD static inline auto make_uniform(Uniform && value) noexcept
	{
		return Uniform{ std::move(value) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_