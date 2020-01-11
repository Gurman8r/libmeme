#include <libmeme/Core/EventHandler.hpp>
#include <libmeme/Core/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventListener * EventHandler::Bank::addListener(int32_t const id, EventHandler * value)
	{
		return EventSystem::addListener(id, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventHandler::EventHandler(target_type const & target)
		: m_target{ target }
	{
	}

	EventHandler::EventHandler(EventHandler const & other)
		: m_target{ other.m_target }
	{
	}

	EventHandler::EventHandler(EventHandler && other) noexcept
		: m_target{}
	{
		swap(std::move(other));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventHandler & EventHandler::operator=(EventHandler const & other)
	{
		EventHandler temp{ other };
		swap(temp);
		return (*this);
	}

	EventHandler & EventHandler::operator=(EventHandler && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void EventHandler::swap(EventHandler & other) noexcept
	{
		if (this != std::addressof(other))
		{
			m_target.swap(other.m_target);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}