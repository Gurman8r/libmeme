#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <libmeme/Graphics/Uniform.hpp>
#include <libmeme/Renderer/Renderer.hpp>

namespace ml
{
	struct ML_GRAPHICS_API shader final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { id_vertex, id_geometry, id_fragment, id_max };

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using shader_source		= typename meta::array<pmr::string, id_max>;
		using attribute_cache	= typename ds::map<hash_t, int32_t>;
		using uniform_cache		= typename ds::map<hash_t, int32_t>;
		using texture_cache		= typename ds::map<int32_t, texture const *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader(allocator_type alloc = {})
			: m_object{}
			, m_source{ std::allocator_arg, alloc }
		{
		}

		shader(shader_source const & source, allocator_type alloc = {})
			: shader{ alloc }
		{
			(void)load_from_source(source);
		}

		shader(fs::path const & v, fs::path const & f, allocator_type alloc = {})
			: shader{ alloc }
		{
			(void)load_from_file(v, f);
		}

		shader(fs::path const & v, fs::path const & g, fs::path const & f, allocator_type alloc = {})
			: shader{ alloc }
		{
			(void)load_from_file(v, g, f);
		}

		shader(shader const & value, allocator_type alloc = {})
			: shader{ alloc }
		{
			(void)load_from_source(value.m_source);
		}

		shader(shader && value, allocator_type alloc = {}) noexcept
			: shader{ alloc }
		{
			swap(std::move(value));
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader & operator=(shader const & value)
		{
			shader temp{ value };
			swap(temp);
			return (*this);
		}

		shader & operator=(shader && value) noexcept
		{
			swap(std::move(value));
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

		bool load_from_file(fs::path const & v_file, fs::path const g_file, fs::path const & f_file)
		{
			return load_from_memory(
				util::get_file_string(v_file),
				util::get_file_string(g_file),
				util::get_file_string(f_file));
		}

		bool load_from_source(shader_source const & value)
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

		bool load_from_memory(pmr::string const & v_src, pmr::string const & f_src)
		{
			if (v_src.empty() || f_src.empty()) { return false; }
		
			m_source = { v_src, {}, f_src };

			m_object = gl::make_shader(v_src.c_str(), nullptr, f_src.c_str());

			return (bool)m_object;
		}

		bool load_from_memory(pmr::string const & v_src, pmr::string const & g_src, pmr::string const & f_src)
		{
			if (v_src.empty() || g_src.empty() || f_src.empty()) { return false; }
		
			m_source = { v_src, g_src, f_src };

			m_object = gl::make_shader(v_src.c_str(), g_src.c_str(), f_src.c_str());

			return (bool)m_object;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void bind(bool bind_textures = true) const { m_object->bind(bind_textures); }

		void unbind() const { m_object->unbind(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> bool set_uniform(cstring name, T && value) noexcept
		{
			if constexpr (std::is_convertible_v<T, uniform::sampler_type>)
			{
				return this->set_uniform(name, value->address());
			}
			else
			{
				return m_object->set_uniform(name, ML_forward(value));
			}
		}

		template <class T
		> bool set_uniform(pmr::string const & name, T && value) noexcept
		{
			return this->set_uniform(name.c_str(), ML_forward(value));
		}

		bool set_uniform(uniform const & value) noexcept
		{
			if (value.name().empty()) { return false; }
			switch (value.type().hash())
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return (bool)m_object; }
		
		ML_NODISCARD gl::handle_t get_handle() const noexcept { return m_object->get_handle(); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shader_source m_source;

		shared<gl::shader_object> m_object;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SHADER_HPP_