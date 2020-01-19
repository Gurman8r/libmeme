#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>

namespace ml
{
	struct Shader::UniformBinder final
	{
		union { uint32_t program; uint32_t cached; int32_t location; };

		explicit UniformBinder(Shader & s, pmr::string const & name)
			: program	{ s ? s.handle() : NULL }
			, cached	{ s ? GL::getProgramHandle(GL::ProgramObject) : NULL }
			, location	{ s ? s.get_uniform_location(name) : -1 }
		{
			if (program && (program != cached))
			{
				GL::useProgram(program);
			}
		}

		~UniformBinder()
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

	Shader::Shader()
		: Shader{ allocator_type{} }
	{
	}

	Shader::Shader(allocator_type const & alloc)
		: m_handle		{ NULL }
		, m_source		{ nullptr, nullptr, nullptr }
		, m_attributes	{ alloc }
		, m_textures	{ alloc }
		, m_uniforms	{ alloc }
	{
	}

	Shader::Shader(shader_source const & source, allocator_type const & alloc)
		: Shader{ alloc }
	{
		load_from_source(source);
	}

	Shader::Shader(path_t const & v, path_t const & f, allocator_type const & alloc)
		: Shader{ alloc }
	{
		load_from_file(v, f);
	}

	Shader::Shader(path_t const & v, path_t const & g, path_t const & f, allocator_type const & alloc)
		: Shader{ alloc }
	{
		load_from_file(v, g, f);
	}

	Shader::Shader(Shader const & other, allocator_type const & alloc)
		: Shader{ alloc }
	{
		load_from_source(other.m_source);
	}

	Shader::Shader(Shader && other, allocator_type const & alloc) noexcept
		: Shader{ alloc }
	{
		swap(std::move(other));
	}

	Shader::~Shader()
	{
		destroy();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::load_from_file(path_t const & v_file, path_t const & f_file)
	{
		return load_from_memory(
			FS::get_file_contents(v_file).c_str(),
			FS::get_file_contents(f_file).c_str()
		);
	}

	bool Shader::load_from_file(path_t const & v_file, path_t const g_file, path_t const & f_file)
	{
		return load_from_memory(
			FS::get_file_contents(v_file).c_str(),
			FS::get_file_contents(g_file).c_str(),
			FS::get_file_contents(f_file).c_str()
		);
	}

	bool Shader::load_from_source(shader_source const & value)
	{
		return load_from_memory(value.vs, value.gs, value.fs);
	}

	bool Shader::load_from_memory(pmr::string const & vs, pmr::string const & fs)
	{
		return load_from_memory(vs.c_str(), nullptr, fs.c_str());
	}

	bool Shader::load_from_memory(pmr::string const & vs, pmr::string const & gs, pmr::string const & fs)
	{
		return load_from_memory(vs.c_str(), gs.c_str(), fs.c_str());
	}

	bool Shader::load_from_memory(C_string vs, C_string fs)
	{
		return compile(
			m_source.vs = vs,
			m_source.gs = nullptr,
			m_source.fs = fs
		) == EXIT_SUCCESS;
	}

	bool Shader::load_from_memory(C_string vs, C_string gs, C_string fs)
	{
		return compile(
			m_source.vs = vs,
			m_source.gs = gs,
			m_source.fs = fs
		) == EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Shader::bind(Shader const * value, bool bindTextures)
	{
		if (!value || !value->m_handle)
		{
			return GL::useProgram(NULL);;
		}

		GL::useProgram(value->m_handle);

		if (bindTextures)
		{
			uint32_t index = 0;
			value->m_textures.for_each([&index](int32_t location, auto const & tex)
			{
				GL::uniform1i(location, index);
				GL::activeTexture(GL::Texture0 + (index++));
				Texture::bind(tex);
			});
		}
	}

	bool Shader::destroy()
	{
		Shader::bind(nullptr);
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

	bool Shader::generate()
	{
		return !m_handle && (m_handle = GL::createProgram());
	}

	void Shader::swap(Shader & other) noexcept
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

	bool Shader::set_uniform(Uniform const & value)
	{
		if (value.name().empty()) { return false; }
		switch (value.type().hash())
		{
		case hashof_v<bool>: if (auto v{ value.get<bool>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<int32_t>: if (auto v{ value.get<int32_t>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<float_t>: if (auto v{ value.get<float_t>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<vec2>: if (auto v{ value.get<vec2>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<vec3>: if (auto v{ value.get<vec3>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<vec4>: if (auto v{ value.get<vec4>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<Color>: if (auto v{ value.get<Color>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<mat2>: if (auto v{ value.get<mat2>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<mat3>: if (auto v{ value.get<mat3>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<mat4>: if (auto v{ value.get<mat4>() })
			return set_uniform(value.name(), v.value());
		
		case hashof_v<Texture>: if (auto v{ value.get<Texture const *>() })
			return set_uniform(value.name(), v.value());
		}
		return false;
	}

	bool Shader::set_uniform(pmr::string const & name, bool value)
	{
		return set_uniform(name, static_cast<int32_t>(value));
	}

	bool Shader::set_uniform(pmr::string const & name, int32_t value)
	{
		UniformBinder const u{ (*this), name };
		if (u) { GL::uniform1i(u.location, value); }
		return u;
	}

	bool Shader::set_uniform(pmr::string const & name, float32_t value)
	{
		UniformBinder const u{ (*this), name };
		if (u) { GL::uniform1f(u.location, value); }
		return u;
	}

	bool Shader::set_uniform(pmr::string const & name, vec2 const & value)
	{
		UniformBinder const u{ (*this), name };
		if (u) { GL::uniform2f(u.location, value[0], value[1]); }
		return u;
	}

	bool Shader::set_uniform(pmr::string const & name, vec3 const & value)
	{
		UniformBinder const u{ (*this), name };
		if (u) { GL::uniform3f(u.location, value[0], value[1], value[2]); }
		return u;
	}

	bool Shader::set_uniform(pmr::string const & name, vec4 const & value)
	{
		UniformBinder const u{ (*this), name };
		if (u) { GL::uniform4f(u.location, value[0], value[1], value[2], value[3]); }
		return u;
	}

	bool Shader::set_uniform(pmr::string const & name, Color const & value)
	{
		return set_uniform(name, value.rgba());
	}

	bool Shader::set_uniform(pmr::string const & name, mat2 const & value)
	{
		UniformBinder const u{ (*this), name };
		if (u) { GL::uniformMatrix2fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool Shader::set_uniform(pmr::string const & name, mat3 const & value)
	{
		UniformBinder const u{ (*this), name };
		if (u) { GL::uniformMatrix3fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool Shader::set_uniform(pmr::string const & name, mat4 const & value)
	{
		UniformBinder const u{ (*this), name };
		if (u) { GL::uniformMatrix4fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool Shader::set_uniform(pmr::string const & name, Texture const & value)
	{
		UniformBinder const u{ (*this), name };
		if (u)
		{
			static auto const max_textures
			{
				static_cast<size_t>(GL::getMaxTextureUnits())
			};

			if (auto it{ m_textures.find(u.location) })
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

	bool Shader::set_uniform(pmr::string const & name, Texture const * value)
	{
		return value ? set_uniform(name, (*value)) : false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Shader::get_attribute_location(pmr::string const & value)
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

	int32_t Shader::get_uniform_location(pmr::string const & value)
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

	int32_t Shader::compile(C_string vs, C_string gs, C_string fs)
	{
		// Shaders Available
		if (!GL::shadersAvailable())
		{
			return EXIT_FAILURE * 1;
		}
		
		// Geometry Shaders Available
		if (gs && !GL::geometryShadersAvailable())
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
		uint32_t v{ 0 };
		switch (GL::compileShader(v, GL::VertexShader, 1, &vs))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, v);
			GL::deleteShader(v);
			break;
		case ML_FAILURE:
			destroy();
			return EXIT_FAILURE * 5;
		}

		// Compile Geometry
		uint32_t g{ 0 };
		switch (GL::compileShader(g, GL::GeometryShader, 1, &gs))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, g);
			GL::deleteShader(g);
			break;
		case ML_FAILURE:
			destroy();
			return EXIT_FAILURE * 6;
		}

		// Compile Fragment
		uint32_t f{ 0 };
		switch (GL::compileShader(f, GL::FragmentShader, 1, &fs))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, f);
			GL::deleteShader(f);
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