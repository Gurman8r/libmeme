-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Workspace
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "libmeme_%{_ACTION}"
	startproject "main"
	configurations{ "Debug", "Release" }
	platforms{ "x86", "x64" }
	filter{ "platforms:*64" }
		architecture "x64"
	filter{ "platforms:*32" }
		architecture "x86"
	filter{ "platforms:*86" }
		architecture "x86"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Globals
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter{ "system:Unix", "system:Mac" }
	sln_dir	= "%{wks.location}/"
	prj_dir	= "%{sln_dir}proj/%{_ACTION}/"
	bin_out	= "%{sln_dir}bin/%{_ACTION}/%{cfg.platform}/%{cfg.buildcfg}/"
	bin_lib	= "%{sln_dir}bin-lib/%{_ACTION}/"
	bin_obj	= "%{sln_dir}bin-obj/%{_ACTION}/"
	lib_in	= "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/"
	ext_dir	= "%{sln_dir}ext/"
	ext_lib	= "%{sln_dir}ext-lib/%{_ACTION}/"
	ext_bin	= "%{sln_dir}ext-bin/%{_ACTION}/"
	ext_in	= "%{ext_bin}%{cfg.platform}/%{cfg.buildcfg}/"
	ml_copy	= "cp -rf"
	ml_move	= "mv -f"
	ml_exe	= ".o"
	ml_dll	= ".so"
	ml_lib	= ".a"
	
filter{ "system:Windows" }
	sln_dir	= "%{wks.location}\\"
	prj_dir	= "%{sln_dir}proj\\%{_ACTION}\\"
	bin_out	= "%{sln_dir}bin\\%{_ACTION}\\%{cfg.platform}\\%{cfg.buildcfg}\\"
	bin_lib	= "%{sln_dir}bin-lib\\%{_ACTION}\\"
	bin_obj	= "%{sln_dir}bin-obj\\%{_ACTION}\\"
	lib_in	= "%{bin_lib}%{cfg.platform}\\%{cfg.buildcfg}\\"
	ext_dir	= "%{sln_dir}ext\\"
	ext_lib	= "%{sln_dir}ext-lib\\%{_ACTION}\\"
	ext_bin	= "%{sln_dir}ext-bin\\%{_ACTION}\\"
	ext_in	= "%{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\"
	ml_copy	= "xcopy /y"
	ml_move	= "move /y"
	ml_exe	= ".exe"
	ml_dll	= ".dll"
	ml_lib	= ".lib"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Generate Project Files
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- VENDOR
--dofile "./premake5/glad.lua"
--dofile "./premake5/lua.lua"
--dofile "./premake5/imgui.lua"
dofile "./premake5/glfw.lua"

-- SDK
dofile "./premake5/assets.lua"
dofile "./premake5/libmeme.lua"
dofile "./premake5/main.lua"

-- PLUGINS
dofile "./premake5/demo.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --