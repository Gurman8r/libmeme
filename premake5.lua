-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Workspace
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "libmeme_%{_ACTION}"
	startproject "main"
	configurations { "Debug", "Release" }
	platforms { "x86", "x64" }
	filter { "platforms:*64" }
		architecture "x64"
	filter { "platforms:*32" }
		architecture "x86"
	filter { "platforms:*86" }
		architecture "x86"
		
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Global Paths
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter { "system:Unix", "system:Mac" }
	sln_dir = "%{wks.location}/"
	prj_dir = "%{sln_dir}proj/%{_ACTION}/"
	bin_out = "%{sln_dir}bin/%{cfg.platform}/%{cfg.buildcfg}/"
	bin_lib = "%{sln_dir}bin-lib/%{cfg.platform}/%{cfg.buildcfg}/"
	bin_obj = "%{sln_dir}bin-obj/"
	ext_dir = "%{sln_dir}ext/"
	ext_lib = "%{sln_dir}ext-lib/"
	ext_bin = "%{sln_dir}ext-bin/"
	ml_copy = "cp -rf"	
	ml_move = "mv -f"
	
filter { "system:Windows" }
	sln_dir = "%{wks.location}\\"
	prj_dir = "%{sln_dir}proj\\%{_ACTION}\\"
	bin_out = "%{sln_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	bin_lib = "%{sln_dir}bin-lib\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	bin_obj = "%{sln_dir}bin-obj\\"
	ext_dir = "%{sln_dir}ext\\"
	ext_lib = "%{sln_dir}ext-lib\\"
	ext_bin = "%{sln_dir}ext-bin\\"
	ml_copy = "xcopy /y"
	ml_move = "move /y"
	

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Generate Project Files
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

dofile "./build/glfw.lua"
dofile "./build/lua.lua"
dofile "./build/raknet.lua"
dofile "./build/imgui.lua"

dofile "./build/libmeme.lua"
dofile "./build/launcher.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --