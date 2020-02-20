#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_ENGINE_API script final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		enum language : int32_t { unknown, python, lua };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script();
		
		explicit script(allocator_type const & alloc);
		
		script(fs::path const & path, allocator_type const & alloc = {});
		
		script(int32_t lang, pmr::string const & text, allocator_type const & alloc = {});
		
		script(script const & other, allocator_type const & alloc = {});
		
		script(script && other, allocator_type const & alloc = {}) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script & operator=(script const & other);

		script & operator=(script && other) noexcept;

		void swap(script & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t execute(pmr::vector<pmr::string> const & args = {});

		template <class ... Args
		> inline int32_t operator()(Args && ... args)
		{
			return execute({ ML_FWD(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const noexcept { return !m_text.empty(); }

		inline auto lang() const noexcept -> int32_t { return m_lang; }

		inline auto str() const noexcept -> pmr::string const & { return m_text; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t		m_lang;
		pmr::string m_text;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_HPP_