project "Server"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"../Ring2.0/vendor/NetCommon/HeaderFiles",
		"../Ring2.0/vendor/NetCommon/asio-1.22.1/include"
	}