-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group			"vendor"
project			"glad"
targetname 		"%{prj.name}"
targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{bin_obj}"
location		"%{prj_dir}vendor/%{prj.name}/"
kind			"StaticLib"
language		"C++"
cppdialect 		"C++17"
staticruntime	"Off"
systemversion	"latest"

defines{
	"_CRT_SECURE_NO_WARNINGS", "NOMINMAX",
	"NOMINMAX",
	"MAKE_LIB",
}

includedirs{
	"%{sln_dir}include",
}

files{
	"%{sln_dir}premake5/%{prj.name}.lua",
	"%{sln_dir}include/%{prj.name}/glad.h",
	"%{sln_dir}src/%{prj.name}/glad.c",
}

libdirs{
	"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
}

links{
	"opengl32",
}

filter{ "configurations:Debug" }
	symbols "On"

filter{ "configurations:Release" }
	optimize "Speed"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --