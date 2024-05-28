workspace "CzenginePlusPlus"
    configurations {"Debug", "Release"}
    platforms { "Win64" }
project "CzenginePlusPlus"
    kind "ConsoleApp"
    language "C++"
    system "Windows"
    architecture "x86_64"
    filter "action:gmake2"
        if os.execute("g++ -v") == 0 then
            toolset "g++"
        end
    files {"**.hpp","**.h","**.cpp"}
    buildcommands {
        "make %{cfg.buildcfg}"
    }
    rebuildcommands {
        "make %{cfg.buildcfg} rebuild"
    }
    cleancommands {
        "make clean ${cfg.buildcfg}"
    }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
    makesettings [[CC=g++]]