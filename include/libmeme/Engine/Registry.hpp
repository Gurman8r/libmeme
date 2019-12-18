#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/StringUtility.hpp>

#define ML_Registry ::ml::Registry<>::getInstance()

#define ML_REGISTER_EX(T, info, func)			\
	static std::optional<std::any> func();					\
	bool Registry<T>::s_registered {			\
		ML_Registry.registrate<T>(info, func)	\
	};											\
	std::optional<std::any> func()

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

		using Code = typename hash_t;
		using Name = typename std::string;
		using Info = typename std::string;
		using Func = typename std::function<std::optional<std::any>()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto codes() const -> const std::unordered_map<Name, Code> & { return m_codes; }
		inline auto funcs() const -> const std::unordered_map<Name, Func> & { return m_funcs; }
		inline auto infos() const -> const std::unordered_map<Name, Info> & { return m_infos; }
		inline auto names() const -> const std::unordered_map<Code, Name> & { return m_names; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::optional<std::any> generate(Name const & name) const
		{
			auto const func{ get_func(name) };
			return func ? (*func)() : std::nullopt;
		}

		inline std::optional<std::any> generate(Code const & code) const
		{
			auto const func{ get_func(code) };
			return func ? (*func)() : std::nullopt;
		}

		template <class T> inline T * generate() const
		{
			return static_cast<T *>(generate(typeof<T>::name()));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool registrate(Name const & name, Info const & info, Code const & code, Func const & func)
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

		template <class T, class F> inline bool registrate(Info const & info, F && func)
		{
			return registrate(
				nameof_v<T>.str(), info, hashof_v<T>, std::forward<F>(func)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::optional<Code> get_code(Name const & name) const
		{
			auto const it{ m_codes.find(name) };
			return (it != m_codes.end()) ? std::make_optional(it->second) : std::nullopt;
		}

		inline std::optional<Func> get_func(Name const & name) const
		{
			auto const it{ m_funcs.find(name) };
			return (it != m_funcs.end()) ? std::make_optional(it->second) : std::nullopt;
		}

		inline std::optional<Func> get_func(Code code) const
		{
			auto const it{ m_names.find(code) };
			return (it != m_names.end()) ? get_func(it->second) : std::nullopt;
		}

		inline std::optional<Info> get_info(Name const & name) const
		{
			auto const it{ m_infos.find(name) };
			return (it != m_infos.end()) ? std::make_optional(it->second) : std::nullopt;
		}

		inline std::optional<Info> get_info(Code code) const
		{
			auto const it{ m_names.find(code) };
			return (it != m_names.end()) ? get_info(it->second) : std::nullopt;
		}

		inline std::optional<Name> get_name(Code code) const
		{
			auto const it{ m_names.find(code) };
			return (it != m_names.end()) ? std::make_optional(it->second) : std::nullopt;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<Registry<>>;

		Registry()
			: m_codes(), m_funcs(), m_infos(), m_names()
		{
		}

		~Registry() {}

		std::unordered_map<std::string, Code>	m_codes; // 
		std::unordered_map<std::string, Func>	m_funcs; // 
		std::unordered_map<std::string, std::string> m_infos; // 
		std::unordered_map<Code, std::string> m_names; // 
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_