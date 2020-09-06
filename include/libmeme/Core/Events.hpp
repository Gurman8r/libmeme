#ifndef _ML_EVENTS_HPP_
#define _ML_EVENTS_HPP_

#include <libmeme/Core/Memory.hpp>

// event declarator helper
#define ML_decl_event(Ev) \
	struct Ev final : _ML impl::event_helper<Ev>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT BASE
	struct ML_NODISCARD event : non_copyable
	{
		constexpr operator hash_t() const noexcept { return m_id; }

	protected:
		constexpr explicit event(hash_t id) noexcept : m_id{ id }
		{
		}

		hash_t const m_id;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT HELPER
	namespace impl
	{
		template <class T
		> struct event_helper : event
		{
			enum : hash_t { ID = hashof_v<T> };

			constexpr event_helper() noexcept : event{ ID }
			{
			}
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT LISTENER
	struct ML_CORE_API event_listener
	{
		explicit event_listener(struct event_bus * bus) : m_event_bus{ bus } {}

		virtual ~event_listener() noexcept; // EOF

		virtual void on_event(event const &) = 0;

	protected:
		friend struct event_bus;

		struct event_bus * const m_event_bus;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT BUS
	struct event_bus : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using category = typename ds::set<event_listener *>;

		event_bus(allocator_type alloc = {}) noexcept : m_categories{ alloc }
		{
		}

		event_bus(event_bus && other, allocator_type alloc = {}) noexcept : event_bus{ alloc }
		{
			m_categories.swap(std::move(other.m_categories));
		}

		virtual ~event_bus() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// fire event
		void fire(event const & ev) noexcept
		{
			// get category
			if (auto const cat{ m_categories.find(ev) })
			{
				// for each listener
				for (auto const & l : (*cat->second))
				{
					// handle event
					l->on_event(ev);
				}
			}
		}

		// fire type
		template <class Ev, class ... Args
		> void fire(Args && ... args) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			this->fire(Ev{ ML_forward(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// subscribe to id
		bool sub(hash_t type, event_listener * listener) noexcept
		{
			// insert listener into category
			return listener && m_categories[type].insert(listener).second;
		}

		// subscribe to type
		template <class Ev
		> bool sub(event_listener * listener) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return this->sub(Ev::ID, listener);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// unsubscribe from id
		void unsub(hash_t type, event_listener * listener) noexcept
		{
			if (!listener) { return; }

			// get category
			if (auto const cat{ m_categories.find(type) })
			{
				// get listener
				if (auto const l{ cat->second->find(listener) }; l != cat->second->end())
				{
					// remove listener
					cat->second->erase(l);
				}
			}
		}

		// unsubscribe from type
		template <class Ev
		> void unsub(event_listener * listener) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			this->unsub(Ev::ID, listener);
		}

		// unsubscribe from all
		void unsub(event_listener * listener) noexcept
		{
			if (!listener) { return; }

			// for each category
			m_categories.for_each([&](hash_t, category & cat) noexcept
			{
				// get listener
				if (auto const l{ cat.find(listener) }; l != cat.end())
				{
					// remove listener
					cat.erase(l);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::map<hash_t, category> m_categories{}; // categories

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// remove listener from all events
	inline event_listener::~event_listener() noexcept
	{
		ML_assert("event bus does not exist" && m_event_bus);

		m_event_bus->unsub(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENTS_HPP_