#ifndef _ML_STREAM_SNIPER_HPP_
#define _ML_STREAM_SNIPER_HPP_

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char, class Al = std::allocator<Ch>
	> struct basic_stream_sniper final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type		= typename basic_stream_sniper		<Ch, Al>;
		using traits_t		= typename std::char_traits			<Ch>;
		using sstream_t		= typename std::basic_stringstream	<Ch, traits_t, Al>;
		using ostream_t		= typename std::basic_ostream		<Ch, traits_t>;
		using streambuf_t	= typename std::basic_streambuf		<Ch, traits_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_stream_sniper(ostream_t * value = {}) noexcept
		{
			(*this)(value);
		}

		basic_stream_sniper(self_type && other) noexcept
		{
			this->swap(std::move(other));
		}

		~basic_stream_sniper() noexcept
		{
			(*this)(nullptr);
		}

		basic_stream_sniper & operator=(self_type && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_cur, other.m_cur);
				std::swap(m_old, other.m_old);
				std::swap(m_str, other.m_str);
			}
		}

		void operator()(ostream_t * value) noexcept
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

		ML_NODISCARD operator bool() const noexcept { return m_cur && m_old; }

		ML_NODISCARD operator sstream_t & () & noexcept { return m_str; }
		
		ML_NODISCARD operator sstream_t const & () const & noexcept { return m_str; }

		ML_NODISCARD operator sstream_t && () && noexcept { return std::move(m_str); }

		ML_NODISCARD operator sstream_t const && () const & noexcept { return std::move(m_str); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		streambuf_t *	m_cur{}; // current stream
		ostream_t *		m_old{}; // previous stream
		sstream_t		m_str{}; // captured data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias stream_sniper = typename basic_stream_sniper<char>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STREAM_SNIPER_HPP_