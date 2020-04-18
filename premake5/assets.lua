-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group			""
project			"assets"
targetname 		"%{prj.name}"
targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{bin_obj}"
location		"%{prj_dir}%{prj.name}/"
debugdir 		"%{bin_out}"
kind			"Utility"

files{
	"%{sln_dir}premake5/**.lua",
	"%{sln_dir}assets/**.**", 
	"%{sln_dir}tools/**.**",
	"%{sln_dir}premake5.lua",
	"%{sln_dir}README.md",
	"%{sln_dir}TODO.txt",
}

filter{ "system:Windows" }
	prebuildcommands{	
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\assimp.dll %{bin_out}",
	}
	
filter{ "system:Windows", "configurations:Debug" }
	prebuildcommands{	
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\glew32d.dll %{bin_out}",
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\python39_d.dll %{bin_out}",
	}
	
filter{ "system:Windows", "configurations:Release" }
	prebuildcommands{
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\glew32.dll %{bin_out}",
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\python39.dll %{bin_out}",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --