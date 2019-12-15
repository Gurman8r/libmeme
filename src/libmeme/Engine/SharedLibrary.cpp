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
	
	SharedLibrary::SharedLibrary(_STD string const & filename)
		: SharedLibrary{}
	{
		this->loadFromFile(filename);
	}

	SharedLibrary::SharedLibrary(SharedLibrary && copy) noexcept
		: SharedLibrary{}
	{
		this->swap(_STD move(copy));
	}

	SharedLibrary::~SharedLibrary()
	{
		this->dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SharedLibrary & SharedLibrary::operator=(SharedLibrary && other) noexcept
	{
		return this->swap(_STD move(other));
	}

	SharedLibrary & SharedLibrary::swap(SharedLibrary & other)
	{
		if (this != _STD addressof(other))
		{
			_STD swap(m_instance, other.m_instance);

			m_functions.swap(other.m_functions);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool SharedLibrary::dispose()
	{
		m_functions.clear();
		return ML_FREE_LIB(m_instance);
	}

	bool SharedLibrary::loadFromFile(_STD string const & filename)
	{
		return (m_instance = ML_LOAD_LIB(filename.c_str()));
	}

	void * SharedLibrary::loadFunction(_STD string const & name)
	{
		if (auto it{ m_functions.find(name) }; it != m_functions.end())
		{
			return it->second;
		}
		else if (void * ptr{ ML_LOAD_FUN(m_instance, name.c_str()) })
		{
			return m_functions.insert(_STD make_pair(name, ptr)).first->second;
		}
		else
		{
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}