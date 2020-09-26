-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group			"plugins"
project			"studio"
targetname 		"%{prj.name}"
targetdir		"%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/plugins/"
objdir			"%{bin_obj}"
location		"%{prj_dir}plugins/%{prj.name}/"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
staticruntime	"Off"
rtti			"Off"
systemversion	"latest"

dependson{
	"main",
}

defines{
	"_CRT_SECURE_NO_WARNINGS", "NOMINMAX",
}

undefines{
	"NDEBUG",
}

includedirs{
	"%{sln_dir}include",
	"%{sln_dir}src/plugins",
	"%{ext_dir}",
	"%{ext_dir}json/include",
	"%{ext_dir}pybind11/include",
	"%{ext_dir}cpython/Include",
	"%{ext_dir}cpython/Include/internal",
	"%{ext_dir}entt/src",
	"%{ext_dir}imgui",
	"%{ext_dir}imgui-node-editor/NodeEditor/Include",
}

files{
	"%{sln_dir}plugins/%{prj.name}/**.**",
}

libdirs{
	"%{bin_lib}", "%{bin_lib}%{cfg.platform}/", "%{bin_lib}%{cfg.platform}/%{cfg.buildcfg}/",
	"%{ext_lib}", "%{ext_lib}%{cfg.platform}/", "%{ext_lib}%{cfg.platform}/%{cfg.buildcfg}/",
}

links{
	"libmeme",
}

postbuildcommands{
	"%{ml_copy} %{lib_in}plugins\\%{prj.name}%{ml_dll} %{bin_out}\\plugins\\",
}

filter{ "configurations:Debug" }
	symbols "On"
	links{
		"python39_d",
	}

filter{ "configurations:Release" }
	optimize "Speed"
	links{
		"python39",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- WINDOWS

filter{ "system:Windows" }
	includedirs{
		"%{ext_dir}cpython/PC",
	}

filter{ "system:Windows", "configurations:Debug" }
	linkoptions{ 
		"/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib",
	}

filter{ "system:Windows", "configurations:Release" }
	linkoptions{
		"/NODEFAULTLIB:LIBCMT.lib",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --