#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/Texture.hpp>
#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	struct Shader::UniformBinder final
	{
		union { uint32_t previous; uint32_t current; int32_t location; };

		UniformBinder(Shader & program, std::string const & name)
			: previous	{ NULL }
			, current	{ program ? program.handle() : NULL }
			, location	{ -1 }
		{
			if (current)
			{
				previous = GL::getProgramHandle(GL::ProgramObject);

				if (current != previous)
				{
					GL::useProgram(current);
				}

				location = program.get_uniform(name);
			}
		}

		~UniformBinder()
		{
			if (current && (current != previous))
			{
				GL::useProgram(previous);
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
		: m_handle{ NULL }
		, m_attribs{}
		, m_source{}
		, m_textures{}
		, m_uniforms{}
	{
	}

	Shader::Shader(Source const & source)
		: Shader{ source.vs, source.gs, source.fs }
	{
	}

	Shader::Shader(std::string const & v, std::string const & f)
		: Shader{}
	{
		loadFromMemory(v, f);
	}

	Shader::Shader(std::string const & v, std::string const & g, std::string const & f)
		: Shader{}
	{
		loadFromMemory(v, g, f);
	}

	Shader::Shader(Shader const & other)
		: Shader{}
	{
	}

	Shader::Shader(Shader && other) noexcept
		: Shader{}
	{
		swap(std::move(other));
	}

	Shader::~Shader() { destroy(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Shader & Shader::operator=(Shader const & other)
	{
		Shader temp{ other };
		swap(temp);
		return (*this);
	}

	Shader & Shader::operator=(Shader && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	void Shader::swap(Shader & other) noexcept
	{
		if (this != std::addressof(other))
		{
			std::swap(m_handle, other.m_handle);
			std::swap(m_source, other.m_source);
			AttribCache{}.swap(m_attribs);
			UniformCache{}.swap(m_uniforms);
			TextureCache{}.swap(m_textures);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::loadFromFile(path_t const & v_file, path_t const & f_file)
	{
		// Vertex
		std::string vs{};
		if (auto const o{ FS::read_file(v_file) }; o && !o.value().empty())
		{
			vs.assign(o.value().cbegin(), o.value().cend());
		}

		// Fragment
		std::string fs{};
		if (auto const o{ FS::read_file(f_file) }; o && !o.value().empty())
		{
			fs.assign(o.value().cbegin(), o.value().cend());
		}

		return loadFromMemory(vs, fs);
	}

	bool Shader::loadFromFile(path_t const & v_file, path_t const g_file, path_t const & f_file)
	{
		// Vertex
		std::string vs{};
		if (auto const o{ FS::read_file(v_file) }; o && !o.value().empty())
		{
			vs.assign(o.value().cbegin(), o.value().cend());
		}

		// Geometry
		std::string gs{};
		if (auto const o{ FS::read_file(g_file) }; o && !o.value().empty())
		{
			gs.assign(o.value().cbegin(), o.value().cend());
		}

		// Fragment
		std::string fs{};
		if (auto const o{ FS::read_file(f_file) }; o && !o.value().empty())
		{
			fs.assign(o.value().cbegin(), o.value().cend());
		}

		return loadFromMemory(vs, gs, fs);
	}

	bool Shader::loadFromSource(Source const & value)
	{
		return loadFromMemory(
			std::string{ value.vs },
			std::string{ value.gs },
			std::string{ value.fs }
		);
	}

	bool Shader::loadFromMemory(std::string const & v_src, std::string const & f_src)
	{
		return loadFromMemory(v_src, std::string{}, f_src);
	}

	bool Shader::loadFromMemory(std::string const & v_src, std::string const & g_src, std::string const & f_src)
	{
		return (compile(
			m_source.vs = (v_src.empty() ? nullptr : v_src.c_str()),
			m_source.gs = (g_src.empty() ? nullptr : g_src.c_str()),
			m_source.fs = (f_src.empty() ? nullptr : f_src.c_str())
		) == EXIT_SUCCESS);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Shader::create()
	{
		return !m_handle && (m_handle = GL::createProgram());
	}

	bool Shader::destroy()
	{
		m_attribs.clear();
		m_uniforms.clear();
		m_textures.clear();
		
		GL::useProgram(NULL);
		
		if (m_handle)
		{
			GL::deleteShader(m_handle);
			
			m_handle = NULL;
			
			GL::flush();
		}
		
		return !(m_handle);
	}

	void Shader::bind(Shader const * value, bool bindTextures)
	{
		if (value && value->m_handle)
		{
			GL::useProgram(value->m_handle);

			if (bindTextures)
			{
				uint32_t i{ 0 };
				for (auto const & [texture, location] : value->m_textures)
				{
					GL::uniform1i(texture->handle(), location);

					GL::activeTexture(GL::Texture0 + (i++));

					Texture::bind(texture);
				}
			}
		}
		else
		{
			GL::useProgram(NULL);
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
		
		case hashof_v<Texture const *>: if (auto v{ value.get<Texture const *>() })
			return set_uniform(value.name(), v.value());
		}
		return false;
	}

	bool Shader::set_uniform(std::string const & name, bool value)
	{
		return set_uniform(name, static_cast<int32_t>(value));
	}

	bool Shader::set_uniform(std::string const & name, int32_t value)
	{
		UniformBinder u{ (*this), name };
		if (u) { GL::uniform1i(u.location, value); }
		return u;
	}

	bool Shader::set_uniform(std::string const & name, float32_t value)
	{
		UniformBinder u{ (*this), name };
		if (u) { GL::uniform1f(u.location, value); }
		return u;
	}

	bool Shader::set_uniform(std::string const & name, vec2 const & value)
	{
		UniformBinder u{ (*this), name };
		if (u) { GL::uniform2f(u.location, value[0], value[1]); }
		return u;
	}

	bool Shader::set_uniform(std::string const & name, vec3 const & value)
	{
		UniformBinder u{ (*this), name };
		if (u) { GL::uniform3f(u.location, value[0], value[1], value[2]); }
		return u;
	}

	bool Shader::set_uniform(std::string const & name, vec4 const & value)
	{
		UniformBinder u{ (*this), name };
		if (u) { GL::uniform4f(u.location, value[0], value[1], value[2], value[3]); }
		return u;
	}

	bool Shader::set_uniform(std::string const & name, Color const & value)
	{
		return set_uniform(name, value.rgba());
	}

	bool Shader::set_uniform(std::string const & name, mat2 const & value)
	{
		UniformBinder u{ (*this), name };
		if (u) { GL::uniformMatrix2fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool Shader::set_uniform(std::string const & name, mat3 const & value)
	{
		UniformBinder u{ (*this), name };
		if (u) { GL::uniformMatrix3fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool Shader::set_uniform(std::string const & name, mat4 const & value)
	{
		UniformBinder u{ (*this), name };
		if (u) { GL::uniformMatrix4fv(u.location, 1, false, value.data()); }
		return u;
	}

	bool Shader::set_uniform(std::string const & name, Texture const & value)
	{
		UniformBinder u{ (*this), name };
		if (u)
		{
			static auto const max_textures
			{
				static_cast<size_t>(GL::getMaxTextureUnits())
			};

			if (auto const it{ m_textures.find(std::addressof(value)) }; it != m_textures.cend())
			{
				it->second;
			}
			else if ((m_textures.size() + 1) < max_textures)
			{
				m_textures.insert(std::make_pair(std::addressof(value), u.location));
			}
			else
			{
				return false;
			}
		}
		return u;
	}

	bool Shader::set_uniform(std::string const & name, Texture const * value)
	{
		return value ? set_uniform(name, (*value)) : false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Shader::get_attribute(std::string const & value)
	{
		if (value.empty()) { return -1; }

		hash_t const code{ Hash(value) };
		
		if (auto const it{ m_attribs.find(code) }; it != m_attribs.end())
		{
			return it->second;
		}
		else
		{
			return m_attribs.insert(std::make_pair(
				code, GL::getAttribLocation(m_handle, value.c_str())
			)).first->second;
		}
	}

	int32_t Shader::get_uniform(std::string const & value)
	{
		if (value.empty()) { return -1; }

		hash_t const code{ Hash(value) };

		if (auto const it{ m_uniforms.find(code) }; it != m_uniforms.end())
		{
			return it->second;
		}
		else
		{
			return m_uniforms.insert(std::make_pair(
				code, GL::getAttribLocation(m_handle, value.c_str())
			)).first->second;
		}
	}

	int32_t Shader::compile(C_String v_src, C_String g_src, C_String f_src)
	{
		// Shaders Available
		if (!GL::shadersAvailable())
		{
			return EXIT_FAILURE * 1;
		}
		
		// Geometry Shaders Available
		if (g_src && !GL::geometryShadersAvailable())
		{
			return EXIT_FAILURE * 2;
		}

		// Create Program
		if (!destroy() || !create())
		{
			return EXIT_FAILURE * 3;
		}

		// Compile Vertex
		uint32_t v{ 0 };
		switch (GL::compileShader(v, GL::VertexShader, 1, &v_src))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, v);
			GL::deleteShader(v);
			break;
		case ML_FAILURE:
			GL::deleteShader(m_handle);
			GL::flush();
			return EXIT_FAILURE * 4;
		}

		// Compile Geometry
		uint32_t g{ 0 };
		switch (GL::compileShader(g, GL::GeometryShader, 1, &g_src))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, g);
			GL::deleteShader(g);
			break;
		case ML_FAILURE:
			GL::deleteShader(m_handle);
			GL::flush();
			return EXIT_FAILURE * 5;
		}

		// Compile Fragment
		uint32_t f{ 0 };
		switch (GL::compileShader(f, GL::FragmentShader, 1, &f_src))
		{
		case ML_SUCCESS:
			GL::attachShader(m_handle, f);
			GL::deleteShader(f);
			break;
		case ML_FAILURE:
			GL::deleteShader(m_handle);
			GL::flush();
			return EXIT_FAILURE * 6;
		}

		// Link Program
		if (!GL::linkProgram(m_handle))
		{
			C_String const log{ GL::getProgramInfoLog(m_handle) };
			GL::deleteShader(m_handle);
			Debug::logError(log);
			GL::flush();
			return EXIT_FAILURE * 7;
		}

		GL::flush();
		return EXIT_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}