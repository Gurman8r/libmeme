#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <libmeme/Renderer/Uniform.hpp>
#include <libmeme/Platform/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API Shader final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Source final
		{
			C_string vs{ nullptr }, gs{ nullptr }, fs{ nullptr };

			constexpr Source() noexcept = default;
		};

		using AttribCache = typename ds::flat_map<std::string, int32_t>;

		using UniformCache = typename ds::flat_map<std::string, int32_t>;

		using TextureCache = typename ds::flat_map<int32_t, struct Texture const *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader();
		Shader(Source const & source);
		Shader(path_t const & v, path_t const & f);
		Shader(path_t const & v, path_t const & g, path_t const & f);
		Shader(Shader const & other);
		Shader(Shader && other) noexcept;
		~Shader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader & operator=(Shader const & other);

		Shader & operator=(Shader && other) noexcept;

		void swap(Shader & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & v_file, path_t const & f_file);

		bool load_from_file(path_t const & v_file, path_t const g_file, path_t const & f_file);

		bool load_from_source(Source const & value);

		bool load_from_memory(std::string const & vs, std::string const & fs);

		bool load_from_memory(std::string const & vs, std::string const & gs, std::string const & fs);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool generate();
		
		bool destroy();

		static void bind(Shader const * value, bool bindTextures = true);

		void bind(bool bindTextures = true) const;

		void unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool set_uniform(Uniform const & value);
		
		bool set_uniform(std::string const & name, bool value);
		
		bool set_uniform(std::string const & name, int32_t value);
		
		bool set_uniform(std::string const & name, float32_t value);
		
		bool set_uniform(std::string const & name, vec2 const & value);
		
		bool set_uniform(std::string const & name, vec3 const & value);
		
		bool set_uniform(std::string const & name, vec4 const & value);
		
		bool set_uniform(std::string const & name, Color const & value);
		
		bool set_uniform(std::string const & name, mat2 const & value);
		
		bool set_uniform(std::string const & name, mat3 const & value);
		
		bool set_uniform(std::string const & name, mat4 const & value);
		
		bool set_uniform(std::string const & name, struct Texture const * value);
		
		bool set_uniform(std::string const & name, struct Texture const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept { return m_handle; }

		ML_NODISCARD inline auto address() const noexcept -> void * { return ML_ADDRESSOF(m_handle); }

		ML_NODISCARD inline auto handle() const noexcept -> uint32_t const & { return m_handle; }

		ML_NODISCARD inline auto attributes() const noexcept -> AttribCache const & { return m_attributes; }

		ML_NODISCARD inline auto textures() const noexcept -> TextureCache const & { return m_textures; }

		ML_NODISCARD inline auto uniforms() const noexcept -> UniformCache const & { return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t get_attribute_location(std::string const & value);

		int32_t get_uniform_location(std::string const & value);

		int32_t compile(C_string vs, C_string gs, C_string fs);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct UniformBinder;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t		m_handle;
		Source			m_source;
		AttribCache		m_attributes;
		UniformCache	m_uniforms;
		TextureCache	m_textures;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_shader(Args && ... args)
	{
		return Shader{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_