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
	"_CRT_SECURE_NO_WARNINGS",
}

includedirs{
	"%{sln_dir}include",
	"%{ext_dir}",
	"%{ext_dir}json/include",
}

files{
	"%{sln_dir}build/%{prj.name}.lua", 
	"%{sln_dir}src/%{prj.name}/**.**",
	"%{sln_dir}assets/**.**", 
	"%{sln_dir}libmeme.json", 
}

libdirs{
	"%{bin_lib}", "%{bin_lib}%{cfg.platform}/", "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/",
	"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
}

links{
	"libmeme",
}

filter{ "configurations:Debug" }
	kind "ConsoleApp"
	symbols "On"

filter{ "configurations:Release" } 
	kind "WindowedApp"
	optimize "Speed"

-- WINDOWS

filter{ "system:Windows" }
	defines{
		"NOMINMAX",
	}
	postbuildcommands{
		"%{ml_copy} %{bin_lib}%{cfg.platform}\\%{cfg.buildcfg}\\%{prj.name}.exe %{bin_out}",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --