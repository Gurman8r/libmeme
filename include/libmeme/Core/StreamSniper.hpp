#ifndef _ML_STREAM_SNIPER_HPP_
#define _ML_STREAM_SNIPER_HPP_

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char, class Tr = std::char_traits<Ch>, class Al = std::allocator<Ch>
	> struct basic_stream_sniper final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type		= typename basic_stream_sniper		<Ch, Tr, Al>;
		using sstream_t		= typename std::basic_stringstream	<Ch, Tr, Al>;
		using ostream_t		= typename std::basic_ostream		<Ch, Tr>;
		using streambuf_t	= typename std::basic_streambuf		<Ch, Tr>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_stream_sniper(ostream_t * value = {}) noexcept
		{
			operator()(value);
		}

		basic_stream_sniper(self_type && other) noexcept
		{
			swap(std::move(other));
		}

		~basic_stream_sniper() noexcept
		{
			operator()(nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline basic_stream_sniper & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_cur, other.m_cur);
				std::swap(m_old, other.m_old);
				m_str.swap(other.m_str);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void operator()(ostream_t * value) noexcept
		{
			if (value && !m_cur && !m_old)
			{
				m_old = value;
				m_cur = m_old->rdbuf(m_str.rdbuf());
				m_str.str({});
			}
			else if (!value && m_cur && m_old)
			{
				m_old->rdbuf(m_cur);
				m_old = nullptr;
				m_cur = nullptr;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept { return m_cur && m_old; }

		ML_NODISCARD inline operator sstream_t & () & noexcept { return m_str; }
		
		ML_NODISCARD inline operator sstream_t const & () const & noexcept { return m_str; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		streambuf_t *	m_cur{}; // current stream
		ostream_t *		m_old{}; // previous stream
		sstream_t		m_str{}; // captured data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_ALIAS stream_sniper = typename basic_stream_sniper<char>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STREAM_SNIPER_HPP_