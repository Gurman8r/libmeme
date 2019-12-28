#include <libmeme/Engine/SharedLibrary.hpp>
#include <libmeme/Core/FileSystem.hpp>

#ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	define ML_LOAD_LIB(file) LoadLibraryA(file)
#	define ML_FREE_LIB(inst) FreeLibrary(static_cast<HINSTANCE>(inst))
#	define ML_LOAD_FUN(inst, name) GetProcAddress(static_cast<HINSTANCE>(inst), name)
#else
#	define ML_LOAD_LIB(file) dlopen(file, RTLD_LOCAL | RTLD_LAZY)
#	define ML_FREE_LIB(inst) (inst && !(inst = nullptr))
#	define ML_LOAD_FUN(inst, name) dlsym(inst, name)
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SharedLibrary::SharedLibrary()
		: m_instance{ nullptr }
		, m_functions{}
	{
	}
	
	SharedLibrary::SharedLibrary(path_t const & path)
		: SharedLibrary{}
	{
		loadFromFile(path);
	}

	SharedLibrary::SharedLibrary(SharedLibrary && copy) noexcept
		: SharedLibrary{}
	{
		swap(std::move(copy));
	}

	SharedLibrary::~SharedLibrary()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SharedLibrary & SharedLibrary::operator=(SharedLibrary && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void SharedLibrary::swap(SharedLibrary & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_instance, other.m_instance);

			std::swap(m_functions, other.m_functions);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool SharedLibrary::dispose()
	{
		m_functions.clear();
		
		return ML_FREE_LIB(m_instance);
	}

	bool SharedLibrary::loadFromFile(path_t const & path)
	{
		return (m_instance = ML_LOAD_LIB(path.string().c_str()));
	}

	void * SharedLibrary::loadFunction(std::string const & name)
	{
		if (auto it{ m_functions.find(name) }; it != m_functions.end())
		{
			return it->second;
		}
		else if (void * ptr{ ML_LOAD_FUN(m_instance, name.c_str()) })
		{
			return m_functions.insert(std::make_pair(name, ptr)).first->second;
		}
		else
		{
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}