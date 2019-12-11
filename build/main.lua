-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group ""
project "main"
	targetname 		"%{prj.name}"
	targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
	objdir			"%{bin_obj}"
	location		"%{prj_dir}libmeme/%{prj.name}/"
	debugdir 		"%{bin_out}"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson {
		"libmeme",
	}
	defines {
		"_CRT_SECURE_NO_WARNINGS", "NOMINMAX",
	}
	includedirs {
		"%{sln_dir}include", "%{ext_dir}",
	}
	files {
		"%{sln_dir}build/%{prj.name}.lua", 
		"%{sln_dir}src/%{prj.name}/**.**",
	}
	libdirs {
		"%{bin_lib}", "%{bin_lib}%{cfg.platform}/", "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
	}
	links {
		"libmeme", "imgui",
	}
	
	filter { "configurations:Debug" }
		symbols "On"
		kind	"ConsoleApp"
	
	filter { "configurations:Release" } 
		optimize "Speed"
		kind	"WindowedApp"
	
	filter { "system:Windows" }
		postbuildcommands { "%{ml_copy} %{bin_lib}%{cfg.platform}\\%{cfg.buildcfg}\\%{prj.name}.exe %{bin_out}" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --