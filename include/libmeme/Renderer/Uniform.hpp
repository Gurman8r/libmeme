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

		using type_t = typename typeof<>;

		using name_t = typename std::string;

		using variable_t = typename std::variant<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, Color,
			mat2, mat3, mat4,
			struct Texture const *
		>;

		using funcion_t = typename std::function<
			variable_t()
		>;

		using data_t = typename std::variant<
			variable_t, funcion_t
		>;

		using storage_t = typename std::tuple<
			type_t, name_t, data_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Uniform();
		Uniform(storage_t const & storage);
		Uniform(storage_t && storage) noexcept;
		Uniform(Uniform const & other);
		Uniform(Uniform && other) noexcept;
		~Uniform();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Uniform & operator=(Uniform const & other);

		Uniform & operator=(Uniform && other) noexcept;

		void swap(Uniform & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline type_t const & type() const noexcept { return std::get<0>(m_storage); }

		inline name_t const & name() const noexcept { return std::get<1>(m_storage); }
		
		inline data_t const & data() const noexcept { return std::get<2>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool is_variable() const noexcept
		{
			return std::holds_alternative<variable_t>(data());
		}

		inline bool is_function() const noexcept
		{
			return std::holds_alternative<funcion_t>(data());
		}

		inline std::optional<variable_t> load() const
		{
			if (is_variable())
			{
				return std::make_optional(std::get<variable_t>(data()));
			}
			else if (is_function())
			{
				return std::make_optional(std::invoke(std::get<funcion_t>(data())));
			}
			else
			{
				return std::nullopt;
			}
		}

		template <class T> inline std::optional<T> get() const
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

		template <class Type, class ... Args> inline decltype(auto) set(Args && ... args)
		{
			return (m_storage = std::make_tuple(
				typeof_v<Type>, name(), std::forward<Args>(args)...
			));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_t m_storage;

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

	template <
		class Type, class Name, class ... Args
	> static inline auto make_uniform(Name && name, Args && ... args) noexcept
	{
		return Uniform{ std::make_tuple(
			typeof_v<Type>, std::move(name), std::forward<Args>(args)...
		) };
	}

	template <class U> static inline auto make_uniform(U && value) noexcept
	{
		return Uniform{ std::make_tuple(
			value.type(), value.name(), value.data()
		) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_