premake.tools.arm_gcc         = {}

local arm_gcc                 = premake.tools.arm_gcc
local gcc                     = premake.tools.gcc

arm_gcc.getcflags             = gcc.getcflags
arm_gcc.getcxxflags           = gcc.getcxxflags
arm_gcc.getforceincludes      = gcc.getforceincludes
arm_gcc.getldflags            = gcc.getldflags
arm_gcc.getcppflags           = gcc.getcppflags
arm_gcc.getdefines            = gcc.getdefines
arm_gcc.getincludedirs        = gcc.getincludedirs
arm_gcc.getLibraryDirectories = gcc.getLibraryDirectories
arm_gcc.getlinks              = gcc.getlinks
arm_gcc.getmakesettings       = gcc.getmakesettings

function arm_gcc.gettoolname (cfg, tool)  
  local prefix = path.getabsolute ("../../arm_env")
  prefix       =  prefix .. "/arm_toolchain/bin/arm-linux-gnueabihf-"
  if     tool == "cc" then
    name = prefix .. "gcc"  
  elseif tool == "cxx" then
    name = prefix .. "g++"
  elseif tool == "ar" then
    name = prefix .. "ar"
  else
    name = nil
  end
  return name
end

function dump(o)
    if type(o) == 'table' then
       local s = '{ '
       for k,v in pairs(o) do
          if type(k) ~= 'number' then k = '"'..k..'"' end
          s = s .. '['..k..'] = ' .. dump(v) .. ','
       end
       return s .. '} '
    else
       return tostring(o)
    end
 end

workspace "CzenginePlusPlus"
    configurations {"Debug", "Release"}
    platforms { "Win64" }
project "CzenginePlusPlus"
    kind "ConsoleApp"
    language "C++"
    system "Windows"
    architecture "x86_64"
    local include_list = { "./include/** " }
    --toolset "g++"
    if os.isdir("./lib") then
        if os.isdir("./lib/SDL2") then
            libdirs { "./lib/SDL2" }
            local dlls_path = os.matchfiles("./lib/SDL2/**.dll")
            local dlls = table.translate(dlls_path, path.getname)
            linkoptions { dlls_path, "-std=c++23" }

            table.insert(include_list, "./lib/SDL2/include/**")
        else
            libdirs { os.findlib("SDL2") }
            table.insert(include_list, os.findheader("SDL2"))
            local dlls_path = os.locate("SDL2.dll")
            if dlls_path ~= nil then
                local dlls = table.translate(dlls_path, path.getname)
                linkoptions { dlls_path }
            end
        end
    end
    includedirs { table.unpack(include_list) }
    files {"**.hpp","**.h","**.cpp"}
    buildoptions "-std=c++23"
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
    makesettings [[cc=g++, cxx=g++]]