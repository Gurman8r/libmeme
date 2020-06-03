#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <libmeme/Core/FileUtility.hpp>
#include <libmeme/Graphics/Uniform.hpp>
#include <libmeme/Graphics/RenderAPI.hpp>

namespace ml
{
	struct shader_asset final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;
		using shader_source = typename meta::array<pmr::string, gl::shader_type_MAX>;

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

		shader_asset(shared<shader> && value, allocator_type alloc = {}) noexcept
			: m_obj{ std::move(value) }
			, m_src{ std::allocator_arg, alloc }
		{
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
				& v{ std::get<gl::vertex_shader>(value) },
				& f{ std::get<gl::fragment_shader>(value) },
				& g{ std::get<gl::geometry_shader>(value) };

			return ((!v.empty() && !f.empty() && !g.empty())
				? load_from_memory(v, f, g)
				: ((!v.empty() && !f.empty())
					? load_from_memory(v, f)
					: false));
		}

		bool load_from_memory(pmr::string const & v_src, pmr::string const & f_src)
		{
			if (v_src.empty() || f_src.empty()) { return false; }
		
			m_src = { v_src, f_src, {} };

			m_obj = shader::create(v_src.c_str(), f_src.c_str(), nullptr);

			return (bool)m_obj;
		}

		bool load_from_memory(pmr::string const & v_src, pmr::string const & f_src, pmr::string const & g_src)
		{
			if (v_src.empty() || f_src.empty() || g_src.empty()) { return false; }
		
			m_src = { v_src, f_src, g_src };

			m_obj = shader::create(v_src.c_str(), f_src.c_str(), g_src.c_str());

			return (bool)m_obj;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void bind(bool bind_textures = true) const { m_obj->bind(bind_textures); }

		void unbind() const { m_obj->unbind(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> bool set_uniform(cstring name, T && value) noexcept
		{
			return m_obj->set_uniform(name, ML_forward(value));
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

		ML_NODISCARD operator bool() const noexcept { return (bool)m_obj; }
		
		ML_NODISCARD gl::handle get_handle() const noexcept { return m_obj->get_handle(); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		shader_source m_src;
		shared<shader> m_obj;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// WIP
	struct shader_cache final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using source_storage = ds::array<ds::map<pmr::string, pmr::string>, gl::shader_type_MAX>;

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

		auto & src(uint32_t i) & noexcept { return m_src[static_cast<size_t>(i)]; }

		auto const & src(uint32_t i) const & noexcept { return m_src[static_cast<size_t>(i)]; }

		pmr::string & src(uint32_t i, pmr::string const & name) & noexcept
		{
			return this->src(i)[name];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_src(uint32_t i, pmr::string const & name, fs::path const & path)
		{
			if (auto const contents{ util::get_file_contents(path) })
			{
				this->src(i, name) = { contents->begin(), contents->end() };
				
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