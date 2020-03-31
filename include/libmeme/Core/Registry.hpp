#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <libmeme/Core/Memory.hpp>
#include <libmeme/Core/StringUtility.hpp>

#define ML_REGISTER_EX(T, factory)					\
	static std::optional<std::any> factory();		\
	bool ml::registry<T>::s_registered {			\
		ml::registrar::registrate<T>(factory)		\
	};												\
	std::optional<std::any> factory()

#define ML_REGISTER(T) \
	ML_REGISTER_EX(T, ML_CONCAT(ML_FACTORY_, T))

// example:
// ML_REGISTER(MyType) { return std::make_optional(MyType{}); }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T
	> struct registry;

	template <
	> struct registry<> { registry() = delete; };

	template <class T
	> struct ML_CORE_API registry<T> final
	{
	private: static bool s_registered;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API registrar final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using code_type = typename size_t;
		using name_type = typename std::string_view;
		using func_type = typename std::function<std::optional<std::any>()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Codes, ID_Names, ID_Factories };

		using storage_type = typename std::tuple<
			ds::flat_map<name_type, code_type>,	// Codes
			ds::flat_map<code_type, name_type>,	// Names
			ds::flat_map<name_type, func_type>	// Factories
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static decltype(auto) codes() noexcept { return std::get<ID_Codes>(m_storage); }
		
		static decltype(auto) names() noexcept { return std::get<ID_Names>(m_storage); }
		
		static decltype(auto) factories() noexcept { return std::get<ID_Factories>(m_storage); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static std::optional<std::any> generate(name_type const & name)
		{
			auto const fn{ get_factory(name) };
			return fn ? std::invoke(fn.value()) : std::nullopt;
		}

		static std::optional<std::any> generate(code_type const & code)
		{
			auto const fn{ get_factory(code) };
			return fn ? std::invoke(fn.value()) : std::nullopt;
		}

		template <class T> static std::optional<std::any> generate()
		{
			return generate(hashof_v<T>);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool registrate(name_type const & name, code_type const & code, func_type const & fn)
		{
			if (factories().contains(name)) { return false; }
			std::get<ID_Codes>(m_storage).insert(name, code);
			std::get<ID_Names>(m_storage).insert(code, name);
			std::get<ID_Factories>(m_storage).insert(name, fn);
			return true;
		}

		template <class T, class Fn
		> static bool registrate(Fn fn)
		{
			return registrate(nameof_v<T>, hashof_v<T>, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static std::optional<code_type> get_code(name_type const & name)
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

		static std::optional<name_type> get_name(code_type code)
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

		static std::optional<func_type> get_factory(name_type const & name)
		{
			if (auto const it{ factories().find(name) })
			{
				return std::make_optional(*it->second);
			}
			else
			{
				return std::nullopt;
			}
		}

		static std::optional<func_type> get_factory(code_type code)
		{
			if (auto const it{ names().find(code) })
			{
				return get_factory(*it->second);
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static storage_type m_storage;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_