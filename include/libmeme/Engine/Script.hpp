#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	struct ML_ENGINE_API Script final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Language { Unknown, Python, Lua };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script();
		explicit Script(path_t const & path);
		explicit Script(Language language, std::string const & text);
		Script(Script const & other);
		Script(Script && other) noexcept;
		~Script();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script & operator=(Script const & other);

		Script & operator=(Script && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void assign(Script const & other);

		void swap(Script & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(path_t const & path);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t execute();
		
		int32_t execute(std::vector<std::string> const & args);

		template <class ... Args> inline int32_t operator()(Args && ... args)
		{
			return execute(std::forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto language() const noexcept -> Language { return m_language; }

		inline auto text() const noexcept -> std::string const & { return m_text; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union
		{
			Language m_language;
			std::string m_text;
		};
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_HPP_