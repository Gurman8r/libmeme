#ifndef _ML_EVENTS_HPP_
#define _ML_EVENTS_HPP_

#include <libmeme/Core/Memory.hpp>

// event declarator helper
#define ML_decl_event(Type) \
	struct Type final : _ML impl::event_helper<Type>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TYPES
	struct event;
	struct event_listener;
	struct event_bus;

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
		template <class Derived
		> struct event_helper : event
		{
			enum : hash_t { ID = hashof_v<Derived> };

			constexpr event_helper() noexcept : event{ ID }
			{
			}
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT LISTENER
	struct ML_CORE_API event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit event_listener(event_bus * bus) : m_bus{ bus }
		{
			ML_assert("INVALID EVENT BUS" && bus);
		}

		virtual ~event_listener() noexcept; // EOF

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ev
		> bool subscribe() noexcept
		{
			return ML_check(m_bus)->add_listener<Ev>(this);
		}

		template <class ... Args
		> bool subscribe(Args && ... args) noexcept
		{
			return ML_check(m_bus)->add_listener(ML_forward(args)..., this);
		}

		template <class Ev
		> void unsubscribe() noexcept
		{
			return ML_check(m_bus)->remove_listener<Ev>(this);
		}

		template <class ... Args
		> void unsubscribe(Args && ... args) noexcept
		{
			return ML_check(m_bus)->remove_listener(ML_forward(args)..., this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		event_bus * const m_bus;

	private:
		friend event_bus;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT BUS
	struct event_bus final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using category = typename ds::set<event_listener *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit event_bus(allocator_type alloc) noexcept : m_categories{ alloc }
		{
		}

		~event_bus() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void fire(event const & ev) noexcept
		{
			if (auto const cat{ m_categories.find(ev) })
			{
				for (auto const & listener : (*cat->second))
				{
					listener->on_event(ev);
				}
			}
		}

		template <class Ev, class ... Args
		> void fire(Args && ... args) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			this->fire(Ev{ ML_forward(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool add_listener(hash_t id, event_listener * value) noexcept
		{
			return value
				&& this == value->m_bus
				&& m_categories[id].insert(value).second;
		}

		template <class Ev
		> bool add_listener(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return this->add_listener(Ev::ID, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void remove_listener(hash_t id, event_listener * value) noexcept
		{
			if (!value || (this != value->m_bus)) { return; }

			if (auto const cat{ m_categories.find(id) })
			{
				if (auto const listener{ cat->second->find(value) }; listener != cat->second->end())
				{
					cat->second->erase(listener);
				}
			}
		}

		template <class Ev
		> void remove_listener(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			this->remove_listener(Ev::ID, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void remove_listener(event_listener * value) noexcept
		{
			if (!value) { return; }

			m_categories.for_each([&](hash_t, category & cat) noexcept
			{
				if (auto const listener{ cat.find(value) }; listener != cat.end())
				{
					cat.erase(listener);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ds::map<hash_t, category> m_categories{}; // categories

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ~event_listener
	inline event_listener::~event_listener() noexcept
	{
		unsubscribe(); // remove listener from all events
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENTS_HPP_