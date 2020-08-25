#ifndef _ML_EVENT_BUS_HPP_
#define _ML_EVENT_BUS_HPP_

#include <libmeme/System/Export.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/NonCopyable.hpp>
#include <libmeme/Core/Singleton.hpp>

// event declarator helper
#define ML_decl_event(type) \
	struct type final : _ML impl::event_helper<type>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct event_bus;

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

	namespace impl
	{
		// EVENT HELPER
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
	struct ML_SYSTEM_API event_listener
	{
		explicit event_listener(event_bus * bus) : m_event_bus{ bus } {}

		virtual ~event_listener() noexcept;

		virtual void on_event(event const &) = 0;

	protected:
		friend event_bus;

		event_bus * const m_event_bus;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT BUS
	struct event_bus : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		event_bus(allocator_type alloc = {}) noexcept : m_listeners{ alloc }
		{
		}

		event_bus(event_bus && other, allocator_type alloc = {}) noexcept : event_bus{ alloc }
		{
			m_listeners.swap(std::move(other.m_listeners));
		}

		virtual ~event_bus() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void fire(event const & ev) noexcept
		{
			// get category
			if (auto const c{ m_listeners.find(ev) })
			{
				// for each listener
				for (auto const & l : (*c->second))
				{
					// handle event
					l->on_event(ev);
				}
			}
		}

		template <class Ev, class ... Args
		> void fire(Args && ... args) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");
			return fire(Ev{ ML_forward(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool add_listener(hash_t type, event_listener * value) noexcept
		{
			// insert listener into category
			return value && m_listeners[type].insert(value).second;
		}
		
		template <class Ev
		> bool add_listener(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");
			return add_listener(Ev::ID, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void remove_listener(hash_t type, event_listener * value) noexcept
		{
			if (!value) { return; }

			// get category
			if (auto const c{ m_listeners.find(type) })
			{
				// get listener
				if (auto const l{ c->second->find(value) }; l != c->second->end())
				{
					// remove listener
					c->second->erase(l);
				}
			}
		}

		void remove_listener(event_listener * value) noexcept
		{
			if (!value) { return; }

			// for each category
			m_listeners.for_each([&](auto, auto & c) noexcept
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
		ds::map<hash_t, ds::set<event_listener *>> m_listeners{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline event_listener::~event_listener() noexcept
	{
		m_event_bus->remove_listener(this); // remove listener from all events
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENT_SYSTEM_HPP_