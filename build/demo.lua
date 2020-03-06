-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "plugins"
project "demo"
	targetname 		"%{prj.name}"
	targetdir		"%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/"
	objdir			"%{bin_obj}"
	location		"%{prj_dir}plugins/%{prj.name}/"
	kind			"SharedLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	rtti			"Off"
	systemversion	"latest"
	dependson {
		"main"
	}
	defines {
		"_CRT_SECURE_NO_WARNINGS", "NOMINMAX",
	}
	includedirs {
		"%{sln_dir}include", "%{ext_dir}", "%{sln_dir}plugins"
	}
	files {
		"%{sln_dir}build/%{prj.name}.lua",
		"%{sln_dir}src/plugins/%{prj.name}.**",
	}
	libdirs {
		"%{bin_lib}", "%{bin_lib}%{cfg.platform}/", "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
	}
	links {
		"libmeme", "imgui",
	}
	
	filter "configurations:Debug"
		symbols "On"
	
	filter "configurations:Release"
		optimize "Speed"
	
	filter { "system:Windows" }
		postbuildcommands { "%{ml_copy} %{bin_lib}%{cfg.platform}\\%{cfg.buildcfg}\\%{prj.name}.dll %{bin_out}" }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --