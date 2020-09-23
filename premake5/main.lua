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

undefines{
	"NDEBUG",
}

includedirs{
	"%{sln_dir}include",
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
	"%{sln_dir}premake5/%{prj.name}.lua",
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
	kind "ConsoleApp"
	symbols "On"
	links{
		"glew32d",
		"python39_d",
	}

filter{ "configurations:Release" }
	kind "WindowedApp"
	optimize "Speed"
	links{
		"glew32",
		"python39",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- WINDOWS

filter{ "system:Windows" }
	includedirs{
		"%{ext_dir}cpython/PC",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --