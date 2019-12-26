#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	struct ML_ENGINE_API SharedLibrary final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Functions = typename std::map<std::string, void *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		SharedLibrary();
		explicit SharedLibrary(std::string const & filename);
		SharedLibrary(SharedLibrary && copy) noexcept;
		~SharedLibrary();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		SharedLibrary & operator=(SharedLibrary && other) noexcept;

		SharedLibrary & swap(SharedLibrary & other);

		inline operator bool() const { return m_instance; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();

		bool loadFromFile(std::string const & filename);

		void * loadFunction(std::string const & name);

		template <
			class Ret, class ... Args
		> inline auto invoke(std::string const & name, Args && ... args)
		{
			auto fun{ reinterpret_cast<Ret(*)(Args...)>(this->loadFunction(name)) };
			return (fun ? fun(std::forward<Args>(args)...) : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto instance() const -> void * const & { return m_instance; }

		inline auto functions() const -> Functions const & { return m_functions; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void * m_instance;
		Functions m_functions;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SHARED_LIBRARY_HPP_