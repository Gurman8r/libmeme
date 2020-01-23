#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	struct shader::uniform_binder final
	{
		union { uint32_t program; uint32_t cached; int32_t location; };

		explicit uniform_binder(shader & s, pmr::string const & name)
			: program	{ s ? s.handle() : NULL }
			, cached	{ s ? GL::getProgramHandle(GL::ProgramObject) : NULL }
			, location	{ s ? s.get_uniform_location(name) : -1 }
		{
			if (program && (program != cached))
			{
				GL::useProgram(program);
			}
		}

		~uniform_binder()
		{
			if (program && (program != cached))
			{
				GL::useProgram(cached);
			}
		}

		inline operator bool() const noexcept
		{
			return (location != -1);
		}
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	shader::shader()
		: shader{ allocator_type{} }
	{
	}

	shader::shader(allocator_type const & alloc)
		: m_handle		{ NULL }
		, m_source		{ nullptr, nullptr, nullptr }
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

	shader::~shader()
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool shader::load_from_file(fs::path const & v_file, fs::path const & f_file)
	{
		return load_from_memory(
			FS::get_file_contents(v_file).c_str(),
			FS::get_file_contents(f_file).c_str()
		);
	}

	bool shader::load_from_file(fs::path const & v_file, fs::path const g_file, fs::path const & f_file)
	{
		return load_from_memory(
			FS::get_file_contents(v_file).c_str(),
			FS::get_file_contents(g_file).c_str(),
			FS::get_file_contents(f_file).c_str()
		);
	}

	bool shader::load_from_source(shader_source const & value)
	{
		return load_from_memory(value.v, value.g, value.f);
	}

	bool shader::load_from_memory(pmr::string const & v, pmr::string const & f)
	{
		return load_from_memory(v.c_str(), nullptr, f.c_str());
	}

	bool shader::load_from_memory(pmr::string const & v, pmr::string const & g, pmr::string const & f)
	{
		return load_from_memory(v.c_str(), g.c_str(), f.c_str());
	}

	bool shader::load_from_memory(C_string v, C_string f)
	{
		return compile(
			m_source.v = v,
			m_source.g = nullptr,
			m_source.f = f
		) == EXIT_SUCCESS;
	}

	bool shader::load_from_memory(C_string v, C_string g, C_string f)
	{
		return compile(
			m_source.v = v,
			m_source.g = g,
			m_source.f = f
		) == EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void shader::bind(shader const * value, bool bindTextures)
	{
		if (!value || !value->m_handle)
		{
			return GL::useProgram(NULL);
		}

		GL::useProgram(value->m_handle);

		if (bindTextures)
		{
			uint32_t index = 0;
			value->m_textures.for_each([&index](int32_t location, auto const & tex)
			{
				GL::uniform1i(location, index);
				GL::activeTexture(GL::Texture0 + (index++));
				texture::bind(tex);
			});
		}
	}

	bool shader::destroy()
	{
		shader::bind(nullptr);
		if (m_handle)
		{
			GL::deleteShader(m_handle);
			m_handle = NULL;
			GL::flush();
			
			m_source = { nullptr, nullptr, nullptr };
			m_attributes.clear();
			m_uniforms.clear();
			m_textures.clear();
		}
		return !(m_handle);
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
		switch (value.type().hash())
		{
		case hashof_v<bool>: if (auto const v{ value.get<bool>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<int32_t>: if (auto const v{ value.get<int32_t>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<float_t>: if (auto const v{ value.get<float_t>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<vec2>: if (auto const v{ value.get<vec2>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<vec3>: if (auto const v{ value.get<vec3>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<vec4>: if (auto const v{ value.get<vec4>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<color>: if (auto const v{ value.get<color>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<mat2>: if (auto const v{ value.get<mat2>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<mat3>: if (auto const v{ value.get<mat3>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<mat4>: if (auto const v{ value.get<mat4>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<texture>: if (auto const v{ value.get<texture const *>() })
			return set_uniform(value.name(), v.value());
		}
		return false;
	}

	bool shader::set_uniform(pmr::string const & name, bool value)
	{
		return set_uniform(name, static_cast<int32_t>(value));
	}

	bool shader::set_uniform(pmr::string const & name, int32_t value)
	{
		uniform_binder const u{ (*this), name };
		if (u) { GL::uniform1i(u.location, value); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, float32_t value)
	{
		uniform_binder const u{ (*this), name };
		if (u) { GL::uniform1f(u.location, value); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, vec2 const & value)
	{
		uniform_binder const u{ (*this), name };
		if (u) { GL::uniform2f(u.location, value[0], value[1]); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, vec3 const & value)
	{
		uniform_binder const u{ (*this), name };
		if (u) { GL::uniform3f(u.location, value[0], value[1], value[2]); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, vec4 const & value)
	{
		uniform_binder const u{ (*this), name };
		if (u) { GL::uniform4f(u.location, value[0], value[1], value[2], value[3]); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, color const & value)
	{
		return set_uniform(name, value.rgba());
	}

	bool shader::set_uniform(pmr::string const & name, mat2 const & value)
	{
		uniform_binder const u{ (*this), name };
		if (u) { GL::uniformMatrix2fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, mat3 const & value)
	{
		uniform_binder const u{ (*this), name };
		if (u) { GL::uniformMatrix3fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, mat4 const & value)
	{
		uniform_binder const u{ (*this), name };
		if (u) { GL::uniformMatrix4fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, texture const & value)
	{
		uniform_binder const u{ (*this), name };
		if (u)
		{
			static auto const max_textures
			{
				static_cast<size_t>(GL::getMaxTextureUnits())
			};

			if (auto const it{ m_textures.find(u.location) })
			{
				(*it->second) = &value;
			}
			else if ((m_textures.size() + 1) < max_textures)
			{
				m_textures.insert(u.location, &value);
			}
			else
			{
				return false;
			}
		}
		return u;
	}

	bool shader::set_uniform(pmr::string const & name, texture const * value)
	{
		return value ? set_uniform(name, (*value)) : false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t shader::get_attribute_location(pmr::string const & value)
	{
		if (auto const it{ m_attributes.find(value) })
		{
			return (*it->second);
		}
		else if (auto const loc{ GL::getAttribLocation(m_handle, value.c_str()) }; loc != -1)
		{
			return (*m_attributes.insert(value, loc).first.second);
		}
		else
		{
			return -1;
		}
	}

	int32_t shader::get_uniform_location(pmr::string const & value)
	{
		if (auto const it{ m_uniforms.find(value) })
		{
			return (*it->second);
		}
		else if (auto const loc{ GL::getUniformLocation(m_handle, value.c_str()) }; loc != -1)
		{
			return (*m_uniforms.insert(value, loc).first.second);
		}
		else
		{
			return -1;
		}
	}

	int32_t shader::compile(C_string v, C_string g, C_string f)
	{
		// Shaders Available
		if (!GL::shadersAvailable())
		{
			return EXIT_FAILURE * 1;
		}

		// Geometry Shaders Available
		if (g && !GL::geometryShadersAvailable())
		{
			return EXIT_FAILURE * 2;
		}

		// Destroy Program
		if (!destroy())
		{
			return EXIT_FAILURE * 3;
		}

		// Generate Program
		if (!generate())
		{
			return EXIT_FAILURE * 4;
		}

		// Compile Vertex
		uint32_t vert{ 0 };
		switch (GL::compileShader(vert, GL::VertexShader, 1, &v))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, vert);
			GL::deleteShader(vert);
			break;
		case ML_FAILURE:
			destroy();
			return EXIT_FAILURE * 5;
		}

		// Compile Geometry
		uint32_t geom{ 0 };
		switch (GL::compileShader(geom, GL::GeometryShader, 1, &g))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, geom);
			GL::deleteShader(geom);
			break;
		case ML_FAILURE:
			destroy();
			return EXIT_FAILURE * 6;
		}

		// Compile Fragment
		uint32_t frag{ 0 };
		switch (GL::compileShader(frag, GL::FragmentShader, 1, &f))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, frag);
			GL::deleteShader(frag);
			break;
		case ML_FAILURE:
			destroy();
			return EXIT_FAILURE * 7;
		}

		// Link Program
		if (!GL::linkProgram(m_handle))
		{
			C_string const log{ GL::getProgramInfoLog(m_handle) };
			debug::log_error(log);
			destroy();
			return EXIT_FAILURE * 8;
		}

		GL::flush();
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}