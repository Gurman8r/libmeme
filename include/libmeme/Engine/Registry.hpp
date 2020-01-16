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

		using code_t = typename size_t;
		using name_t = typename std::string_view;
		using info_t = typename std::string_view;
		using func_t = typename std::function<std::optional<std::any>()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Codes, ID_Funcs, ID_Infos, ID_Names };

		using storage_type = typename std::tuple<
			ds::flat_map<name_t, code_t>, // Codes
			ds::flat_map<name_t, func_t>, // Funcs
			ds::flat_map<name_t, info_t>, // Infos
			ds::flat_map<code_t, name_t>  // Names
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline decltype(auto) codes() const noexcept { return std::get<ID_Codes>(m_storage); }
		inline decltype(auto) funcs() const noexcept { return std::get<ID_Funcs>(m_storage); }
		inline decltype(auto) infos() const noexcept { return std::get<ID_Infos>(m_storage); }
		inline decltype(auto) names() const noexcept { return std::get<ID_Names>(m_storage); }

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
			return generate(hashof_t<T>);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool registrate(name_t const & name, info_t const & info, code_t const & code, func_t const & func)
		{
			if (funcs().contains(name)) { return false; }
			std::get<ID_Codes>(m_storage).insert(name, code);
			std::get<ID_Funcs>(m_storage).insert(name, func);
			std::get<ID_Infos>(m_storage).insert(name, info);
			std::get<ID_Names>(m_storage).insert(code, name);
			return true;
		}

		template <class T, class Info, class Func
		> inline bool registrate(Info && info, Func && func)
		{
			return registrate(
				nameof_t<T>, std::forward<Info>(info), hashof_t<T>, std::forward<Func>(func)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::optional<code_t> get_code(name_t const & name) const
		{
			if (auto const it{ codes().find(name) })
			{
				return std::make_optional(*it->second);
			}
			else
			{
				return std::nullopt;
			}
		}

		inline std::optional<func_t> get_func(name_t const & name) const
		{
			if (auto const it{ funcs().find(name) })
			{
				return std::make_optional(*it->second);
			}
			else
			{
				return std::nullopt;
			}
		}

		inline std::optional<func_t> get_func(code_t code) const
		{
			if (auto const it{ names().find(code) })
			{
				return get_func(*it->second);
			}
			else
			{
				return std::nullopt;
			}
		}

		inline std::optional<info_t> get_info(name_t const & name) const
		{
			if (auto const it{ infos().find(name) })
			{
				return std::make_optional(*it->second);
			}
			else
			{
				return std::nullopt;
			}
		}

		inline std::optional<name_t> get_name(code_t code) const
		{
			if (auto const it{ names().find(code) })
			{
				return std::make_optional(*it->second);
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<Registry<>>;

		Registry() noexcept;

		~Registry();

		storage_type m_storage;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_