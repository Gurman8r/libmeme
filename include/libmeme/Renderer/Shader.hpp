#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <libmeme/Renderer/Uniform.hpp>
#include <libmeme/Core/DenseMap.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API Shader final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Source final
		{
			C_String vs{ nullptr }, gs{ nullptr }, fs{ nullptr };

			constexpr Source() noexcept = default;
		};

		using AttribCache = typename dense_map<hash_t, int32_t>;

		using UniformCache = typename dense_map<hash_t, int32_t>;

		using TextureCache = typename dense_map<struct Texture const *, int32_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader();
		explicit Shader(Source const & source);
		explicit Shader(path_t const & v, path_t const & f);
		explicit Shader(path_t const & v, path_t const & g, path_t const & f);
		Shader(Shader const & other);
		Shader(Shader && other) noexcept;
		~Shader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader & operator=(Shader const & other);

		Shader & operator=(Shader && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(Shader & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(path_t const & v_file, path_t const & f_file);

		bool loadFromFile(path_t const & v_file, path_t const g_file, path_t const & f_file);

		bool loadFromSource(Source const & value);

		bool loadFromMemory(std::string const & vs, std::string const & fs);

		bool loadFromMemory(std::string const & vs, std::string const & gs, std::string const & fs);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool generate();
		
		bool destroy();

		static void bind(Shader const * value, bool bindTextures = true);

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

		inline operator bool() const noexcept { return m_handle; }

		inline auto handle() const noexcept -> uint32_t const & { return m_handle; }

		inline auto address() const noexcept -> void * { return ML_ADDRESSOF(m_handle); }

		inline auto attributes() const noexcept -> AttribCache const & { return m_attributes; }

		inline auto textures() const noexcept -> TextureCache const & { return m_textures; }

		inline auto uniforms() const noexcept -> UniformCache const & { return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t get_attribute(std::string const & value);

		int32_t get_uniform(std::string const & value);

		int32_t compile(C_String vs, C_String gs, C_String fs);

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

	template <class ... Args> static inline auto make_shader(Args && ... args)
	{
		return Shader{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_