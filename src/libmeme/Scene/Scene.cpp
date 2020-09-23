#include <libmeme/Scene/Scene.hpp>

namespace ml
{
	void testign()
	{
		struct C1 {}; struct C2 {};
		scene s{};
		entity e{ s.create("") };
		e.add<C1>();
		s->group<C1, C2>();
		s->view<C1>();
	}
}