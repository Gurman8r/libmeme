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
	"glfw",
}

defines{
	"_CRT_SECURE_NO_WARNINGS",
	"ML_API=ML_API_EXPORT",
	"ML_RENDERER_OPENGL",
	--"ML_OPENGL_LOADER_GLAD",
	"ML_OPENGL_LOADER_GLEW",
	"IMGUI_IMPL_OPENGL_LOADER_GLEW",
}

includedirs{
	"%{sln_dir}build/%{prj.name}.lua",
	"%{sln_dir}include",
	"%{ext_dir}",
	"%{ext_dir}json/include",
	"%{ext_dir}imgui",
	"%{ext_dir}pybind11/include",
	"%{ext_dir}cpython/Include",
	"%{ext_dir}cpython/Include/internal",
	"%{ext_dir}assimp/include",
	"%{ext_dir}freetype2/include",
	"%{ext_dir}freetype2/include/freetype",
	"%{ext_dir}glfw/include",
	"%{ext_dir}raknet/Source",
}

files{
	"%{sln_dir}build/%{prj.name}.lua",
	"%{sln_dir}include/libmeme/**.hpp",
	"%{sln_dir}src/libmeme/**.cpp",
	"%{sln_dir}tools/**.**",
	"%{sln_dir}premake5.lua",
	"%{sln_dir}README.md",

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

filter { "configurations:Debug" }
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
	defines{
		"NOMINMAX",
		"IMGUI_API=__declspec(dllexport)",
	}
	includedirs{
		"%{ext_dir}cpython/PC",
	}
	postbuildcommands{	
		"%{ml_copy} %{bin_lib}%{cfg.platform}\\%{cfg.buildcfg}\\%{prj.name}.dll %{bin_out}",
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\assimp.dll %{bin_out}",
	}
	
filter{ "system:Windows", "configurations:Debug" }
	postbuildcommands{	
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\glew32d.dll %{bin_out}",
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\python39_d.dll %{bin_out}",
	}
	linkoptions{
		"/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib",
	}
	
filter{ "system:Windows", "configurations:Release" }
	postbuildcommands{
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\glew32.dll %{bin_out}",
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\python39.dll %{bin_out}",
	}
	linkoptions{
		"/NODEFAULTLIB:LIBCMT.lib",
	}
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --