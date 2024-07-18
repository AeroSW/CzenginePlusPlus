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

function addLib(lib_list, path, lib_name)
    local lib_path = path .. "/" .. lib_name
    print(lib_path)
    if os.isdir(lib_path) then
        table.insert(lib_list, lib_path)
    else
        local libs = os.findlib(lib_name)
        table.insert(lib_list, os.findlib(lib_name))
    end
    return lib_list
end
function addInclude(inc_list, path, lib_name, include_path)
    local lib_path = path .. "/" .. lib_name
    local inc_path
    if os.isdir(lib_path) then
        inc_path = { lib_path .. "/" .. include_path }
    else
        inc_path = os.findheader(lib_name)
    end
    if inc_path ~= nil and type(next(inc_path)) ~= "nil" then
        for _, inc in ipairs(inc_path) do
            table.insert(inc_list, inc)
        end
    end
    return inc_list
end
function addDlls(dll_list, path, lib_name, dll_query)
    local lib_path = path .. "/" .. lib_name
    local dll_path
    if os.isdir(lib_path) then
        dll_path = os.matchfiles(lib_path .. "/**.dll")
    else
        dll_path = os.locate(dll_query)
    end
    if dll_path ~= nil and type(next(dll_path)) ~= "nil" then
        local dlls = table.translate(dll_path, path.getname)
        for _, dll in ipairs(dll_path) do
            table.insert(dll_list, dll)
        end
    end
    return dll_list
end

workspace "CzenginePlusPlus"
    configurations {"Debug", "Release"}
    platforms { "Win64" }
project "CzenginePlusPlus"
    kind "ConsoleApp"
    language "C++"
    system "Windows"
    architecture "x86_64"
    local lib_list = {}
    local include_list = { 
        "./include/**",
        "./extern/__include__/DearImGui/",
        "./extern/__include__/DearImGui/**",
        "./extern/__include__/PugiXml/src",
        "./extern/__include__/Toml++/include",
        "./extern/__include__/Toml++/include/**"
    }
    local link_list = {}

    -- Add external libraries...  TODO: Adjust to list and for-loop
    extern_libs_with_include_folder = { "SDL2", "SDL2_ttf"}
    for _, extern_lib in ipairs(extern_libs_with_include_folder) do
        lib_list = addLib(lib_list, "./extern/__vendor__", extern_lib)
        include_list = addInclude(include_list, "./extern/__vendor__", extern_lib, "include/**")
        link_list = addDlls(link_list, "./extern/__vendor__", extern_lib, extern_lib .. ".dll")
    end
    extern_libs_with_Include_folder = {"Vulkan"}
    for _, extern_lib in ipairs(extern_libs_with_Include_folder) do
        --table.insert(lib_list, "lib/Vulkan/Lib/vulkan-1")
        --lib_list = addLib(lib_list, "./lib", extern_lib)
        include_list = addInclude(include_list, "./extern/__vendor__", extern_lib, "Include")
        table.insert(lib_list, "./lib/Vulkan/Bin")
        --link_list = addDlls(link_list, "./lib", extern_lib, extern_lib .. ".dll")
    end

    links { "./extern/__vendor__/Vulkan/Lib/vulkan-1" }

    print("lib dump")
    print(dump(lib_list))
    print("include dump")
    print(dump(include_list))
    print("link dump")
    print(dump(link_list))

    libdirs { table.unpack(lib_list) }
    linkoptions { table.concat(link_list, " ") .. " -std=c++23 -mwindows" }
    includedirs { table.unpack(include_list) }
    files {"**.hpp","**.h","**.cpp"}
    removefiles {
        "extern/__vendor__/**/*.cpp", 
        "extern/__vendor__/**/*.c",
        "extern/__include__/PugiXml/tests/*.cpp",
        "extern/__include__/PugiXml/docs/**/*.cpp",
        "extern/__include__/DearImGui/examples/**.cpp",
        "extern/__include__/DearImGui/misc/**/*.cpp",
        "extern/__include__/DearImGui/backends/*allegro5.cpp", -- Remove DearImGui Backends not being used.
        "extern/__include__/DearImGui/backends/*android.cpp",
        "extern/__include__/DearImGui/backends/*dx10.cpp",
        "extern/__include__/DearImGui/backends/*dx11.cpp",
        "extern/__include__/DearImGui/backends/*dx12.cpp",
        "extern/__include__/DearImGui/backends/*dx9.cpp",
        "extern/__include__/DearImGui/backends/*glfw.cpp",
        "extern/__include__/DearImGui/backends/*opengl*.cpp",
        "extern/__include__/DearImGui/backends/*glut.cpp",
        "extern/__include__/DearImGui/backends/*metal.cpp",
        "extern/__include__/DearImGui/backends/*osx.h",
        "extern/__include__/DearImGui/backends/*sdl3.cpp",
        "extern/__include__/DearImGui/backends/*sdlrenderer3.cpp",
        "extern/__include__/DearImGui/backends/*wgpu.cpp",
        "extern/__include__/DearImGui/backends/*win32.cpp"
    }
    buildoptions "-std=c++23 -mwindows"
    filter "files:**"
    buildcommands {
        "make %{cfg.buildcfg} -mwindow",
        "cp -rf czengine-interface/ \"%[bin/project.config]\"",
    }
    rebuildcommands {
        "make %{cfg.buildcfg} rebuild"
    }
    cleancommands {
        "make clean ${cfg.buildcfg}"
    }
    postbuildcommands {
        "cp -rf czengine-interface/ \"%[bin/project.config]\""
    }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
    makesettings [[cc=g++, cxx=g++]]