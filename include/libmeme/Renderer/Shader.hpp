#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <libmeme/Renderer/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API Shader final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Source final
		{
			C_String vs{ "" }, gs{ "" }, fs{ "" };

			constexpr Source() noexcept = default;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader();
		explicit Shader(Source const & source);
		explicit Shader(std::string const & v, std::string const & f);
		explicit Shader(std::string const & v, std::string const & g, std::string const & f);
		Shader(Shader const & other);
		Shader(Shader && other) noexcept;
		~Shader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Shader & operator=(Shader const & other);

		Shader & operator=(Shader && other) noexcept;
		
		void swap(Shader & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(path_t const & v_file, path_t const & f_file);

		bool loadFromFile(path_t const & v_file, path_t const g_file, path_t const & f_file);

		bool loadFromSource(Source const & value);

		bool loadFromMemory(std::string const & v_src, std::string const & f_src);

		bool loadFromMemory(std::string const & v_src, std::string const & g_src, std::string const & f_src);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create();
		
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

		inline auto handle() const -> uint32_t const & { return m_handle; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		int32_t get_attribute(std::string const & value);

		int32_t get_uniform(std::string const & value);

		int32_t compile(C_String v_src, C_String g_src, C_String f_src);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct UniformBinder;

		using AttribCache = std::map<hash_t, int32_t>;
		
		using UniformCache = std::map<hash_t, int32_t>;
		
		using TextureCache = std::map<struct Texture const *, int32_t>;

		union
		{
			uint32_t		m_handle;
			Source			m_source;
			AttribCache		m_attribs;
			TextureCache	m_textures;
			UniformCache	m_uniforms;
		};

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