workspace "sdmg"
    architecture "x64"
    startproject "sdmg"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["SDL2"] = "sdmg/vendor/SDL2/include"
IncludeDir["SDL2_image"] = "sdmg/vendor/SDL2_image/include"
IncludeDir["SDL2_mixer"] = "sdmg/vendor/SDL2_mixer/include"
IncludeDir["SDL2_ttf"] = "sdmg/vendor/SDL2_ttf/include"
IncludeDir["JSONParser"] = "sdmg/vendor/JSONParser/include"

project "sdmg"
    location "sdmg"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.SDL2_image}",
        "%{IncludeDir.SDL2_mixer}",
        "%{IncludeDir.SDL2_ttf}",
        "%{IncludeDir.JSONParser}"
    }

    links
    {
        "sdmg/vendor/SDL2/lib/x64/SDL2.lib",
        "sdmg/vendor/SDL2/lib/x64/SDL2main.lib",
        "sdmg/vendor/SDL2_image/lib/x64/SDL2_image.lib",
        "sdmg/vendor/SDL2_mixer/lib/x64/SDL2_mixer.lib",
        "sdmg/vendor/SDL2_ttf/lib/x64/SDL2_ttf.lib",
        "sdmg/vendor/JSONParser/lib/JSONParser.lib"
    }

    filter "system:windows"
        systemversion "latest"


    filter "configurations:Debug"
        defines "SDMG_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "SDMG_RELEASE"
        runtime "Release"
        optimize "On"