#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/FileUtility.hpp>

namespace ml
{
	struct shader::uniform_binder final
	{
		union { uint32_t program; uint32_t cached; int32_t location; };

		explicit uniform_binder(shader & s, pmr::string const & name) noexcept
			: program	{ s ? s.handle() : NULL }
			, cached	{ s ? GL::getProgramHandle(GL::ProgramObject) : NULL }
			, location	{ s ? s.get_uniform_location(name) : -1 }
		{
			if (program && (program != cached))
			{
				GL::useProgram(program);
			}
		}

		~uniform_binder() noexcept
		{
			if (program && (program != cached))
			{
				GL::useProgram(cached);
			}
		}

		operator bool() const noexcept { return location != -1; }

		operator int32_t() const noexcept { return location; }
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shader::shader(allocator_type const & alloc)
		: m_handle		{ NULL }
		, m_source		{ std::allocator_arg, alloc }
		, m_attributes	{ alloc }
		, m_textures	{ alloc }
		, m_uniforms	{ alloc }
	{
	}

	shader::shader(shader_source const & source, allocator_type const & alloc)
		: shader{ alloc }
	{
		load_from_source(source);
	}

	shader::shader(fs::path const & v, fs::path const & f, allocator_type const & alloc)
		: shader{ alloc }
	{
		load_from_file(v, f);
	}

	shader::shader(fs::path const & v, fs::path const & g, fs::path const & f, allocator_type const & alloc)
		: shader{ alloc }
	{
		load_from_file(v, g, f);
	}

	shader::shader(shader const & other, allocator_type const & alloc)
		: shader{ alloc }
	{
		load_from_source(other.m_source);
	}

	shader::shader(shader && other, allocator_type const & alloc) noexcept
		: shader{ alloc }
	{
		swap(std::move(other));
	}

	shader::~shader() noexcept
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shader::load_from_file(fs::path const & v_file, fs::path const & f_file)
	{
		return load_from_memory(
			util::get_file_string(v_file).c_str(),
			util::get_file_string(f_file).c_str()
		);
	}

	bool shader::load_from_file(fs::path const & v_file, fs::path const g_file, fs::path const & f_file)
	{
		return load_from_memory(
			util::get_file_string(v_file).c_str(),
			util::get_file_string(g_file).c_str(),
			util::get_file_string(f_file).c_str()
		);
	}

	bool shader::load_from_source(shader_source const & value)
	{
		auto const
			& v{ std::get<ID_Vert>(value) },
			& g{ std::get<ID_Geom>(value) },
			& f{ std::get<ID_Frag>(value) };

		if (!v.empty() && !g.empty() && !f.empty())
		{
			return load_from_memory(v, g, f);
		}
		else if (!v.empty() && !f.empty())
		{
			return load_from_memory(v, f);
		}
		else
		{
			return false;
		}
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

				value->m_textures.for_each([&index](int32_t location, texture const * tex)
				{
					GL::uniform1i(location, index);

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

	void shader::swap(shader & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_handle, other.m_handle);
			std::swap(m_source, other.m_source);
			m_attributes.swap(other.m_attributes);
			m_uniforms.swap(other.m_uniforms);
			m_textures.swap(other.m_textures);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shader::set_uniform(uniform const & value)
	{
		if (!value) { return false; }
		switch (value.type().guid())
		{
		case hashof_v<bool>: if (auto const v{ value.get<bool>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<int32_t>: if (auto const v{ value.get<int32_t>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<float_t>: if (auto const v{ value.get<float_t>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<vec2>: if (auto const v{ value.get<vec2>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<vec3>: if (auto const v{ value.get<vec3>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<vec4>: if (auto const v{ value.get<vec4>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<color>: if (auto const v{ value.get<color>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<mat2>: if (auto const v{ value.get<mat2>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<mat3>: if (auto const v{ value.get<mat3>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<mat4>: if (auto const v{ value.get<mat4>() })
			return set_uniform(value.name(), *v);
		
		case hashof_v<texture>: if (auto const v{ value.get<texture>() })
			return set_uniform(value.name(), *v);
		}
		return false;
	}

	bool shader::set_uniform(pmr::string const & name, bool value)
	{
		return set_uniform(name, static_cast<int32_t>(value));
	}

	bool shader::set_uniform(pmr::string const & name, int32_t value)
	{
		uniform_binder const u{ *this, name };
		if (u) { GL::uniform1i(u, value); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, float32_t value)
	{
		uniform_binder const u{ *this, name };
		if (u) { GL::uniform1f(u, value); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, vec2 const & value)
	{
		uniform_binder const u{ *this, name };
		if (u) { GL::uniform2f(u, value[0], value[1]); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, vec3 const & value)
	{
		uniform_binder const u{ *this, name };
		if (u) { GL::uniform3f(u, value[0], value[1], value[2]); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, vec4 const & value)
	{
		uniform_binder const u{ *this, name };
		if (u) { GL::uniform4f(u, value[0], value[1], value[2], value[3]); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, color const & value)
	{
		return set_uniform(name, value.rgba());
	}

	bool shader::set_uniform(pmr::string const & name, mat2 const & value)
	{
		uniform_binder const u{ *this, name };
		if (u) { GL::uniformMatrix2fv(u, 1, false, value.data()); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, mat3 const & value)
	{
		uniform_binder const u{ *this, name };
		if (u) { GL::uniformMatrix3fv(u, 1, false, value.data()); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, mat4 const & value)
	{
		uniform_binder const u{ *this, name };
		if (u) { GL::uniformMatrix4fv(u, 1, false, value.data()); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, texture const * value)
	{
		static auto const max_texture_units
		{
			static_cast<size_t>(GL::getMaxTextureUnits())
		};

		uniform_binder const u{ *this, name };
		if (u)
		{
			if (auto const it{ m_textures.find(u) })
			{
				(*it->second) = value;
			}
			else if ((m_textures.size() + 1) < max_texture_units)
			{
				m_textures.insert(u, value);
			}
		}
		return u;
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
		// check shaders available
		if (!GL::shadersAvailable())
		{
			return EXIT_FAILURE * 1;
		}

		// check geometry shaders available
		if (g_src && !GL::geometryShadersAvailable())
		{
			return EXIT_FAILURE * 2;
		}

		// destroy program
		if (!destroy())
		{
			return EXIT_FAILURE * 3;
		}

		// generate program
		if (!generate())
		{
			return EXIT_FAILURE * 4;
		}

		// compile vertex
		uint32_t vert{};
		switch (GL::compileShader(vert, GL::VertexShader, 1, &v_src))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, vert);
			GL::deleteShader(vert);
			break;
		case ML_FAILURE:
			destroy();
			return EXIT_FAILURE * 5;
		}

		// compile geometry
		uint32_t geom{};
		switch (GL::compileShader(geom, GL::GeometryShader, 1, &g_src))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, geom);
			GL::deleteShader(geom);
			break;
		case ML_FAILURE:
			destroy();
			return EXIT_FAILURE * 6;
		}

		// compile fragment
		uint32_t frag{};
		switch (GL::compileShader(frag, GL::FragmentShader, 1, &f_src))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, frag);
			GL::deleteShader(frag);
			break;
		case ML_FAILURE:
			destroy();
			return EXIT_FAILURE * 7;
		}

		// link
		if (!GL::linkProgram(m_handle))
		{
			cstring const log{ GL::getProgramInfoLog(m_handle) };
			debug::log::error(log);
			destroy();
			return EXIT_FAILURE * 8;
		}

		GL::flush();
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}