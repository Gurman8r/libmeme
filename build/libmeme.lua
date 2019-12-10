-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group ""
project "libmeme"
	targetname 		"%{prj.name}"
	targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
	objdir			"%{bin_obj}"
	location		"%{prj_dir}libmeme/%{prj.name}/"
	kind			"SharedLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson {
		"glfw", "imgui", "lua", "raknet",
	}
	defines {
		"ML_AUDIO_EXPORTS", 
		"ML_CORE_EXPORTS", 
		"ML_EDITOR_EXPORTS", 
		"ML_ENGINE_EXPORTS", 
		"ML_GRAPHICS_EXPORTS", 
		"ML_NETWORK_EXPORTS", 
		"ML_WINDOW_EXPORTS", 
		"ML_INI_FILENAME=\"../../../libmeme.ini\"",
		"ML_IMPL_NEW=std::malloc",
		"ML_IMPL_DELETE=std::free",
		"ML_IMPL_PLATFORM_GLFW",
		"ML_IMPL_RENDERER_OPENGL",
		"ML_IMPL_OPENGL_LOADER_GLEW",
		"GLEW_STATIC",
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
	}
	includedirs {
		"%{sln_dir}build/%{prj.name}.lua",
		"%{sln_dir}include",
		"%{ext_dir}",
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
	files {
		"%{sln_dir}build/%{prj.name}.lua",
		"%{sln_dir}build/%{prj.name}.lua",
		"%{sln_dir}assets/**.**", 
		"%{sln_dir}include/libmeme/**.hpp",
		"%{sln_dir}src/libmeme/**.cpp",
		"%{sln_dir}tools/**.**",
		"%{sln_dir}libmeme.ini", 
		"%{sln_dir}libmeme.py", 
		"%{sln_dir}premake5.lua",
		"%{sln_dir}README.md",
	}
	libdirs {
		"%{bin_lib}", "%{bin_lib}%{cfg.platform}/", "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
	}
	links {
		"lua", "raknet", "imgui", "glfw",
		"opengl32", "glew32s", 
		"assimp", "IrrXML", "zlibstatic", "freetype",
	}
	
	filter { "system:Windows" }
		includedirs { "%{ext_dir}cpython/PC" }
		links { "ws2_32" }
		postbuildcommands {	
			"%{ml_copy} %{bin_lib}%{cfg.platform}\\%{cfg.buildcfg}\\%{prj.name}.dll %{bin_out}",
			"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\assimp.dll %{bin_out}",
		}
	
	filter { "system:Windows", "configurations:Debug" }
		symbols "On"
		links { "python39_d" }
		postbuildcommands {	
			"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\python39_d.dll %{bin_out}",
		}
		linkoptions {
			"/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib"
		}
	
	filter { "system:Windows", "configurations:Release" }
		optimize "Speed"
		links { "python39" }
		postbuildcommands {	
			"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\python39.dll %{bin_out}",
		}
		linkoptions {
			"/NODEFAULTLIB:LIBCMT.lib"
		}
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --