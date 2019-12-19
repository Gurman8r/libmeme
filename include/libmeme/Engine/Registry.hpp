#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/StringUtility.hpp>

#define ML_Registry ::ml::Registry<>::getInstance()

#define ML_REGISTER_EX(T, info, factory)			\
	static std::optional<std::any> factory();		\
	bool Registry<T>::s_registered {				\
		ML_Registry.registrate<T>(info, factory)	\
	};												\
	std::optional<std::any> factory()

#define ML_REGISTER(T, info) \
	ML_REGISTER_EX(T, info, ML_CONCAT(ML_FACTORY_, T))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct Registry;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ML_ENGINE_API Registry<T> final
	{
	private: Registry() = delete;
	protected: static bool s_registered;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct ML_ENGINE_API Registry<> final : public Singleton<Registry<>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using code_t = typename hash_t;
		using name_t = typename std::string_view;
		using info_t = typename std::string_view;
		using func_t = typename std::function<std::optional<std::any>()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto codes() const -> const std::unordered_map<name_t, code_t> & { return m_codes; }
		inline auto funcs() const -> const std::unordered_map<name_t, func_t> & { return m_funcs; }
		inline auto infos() const -> const std::unordered_map<name_t, info_t> & { return m_infos; }
		inline auto names() const -> const std::unordered_map<code_t, name_t> & { return m_names; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::optional<std::any> generate(name_t const & name) const
		{
			auto const func{ get_func(name) };
			return func ? std::invoke(func.value()) : std::nullopt;
		}

		inline std::optional<std::any> generate(code_t const & code) const
		{
			auto const func{ get_func(code) };
			return func ? std::invoke(func.value()) : std::nullopt;
		}

		template <class T> inline std::optional<std::any> generate() const
		{
			return generate(hashof_v<T>);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool registrate(name_t const & name, info_t const & info, code_t const & code, func_t const & func)
		{
			if (m_funcs.find(name) == m_funcs.end())
			{
				m_codes.insert({ name, code });
				m_names.insert({ code, name });
				m_funcs.insert({ name, func });
				m_infos.insert({ name, info });
				return true;
			}
			return false;
		}

		template <
			class T, class Info, class Func
		> inline bool registrate(Info && info, Func && func)
		{
			return registrate(
				name_t{ nameof_v<T>.data(), nameof_v<T>.size() },
				std::forward<Info>(info),
				hashof_v<T>,
				std::forward<Func>(func)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::optional<code_t> get_code(name_t const & name) const
		{
			if (auto const it{ m_codes.find(name) }; it != m_codes.cend())
			{
				return std::make_optional(it->second);
			}
			return std::nullopt;
		}

		inline std::optional<func_t> get_func(name_t const & name) const
		{
			if (auto const it{ m_funcs.find(name) }; it != m_funcs.cend())
			{
				return std::make_optional(it->second);
			}
			return std::nullopt;
		}

		inline std::optional<func_t> get_func(code_t code) const
		{
			if (auto const it{ m_names.find(code) }; it != m_names.cend())
			{
				return get_func(it->second);
			}
			return std::nullopt;
		}

		inline std::optional<info_t> get_info(name_t const & name) const
		{
			if (auto const it{ m_infos.find(name) }; it != m_infos.cend())
			{
				return std::make_optional(it->second);
			}
			return std::nullopt;
		}

		inline std::optional<info_t> get_info(code_t code) const
		{
			if (auto const it{ m_names.find(code) }; it != m_names.cend())
			{
			}
			return std::nullopt;
		}

		inline std::optional<name_t> get_name(code_t code) const
		{
			if (auto const it{ m_names.find(code) }; it != m_names.cend())
			{
				return std::make_optional(it->second);
			}
			return std::nullopt;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<Registry<>>;

		Registry() noexcept;

		~Registry();

		union
		{
			std::unordered_map<name_t, code_t> m_codes;
			std::unordered_map<name_t, func_t> m_funcs;
			std::unordered_map<name_t, info_t> m_infos;
			std::unordered_map<code_t, name_t> m_names;
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_