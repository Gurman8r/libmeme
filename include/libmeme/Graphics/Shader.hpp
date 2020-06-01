#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <libmeme/Core/FileUtility.hpp>
#include <libmeme/Graphics/Uniform.hpp>
#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml
{
	struct shader final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using shader_source		= typename meta::array<pmr::string, gl::shader_type_MAX>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader(allocator_type alloc = {}) : m_object{}, m_source{ std::allocator_arg, alloc }
		{
		}

		shader(shader_source const & source, allocator_type alloc = {}) : shader{ alloc }
		{
			load_from_source(source);
		}

		shader(fs::path const & v, fs::path const & f, allocator_type alloc = {}) : shader{ alloc }
		{
			load_from_file(v, f);
		}

		shader(fs::path const & v, fs::path const & f, fs::path const & g, allocator_type alloc = {}) : shader{ alloc }
		{
			load_from_file(v, f, g);
		}

		shader(shader const & other, allocator_type alloc = {}) : shader{ alloc }
		{
			load_from_source(other.m_source);
		}

		shader(shader && other, allocator_type alloc = {}) noexcept : shader{ alloc }
		{
			swap(std::move(other));
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader & operator=(shader const & other)
		{
			shader temp{ other };
			swap(temp);
			return (*this);
		}

		shader & operator=(shader && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		void swap(shader & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_object.swap(value.m_object);
				m_source.swap(value.m_source);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & v_file, fs::path const & f_file)
		{
			return load_from_memory(
				util::get_file_string(v_file),
				util::get_file_string(f_file));
		}

		bool load_from_file(fs::path const & v_file, fs::path const & f_file, fs::path const g_file)
		{
			return load_from_memory(
				util::get_file_string(v_file),
				util::get_file_string(f_file),
				util::get_file_string(g_file));
		}

		bool load_from_source(shader_source const & value)
		{
			auto const
				& v{ std::get<gl::shader_type_vertex>(value) },
				& f{ std::get<gl::shader_type_fragment>(value) },
				& g{ std::get<gl::shader_type_geometry>(value) };

			return ((!v.empty() && !f.empty() && !g.empty())
				? load_from_memory(v, f, g)
				: ((!v.empty() && !f.empty())
					? load_from_memory(v, f)
					: false));
		}

		bool load_from_memory(pmr::string const & v_src, pmr::string const & f_src)
		{
			if (v_src.empty() || f_src.empty()) { return false; }
		
			m_source = { v_src, f_src, {} };

			m_object = shader_object::create(v_src.c_str(), f_src.c_str(), nullptr);

			return (bool)m_object;
		}

		bool load_from_memory(pmr::string const & v_src, pmr::string const & f_src, pmr::string const & g_src)
		{
			if (v_src.empty() || f_src.empty() || g_src.empty()) { return false; }
		
			m_source = { v_src, f_src, g_src };

			m_object = shader_object::create(v_src.c_str(), f_src.c_str(), g_src.c_str());

			return (bool)m_object;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void bind(bool bind_textures = true) const { m_object->bind(bind_textures); }

		void unbind() const { m_object->unbind(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> bool set_uniform(cstring name, T && value) noexcept
		{
			return m_object->set_uniform(name, ML_forward(value));
		}

		template <class T
		> bool set_uniform(pmr::string const & name, T && value) noexcept
		{
			return this->set_uniform(name.c_str(), ML_forward(value));
		}

		bool set_uniform(uniform const & u) noexcept
		{
			if (u.name().empty()) { return false; }
			switch (u.type().hash())
			{
			default					: return false;
			case hashof_v<bool>		: return set_uniform(u.name(), *u.get<bool>());
			case hashof_v<int32_t>	: return set_uniform(u.name(), *u.get<int32_t>());
			case hashof_v<float_t>	: return set_uniform(u.name(), *u.get<float_t>());
			case hashof_v<vec2>		: return set_uniform(u.name(), *u.get<vec2>());
			case hashof_v<vec3>		: return set_uniform(u.name(), *u.get<vec3>());
			case hashof_v<vec4>		: return set_uniform(u.name(), *u.get<vec4>());
			case hashof_v<color>	: return set_uniform(u.name(), *u.get<color>());
			case hashof_v<mat2>		: return set_uniform(u.name(), *u.get<mat2>());
			case hashof_v<mat3>		: return set_uniform(u.name(), *u.get<mat3>());
			case hashof_v<mat4>		: return set_uniform(u.name(), *u.get<mat4>());
			case hashof_v<texture2d>: return set_uniform(u.name(), *u.get<texture2d>());
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return (bool)m_object; }
		
		ML_NODISCARD gl::handle get_handle() const noexcept { return m_object->get_handle(); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shader_source m_source;
		shared<shader_object> m_object;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SHADER_HPP_