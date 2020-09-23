#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <libmeme/Client/ClientAPI.hpp>

#ifndef ML_PLUGIN_API
#define ML_PLUGIN_API ML_API_EXPORT
#endif

namespace ml
{
	struct application;

	ML_decl_handle(plugin_id);

	struct ML_CLIENT_API plugin : system_object<plugin>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit plugin(application * app, void * user);

		virtual ~plugin() noexcept override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event const &) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_app()	const noexcept -> application * { return m_app; }

		ML_NODISCARD auto get_user() const noexcept -> void * { return m_user; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		application * const	m_app	; // application pointer
		void * const		m_user	; // user pointer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PLUGIN_HPP_