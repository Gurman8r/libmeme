#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <libmeme/Renderer/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API shader final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum id_ : size_t { id_vertex, id_geometry, id_fragment, id_max };

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using shader_source		= typename meta::array<pmr::string, id_max>;
		using attribute_cache	= typename ds::flat_map<hash_t, int32_t>;
		using uniform_cache		= typename ds::flat_map<hash_t, int32_t>;
		using texture_cache		= typename ds::flat_map<int32_t, texture const *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader() noexcept : shader{ allocator_type{} } {}
		
		explicit shader(allocator_type const & alloc);
		
		shader(shader_source const & source, allocator_type const & alloc = {});
		
		shader(fs::path const & v, fs::path const & f, allocator_type const & alloc = {});
		
		shader(fs::path const & v, fs::path const & g, fs::path const & f, allocator_type const & alloc = {});
		
		shader(shader const & value, allocator_type const & alloc = {});
		
		shader(shader && value, allocator_type const & alloc = {}) noexcept;
		
		~shader() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader & shader::operator=(shader const & value)
		{
			shader temp{ value };
			swap(temp);
			return (*this);
		}

		shader & shader::operator=(shader && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & v_file, fs::path const & f_file);

		bool load_from_file(fs::path const & v_file, fs::path const g_file, fs::path const & f_file);

		bool load_from_source(shader_source const & value);

		bool load_from_memory(pmr::string const & v_src, pmr::string const & f_src);

		bool load_from_memory(pmr::string const & v_src, pmr::string const & g_src, pmr::string const & f_src);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(shader const * value, bool bind_textures = true);

		void bind(bool bind_textures = true) const { bind(this, bind_textures); }

		void unbind() const { bind(nullptr, false); }

		bool destroy();
		
		bool generate();

		void swap(shader & value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool set_uniform(uniform const & value);
		
		bool set_uniform(pmr::string const & name, bool value);
		
		bool set_uniform(pmr::string const & name, int32_t value);
		
		bool set_uniform(pmr::string const & name, float32_t value);
		
		bool set_uniform(pmr::string const & name, vec2 const & value);
		
		bool set_uniform(pmr::string const & name, vec3 const & value);
		
		bool set_uniform(pmr::string const & name, vec4 const & value);
		
		bool set_uniform(pmr::string const & name, color const & value);
		
		bool set_uniform(pmr::string const & name, mat2 const & value);
		
		bool set_uniform(pmr::string const & name, mat3 const & value);
		
		bool set_uniform(pmr::string const & name, mat4 const & value);
		
		bool set_uniform(pmr::string const & name, texture const * value);
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool nonzero() const noexcept { return m_handle; }

		ML_NODISCARD operator bool() const noexcept { return this->nonzero(); }

		ML_NODISCARD auto address() const noexcept -> void * { return ML_addressof(m_handle); }

		ML_NODISCARD auto handle() const noexcept -> uint32_t const & { return m_handle; }

		ML_NODISCARD auto source() const noexcept -> shader_source const & { return m_source; }

		ML_NODISCARD auto attributes() const noexcept -> attribute_cache const & { return m_attributes; }

		ML_NODISCARD auto textures() const noexcept -> texture_cache const & { return m_textures; }

		ML_NODISCARD auto uniforms() const noexcept -> uniform_cache const & { return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t get_attribute_location(pmr::string const & value);

		int32_t get_uniform_location(pmr::string const & value);

		int32_t compile(cstring v_src, cstring g_src, cstring f_src);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct uniform_binder;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t		m_handle;
		shader_source	m_source;
		attribute_cache	m_attributes;
		uniform_cache	m_uniforms;
		texture_cache	m_textures;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD inline auto make_shader(Args && ... args)
	{
		return shader{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_