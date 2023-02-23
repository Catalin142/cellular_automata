project "cellular_automata"
	language "C"
	cdialect "Default"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"**.c",
		"**.h",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		kind "ConsoleApp"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		kind "ConsoleApp"
		optimize "on"