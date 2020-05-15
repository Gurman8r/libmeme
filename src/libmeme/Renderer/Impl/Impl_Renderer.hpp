#ifndef _ML_IMPL_RENDERER_HPP_
#define _ML_IMPL_RENDERER_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Renderer/RendererAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// DirectX
#if defined(ML_RENDERER_DIRECTX)
#	error DirectX is not supported.
#endif // ML_RENDERER_DIRECTX

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// OpenGL
#if defined(ML_IMPL_RENDERER_OPENGL)
#	if defined(ML_IMPL_OPENGL_ES2)
#		include <GLES2/gl2.h>
#	elif defined(ML_IMPL_OPENGL_ES3)
#		if defined(ML_os_apple && (TARGET_OS_IOS || TARGET_OS_TV))
#			include <OpenGLES/ES3/gl.h>
#		else
#			include <GLES3/gl3.h>
#		endif
#	elif defined(ML_IMPL_OPENGL_LOADER_GLEW)
#		include <GL/glew.h>
#	elif defined(ML_IMPL_OPENGL_LOADER_GL3W)
#		include <GL/gl3w.h>
#	elif defined(ML_IMPL_OPENGL_LOADER_GLAD)
#		include <glad/glad.h>
#	elif defined(ML_IMPL_OPENGL_LOADER_CUSTOM)
#		include ML_OPENGL_LOADER_CUSTOM
#	else
#		error Unknown or invalid opengl loader specified.
#	endif
#endif // ML_IMPL_RENDERER_OPENGL

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Vulkan
#if defined(ML_IMPL_RENDERER_VULKAN)
#	error Vulkan is not supported.
#endif // ML_IMPL_RENDERER_VULKAN

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_IMPL_RENDERER_HPP_