#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	struct ML_ENGINE_API Script final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Language
		{
			Unknown, Python, Lua
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script();
		explicit Script(std::string const & filename);
		Script(Script const & copy);
		Script(Script && copy) noexcept;
		~Script();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script & operator=(Script const & other) noexcept;

		Script & operator=(Script && other) noexcept;

		void swap(Script & other);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(std::string const & filename);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto text() const noexcept -> std::string const & { return m_text; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union
		{
			std::string m_text;
		};
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCRIPT_HPP_