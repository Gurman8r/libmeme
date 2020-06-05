#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

// WIP

#include <libmeme/Core/FileUtility.hpp>
#include <libmeme/Graphics/Uniform.hpp>
#include <libmeme/Graphics/RenderAPI.hpp>

// SHADER ASSET
namespace ml
{
	struct shader_asset final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;
		
		using shader_source = typename meta::array<pmr::string, gl::shader_type_max>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader_asset(allocator_type alloc = {}) : m_obj{}, m_src{ std::allocator_arg, alloc }
		{
		}

		shader_asset(shader_source const & source, allocator_type alloc = {}) : shader_asset{ alloc }
		{
			load_from_source(source);
		}

		shader_asset(fs::path const & v, fs::path const & f, allocator_type alloc = {}) : shader_asset{ alloc }
		{
			load_from_file(v, f);
		}

		shader_asset(fs::path const & v, fs::path const & f, fs::path const & g, allocator_type alloc = {}) : shader_asset{ alloc }
		{
			load_from_file(v, f, g);
		}

		shader_asset(shader_asset const & other, allocator_type alloc = {}) : shader_asset{ alloc }
		{
			load_from_source(other.m_src);
		}

		shader_asset(shader_asset && other, allocator_type alloc = {}) noexcept : shader_asset{ alloc }
		{
			swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader_asset & operator=(shader_asset const & other)
		{
			shader_asset temp{ other };
			swap(temp);
			return (*this);
		}

		shader_asset & operator=(shader_asset && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		void swap(shader_asset & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_obj.swap(value.m_obj);
				m_src.swap(value.m_src);
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
			else { m_src = { v_src, f_src, {} }; }
			
			if (!m_obj) { m_obj = gl::shader::allocate(); }
			else { m_obj->destroy().generate(); }

			m_obj->attach(gl::shader_type_vertex, v_src.c_str());
			m_obj->attach(gl::shader_type_fragment, f_src.c_str());
			m_obj->link();
			
			return (bool)m_obj;
		}

		bool load_from_memory(pmr::string const & v_src, pmr::string const & f_src, pmr::string const & g_src)
		{
			if (v_src.empty() || f_src.empty() || g_src.empty()) { return false; }
			else { m_src = { v_src, f_src, g_src }; }
			
			if (!m_obj) { m_obj = gl::shader::allocate(); }
			else { m_obj->destroy().generate(); }
			
			m_obj->attach(gl::shader_type_vertex, v_src.c_str());
			m_obj->attach(gl::shader_type_fragment, f_src.c_str());
			m_obj->attach(gl::shader_type_geometry, g_src.c_str());
			m_obj->link();

			return (bool)m_obj;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void bind(bool bind_textures = true) const
		{
			m_obj->bind();

			if (bind_textures)
			{
				m_obj->bind_textures();
			}
		}

		void unbind() const { m_obj->unbind(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> bool set_uniform(cstring name, T const & value) noexcept
		{
			return m_obj->set_uniform(name, value);
		}

		template <class T
		> bool set_uniform(pmr::string const & name, T const & value) noexcept
		{
			return this->set_uniform(name.c_str(), value);
		}

		bool set_uniform(uniform const & u) noexcept
		{
			if (u.name().empty()) { return false; }
			switch (u.type().hash())
			{
			default						: return false;
			case hashof_v<bool>			: return set_uniform(u.name(), *u.get<bool>());
			case hashof_v<int32_t>		: return set_uniform(u.name(), *u.get<int32_t>());
			case hashof_v<float_t>		: return set_uniform(u.name(), *u.get<float_t>());
			case hashof_v<vec2>			: return set_uniform(u.name(), *u.get<vec2>());
			case hashof_v<vec3>			: return set_uniform(u.name(), *u.get<vec3>());
			case hashof_v<vec4>			: return set_uniform(u.name(), *u.get<vec4>());
			case hashof_v<color>		: return set_uniform(u.name(), *u.get<color>());
			case hashof_v<mat2>			: return set_uniform(u.name(), *u.get<mat2>());
			case hashof_v<mat3>			: return set_uniform(u.name(), *u.get<mat3>());
			case hashof_v<mat4>			: return set_uniform(u.name(), *u.get<mat4>());
			case hashof_v<gl::texture2d>: return set_uniform(u.name(), *u.get<gl::texture2d>());
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return (bool)m_obj; }
		
		ML_NODISCARD gl::handle_t get_handle() const noexcept { return m_obj->get_handle(); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shader_source m_src;
		shared<gl::shader> m_obj;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// SHADER CACHE
namespace ml
{
	struct shader_cache final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using source_storage = ds::array<ds::map<pmr::string, pmr::string>, gl::shader_type_max>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader_cache(allocator_type alloc = {})
			: m_src{ alloc }
		{
		}

		shader_cache(shader_cache && other, allocator_type alloc = {}) noexcept
			: shader_cache{ alloc }
		{
			this->swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader_cache & operator=(shader_cache && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(shader_cache & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_src.swap(other.m_src);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto & get(uint32_t i) & noexcept { return m_src[static_cast<size_t>(i)]; }

		auto const & get(uint32_t i) const & noexcept { return m_src[static_cast<size_t>(i)]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pmr::string & str(uint32_t i, pmr::string const & name) & noexcept
		{
			return this->get(i)[name];
		}

		cstring c_str(uint32_t i, pmr::string const & name) noexcept
		{
			return str(i, name).c_str();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool read_file(uint32_t i, pmr::string const & name, fs::path const & path)
		{
			if (auto const contents{ util::get_file_contents(path) })
			{
				this->str(i, name) = { contents->begin(), contents->end() };

				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		source_storage m_src;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SHADER_HPP_