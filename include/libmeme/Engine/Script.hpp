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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & path);

		void swap(script & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t execute() noexcept;

		inline auto operator()() noexcept
		{
			return execute();
		}

		inline auto operator()(fs::path const & path) noexcept
		{
			load_from_file(path);
			return execute();
		}

		inline auto operator()(int32_t lang, pmr::string && text) noexcept
		{
			m_lang = lang;
			m_text = ML_FWD(text);
			return execute();
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