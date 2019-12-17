#ifndef _ML_UNIFORM_HPP_
#define _ML_UNIFORM_HPP_

#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Core/TypeOf.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/TypePack.hpp>

namespace ml
{
	struct ML_RENDERER_API Uniform final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allowed_types = typename detail::pack<
			bool, int32_t, float32_t,
			vec2, vec3, vec4, Color,
			mat2, mat3, mat4,
			struct Texture *
		>;

		using type_t = typename typeof<>;

		using name_t = typename std::string;

		using data_t = typename detail::unpack_t<
			std::variant, allowed_types::apply
		>;

		using storage_t = typename std::tuple<
			type_t, name_t, data_t
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Uniform();
		Uniform(storage_t const & storage);
		Uniform(storage_t && storage) noexcept;
		Uniform(Uniform const & copy);
		Uniform(Uniform && copy) noexcept;
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

		template <class T> inline bool holds() const noexcept
		{
			return std::holds_alternative<T>(data());
		}

		template <class T> inline auto const & store(T && value) noexcept
		{
			return (std::get<2>(m_storage) = std::forward<T>(value));
		}

		template <class T> inline decltype(auto) load() const
		{
			return std::get<T>(data());
		}

		template <class T> inline std::optional<T> evaluate() const
		{
			return holds<T>() ? std::make_optional(load<T>()) : std::nullopt;
		}

		template <class T> inline auto exchange(T && value)
		{
			return std::exchange(std::get<2>(m_storage), std::forward<T>(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union { storage_t m_storage; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Type, class Name, class ... Data
	> static inline auto make_uniform(Name && name, Data && ... args)
	{
		return Uniform{ std::make_tuple(
			typeof_v<Type>,
			std::forward<Name>(name),
			std::forward<Data>(args)...
		) };
	}

	template <class U> static inline auto make_uniform(U && value)
	{
		return Uniform{ std::make_tuple(
			value.type(),
			value.name(),
			value.data()
		) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_