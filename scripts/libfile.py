from dataclasses import dataclass
from shared import AnsiColors
from zipfile import ZipFile

import shutil
import os

@dataclass
class HandleDetails:
    filename: str
    file_working_dir: str
    directory: str = None
    include: str = None
    lib: str = None

def getFileName(url: str):
    url_parts = url.rsplit('/', 1)
    if (len(url_parts) == 0):
        url_parts = url.rsplit('\\', 1)
    if (len(url_parts) == 0):
        print(f"\t{AnsiColors.RED}Invalid file download URL: {url}.")
        exit(1)
    return url_parts[-1]

def handleZip(working_dir: str, filename: str, directory: str = None, include_path: str = None, lib_path: str = None):
    print(f'{AnsiColors.CYAN}Working in {AnsiColors.BOLD}{working_dir}')
    filepath = os.path.join(working_dir, filename)
    tmp_dir = os.path.join(working_dir, 'tmp')
    os.makedirs(tmp_dir)
    if (not os.path.exists(filepath) or not os.path.isfile(filepath)):
        print(f"\t{AnsiColors.RED}Downloaded file does not exist...\n\t\t{filepath}")
        exit(1)
    print(f"\t{AnsiColors.CYAN}Unzipping file, {AnsiColors.YELLOW}[{filepath}]{AnsiColors.CYAN}, to temp folder, {AnsiColors.YELLOW}[{tmp_dir}]{AnsiColors.CYAN}")
    unzipFile(filepath, tmp_dir)
    print(f"\t{AnsiColors.GREEN}Finished unzipping {AnsiColors.YELLOW}[{filepath}]{AnsiColors.CYAN}, moving required files to {AnsiColors.YELLOW}[{working_dir}]{AnsiColors.CYAN}.")
    moveRequiredContent(working_dir, tmp_dir, directory, include_path, lib_path)
    print(f"\t{AnsiColors.GREEN}Finished moving files, deleting the temp directory, {AnsiColors.YELLOW}[{tmp_dir}]{AnsiColors.CYAN}.")
    shutil.rmtree(tmp_dir, ignore_errors=True)
    print(f"\t{AnsiColors.GREEN}Finished deleting the temporary folder. Deleting zip file.")
    os.remove(filepath)
    print(f"\t{AnsiColors.GREEN}Finished deleting zip file.")

def unzipFile(filepath: str, odir: str):
    with ZipFile(filepath, 'r') as zip_ref:
        zip_ref.extractall(odir)

def moveRequiredContent(to_dir: str, from_dir: str, directory: str = None, include_path: str = None, lib_path: str = None):
    if (directory is not None):
        dir_full_path = os.path.join(from_dir, directory)
        if (os.path.exists(dir_full_path) and os.path.isdir(dir_full_path)):
            shutil.copytree(dir_full_path, to_dir, dirs_exist_ok=True)

    if (include_path is not None):
        inc_full_path = os.path.join(from_dir, include_path)
        if (os.path.exists(inc_full_path) and os.path.isdir(inc_full_path)):
            shutil.copytree(inc_full_path, os.path.join(to_dir, "include"), dirs_exist_ok=True)

    if (lib_path is not None):
        lib_full_path = os.path.join(from_dir, lib_path)
        if (os.path.exists(lib_full_path) and os.path.isdir(lib_full_path)):
            shutil.copytree(lib_full_path, os.path.join(to_dir, "lib"), dirs_exist_ok=True)

def handleExe(working_dir: str, download_url: str):
    print(f'{AnsiColors.CYAN}Working in {AnsiColors.BOLD}{working_dir}')
    url_parts = download_url.rsplit('/', 1)
    if (len(url_parts) == 0):
        url_parts = download_url.rsplit('\\', 1)
    if (len(url_parts) == 0):
        print(f"\t{AnsiColors.RED}Invalid file download URL: {download_url}.")
        exit(1)
    filename = url_parts[-1]
    filepath = os.path.join(working_dir, filename)