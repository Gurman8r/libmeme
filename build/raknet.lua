-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Vendor"
project "RakNet"
	targetname 		"%{prj.name}"
	location		"%{prj_dir}vendor/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"StaticLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"_WINSOCK_DEPRECATED_NO_WARNINGS",
	}
	includedirs {
		"%{ext_dir}RakNet/Source",
	}
	files {
		"%{sln_dir}build/%{prj.name}.lua",
		"%{ext_dir}RakNet/Source/**.h",
		"%{ext_dir}RakNet/Source/**.cpp",
	}
	libdirs {
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
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
		linkoptions { 
			"/NODEFAULTLIB:LIBCMT.lib",
		}
		
	filter { "system:Windows" }
		linkoptions {  }
		links { "ws2_32" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --