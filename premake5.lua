workspace "Ring2.0"
	architecture "x64"
	startproject "Ring2.0"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Ring2.0/vendor/GLFW/include"
IncludeDir["Glad"] = "Ring2.0/vendor/Glad/include"
IncludeDir["NetCommon"] = "Ring2.0/vendor/NetCommon/HeaderFiles"

include "Ring2.0/vendor/GLFW"
include "Ring2.0/vendor/Glad"
include "Ring2.0/vendor/NetCommon"
include "Server"

project "Ring2.0"
	location "Ring2.0"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{prj.name}/src",
		"%{prj.name}/vendor/NetCommon/asio-1.22.1/include",
		"%{IncludeDir.NetCommon}"
	}

	links
	{
		"GLFW",
		"Glad"
		
	}


	filter "system:windows"
	           systemversion "latest"
            	staticruntime "On"

		defines
		{
			"SB_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "RING_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RING_RELEASE"
		runtime "Release"
		optimize "on"

	

