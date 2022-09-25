-- ALEngine Premake file
project "ALEngine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	location "gam_200"

	targetdir ("../bin" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/Include/**.h",
		"%{prj.name}/Source/**.cpp"
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
		
	filter { "configurations:Debug" }
		defines "_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter { "configurations:Release" }
		runtime "Release"
		optimize "On"

	includedirs
	{
		"%{prj.name}/Extern/include/spdlog/include",
		"%{prj.name}/Extern/include/imgui"		
	}
	