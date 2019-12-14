-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "vendor"
project "imgui"
	targetname 		"%{prj.name}"
	targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
	objdir			"%{bin_obj}"
	location		"%{prj_dir}vendor/%{prj.name}/"
	kind			"StaticLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson {
		"glfw", 
	}
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"GLEW_STATIC",
		"IMGUI_IMPL_OPENGL_LOADER_GLEW",
	}
	includedirs {
		"%{sln_dir}include",
		"%{ext_dir}",
		"%{ext_dir}imgui",
		"%{ext_dir}imgui/examples",
		"%{ext_dir}glfw/include",
	}
	files {
		"%{sln_dir}build/%{prj.name}.lua",
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
	}
	libdirs {
		"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
	}
	links {
		"opengl32", "glfw", "glew32s"
	}
	
	filter { "configurations:Debug" }
		symbols "On" 
	
	filter { "system:Windows", "configurations:Debug" }
		linkoptions { 
			"/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib"
		}
		
	filter { "configurations:Release" }
		optimize "Speed"
	
	filter { "system:Windows", "configurations:Release" }
		linkoptions { "/NODEFAULTLIB:LIBCMT.lib" }

	filter { "system:Windows" }
		defines { "IMGUI_API=__declspec(dllexport)" }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --