-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group			""
project			"assets"
targetname 		"%{prj.name}"
targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{bin_obj}"
location		"%{prj_dir}%{prj.name}/"
debugdir 		"%{bin_out}"
kind			"None"

files{
	"%{sln_dir}premake5/**.lua",
	"%{sln_dir}assets/**.**", 
	"%{sln_dir}tools/**.**",
	"%{sln_dir}premake5.lua",
	"%{sln_dir}README.md",
	"%{sln_dir}TODO.txt",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --