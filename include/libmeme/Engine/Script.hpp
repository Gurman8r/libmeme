#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Platform/FileSystem.hpp>

namespace ml
{
	struct ML_ENGINE_API script final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		enum language { unknown, python, lua };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script();
		
		explicit script(allocator_type const & alloc);
		
		script(path_t const & path, allocator_type const & alloc = {});
		
		script(language lang, pmr::string const & text, allocator_type const & alloc = {});
		
		script(script const & other, allocator_type const & alloc = {});
		
		script(script && other, allocator_type const & alloc = {}) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script & operator=(script const & other);

		script & operator=(script && other) noexcept;

		void swap(script & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t execute();
		
		int32_t execute(pmr::vector<pmr::string> const & args);

		template <class ... Args
		> inline int32_t operator()(Args && ... args)
		{
			return execute({ std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const noexcept { return !m_str.empty(); }

		inline auto lang() const noexcept -> language { return m_lang; }

		inline auto str() const noexcept -> pmr::string const & { return m_str; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		language m_lang;

		pmr::string m_str;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_HPP_