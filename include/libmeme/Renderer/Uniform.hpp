#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Core/TypeOf.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	struct ML_RENDERER_API Uniform final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type_t = typename typeof<>;

		using name_t = typename std::string;

		using var_t = typename std::variant<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, Color,
			mat2, mat3, mat4,
			struct Texture const *
		>;

		using func_t = typename std::function<
			var_t()
		>;

		using data_t = typename std::variant<
			var_t, func_t
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

		inline bool is_variable() const noexcept { return std::holds_alternative<var_t>(data()); }

		inline bool is_function() const noexcept { return std::holds_alternative<func_t>(data()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline std::optional<T> get() const
		{
			if (is_variable())
			{
				auto const & temp{ std::get<var_t>(data()) };
				return std::holds_alternative<T>(temp)
					? std::make_optional<T>(std::get<T>(temp))
					: std::nullopt;
			}
			else if (is_function())
			{
				auto const & temp{ std::invoke(std::get<func_t>(data())) };
				return std::holds_alternative<T>(temp)
					? std::make_optional<T>(std::get<T>(temp))
					: std::nullopt;
			}
			return std::nullopt;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: union { storage_t m_storage; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Type, class Name, class ... Data
	> static inline auto make_uniform(Name && name, Data && ... args) noexcept
	{
		return Uniform{ std::make_tuple(
			typeof_v<Type>, std::forward<Name>(name), std::forward<Data>(args)...
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