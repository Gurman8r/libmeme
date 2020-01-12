#ifndef _ML_EVENT_HANDLER_HPP_
#define _ML_EVENT_HANDLER_HPP_

#include <libmeme/Core/EventListener.hpp>
#include <libmeme/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API EventHandler final : public Trackable, public EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_CORE_API Pool final : public Trackable, public NonCopyable
		{
			Pool() noexcept : m_handlers{} {}

			EventListener * add_listener(int32_t const id, EventHandler * value);

			template <class Ev, class ... Args
			> inline EventListener * add_listener(Args && ... args)
			{
				return add_listener(Ev::ID, &m_handlers.emplace_back(std::forward<Args>(args)...));
			}

		private:
			std::vector<EventHandler> m_handlers;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using target_type = typename std::function<void(Event const &)>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		EventHandler(target_type const & target);

		EventHandler(EventHandler const & other);

		EventHandler(EventHandler && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		EventHandler & operator=(EventHandler const & other);

		EventHandler & operator=(EventHandler && other) noexcept;

		void swap(EventHandler & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void onEvent(Event const & value) override
		{
			return std::invoke(m_target, value);
		}

		inline void operator()(Event const & value)
		{
			return onEvent(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		target_type m_target;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_HANDLER_HPP_