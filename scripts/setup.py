# Imports
from download import downloadDependency, handleSubmodules
from libfile import HandleDetails, handleZip, handleExe, handleOtherFiles, getFileName
from shared import AnsiColors
from typing import List
import argparse
import platform as sys_platform
import os
import sys
import tomllib

parser = argparse.ArgumentParser(
    prog="Czengine Setup",
    description="Installs dependencies relating to Czengine",
    epilog="CMD Line Arguments are optional.\nAuthor: Kenneth Cornett (Czara Software)"
)

parser.add_argument('-p', '--platform', type=str, choices=['x86','x64'], required=False,
                    nargs=1, help="Specifies to use either 32bit or 64bit binaries.")
parser.add_argument('-c', '--compiler', type=str, choices=['MinGW','VisualStudio'], required=False,
                    nargs=1, help="Specifies which compiler you plan to use. (g++) vs (vc++)")
parser.add_argument('-l', '--libs', type=str, choices=['vendor','include','all'], required=False,
                    nargs=1, default='all', help="Specifies which compiler you plan to use. (g++) vs (vc++)")

OP_SYSTEM = sys_platform.system()
IS_64BIT = sys.maxsize > 2**32
CONFIG_PATH = "./config/dependencies.config.toml"

def finalizePath(path, lib_version, lib_cxx = ""):
    tmp = path
    if "{version}" in tmp:
        tmp = tmp.replace("{version}", lib_version)
    if "{operating_system}" in tmp:
        tmp = tmp.replace("{operating_system}", OP_SYSTEM.lower())
    if "{compiler}" in tmp:
        tmp = tmp.replace("{compiler}", lib_cxx)
    return tmp

def compilerSelect():
    selection = int(input(f'{AnsiColors.BLUE}Please select which compiler you plan to use:\n\t(1) MinGW (2) Visual Studio{AnsiColors.DEFAULT}\n\t> '))
    return ['MinGW','VisualStudio'][selection - 1]

def platformSizeSelect():
    if (IS_64BIT):
        use_64bit = input(f'{AnsiColors.BLUE}Your system is {AnsiColors.YELLOW}{OP_SYSTEM}{AnsiColors.BLUE} 64bit.\nWould you like to use 64bit binaries?{AnsiColors.DEFAULT}\n\t("y","n") > ')
        if (use_64bit.casefold() == 'y'.casefold() or use_64bit.casefold() == "yes".casefold()):
            return "x64"
    return "x86"

def versionSelect(lib_name, avail_versions):
    if (len(avail_versions) == 1):
        return avail_versions[0]
    print(f"{AnsiColors.CYAN}Please select a library version for {AnsiColors.YELLOW}{lib_name}{AnsiColors.CYAN} to install")
    while (True):
        selection = input(f"Options: {AnsiColors.YELLOW}{*avail_versions,}{AnsiColors.DEFAULT} > ")
        if (selection not in avail_versions):
            print(f"{AnsiColors.RED}Invalid option... Please select again ([ctrl + c] to exit installation)")
        else:
            return selection

def handleDownloadedFiles(file_details: HandleDetails):
        file_len = len(file_details.filename)
        extension = file_details.filename[file_len-4:file_len]
        if (".zip" in extension):
            handleZip(
                file_details.file_working_dir,
                file_details.filename,
                file_details.directory,
                file_details.include,
                file_details.lib,
                file_details.src)
        elif(".exe" in extension):
            handleExe(
                file_details.file_working_dir,
                file_details.filename,
                file_details.directory,
                file_details.include,
                file_details.lib,
                file_details.src)
        else:
            handleOtherFiles(
                file_details.file_working_dir,
                file_details.filename,
                file_details.directory,
                file_details.include,
                file_details.lib,
                file_details.src)


def processVendorLibs(libs, base_path: str):
    for library in libs:
        lib_name = library["name"]
        if (os.path.exists(os.path.join(base_path,lib_name)) and len(os.listdir(os.path.join(base_path,lib_name))) != 0):
            print(f"{AnsiColors.YELLOW}\t{lib_name}{AnsiColors.CYAN} directory already exists and is non-empty.")
            continue
        print(f"{AnsiColors.CYAN}\tProcessing {AnsiColors.YELLOW}{lib_name}{AnsiColors.CYAN} dependency.")
        lib_url = library["url"]
        lib_version = versionSelect(lib_name, library["versions"])
        lib_cxx_name = None
        lib_platform = None
        if (compiler in library):
            lib_cxx_name = library[compiler]["compiler"]
            if (platform in library[compiler]):
                lib_platform = library[compiler][platform]
        f_url = finalizePath(lib_url, lib_version, lib_cxx_name)
        download_dir = os.path.join(base_path, lib_name)
        downloadDependency(lib_name, f_url, download_dir)
        filename = getFileName(f_url)
        curr_file_details = HandleDetails(filename, download_dir)
        if lib_platform is not None:
            if "directory" in lib_platform:
                curr_file_details.directory = finalizePath(lib_platform["directory"], lib_version, lib_cxx_name)
            if "lib" in lib_platform:
                curr_file_details.lib = finalizePath(lib_platform["lib"], lib_version, lib_cxx_name)
        if compiler in library and "include" in library[compiler]:
            curr_file_details.include = finalizePath(library[compiler]["include"], lib_version, lib_cxx_name)
        handleDownloadedFiles(curr_file_details)

