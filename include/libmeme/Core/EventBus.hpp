#ifndef _ML_EVENT_BUS_HPP_
#define _ML_EVENT_BUS_HPP_

#include <libmeme/Core/Memory.hpp>

// event declarator helper
#define ML_decl_event(type) \
	struct type final : _ML impl::event_helper<type>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT
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
		template <class T> struct event_helper : event
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

		virtual ~event_listener() noexcept;

		virtual void on_event(event const &) = 0;

	private:
		struct event_bus * const m_event_bus;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT BUS
	struct event_bus : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		event_bus(allocator_type alloc = {}) noexcept : m_subs{ alloc }
		{
		}

		event_bus(event_bus && other, allocator_type alloc = {}) noexcept : event_bus{ alloc }
		{
			m_subs.swap(std::move(other.m_subs));
		}

		virtual ~event_bus() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// fire event
		void fire(event const & ev) noexcept
		{
			// get category
			if (auto const c{ m_subs.find(ev) })
			{
				// for each listener
				for (auto const & l : (*c->second))
				{
					// handle event
					l->on_event(ev);
				}
			}
		}

		// fire templated
		template <class Ev, class ... Args
		> void fire(Args && ... args) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return this->fire(Ev{ ML_forward(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// subscribe to type
		bool sub(event_listener * value, hash_t id) noexcept
		{
			// insert listener into category
			return value && m_subs[id].insert(value).second;
		}

		// subscribe templated
		template <class Ev
		> bool sub(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return this->sub(value, Ev::ID);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// unsubscribe from type
		void unsub(event_listener * value, hash_t id) noexcept
		{
			if (!value) { return; }

			// get category
			if (auto const c{ m_subs.find(id) })
			{
				// get listener
				if (auto const l{ c->second->find(value) }; l != c->second->end())
				{
					// remove listener
					c->second->erase(l);
				}
			}
		}

		// unsubscribe templated
		template <class Ev
		> void unsub(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			this->unsub(value, Ev::ID);
		}

		// unsubscribe from all
		void unsub(event_listener * value) noexcept
		{
			if (!value) { return; }

			// for each category
			m_subs.for_each([&](auto, auto & c) noexcept
			{
				// get listener
				if (auto const l{ c.find(value) }; l != c.end())
				{
					// remove listener
					c.erase(l);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::map<hash_t, ds::set<event_listener *>> m_subs{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline event_listener::~event_listener() noexcept
	{
		ML_assert("event bus does not exist" && m_event_bus);

		m_event_bus->unsub(this); // remove listener from all events
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_