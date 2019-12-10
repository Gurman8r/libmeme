#ifndef _ML_RANDOM_HPP_
#define _ML_RANDOM_HPP_

#include <libmeme/Core/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/Core.hpp>

#define ML_Random ::ml::Random::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Random final : public Singleton<Random>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline T roll() const
		{
			static_assert(std::is_trivial_v<T>);
			return static_cast<T>(std::rand());
		}

		template <class T> inline T roll(T max) const
		{ 
			return (roll<T>() / max);
		}

		template <class T> inline T roll(T min, T max) const
		{
			return (min + (roll<T>(max) * (max - min)));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<Random>;

		Random() { std::srand((uint32_t)std::time(nullptr)); }

		~Random() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RANDOM_HPP_