def processIncludeLibs(libs, base_path: str):
    for library in libs:
        lib_name = library["name"]
        if (os.path.exists(os.path.join(base_path,lib_name)) and len(os.listdir(os.path.join(base_path,lib_name))) != 0):
            print(f"{AnsiColors.YELLOW}\t{lib_name}{AnsiColors.CYAN} directory already exists and is non-empty.")
            continue
        lib_url = library["url"]
        commits = None
        if "commits" in library:
            commits = library["commits"]
        download_url = os.path.join(base_path, lib_name)
        downloadDependency(lib_name, lib_url, download_url)
        filename = getFileName(lib_url)
        curr_file_details = HandleDetails(filename, download_url)
        if "include" in library:
            curr_file_details.include = library["include"]
        if "source" in library:
            curr_file_details.src = library["source"]
        if "lib" in library:
            curr_file_details.lib = library["lib"]
        handleDownloadedFiles(curr_file_details)

def parseVendors(compiler, platform, config_data):
    ext_vendor_dir: dict = config_data["external"]["vendor"]["directory"]
    path: str = ext_vendor_dir["path"]
    if ("type" in ext_vendor_dir and ext_vendor_dir["type"] == "partial"):
        curr_dir = os.path.abspath(os.curdir)
        # Check if we are somehow printing the "scripts" directory.
        counter = 2
        while ("scripts" in curr_dir[len(curr_dir)-20:len(curr_dir)] and counter > 0):
            # If so, then move up a directory.
            counter -= 1
            os.chdir("..")
            curr_dir = os.path.abspath(os.curdir)
        path = os.path.join(curr_dir, "extern", path)
    print(f"{AnsiColors.GREEN}Creating directory: {path}.")
    os.makedirs(path, exist_ok=True)
    processVendorLibs(config_data["external"]["vendor"]["libraries"], path)

def parseSrcIncludes(compiler, platform, config_data):
    ext_inc_dir: dict = config_data["external"]["include"]["directory"]
    path: str = ext_inc_dir["path"]
    if ("type" in ext_inc_dir and ext_inc_dir["type"] == "partial"):
        curr_dir = os.path.abspath(os.curdir)
        # Check if we are somehow printing the "scripts" directory.
        counter = 2
        while ("scripts" in curr_dir[len(curr_dir)-20:len(curr_dir)] and counter > 0):
            # If so, then move up a directory.
            counter -= 1
            os.chdir("..")
            curr_dir = os.path.abspath(os.curdir)
        path = os.path.join(curr_dir, "extern", path)
    print(f"{AnsiColors.GREEN}Creating directory: {path}.")
    os.makedirs(path, exist_ok=True)
    processIncludeLibs(config_data["external"]["include"]["libraries"], path)
            
def startProcess(libs: str = 'all', compiler = None, platform = None):
    if (compiler is None):
        compiler = compilerSelect()
    if (platform is None or not IS_64BIT):
        platform = platformSizeSelect()
    data = None
    with open(CONFIG_PATH, "rb") as config_file:
        data = tomllib.load(config_file)
    if data is None:
        raise Exception("Failed to parse TOML File.")
    if libs == 'all' or libs == 'vendor':
        parseVendors(compiler, platform, data)
    if libs == 'all' or libs == 'include':
        handleSubmodules()
        parseSrcIncludes(compiler, platform, data)

try:
    args = parser.parse_args()
    compiler = None
    platform = None
    if __name__ == '__main__':
        if (args.compiler):
            compiler = args.compiler
        if (args.platform):
            platform = args.platform
        if (args.libs):
            libs = args.libs
        startProcess(libs, compiler, platform)
except argparse.ArgumentError:
    print(f"{AnsiColors.RED}Invalid command line argument provided: {parser.error}")
except argparse.ArgumentTypeError:
    print(f"{AnsiColors.RED}Invalid command line argument type provided: {parser.error}")

