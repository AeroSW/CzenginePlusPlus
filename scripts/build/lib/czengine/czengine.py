import argparse
import os
import shutil
import subprocess
from pathlib import Path
from utility.shared import AnsiColors

def main():
    def configureRootDirectory(path: str = os.getcwd()):
        expected_root_name = "CzenginePlusPlus"
        return path.split(expected_root_name)[0] + expected_root_name

    parser = argparse.ArgumentParser(
        prog="Czengine Setup",
        description="Installs dependencies relating to Czengine",
        epilog="CMD Line Arguments are optional.\nAuthor: Kenneth Cornett (Czara Software)"
    )


    root_dir = configureRootDirectory()
    debug_dir = "bin/Win64/Debug"
    release_dir = "bin/Win64/Release"
    main_name = "CzenginePlusPlus"
    test_name = "CzengineTest"
    build_modes = {"debug": "debug_win64", "release": "release_win64"}

    subparsers = parser.add_subparsers(dest='cmd', help="Determine main commands being executed.")
    # Build related commands (utilizing `make`)
    build_parser = subparsers.add_parser("build", help="Build target executable in target mode (debug|release)")
    build_parser.add_argument("-m", "--mode", type=str, choices=[*build_modes.keys()], default='debug',
                                required=False, nargs=1, help=f"Which target version ({"|".join([*build_modes.keys()])}) are you building?")
    build_parser.add_argument("-t", "--target", type=str, choices=["all", test_name, main_name], default='all',
                            required=False, nargs=1, help=f"Target project to build. ({test_name}|{main_name})")
    build_parser.add_argument("-c", "--clean", action="store_true", help="Execute a clean before building.")
    # Launch related commands (Executing target)
    launch_parser = subparsers.add_parser("launch", help="Launches target executable.")
    launch_parser.add_argument("-d", "--debug", action="store_true", help="Flag that denotes launching with compiler's debugger.")
    launch_parser.add_argument("-t", "--target", type=str, choices=[test_name, main_name],
                            required=True, nargs=1, help=f"Target project to launch. ({test_name}|{main_name})")
    # Premake related commands
    premake_parser = subparsers.add_parser("premake5", help="Commands relating to premake5.\nClean, Build, or both. Clean executes prior to build.")
    premake_group = premake_parser.add_mutually_exclusive_group(required=True)
    premake_group.add_argument("-b", "--build", action="store_true", help="Attempts to generate fresh Makefiles.")
    premake_group.add_argument("-c", "--clean", action="store_true", help="Attempts to remove and clean up Makefiles.")
    # General Clean commands
    clean_parser = subparsers.add_parser("clean", help="Provide ability to clean project files without requiring another process.")
    clean_group = clean_parser.add_mutually_exclusive_group(required=True)
    clean_group.add_argument("--all", action="store_true", help="Cleans all generated project files. (Build and Premake)")
    clean_group.add_argument("--premake", action="store_true", help="Cleans premake5 generated project files.")
    clean_group.add_argument("--build", action="store_true", help="Cleans project files generated from source code.")

    def pathJoin(*argv):
        return Path(os.path.join(*argv))

    def executeCmd(cmd, args: list = [], outFile: str = None, cwd: str = root_dir):
        concat = " ".join(args)
        print(f"{AnsiColors.GREEN}Executing `{AnsiColors.YELLOW}{cmd} {concat}{AnsiColors.GREEN}`{AnsiColors.DEFAULT}")
        active_process = subprocess.Popen([cmd, *args], cwd=cwd, close_fds=True)
        active_process.wait()

    def getTargetPath(target):
        release_path = pathJoin(root_dir, release_dir)
        debug_path = pathJoin(root_dir, debug_dir)
        print(f"{AnsiColors.CYAN}Checking if `{AnsiColors.YELLOW}{release_path}{AnsiColors.CYAN}` exists.{AnsiColors.DEFAULT}")
        if (release_path.exists() and pathJoin(release_path, f"{target}.exe").exists()):
            return release_path
        print(f"{AnsiColors.CYAN}Checking if `{AnsiColors.YELLOW}{debug_path}{AnsiColors.CYAN}` exists.{AnsiColors.DEFAULT}")
        if (debug_path.exists() and pathJoin(debug_path, f"{target}.exe").exists()):
            return debug_path
        raise Exception(f"{AnsiColors.RED}{target} executable does not exist.\nHave you tried building for the mode you want to execute?{AnsiColors.DEFAULT}")
        
    def launchTarget(target, debug: bool = False):
        args = []
        if target == test_name:
            args = [">", str(pathJoin(root_dir,debug_dir,"output","tst-czengine.result.txt"))]
        if debug:
            dbg_path = pathJoin(root_dir, debug_dir)
            target_exe = pathJoin(dbg_path, f"{target}.exe")
            print(f"{root_dir}/{debug_dir}/{target}.exe\nExists: {dbg_path.exists()}\nFile Exists: {target_exe.is_file()}")
            if not dbg_path.exists() or not target_exe.exists():
                raise Exception(f"{AnsiColors.RED}Debug path does not exist.  Have you tried building debug mode?{AnsiColors.DEFAULT}")
            executeCmd('gdb', args=["-ex=r", "--args", str(target_exe), *args], cwd=str(dbg_path))
            return
        target_path = getTargetPath(target)
        target_exe = pathJoin(target_path, f"{target}.exe")
        if not target_path.exists() or not target_exe.exists():
            raise Exception(f"{AnsiColors.RED}{target} path does not exist.  Have you tried building?{AnsiColors.DEFAULT}")
        executeCmd(str(target_exe), args=args, cwd=str(target_path))

    def buildTarget(target, config):
        print(f"{AnsiColors.CYAN}Attempting to {AnsiColors.BLUE}{AnsiColors.BOLD}make {AnsiColors.DEFAULT}{AnsiColors.YELLOW}{target}{AnsiColors.DEFAULT}")
        make_file = pathJoin(root_dir, "Makefile")
        target_make = pathJoin(root_dir, f"{target}.make")
        if not make_file.is_file() or not target_make.is_file():
            print(f"{AnsiColors.RED}Makefile configurations are missing.\n{AnsiColors.CYAN}Calling {AnsiColors.YELLOW}`premake5 gmake2`.{AnsiColors.DEFAULT}")
            buildPremake5()
        executeCmd("make", [str(target), "CXX=g++", f"config={config}"])

    def buildPremake5():
        print(f"{AnsiColors.CYAN}Executing {AnsiColors.YELLOW}`premake5 gmake2`{AnsiColors.DEFAULT}")
        executeCmd("premake5", ["gmake2"])

    def cleanBuild():
        print(f"{AnsiColors.CYAN}Cleaning object files and bin directory.{AnsiColors.DEFAULT}")
        shutil.rmtree(pathJoin(root_dir, "obj"), ignore_errors=True)
        shutil.rmtree(pathJoin(root_dir, "bin"), ignore_errors=True)
    def cleanPremake5():
        print(f"{AnsiColors.CYAN}Cleaning Makefiles{AnsiColors.DEFAULT}")
        Path.unlink(pathJoin(root_dir, f"{main_name}.make"), missing_ok=True)
        Path.unlink(pathJoin(root_dir, f"{test_name}.make"), missing_ok=True)
        Path.unlink(pathJoin(root_dir, "Makefile"), missing_ok=True)

    try:
        args = parser.parse_args()
        if (args.cmd == "build"):
            print(f"{AnsiColors.GREEN}{build_modes.keys()}{AnsiColors.DEFAULT}")
            config = build_modes[str(args.mode[0])]
            target = args.target[0]
            if (args.clean):
                cleanBuild()
            if (target == 'all'):
                buildTarget(test_name, config)
                buildTarget(main_name, config)
            else:
                buildTarget(target, config)
        elif (args.cmd == "launch"):
            dbg = False
            if args.debug:
                dbg = True
            target = args.target[0]
            launchTarget(target, dbg)
        elif (args.cmd == "premake5"):
            if (args.clean):
                cleanPremake5()
            if (args.build):
                buildPremake5()
        elif (args.cmd == "clean"):
            if args.all:
                cleanBuild()
                cleanPremake5()
            if args.build and not args.all:
                cleanBuild()
            if args.premake and not args.all:
                cleanPremake5()
        else:
            raise "Invalic command used in this script."
    except argparse.ArgumentError:
        print(f"{AnsiColors.RED}Invalid command line argument provided: {parser.error}{AnsiColors.DEFAULT}")
    except argparse.ArgumentTypeError:
        print(f"{AnsiColors.RED}Invalid command line argument type provided: {parser.error}{AnsiColors.DEFAULT}")
    except (Exception,) as err:
        print(err)

if __name__ == "__main__":
    main()
