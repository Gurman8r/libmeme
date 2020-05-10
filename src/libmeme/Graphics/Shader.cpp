#include <libmeme/Graphics/Shader.hpp>
#include <libmeme/Graphics/Texture.hpp>
#include <libmeme/Graphics/GL.hpp>
#include <libmeme/Core/FileUtility.hpp>

namespace ml
{
	struct shader::uniform_binder final : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum { unknown = -1 };

		union { int32_t location; uint32_t current; uint32_t previous; };

		inline operator bool() const noexcept { return (unknown < location); }

		inline operator int32_t() const noexcept { return location; }

		template <class Fn
		> uniform_binder(shader & s, pmr::string const & name, Fn && fn) noexcept
			: location{ unknown }, current{}, previous{}
		{
			if (current = s.m_handle)
			{
				previous = GL::getProgramHandle(GL::ProgramObject);

				if (current != previous)
				{
					GL::useProgram(current);
				}

				location = s.get_uniform_location(name);
			}
			if (*this)
			{
				std::invoke(ML_forward(fn), *this);
			}
		}

		~uniform_binder() noexcept
		{
			if (current && (current != previous))
			{
				GL::useProgram(previous);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shader::shader(allocator_type alloc)
		: m_handle		{}
		, m_source		{ std::allocator_arg, alloc }
		, m_attributes	{ alloc }
		, m_textures	{ alloc }
		, m_uniforms	{ alloc }
	{
	}

	shader::shader(shader_source const & source, allocator_type alloc)
		: shader{ alloc }
	{
		(void)load_from_source(source);
	}

	shader::shader(fs::path const & v, fs::path const & f, allocator_type alloc)
		: shader{ alloc }
	{
		(void)load_from_file(v, f);
	}

	shader::shader(fs::path const & v, fs::path const & g, fs::path const & f, allocator_type alloc)
		: shader{ alloc }
	{
		(void)load_from_file(v, g, f);
	}

	shader::shader(shader const & value, allocator_type alloc)
		: shader{ alloc }
	{
		(void)load_from_source(value.m_source);
	}

	shader::shader(shader && value, allocator_type alloc) noexcept
		: shader{ alloc }
	{
		swap(std::move(value));
	}

	shader::~shader() noexcept
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shader::load_from_file(fs::path const & v_file, fs::path const & f_file)
	{
		return load_from_memory(
			util::get_file_string(v_file),
			util::get_file_string(f_file)
		);
	}

	bool shader::load_from_file(fs::path const & v_file, fs::path const g_file, fs::path const & f_file)
	{
		return load_from_memory(
			util::get_file_string(v_file),
			util::get_file_string(g_file),
			util::get_file_string(f_file)
		);
	}

	bool shader::load_from_source(shader_source const & value)
	{
		auto const
			& v{ std::get<id_vertex>(value) },
			& g{ std::get<id_geometry>(value) },
			& f{ std::get<id_fragment>(value) };

		return ((!v.empty() && !g.empty() && !f.empty())
			? load_from_memory(v, g, f)
			: ((!v.empty() && !f.empty())
				? load_from_memory(v, f)
				: false));
	}

	bool shader::load_from_memory(pmr::string const & v_src, pmr::string const & f_src)
	{
		if (v_src.empty() || f_src.empty()) { return false; }
		
		m_source = { v_src, {}, f_src };
		
		return EXIT_SUCCESS == compile(v_src.c_str(), nullptr, f_src.c_str());
	}

	bool shader::load_from_memory(pmr::string const & v_src, pmr::string const & g_src, pmr::string const & f_src)
	{
		if (v_src.empty() || g_src.empty() || f_src.empty()) { return false; }
		
		m_source = { v_src, g_src, f_src };
		
		return EXIT_SUCCESS == compile(v_src.c_str(), g_src.c_str(), f_src.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void shader::bind(shader const * value, bool bind_textures)
	{
		if (!value || !value->m_handle)
		{
			GL::useProgram(NULL);
		}
		else
		{
			GL::useProgram(value->m_handle);

			if (bind_textures)
			{
				uint32_t index{};

				value->m_textures.for_each([&index](int32_t loc, texture const * tex)
				{
					GL::uniform1i(loc, index);

					GL::activeTexture(GL::Texture0 + (index++));

					texture::bind(tex);
				});
			}
		}
	}

	bool shader::destroy()
	{
		unbind();
		if (m_handle)
		{
			GL::deleteShader(m_handle);
			m_handle = NULL;
			GL::flush();
			
			meta::for_tuple(m_source, [&](auto & s) { s.clear(); });
			m_attributes.clear();
			m_uniforms.clear();
			m_textures.clear();
		}
		return !m_handle;
	}

	bool shader::generate()
	{
		return !m_handle && (m_handle = GL::createProgram());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shader::set_uniform(uniform const & value)
	{
		if (value.name().empty()) { return false; }
		switch (value.type().guid())
		{
		default					: return false;
		case hashof_v<bool>		: return set_uniform(value.name(), *value.get<bool>());
		case hashof_v<int32_t>	: return set_uniform(value.name(), *value.get<int32_t>());
		case hashof_v<float_t>	: return set_uniform(value.name(), *value.get<float_t>());
		case hashof_v<vec2>		: return set_uniform(value.name(), *value.get<vec2>());
		case hashof_v<vec3>		: return set_uniform(value.name(), *value.get<vec3>());
		case hashof_v<vec4>		: return set_uniform(value.name(), *value.get<vec4>());
		case hashof_v<color>	: return set_uniform(value.name(), *value.get<color>());
		case hashof_v<mat2>		: return set_uniform(value.name(), *value.get<mat2>());
		case hashof_v<mat3>		: return set_uniform(value.name(), *value.get<mat3>());
		case hashof_v<mat4>		: return set_uniform(value.name(), *value.get<mat4>());
		case hashof_v<texture>	: return set_uniform(value.name(), *value.get<texture>());
		}
	}

	bool shader::set_uniform(pmr::string const & name, bool value)
	{
		return set_uniform(name, static_cast<int32_t>(value));
	}

	bool shader::set_uniform(pmr::string const & name, int32_t value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			GL::uniform1i(loc, value);
		});
	}

	bool shader::set_uniform(pmr::string const & name, float32_t value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			GL::uniform1f(loc, value);
		});
	}

	bool shader::set_uniform(pmr::string const & name, vec2 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			GL::uniform2f(loc, value[0], value[1]);
		});
	}

	bool shader::set_uniform(pmr::string const & name, vec3 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			GL::uniform3f(loc, value[0], value[1], value[2]);
		});
	}

	bool shader::set_uniform(pmr::string const & name, vec4 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			GL::uniform4f(loc, value[0], value[1], value[2], value[3]);
		});
	}

	bool shader::set_uniform(pmr::string const & name, color const & value)
	{
		return set_uniform(name, value.rgba());
	}

	bool shader::set_uniform(pmr::string const & name, mat2 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			GL::uniformMatrix2fv(loc, 1, false, value.data());
		});
	}

	bool shader::set_uniform(pmr::string const & name, mat3 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			GL::uniformMatrix3fv(loc, 1, false, value.data());
		});
	}

	bool shader::set_uniform(pmr::string const & name, mat4 const & value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			GL::uniformMatrix4fv(loc, 1, false, value.data());
		});
	}

	bool shader::set_uniform(pmr::string const & name, texture const * value)
	{
		return uniform_binder(*this, name, [&](int32_t loc) noexcept
		{
			static auto const max_texture_units
			{
				static_cast<size_t>(GL::getMaxTextureUnits())
			};
			if (auto const it{ m_textures.find(loc) })
			{
				(*it->second) = value;
			}
			else if ((m_textures.size() + 1) < max_texture_units)
			{
				m_textures.insert(loc, value);
			}
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t shader::get_attribute_location(pmr::string const & value)
	{
		return m_attributes.find_or_add_fn(
			util::hash(value),
			&GL::getAttribLocation, m_handle, value.c_str());
	}

	int32_t shader::get_uniform_location(pmr::string const & value)
	{
		return m_uniforms.find_or_add_fn(
			util::hash(value),
			&GL::getUniformLocation, m_handle, value.c_str());
	}

	int32_t shader::compile(cstring v_src, cstring g_src, cstring f_src)
	{
		auto print_errors = [](uint32_t obj) noexcept
		{
			pmr::stringstream ss{ GL::getProgramInfoLog(obj) };
			pmr::string line;
			while (std::getline(ss, line))
			{
				std::cout << line << '\n';
			}
		};

		auto compile_shader = [&](uint32_t & obj, uint32_t type, cstring const * src) noexcept
		{
			if (!src || !*src) { return -1; } // no source provided
			if (!(obj = GL::createShader(type))) { return 0; } // failed to create
			GL::shaderSource(obj, 1, src, nullptr);
			if (!GL::compileShader(obj)) // failed to compile
			{
				print_errors(obj); // compiler errors
				GL::deleteShader(obj);
				return 0;
			}
			return 1; // success
		};

		// check shaders available
		if (!GL::shadersAvailable()) { return EXIT_FAILURE * 1; }

		// check geometry shaders available
		if (g_src && !GL::geometryShadersAvailable()) { return EXIT_FAILURE * 2; }

		// destroy program
		if (!destroy()) { return EXIT_FAILURE * 3; }

		// generate program
		if (!generate()) { return EXIT_FAILURE * 4; }
		
		// vertex
		uint32_t v_obj{};
		switch (compile_shader(v_obj, GL::VertexShader, &v_src))
		{
		case 1: { GL::attachShader(m_handle, v_obj); GL::deleteShader(v_obj); } break;
		case 0: { destroy(); return EXIT_FAILURE * 5; }
		}

		// geometry
		uint32_t g_obj{};
		switch (compile_shader(g_obj, GL::GeometryShader, &g_src))
		{
		case 1: { GL::attachShader(m_handle, g_obj); GL::deleteShader(g_obj); } break;
		case 0: { destroy(); return EXIT_FAILURE * 6; }
		}

		// fragment
		uint32_t f_obj{};
		switch (compile_shader(f_obj, GL::FragmentShader, &f_src))
		{
		case 1: { GL::attachShader(m_handle, f_obj); GL::deleteShader(f_obj); } break;
		case 0: { destroy(); return EXIT_FAILURE * 7; }
		}

		// link program
		if (!GL::linkProgram(m_handle))
		{
			print_errors(m_handle); // linker errors
			destroy();
			return EXIT_FAILURE * 8;
		}

		GL::flush();
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}