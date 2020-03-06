#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <libmeme/Renderer/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API shader final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { ID_Vert, ID_Geom, ID_Frag, ID_MAX };

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using shader_source		= typename meta::array<pmr::string, ID_MAX>;
		using attribute_cache	= typename ds::flat_map<pmr::string, int32_t>;
		using uniform_cache		= typename ds::flat_map<pmr::string, int32_t>;
		using texture_cache		= typename ds::flat_map<int32_t, struct texture const *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shader();
		
		explicit shader(allocator_type const & alloc);
		
		shader(shader_source const & source, allocator_type const & alloc = {});
		
		shader(filesystem::path const & v, filesystem::path const & f, allocator_type const & alloc = {});
		
		shader(filesystem::path const & v, filesystem::path const & g, filesystem::path const & f, allocator_type const & alloc = {});
		
		shader(shader const & other, allocator_type const & alloc = {});
		
		shader(shader && other, allocator_type const & alloc = {}) noexcept;
		
		~shader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline shader & shader::operator=(shader const & other)
		{
			shader temp{ other };
			this->swap(temp);
			return (*this);
		}

		inline shader & shader::operator=(shader && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(filesystem::path const & v_file, filesystem::path const & f_file);

		bool load_from_file(filesystem::path const & v_file, filesystem::path const g_file, filesystem::path const & f_file);

		bool load_from_source(shader_source const & value);

		bool load_from_memory(pmr::string const & v, pmr::string const & f);

		bool load_from_memory(pmr::string const & v, pmr::string const & g, pmr::string const & f);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(shader const * value, bool bindTextures = true);

		inline void bind(bool bindTextures = true) const { bind(this, bindTextures); }

		inline void unbind() const { bind(nullptr, false); }

		bool destroy();
		
		bool generate();

		void swap(shader & other) noexcept;

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
		
		bool set_uniform(pmr::string const & name, struct texture const * value);
		
		bool set_uniform(pmr::string const & name, struct texture const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept { return m_handle; }

		ML_NODISCARD inline auto address() const noexcept -> void * { return ML_addressof(m_handle); }

		ML_NODISCARD inline auto handle() const noexcept -> uint32_t const & { return m_handle; }

		ML_NODISCARD inline auto source() const noexcept -> shader_source const & { return m_source; }

		ML_NODISCARD inline auto attributes() const noexcept -> attribute_cache const & { return m_attributes; }

		ML_NODISCARD inline auto textures() const noexcept -> texture_cache const & { return m_textures; }

		ML_NODISCARD inline auto uniforms() const noexcept -> uniform_cache const & { return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t get_attribute_location(pmr::string const & value);

		int32_t get_uniform_location(pmr::string const & value);

		int32_t compile(cstring v, cstring g, cstring f);

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
	> ML_NODISCARD static inline auto make_shader(Args && ... args)
	{
		return shader{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_