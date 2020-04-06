-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group			""
project			"vendor"
targetname 		"%{prj.name}"
targetdir		"%{bin_lib}/%{cfg.platform}/%{cfg.buildcfg}/"
objdir			"%{bin_obj}"
location		"%{prj_dir}vendor/%{prj.name}/"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
staticruntime	"Off"
systemversion	"latest"

dependson {
}

defines {
	"_CRT_SECURE_NO_WARNINGS",
	"IMGUI_IMPL_OPENGL_LOADER_GLEW",
	--"IMGUI_IMPL_OPENGL_LOADER_GLAD",
	--"GLEW_STATIC",
	--"GLEW_BUILD"
}

includedirs {
	-- glfw
	"%{ext_dir}glfw/include",
	"%{ext_dir}glfw/include",
	"%{ext_dir}glfw/src",

	-- imgui
	"%{sln_dir}include",
	"%{ext_dir}",
	"%{ext_dir}imgui",
	"%{ext_dir}imgui/examples",
	"%{ext_dir}glfw/include",
}

files {
	"%{sln_dir}build/%{prj.name}.lua",

	-- glfw
	"%{ext_dir}glfw/include/glfw/**.h",
	"%{ext_dir}glfw/src/context.c", 
	"%{ext_dir}glfw/src/egl_context.c",
	"%{ext_dir}glfw/src/egl_context.h",
	"%{ext_dir}glfw/src/glfw_config.h",  
	"%{ext_dir}glfw/src/init.c", 
	"%{ext_dir}glfw/src/input.c", 
	"%{ext_dir}glfw/src/internal.h", 
	"%{ext_dir}glfw/src/mappings.h", 
	"%{ext_dir}glfw/src/monitor.c", 
	"%{ext_dir}glfw/src/osmesa_context.c",
	"%{ext_dir}glfw/src/osmesa_context.h",
	"%{ext_dir}glfw/src/vulkan.c",
	"%{ext_dir}glfw/src/wgl_context.c",
	"%{ext_dir}glfw/src/wgl_context.h",
	"%{ext_dir}glfw/src/window.c",

	-- imgui
	"%{ext_dir}imgui/*.h",
	"%{ext_dir}imgui/*.cpp",
	"%{ext_dir}imgui/examples/imgui_impl_glfw.h",
	"%{ext_dir}imgui/examples/imgui_impl_glfw.cpp",
	"%{ext_dir}imgui/examples/imgui_impl_opengl3.h",
	"%{ext_dir}imgui/examples/imgui_impl_opengl3.cpp",
	"%{ext_dir}ImGuiColorTextEdit/*.h",
	"%{ext_dir}ImGuiColorTextEdit/*.cpp",
	"%{ext_dir}ImGuizmo/*.h",
	"%{ext_dir}ImGuizmo/*.cpp",
	"%{ext_dir}imgui_club/imgui_memory_editor/imgui_memory_editor.h",
	
	-- lua
	"%{ext_dir}lua/lapi.h", "%{ext_dir}lua/lauxlib.h", "%{ext_dir}lua/lcode.h", "%{ext_dir}lua/lctype.h",
	"%{ext_dir}lua/ldebug.h", "%{ext_dir}lua/ldo.h", "%{ext_dir}lua/lfunc.h", "%{ext_dir}lua/lgc.h",
	"%{ext_dir}lua/ljumptab.h", "%{ext_dir}lua/llex.h", "%{ext_dir}lua/llimits.h", "%{ext_dir}lua/lmem.h",
	"%{ext_dir}lua/lobject.h", "%{ext_dir}lua/lopcodes.h", "%{ext_dir}lua/lopnames.h", "%{ext_dir}lua/lparser.h",
	"%{ext_dir}lua/lprefix.h", "%{ext_dir}lua/lstate.h", "%{ext_dir}lua/lstring.h", "%{ext_dir}lua/ltable.h",
	"%{ext_dir}lua/ltests.h", "%{ext_dir}lua/ltm.h", "%{ext_dir}lua/luaconf.h", "%{ext_dir}lua/lua.h",
	"%{ext_dir}lua/lualib.h", "%{ext_dir}lua/lundump.h", "%{ext_dir}lua/lvm.h", "%{ext_dir}lua/lzio.h",
	"%{ext_dir}lua/lapi.c", "%{ext_dir}lua/lauxlib.c", "%{ext_dir}lua/lbaselib.c", "%{ext_dir}lua/lcode.c",
	"%{ext_dir}lua/lcorolib.c", "%{ext_dir}lua/lctype.c", "%{ext_dir}lua/ldblib.c", "%{ext_dir}lua/ldebug.c",
	"%{ext_dir}lua/ldo.c", "%{ext_dir}lua/ldump.c", "%{ext_dir}lua/lfunc.c", "%{ext_dir}lua/lgc.c",
	"%{ext_dir}lua/linit.c", "%{ext_dir}lua/liolib.c", "%{ext_dir}lua/llex.c", "%{ext_dir}lua/lmathlib.c",
	"%{ext_dir}lua/lmem.c", "%{ext_dir}lua/loadlib.c", "%{ext_dir}lua/lobject.c", "%{ext_dir}lua/lopcodes.c",
	"%{ext_dir}lua/loslib.c", "%{ext_dir}lua/lparser.c", "%{ext_dir}lua/lstate.c", "%{ext_dir}lua/lstring.c",
	"%{ext_dir}lua/lstrlib.c", "%{ext_dir}lua/ltable.c", "%{ext_dir}lua/ltablib.c", "%{ext_dir}lua/ltests.c",
	"%{ext_dir}lua/ltm.c", "%{ext_dir}lua/lua.c", "%{ext_dir}lua/lundump.c", "%{ext_dir}lua/lutf8lib.c",
	"%{ext_dir}lua/lvm.c", "%{ext_dir}lua/lzio.c",
}

libdirs {
	"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
}

links {
	"opengl32", --"glew32s",
}

filter { "configurations:Debug" }
	symbols "On"
	links { "glew32d", }
		
filter { "configurations:Release" }
	optimize "Speed"
	links { "glew32", }

-- WINDOWS

filter { "system:Windows" }
	defines {
		"NOMINMAX",
		"IMGUI_API=__declspec(dllexport)",
		"_GLFW_WIN32=1",
		"_GLFW_BUILD_DLL",
		"LUA_BUILD_AS_DLL",
	}
	files {
		"%{ext_dir}glfw/src/win32_init.c",
		"%{ext_dir}glfw/src/win32_joystick.c",
		"%{ext_dir}glfw/src/win32_joystick.h",
		"%{ext_dir}glfw/src/win32_monitor.c",
		"%{ext_dir}glfw/src/win32_platform.h",
		"%{ext_dir}glfw/src/win32_thread.c",
		"%{ext_dir}glfw/src/win32_time.c",
		"%{ext_dir}glfw/src/win32_window.c",
	}
	postbuildcommands {
		"%{ml_copy} %{bin_lib}%{cfg.platform}\\%{cfg.buildcfg}\\%{prj.name}.dll %{bin_out}",
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\assimp.dll %{bin_out}",
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\glew32.dll %{bin_out}",
	}

filter { "system:Windows", "configurations:Debug" }
	linkoptions { 
		"/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib"
	}
	postbuildcommands {
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\glew32d.dll %{bin_out}",
	}

filter { "system:Windows", "configurations:Release" }
	linkoptions {
		"/NODEFAULTLIB:LIBCMT.lib"
	}
	postbuildcommands {
		"%{ml_copy} %{ext_bin}%{cfg.platform}\\%{cfg.buildcfg}\\glew32.dll %{bin_out}",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --