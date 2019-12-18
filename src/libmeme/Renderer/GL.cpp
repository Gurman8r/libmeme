#ifdef ML_IMPL_RENDERER_OPENGL

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Renderer/GL.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/EventSystem.hpp>
#include <libmeme/Renderer/GraphicsEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ML_IMPL_OPENGL_ES2)
#	include <GLES2/gl2.h>
#elif defined(ML_IMPL_OPENGL_ES3)
#	if defined(ML_SYSTEM_APPLE && (TARGET_OS_IOS || TARGET_OS_TV))
#		include <OpenGLES/ES3/gl.h>
#	else
#		include <GLES3/gl3.h>
#	endif
#elif defined(ML_IMPL_OPENGL_LOADER_GLEW)
#	include <GL/glew.h>
#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
#	include <GL/gl3w.h>
#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
#	include <GL/glad.h>
#elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
#	include ML_IMPL_OPENGL_LOADER_CUSTOM
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Errors
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto GL::getError() -> uint32_t
	{
		return glGetError();
	}

	void GL::checkError(C_String file, uint32_t line, C_String expr)
	{
		if (const auto code{ getError() })
		{
			ML_EventSystem.fireEvent<RenderErrorEvent>(file, line, expr, code);
		}
	}


	// Initialization
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool GL::init()
	{
#if defined(ML_IMPL_OPENGL_LOADER_GLEW)
		glewExperimental = true;
		return (glewInit() == GLEW_OK);
#elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
		return (gl3wInit() != 0);
#elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
		return (gladLoadGL() == 0);
#elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
		return false;
#endif
	}

	void GL::validateVersion(uint32_t & major, uint32_t & minor)
	{
		major = (uint32_t)getInteger(GL::MajorVersion);
		minor = (uint32_t)getInteger(GL::MinorVersion);

		if (getError() == GL::InvalidEnum)
		{
			if (auto version{ getString(GL::Version) })
			{
				major = version[0] - '0';
				minor = version[2] - '0';
				Debug::logWarning("Using OpenGL Version: {0}.{1}", major, minor);
			}
			else
			{
				major = 1;
				minor = 1;
				Debug::logWarning("Can't get the version number, assuming 1.1");
			}
		}

		if (!shadersAvailable())
		{
			Debug::logError("Shaders are not available on your system.");
		}

		if (!geometryShadersAvailable())
		{
			Debug::logError("Geometry shaders are not available on your system.");
		}

		if (!framebuffersAvailable())
		{
			Debug::logWarning("Framebuffers Unavailable");
		}

		if (!edgeClampAvailable())
		{
			Debug::logWarning("Texture Edge Clamp Unavailable");
		}

		if (!textureSrgbAvailable())
		{
			Debug::logWarning("Texture sRGB Unavailable");
		}
	}


	// Flags
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool GL::isEnabled(uint32_t value)
	{
		bool temp{ false };
		glCheck(temp = glIsEnabled(value));
		return temp;
	}

	bool GL::enable(uint32_t value, bool cond)
	{
		if (cond)
		{
			glCheck(glEnable(value));
			return isEnabled(value);
		}
		return true;
	}

	bool GL::disable(uint32_t value, bool cond)
	{
		if (cond)
		{
			glCheck(glDisable(value));
			return !isEnabled(value);
		}
		return true;
	}


	// Getters
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto GL::getString(uint32_t name) -> C_String
	{
		C_String temp{ nullptr };
		glCheck(temp = reinterpret_cast<C_String>(glGetString(name)));
		return temp;
	}

	auto GL::getString(uint32_t name, uint32_t index) -> C_String
	{
		C_String temp{ nullptr };
		glCheck(temp = reinterpret_cast<C_String>(glGetStringi(name, index)));
		return temp;
	}

	bool GL::getBool(uint32_t name)
	{
		uint8_t temp{ 0 };
		glCheck(glGetBooleanv(name, &temp));
		return (bool)temp;
	}

	auto GL::getDouble(uint32_t name) -> float64_t
	{
		float64_t temp{ 0 };
		glCheck(glGetDoublev(name, &temp));
		return temp;
	}

	auto GL::getFloat(uint32_t name) -> float32_t
	{
		float32_t temp{ 0 };
		glCheck(glGetFloatv(name, &temp));
		return temp;
	}

	auto GL::getInteger(uint32_t name) -> int32_t
	{
		int32_t temp{ 0 };
		glCheck(getInteger(name, &temp));
		return temp;
	}

	auto GL::getInteger(uint32_t name, int32_t * params) -> int32_t *
	{
		glCheck(glGetIntegerv(name, params));
		return params;
	}


	// Functions
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void GL::activeTexture(uint32_t value)
	{
		glCheck(glActiveTexture(value));
	}

	void GL::alphaFunc(uint32_t func, float32_t value)
	{
		glCheck(glAlphaFunc(func, value));
	}

	void GL::blendFunc(uint32_t sFactor, uint32_t dFactor)
	{
		glCheck(glBlendFunc(sFactor, dFactor));
	}

	void GL::blendEquation(uint32_t equation)
	{
		glCheck(glBlendEquation(equation));
	}

	void GL::cullFace(uint32_t mode)
	{
		glCheck(glCullFace(mode));
	}

	void GL::depthFunc(uint32_t cmp)
	{
		glCheck(glDepthFunc(cmp));
	}

	void GL::depthMask(bool value)
	{
		glCheck(glDepthMask(value));
	}

	void GL::viewport(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		glCheck(glViewport(x, y, w, h));
	}

	void GL::blendEquationSeparate(uint32_t modeRGB, uint32_t modeAlpha)
	{
		glCheck(glBlendEquationSeparate(modeRGB, modeAlpha));
	}

	void GL::blendFuncSeparate(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha)
	{
		glCheck(glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha));
	}

	void GL::polygonMode(uint32_t face, uint32_t mode)
	{
		glCheck(glPolygonMode(face, mode));
	}


	// Drawing
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void GL::clear(uint32_t mask)
	{
		glCheck(glClear(mask));
	}

	void GL::clearColor(float32_t r, float32_t g, float32_t b, float32_t a)
	{
		glCheck(glClearColor(r, g, b, a));
	}

	void GL::drawElements(uint32_t mode, int32_t count, uint32_t type, void * indices)
	{
		glCheck(glDrawElements(mode, count, type, indices));
	}

	void GL::drawArrays(uint32_t mode, int32_t first, int32_t count)
	{
		glCheck(glDrawArrays(mode, first, count));
	}

	void GL::flush()
	{
		glCheck(glFlush());
	}



	// Buffers
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto GL::genBuffer() -> uint32_t
	{
		return genBuffers(1);
	}

	auto GL::genBuffers(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenBuffers(count, &temp));
		return temp;
	}

	auto GL::genVertexArray() -> uint32_t
	{
		return genVertexArrays(1);
	}

	auto GL::genVertexArrays(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenVertexArrays(count, &temp));
		return temp;
	}

	void GL::bindBuffer(uint32_t type, uint32_t buffer)
	{
		glCheck(glBindBuffer(type, buffer));
	}

	void GL::bindVertexArray(uint32_t value)
	{
		glCheck(glBindVertexArray(value));
	}

	void GL::bufferData(uint32_t type, uint32_t size, void * data, uint32_t usage)
	{
		glCheck(glBufferData(type, size, data, usage));
	}

	void GL::bufferSubData(uint32_t target, int32_t offset, int32_t size, void * data)
	{
		glCheck(glBufferSubData(target, offset, size, data));
	}

	bool GL::deleteBuffer(uint32_t const * value)
	{
		return value ? ML_TRUE_EXPR(deleteBuffers(1, value)) : false;
	}

	void GL::deleteBuffers(uint32_t count, uint32_t const * buffers)
	{
		glCheck(glDeleteBuffers(count, buffers));
	}

	bool GL::deleteVertexArray(uint32_t const * value)
	{
		return value ? ML_TRUE_EXPR(deleteVertexArrays(1, value)) : false;
	}

	void GL::deleteVertexArrays(uint32_t count, uint32_t const * arrays)
	{
		glCheck(glDeleteVertexArrays(count, arrays));
	}

	void GL::vertexAttribPointer(uint32_t index, uint32_t size, uint32_t type, bool normalized, uint32_t stride, void * pointer)
	{
		glCheck(glVertexAttribPointer(
			index,
			size,
			type,
			normalized,
			stride,
			pointer
		));
	}

	void GL::vertexAttribPointer(uint32_t index, uint32_t size, uint32_t type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width)
	{
#ifdef ML_CC_MSC
#	pragma warning(push)
#	pragma warning(disable: 4312)
#	pragma warning(disable: 26451)
#endif
		return vertexAttribPointer(
			index,
			size,
			type,
			normalized,
			(stride * width),
			// causes a warning in 64-bit
			reinterpret_cast<void *>(offset * width)
		);
#ifdef ML_CC_MSC
#	pragma warning(pop)
#endif
	}

	void GL::enableVertexAttribArray(uint32_t index)
	{
		glCheck(glEnableVertexAttribArray(index));
	}



	// Textures
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool GL::edgeClampAvailable()
	{
		static bool checked{ false };
		static bool temp{ false };
		if (!checked && (checked = true))
		{
			temp = GL_EXT_texture_edge_clamp || GLEW_EXT_texture_edge_clamp;
		}
		return temp;
	}

	bool GL::textureSrgbAvailable()
	{
		static bool checked{ false };
		static bool temp{ false };
		if (!checked && (checked = true))
		{
			temp = GL_EXT_texture_sRGB;
		}
		return temp;
	}

	bool GL::nonPowerOfTwoAvailable()
	{
		static bool checked{ false };
		static bool temp{ false };
		if (!checked && (checked = true))
		{
			temp = GLEW_ARB_texture_non_power_of_two;
		}
		return temp;
	}

	auto GL::getMaxTextureUnits() -> int32_t
	{
		static int32_t temp{ 0 };
		static bool checked{ false };
		if (!checked && (checked = true))
		{
			temp = getInteger(GL::MaxCombTexImgUnits);
		}
		return temp;
	}

	auto GL::getMaxTextureSize() -> uint32_t
	{
		static uint32_t temp{ 0 };
		static bool checked{ false };
		if (!checked && (checked = true))
		{
			temp = (uint32_t)(getInteger(GL::MaxTextureSize));
		}
		return temp;
	}

	auto GL::getValidTextureSize(uint32_t value) -> uint32_t
	{
		if (!nonPowerOfTwoAvailable())
		{
			uint32_t powerOfTwo = 1;
			while (powerOfTwo < value)
			{
				powerOfTwo *= 2;
			}
			return powerOfTwo;
		}
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto GL::genTexture() -> uint32_t
	{
		return genTextures(1);
	}

	auto GL::genTextures(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenTextures(count, &temp));
		return temp;
	}

	bool GL::deleteTexture(uint32_t const * value)
	{
		return value ? ML_TRUE_EXPR(deleteTextures(1, value)) : false;
	}

	void GL::deleteTextures(uint32_t count, uint32_t const * id)
	{
		glCheck(glDeleteTextures(count, id));
	}

	void GL::bindTexture(uint32_t target, uint32_t id)
	{
		glCheck(glBindTexture(target, id));
	}

	void GL::texImage2D(uint32_t target, int32_t level, uint32_t internalFormat, int32_t width, int32_t height, int32_t border, uint32_t format, uint32_t type, void * pixels)
	{
		glCheck(glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels));
	}

	void GL::texSubImage2D(uint32_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t width, int32_t height, uint32_t format, uint32_t type, void * pixels)
	{
		glCheck(glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels));
	}

	void GL::texParameter(uint32_t target, uint32_t name, uint32_t param)
	{
		glCheck(glTexParameteri(target, name, param));
	}

	void GL::getTexImage(uint32_t target, int32_t level, uint32_t format, uint32_t type, void * pixels)
	{
		glCheck(glGetTexImage(target, level, format, type, pixels));
	}

	void GL::generateMipmap(uint32_t target)
	{
		glCheck(glGenerateMipmap(target));
	}

	void GL::pixelStore(uint32_t name, int32_t param)
	{
		glCheck(glPixelStorei(name, param));
	}

	void GL::scissor(int32_t x, int32_t y, int32_t width, int32_t height)
	{
		glCheck(glScissor(x, y, width, height));
	}

	void GL::bindSampler(uint32_t unit, int32_t sampler)
	{
		glCheck(glBindSampler(unit, sampler));
	}


	// Framebuffers
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool GL::framebuffersAvailable()
	{
		static bool checked{ false };
		static bool temp{ false };
		if (!checked)
		{
			checked = true;
			temp = GL_EXT_framebuffer_object && GL_EXT_framebuffer_blit;
		}
		return temp;
	}

	auto GL::genFramebuffer() -> uint32_t
	{
		return genFramebuffers(1);
	}

	auto GL::genFramebuffers(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenFramebuffers(count, &temp));
		return temp;
	}

	auto GL::checkFramebufferStatus(uint32_t target) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(temp = glCheckFramebufferStatus(target));
		return temp;
	}

	void GL::bindFramebuffer(uint32_t target, uint32_t framebuffer)
	{
		glCheck(glBindFramebuffer(target, framebuffer));
	}

	bool GL::deleteFramebuffer(uint32_t const * value)
	{
		return value ? ML_TRUE_EXPR(deleteFramebuffers(1, value)) : false;
	}

	void GL::deleteFramebuffers(uint32_t count, uint32_t const * framebuffers)
	{
		glCheck(glDeleteFramebuffers(count, framebuffers));
	}

	void GL::blitFramebuffer(int32_t srcX0, int32_t srcY0, int32_t srcX1, int32_t srcY1, int32_t dstX0, int32_t dstY0, int32_t dstX1, int32_t dstY1, uint8_t mask, uint32_t filter)
	{
		glCheck(glBlitFramebuffer(
			srcX0, srcY0, srcX1, srcY1,
			dstX0, dstY0, dstX1, dstY1,
			mask,
			filter
		));
	}

	void GL::framebufferTexture2D(uint32_t target, uint32_t attachment, uint32_t textarget, uint32_t texture, int32_t level)
	{
		glCheck(glFramebufferTexture2D(target, attachment, textarget, texture, level));
	}


	// Renderbuffers
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto GL::genRenderbuffer() -> uint32_t
	{
		return genRenderbuffers(1);
	}

	auto GL::genRenderbuffers(uint32_t count) -> uint32_t
	{
		uint32_t temp{ 0 };
		glCheck(glGenRenderbuffers(count, &temp));
		return temp;
	}

	bool GL::deleteRenderbuffer(uint32_t const * value)
	{
		return value ? ML_TRUE_EXPR(deleteRenderbuffers(1, value)) : false;
	}

	void GL::deleteRenderbuffers(uint32_t count, uint32_t const * renderbuffers)
	{
		glCheck(glDeleteRenderbuffers(count, renderbuffers));
	}

	void GL::bindRenderbuffer(uint32_t target, uint32_t renderbuffer)
	{
		glCheck(glBindRenderbuffer(target, renderbuffer));
	}

	void GL::renderbufferStorage(uint32_t target, uint32_t value, int32_t width, int32_t height)
	{
		glCheck(glRenderbufferStorage(target, value, width, height));
	}

	void GL::framebufferRenderbuffer(uint32_t target, uint32_t attachment, uint32_t renderbuffertarget, uint32_t renderbuffer)
	{
		glCheck(glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));

	}


	// Shaders
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool GL::shadersAvailable()
	{
		static bool checked{ false };
		static bool available{ false };
		if (!checked && (checked = true))
		{
			available =
				GL_ARB_multitexture &&
				GL_ARB_shading_language_100 &&
				GL_ARB_shader_objects &&
				GL_ARB_vertex_shader &&
				GL_ARB_fragment_shader;
		}
		return available;
	}

	bool GL::geometryShadersAvailable()
	{
		static bool available = false;
		static bool checked{ false };
		if (!checked && (checked = true))
		{
			checked = true;
			available = shadersAvailable() && GL_ARB_geometry_shader4;
		}
		return available;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	auto GL::getProgramInfoLog(uint32_t obj) -> C_String
	{
		static char temp[512];
#ifdef GL_ARB_shader_objects
		glCheck(glGetInfoLogARB(obj, sizeof(temp), 0, temp));
#else
		glCheck(glGetProgramInfoLog(obj, sizeof(temp), 0, temp));
#endif
		return temp;
	}

	auto GL::getProgramHandle(uint32_t name) -> uint32_t
	{
		uint32_t temp{ 0 };
#ifdef GL_ARB_shader_objects
		glCheck(temp = glGetHandleARB(name));
#else
#	error FIXME
#endif
		return temp;
	}

	auto GL::createProgram() -> uint32_t
	{
		uint32_t temp{ 0 };
#ifdef GL_ARB_shader_objects
		glCheck(temp = glCreateProgramObjectARB());
#else
		glCheck(temp = glCreateProgram());
#endif
		return temp;
	}

	auto GL::createShader(uint32_t type) -> uint32_t
	{
		uint32_t temp{ 0 };
#ifdef GL_ARB_shader_objects
		glCheck(temp = glCreateShaderObjectARB(type));
#else
		glCheck(temp = glCreateShader(type));
#endif
		return temp;
	}

	auto GL::getProgramParameter(int32_t obj, uint32_t param) -> int32_t
	{
		int32_t temp{ 0 };
#ifdef GL_ARB_shader_objects
		glCheck(glGetObjectParameterivARB(obj, param, &temp));
#else
		glCheck(glGetProgramiv(obj, param, &temp));
#endif
		return temp;
	}

	auto GL::getAttribLocation(uint32_t program, C_String name) -> int32_t
	{
		int32_t temp{ 0 };
#ifdef GL_ARB_shader_objects
		glCheck(temp = glGetAttribLocationARB(program, name));
#else
		glCheck(temp = glGetAttribLocation(program, name));
#endif
		return temp;
	}

	auto GL::getUniformLocation(uint32_t program, C_String name) -> int32_t
	{
		int32_t temp{ 0 };
#ifdef GL_ARB_shader_objects
		glCheck(temp = glGetUniformLocationARB(program, name));
#else
		glCheck(temp = glGetUniformLocation(program, name));
#endif
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void GL::useProgram(uint32_t obj)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUseProgramObjectARB(obj));
#else
		glCheck(glUseProgram(obj));
#endif
	}

	void GL::deleteShader(uint32_t obj)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glDeleteObjectARB(obj));
#else
		glCheck(glDeleteShader(obj));
#endif
	}

	void GL::detachShader(uint32_t containerObj, uint32_t obj)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glDetachObjectARB(containerObj, obj));
#else
		glCheck(glDetachShader(containerObj, obj));
#endif
	}

	void GL::attachShader(uint32_t containerObj, uint32_t obj)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glAttachObjectARB(containerObj, obj));
#else
		glCheck(glAttachShader(containerObj, obj));
#endif
	}

	void GL::shaderSource(uint32_t obj, int32_t count, C_String const * src, int32_t const * length)
	{
		glCheck(glShaderSource(obj, count, &src[0], length));
	}

	auto GL::compileShader(uint32_t obj) -> int32_t
	{
#ifdef GL_ARB_shader_objects
		glCheck(glCompileShaderARB(obj));
#else
		glCheck(glCompileShader(obj));
#endif
		return getProgramParameter(obj, GL::ObjectCompileStatus);
	}

	auto GL::compileShader(uint32_t & obj, uint32_t type, int32_t count, C_String const * source) -> int32_t
	{
		C_String log{ nullptr };
		return compileShader(obj, type, count, source, log);
	}

	auto GL::compileShader(uint32_t & obj, uint32_t type, int32_t count, C_String const * source, C_String & log) -> int32_t
	{
		log = nullptr;
		if ((count < 1) || !source || !(*source))
		{
			return ML_WARNING; // -1 (true)
		}
		else if (obj = createShader(type))
		{
			shaderSource(obj, count, source, nullptr);
			
			if (!compileShader(obj))
			{
				log = getProgramInfoLog(obj);
				
				deleteShader(obj);
				
				return ML_FAILURE; // 0 (false)
			}

			return ML_SUCCESS; // +1 (true)
		}
		return ML_FAILURE; // 0 (false)
	}

	auto GL::linkProgram(uint32_t obj) -> int32_t
	{
#ifdef GL_ARB_shader_objects
		glCheck(glLinkProgramARB(obj));
#else
		glCheck(glLinkProgram(obj));
#endif
		return getProgramParameter(obj, GL::ObjectLinkStatus);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void GL::uniform1i(int32_t location, int32_t value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform1iARB(location, value));
#else
		glCheck(glUniform1i(location, value));
#endif
	}

	void GL::uniform2i(int32_t location, int32_t x, int32_t y)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform2iARB(location, x, y));
#else
		glCheck(glUniform2i(location, x, y));
#endif
	}

	void GL::uniform3i(int32_t location, int32_t x, int32_t y, int32_t z)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform3iARB(location, x, y, z));
#else
		glCheck(glUniform3i(location, x, y, z));
#endif
	}

	void GL::uniform4i(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform4iARB(location, x, y, z, w));
#else
		glCheck(glUniform4i(location, x, y, z, w));
#endif
	}

	void GL::uniform1iv(int32_t location, uint32_t count, int32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform1ivARB(location, count, value));
#else
		glCheck(glUniform1iv(location, count, value));
#endif
	}

	void GL::uniform2iv(int32_t location, uint32_t count, int32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform2ivARB(location, count, value));
#else
		glCheck(glUniform2iv(location, count, value));
#endif
	}

	void GL::uniform3iv(int32_t location, uint32_t count, int32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform3ivARB(location, count, value));
#else
		glCheck(glUniform3iv(location, count, value));
#endif
	}

	void GL::uniform4iv(int32_t location, uint32_t count, int32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform4ivARB(location, count, value));
#else
		glCheck(glUniform4iv(location, count, value));
#endif
	}

	void GL::uniform1f(int32_t location, float32_t value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform1fARB(location, value));
#else
		glCheck(glUniform1f(location, value));
#endif
	}

	void GL::uniform2f(int32_t location, float32_t x, float32_t y)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform2fARB(location, x, y));
#else
		glCheck(glUniform2f(location, x, y));
#endif
	}

	void GL::uniform3f(int32_t location, float32_t x, float32_t y, float32_t z)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform3fARB(location, x, y, z));
#else
		glCheck(glUniform3f(location, x, y, z));
#endif
	}

	void GL::uniform4f(int32_t location, float32_t x, float32_t y, float32_t z, float32_t w)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform4fARB(location, x, y, z, w));
#else
		glCheck(glUniform4f(location, x, y, z, w));
#endif
	}

	void GL::uniform1fv(int32_t location, uint32_t count, float32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform1fvARB(location, count, value));
#else
		glCheck(glUniform1fv(location, count, value));
#endif
	}

	void GL::uniform2fv(int32_t location, uint32_t count, float32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform2fvARB(location, count, value));
#else
		glCheck(glUniform2fv(location, count, value));
#endif
	}

	void GL::uniform3fv(int32_t location, uint32_t count, float32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform3fvARB(location, count, value));
#else
		glCheck(glUniform3fv(location, count, value));
#endif
	}

	void GL::uniform4fv(int32_t location, uint32_t count, float32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniform4fvARB(location, count, value));
#else
		glCheck(glUniform4fv(location, count, value));
#endif
	}

	void GL::uniformMatrix2fv(int32_t location, uint32_t count, bool transpose, float32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniformMatrix2fvARB(location, count, transpose, value));
#else
		glCheck(glUniformMatrix2fv(location, count, transpose, value));
#endif
	}

	void GL::uniformMatrix3fv(int32_t location, uint32_t count, bool transpose, float32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniformMatrix3fvARB(location, count, transpose, value));
#else
		glCheck(glUniformMatrix3fv(location, count, transpose, value));
#endif
	}

	void GL::uniformMatrix4fv(int32_t location, uint32_t count, bool transpose, float32_t const * value)
	{
#ifdef GL_ARB_shader_objects
		glCheck(glUniformMatrix4fvARB(location, count, transpose, value));
#else
		glCheck(glUniformMatrix4fv(location, count, transpose, value));
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif