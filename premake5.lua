workspace "Artless"
	architecture "x64"
	
	configurations
	{
		"Debug", 
		"Release",
		"Dist"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
project "ArtlessEngine"
	location "gam_200"
	kind "SharedLib"
	language "C++"
	
	targetdir ("" .. outputdir .. "/%{prj.name}")
	objdir ("" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/Extern/include/spdlog/include",
		"%{prj.name}/Extern/include/imgui"		
	}
	
	filter "systems:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0"
		
		defines
		{
			"_DEBUG",
			"_CONSOLE"
		}
		
		postbuildcommands
		{
			
		}
		
	filter "configurations:Debug"
		defines "_DEBUG"
		symbols "On"
		