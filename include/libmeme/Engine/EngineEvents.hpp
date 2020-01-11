#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <libmeme/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class EngineEventType
	{
		MIN_ENGINE_EVENT = Event::EV_ENGINE,

		EV_Enter,		// 
		EV_Load,		// 
		EV_BeginLoop,	// 
		EV_Update,		// 
		EV_Draw,		// 
		EV_EndLoop,		// 
		EV_Unload,		// 
		EV_Exit,		// 

		MAX_ENGINE_EVENT
	};

	static_assert(
		(int32_t)EngineEventType::MAX_ENGINE_EVENT < 
		(int32_t)EngineEventType::MIN_ENGINE_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
	);

	template <EngineEventType ID> struct EngineEvent : public T_Event<EngineEventType, ID> 
	{
	};


	// Startup
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct EnterEvent final : public EngineEvent<EngineEventType::EV_Enter>
	{
		int32_t const argc;
		C_String const * argv;

		constexpr EnterEvent(int32_t const argc, C_String const * argv) noexcept
			: argc{ argc }
			, argv{ argv }
		{
		}
	};

	struct LoadEvent final : public EngineEvent<EngineEventType::EV_Load>
	{
		constexpr LoadEvent() {}
	};


	// Loop
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BeginLoopEvent final : public EngineEvent<EngineEventType::EV_BeginLoop>
	{
		constexpr BeginLoopEvent() {}
	};

	struct UpdateEvent final : public EngineEvent<EngineEventType::EV_Update>
	{
		constexpr UpdateEvent() {}
	};

	struct DrawEvent final : public EngineEvent<EngineEventType::EV_Draw>
	{
		constexpr DrawEvent() {}
	};

	struct EndLoopEvent final : public EngineEvent<EngineEventType::EV_EndLoop>
	{
		constexpr EndLoopEvent() {}
	};


	// Shutdown
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct UnloadEvent final : public EngineEvent<EngineEventType::EV_Unload>
	{
		constexpr UnloadEvent() {}
	};

	struct ExitEvent final : public EngineEvent<EngineEventType::EV_Exit>
	{
		constexpr ExitEvent() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_