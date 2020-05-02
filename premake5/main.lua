-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group			""
project			"main"
targetname 		"%{prj.name}"
targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{bin_obj}"
location		"%{prj_dir}%{prj.name}/"
debugdir 		"%{bin_out}"
language		"C++"
cppdialect 		"C++17"
staticruntime	"Off"
rtti			"Off"
systemversion	"latest"

dependson{
	"libmeme",
}

defines{
	"_CRT_SECURE_NO_WARNINGS", "NOMINMAX",
}

includedirs{
	"%{sln_dir}include",
	"%{ext_dir}",
	"%{ext_dir}json/include",
}

files{
	"%{sln_dir}src/%{prj.name}/**.**",
}

libdirs{
	"%{bin_lib}", "%{bin_lib}%{cfg.platform}/", "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/",
	"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
}

links{
	"libmeme",
}

postbuildcommands{
	"%{ml_copy} %{lib_in}%{prj.name}%{ml_exe} %{bin_out}",
}

filter{ "configurations:Debug" }
	symbols "On"
	kind "ConsoleApp"

filter{ "configurations:Release" } 
	optimize "Speed"
	kind "WindowedApp"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --