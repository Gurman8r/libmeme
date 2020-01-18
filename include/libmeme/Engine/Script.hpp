#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Platform/FileSystem.hpp>

namespace ml
{
	struct ML_ENGINE_API Script final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		enum class Language { Unknown, Python, Lua };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script();
		
		explicit Script(allocator_type const & alloc);
		
		Script(path_t const & path, allocator_type const & alloc = {});
		
		Script(Language language, pmr::string const & text, allocator_type const & alloc = {});
		
		Script(Script const & other, allocator_type const & alloc = {});
		
		Script(Script && other, allocator_type const & alloc = {}) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script & operator=(Script const & other);

		Script & operator=(Script && other) noexcept;

		void swap(Script & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & path);

		int32_t execute();
		
		int32_t execute(pmr::vector<pmr::string> const & args);

		template <class ... Args> inline int32_t operator()(Args && ... args)
		{
			return execute(std::forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto language() const noexcept -> Language { return m_language; }

		inline auto text() const noexcept -> pmr::string const & { return m_text; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Language m_language;

		pmr::string m_text;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_HPP_