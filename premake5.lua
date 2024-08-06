---@diagnostic disable: undefined-global
require "premake5.extern"
require "premake5.utils"

local LOG_FILE = nil
local PRINT_MSG = function(msg)
    print(msg)
end

function setup(proj_name)
    local path_sep = package.config:sub(1,1)
    local log_path = "." .. path_sep .. "premake5" .. path_sep .. "logs"

    if (os.isdir(log_path) == false) then
        PRINT_MSG("Log directory for Premake5 does not exist...\nCreating directory [premake5/logs]")
        os.execute("mkdir " .. log_path)
    end
    LOG_FILE = io.open("./premake5/logs/premake5." .. proj_name .. ".log", "w")
    if (LOG_FILE) then
        PRINT_MSG = function(msg)
            LOG_FILE:write(msg .. "\n")
            LOG_FILE:flush()
        end
    end
end

function cleanup()
    if (LOG_FILE) then
        LOG_FILE:close()
        LOG_FILE = nil
        -- Reset PRINT_MSG to use default console `print`
        PRINT_MSG = function(msg)
            print(msg)
        end
    end
end

local spacer = "---------------------------------"
local double_spacer = spacer .. "\n" .. spacer

function printSection(header, body)
    PRINT_MSG(header)
    PRINT_MSG(spacer)
    PRINT_MSG(body)
    PRINT_MSG(double_spacer)
end

function setupSharedPremakeConfig(addtnl_build_cmds)
    setmetatable(addtnl_build_cmds, {__index={def_value=""}})
    local build_cmds = addtnl_build_cmds[1] or addtnl_build_cmds.def_value
    bo = "-std=c++23 " .. build_cmds
    buildoptions { "-std=c++23 " .. (build_cmds) }
    filter "files:**"
    buildcommands {
        "make %{cfg.buildcfg} " .. (build_cmds)
    }
    rebuildcommands {
        "make %{cfg.buildcfg} rebuild"
    }
    cleancommands {
        "make clean ${cfg.buildcfg}"
    }
    postbuildcommands {}
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
    makesettings {'CC=g++', 'CXX=g++'}
end


workspace "CzenginePlusPlus"
    configurations {"Debug", "Release"}
    platforms { "Win64" }
project "CzenginePlusPlus"
    kind "WindowedApp"
    language "C++"
    system "Windows"
    architecture "x86_64"

    -- Project Configuration
    setup("CzenginePlusPlus")
    PRINT_MSG(double_spacer)
    PRINT_MSG("Premake5 build [CzenginePlusPlus]")
    PRINT_MSG(double_spacer)
    printSection("Linked Libraries", prettyPrintTable(libs))
    libdirs { table.unpack(libs) }
    printSection("Included Library Headers", prettyPrintTable(includes))
    includedirs { table.unpack(includes) }
    printSection("Linked Static and Dynamic Libraries", prettyPrintTable(links))
    links { table.unpack(slinks) }
    printSection("Removed Files", prettyPrintTable({ table.unpack(excludes), "tst/*.cpp", "tst/**/*.cpp" }))
    files {"**.hpp","**.h","**.cpp","**.cc"}
    removefiles {
        "extern/__include__/GoogleTest/**/*.cc",  -- Exclude unit test frameworks from Main build.
        "extern/__include__/GoogleTest/**/*.h",
        "extern/__include__/JoMock/**/*.cpp",
        "extern/__include__/JoMock/**/*.h",
        "tst/*.cpp",
        "tst/**/*.cpp",
        table.unpack(excludes)
    }
    cleanup()
    setupSharedPremakeConfig({ "-mwindows" })

project 'CzengineTest'
    kind "ConsoleApp"
    language "C++"
    system "Windows"
    architecture "x86_64"

    -- Test Configuration
    setup("CzengineTest")
    PRINT_MSG(double_spacer)
    PRINT_MSG("Premake5 build [CzenginePlusPlus]")
    PRINT_MSG(double_spacer)
    printSection("Linked Libraries", prettyPrintTable(libs))
    libdirs { table.unpack(libs) }
    printSection("Included Library Headers", prettyPrintTable(includes))
    includedirs { "tst/**", table.unpack(includes) }
    printSection("Linked Static and Dynamic Libraries", prettyPrintTable(links))
    links { slinks }
    -- As an engineer, I want to assume the best for my peers and assume the external
    -- libraries included in this project are tested and building.
    files {"**.hpp","**.h","**.cpp","**.cc"}
    printSection("Files being excluded from build", prettyPrintTable({ "./src/run-czengine.cpp", table.unpack(excludes) }))
    removefiles { "src/run-czengine.cpp", table.unpack(excludes) }
    cleanup()
    setupSharedPremakeConfig({})

