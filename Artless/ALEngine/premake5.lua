-- ALEngine Premake file
project "ALEngine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	location "gam_200"

	targetdir ("../bin" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int" .. outputdir .. "/%{prj.name}")

	files {
		"Include/**.h",
		"Source/**.cpp"
	}

	includedirs
	{
		"../Extern/include/",
		"../Extern/include/spdlog/include",
		"Include/Editor/imgui",
		"Include/Editor/ImGuizmo",
		"Include"
	}
		
	filter { "configurations:Debug" }
		defines "_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter { "configurations:Release" }
		runtime "Release"
		optimize "On"
	