-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group			""
project			"assets"
targetname 		"%{prj.name}"
targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{bin_obj}"
location		"%{prj_dir}%{prj.name}/"
debugdir 		"%{bin_out}"
kind			"Utility"

dependson{
	"glfw",
}

files{
	"%{sln_dir}premake5/%{prj.name}.lua",
	"%{sln_dir}assets/**.**",
	"%{sln_dir}premake5.lua",
	"%{sln_dir}README.md",
	"%{sln_dir}TODO.txt",
}

prebuildcommands{
	"%{ml_copy} %{ext_in}assimp%{ml_dll} %{bin_out}",
}

filter{ "configurations:Debug" }
	prebuildcommands{
		"%{ml_copy} %{ext_in}glew32d%{ml_dll} %{bin_out}",
		"%{ml_copy} %{ext_in}python39_d%{ml_dll} %{bin_out}",
	}

filter{ "configurations:Release" }
	prebuildcommands{
		"%{ml_copy} %{ext_in}glew32%{ml_dll} %{bin_out}",
		"%{ml_copy} %{ext_in}python39%{ml_dll} %{bin_out}",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --