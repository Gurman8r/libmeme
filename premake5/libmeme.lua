-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group			""
project			"libmeme"
targetname 		"%{prj.name}"
targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{bin_obj}"
location		"%{prj_dir}%{prj.name}/"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
rtti			"Off"
staticruntime	"Off"
systemversion	"latest"

dependson{
	"assets",
}

defines{
	"_CRT_SECURE_NO_WARNINGS", "NOMINMAX",
	"ML_API=ML_API_EXPORT",
	"ML_PLATFORM_GLFW",
	"ML_RENDERER_OPENGL",
	--"ML_OPENGL_LOADER_GLAD",
	"ML_OPENGL_LOADER_GLEW",

	-- imgui
	"IMGUI_USER_CONFIG=<libmeme/Config.hpp>",
	"IMGUI_API=ML_API_EXPORT",
	"IMGUI_IMPL_OPENGL_LOADER_GLEW",
}

includedirs{
	"%{sln_dir}premake5/%{prj.name}.lua",
	"%{sln_dir}include",
	"%{ext_dir}",
	"%{ext_dir}json/include",
	"%{ext_dir}pybind11/include",
	"%{ext_dir}cpython/Include",
	"%{ext_dir}cpython/Include/internal",
	"%{ext_dir}assimp/include",
	"%{ext_dir}freetype2/include",
	"%{ext_dir}freetype2/include/freetype",
	"%{ext_dir}glfw/include",

	-- imgui
	"%{ext_dir}imgui",
	"%{ext_dir}imgui-node-editor/NodeEditor/Include",
}

files{
	"%{sln_dir}premake5/%{prj.name}.lua",
	"%{sln_dir}include/libmeme/**.hpp",
	"%{sln_dir}src/libmeme/**.cpp",

	-- imgui
	"%{ext_dir}imgui/*.h",
	"%{ext_dir}imgui/*.cpp",
	"%{ext_dir}imgui/examples/imgui_impl_glfw.h",
	"%{ext_dir}imgui/examples/imgui_impl_glfw.cpp",
	"%{ext_dir}imgui/examples/imgui_impl_opengl3.h",
	"%{ext_dir}imgui/examples/imgui_impl_opengl3.cpp",
	"%{ext_dir}ImGuiColorTextEdit/*.h",
	"%{ext_dir}ImGuiColorTextEdit/*.cpp",
	"%{ext_dir}ImGuizmo/*.h",
	"%{ext_dir}ImGuizmo/*.cpp",
	"%{ext_dir}imgui_club/imgui_memory_editor/imgui_memory_editor.h",
	"%{ext_dir}imgui-node-editor/NodeEditor/Include/**.**",
	"%{ext_dir}imgui-node-editor/NodeEditor/Source/**.**",
}

libdirs{
	"%{bin_lib}", "%{bin_lib}%{cfg.platform}/", "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/",
	"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
}

links{
	"glfw",
	"opengl32",
	"freetype",
	"assimp",
	"IrrXML",
	"zlibstatic",
}

postbuildcommands{
	"%{ml_copy} %{lib_in}%{prj.name}%{ml_dll} %{bin_out}",
}

filter{ "configurations:Debug" }
	symbols "On"
	links{
		"glew32d",
		"python39_d",
	}

filter{ "configurations:Release" }
	optimize "Speed"
	links{
		"glew32",
		"python39",
	}

-- WINDOWS

filter{ "system:Windows" }
	includedirs{
		"%{ext_dir}cpython/PC",
	}

filter{ "system:Windows", "configurations:Debug" }
	linkoptions{
		"/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib"
	}

filter{ "system:Windows", "configurations:Release" }
	linkoptions{
		"/NODEFAULTLIB:LIBCMT.lib"
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --