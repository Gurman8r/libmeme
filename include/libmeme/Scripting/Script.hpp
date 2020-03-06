#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <libmeme/Scripting/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_SCRIPTING_API script final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class api final
		{
		public:
			enum language { unknown, lua, python };

			template <class Str = cstring
			> static constexpr int32_t language_ext(Str const & str) noexcept
			{
				switch (util::hash(str))
				{
				default: return language::unknown;

				case util::hash(".lua"): return language::lua;

				case util::hash(".py"): return language::python;
				}
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit script(allocator_type const & alloc = {});
		
		script(filesystem::path const & path, allocator_type const & alloc = {});
		
		script(int32_t lang, pmr::string const & text, allocator_type const & alloc = {});
		
		script(script const & other, allocator_type const & alloc = {});
		
		script(script && other, allocator_type const & alloc = {}) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script & operator=(script const & other);

		script & operator=(script && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		script & load_from_file(filesystem::path const & path);

		void swap(script & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static int32_t startup();

		static int32_t shutdown();

		static int32_t execute(int32_t lang, pmr::string const & text) noexcept;

		static int32_t execute(filesystem::path const & path) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline int32_t execute() noexcept
		{
			return execute(m_lang, m_text);
		}

		inline int32_t operator()() noexcept
		{
			return execute();
		}

		inline int32_t operator()(filesystem::path const & path) noexcept
		{
			load_from_file(path);
			return execute();
		}

		inline int32_t operator()(int32_t lang, pmr::string && text) noexcept
		{
			return set_lang(lang).set_text(ML_FWD(text)).execute();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept
		{
			return ((m_lang != script::api::unknown) && !m_text.empty());
		}

		ML_NODISCARD inline auto get_lang() const noexcept -> int32_t { return m_lang; }

		ML_NODISCARD inline auto get_text() const noexcept -> pmr::string const & { return m_text; }

		inline auto set_lang(int32_t value) noexcept -> script & { m_lang = value; return (*this); }

		inline auto set_text(pmr::string && value) noexcept -> script & { m_text = ML_FWD(value); return (*this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t		m_lang;
		pmr::string m_text;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_HPP_