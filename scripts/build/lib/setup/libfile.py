from dataclasses import dataclass
from utility.shared import AnsiColors
from zipfile import ZipFile

import shutil
import subprocess
import os

@dataclass
class HandleDetails:
    filename: str
    file_working_dir: str
    directory: str = None
    include: str = None
    lib: str = None
    src: str = None

def getFileName(url: str):
    url_parts = url.rsplit('/', 1)
    if (len(url_parts) == 0):
        url_parts = url.rsplit('\\', 1)
    if (len(url_parts) == 0):
        print(f"\t{AnsiColors.RED}Invalid file download URL: {url}.{AnsiColors.DEFAULT}")
        exit(1)
    return url_parts[-1]

def handleZip(handle_details: HandleDetails):
    print(f'{AnsiColors.CYAN}Working in {AnsiColors.BOLD}{handle_details.file_working_dir}{AnsiColors.DEFAULT}')
    filepath = os.path.join(handle_details.file_working_dir, handle_details.filename)
    tmp_dir = os.path.join(handle_details.file_working_dir, 'tmp')
    os.makedirs(tmp_dir)
    if (not os.path.exists(filepath) or not os.path.isfile(filepath)):
        print(f"\t{AnsiColors.RED}Downloaded file does not exist...\n\t\t{filepath}{AnsiColors.DEFAULT}")
        exit(1)
    print(f"\t{AnsiColors.CYAN}Unzipping file, {AnsiColors.YELLOW}[{filepath}]{AnsiColors.CYAN}, to temp folder, {AnsiColors.YELLOW}[{tmp_dir}]{AnsiColors.DEFAULT}")
    unzipFile(filepath, tmp_dir)
    print(f"\t{AnsiColors.GREEN}Finished unzipping {AnsiColors.YELLOW}[{filepath}]{AnsiColors.CYAN}, moving required files to {AnsiColors.YELLOW}[{handle_details.file_working_dir}]{AnsiColors.CYAN}.{AnsiColors.DEFAULT}")
    moveRequiredContent(handle_details.file_working_dir, tmp_dir, handle_details)
    print(f"\t{AnsiColors.GREEN}Finished moving files, deleting the temp handle_details.directory, {AnsiColors.YELLOW}[{tmp_dir}]{AnsiColors.CYAN}.{AnsiColors.DEFAULT}")
    shutil.rmtree(tmp_dir, ignore_errors=True)
    print(f"\t{AnsiColors.GREEN}Finished deleting the temporary folder. Deleting zip file.")
    os.remove(filepath)
    print(f"\t{AnsiColors.GREEN}Finished deleting zip file.")

def unzipFile(filepath: str, odir: str):
    with ZipFile(filepath, 'r') as zip_ref:
        zip_ref.extractall(odir)

def moveRequiredContent(to_dir: str, from_dir: str, handle_details: HandleDetails):
    if (handle_details.directory is not None):
        dir_full_path = os.path.join(from_dir, handle_details.directory)
        if (os.path.exists(dir_full_path) and os.path.isdir(dir_full_path)):
            shutil.copytree(dir_full_path, to_dir, dirs_exist_ok=True)

    if (handle_details.include is not None):
        inc_full_path = os.path.join(from_dir, handle_details.include)
        if (os.path.exists(inc_full_path) and os.path.isdir(inc_full_path)):
            shutil.copytree(inc_full_path, os.path.join(to_dir, "include"), dirs_exist_ok=True, ignore=shutil.ignore_patterns(
                '*.c', '*.cpp', '.o', '.a', '.dll', '.lib'))

    if (handle_details.lib is not None):
        lib_full_path = os.path.join(from_dir, handle_details.lib)
        if (os.path.exists(lib_full_path) and os.path.isdir(lib_full_path)):
            shutil.copytree(lib_full_path, os.path.join(to_dir, "lib"), dirs_exist_ok=True, ignore=shutil.ignore_patterns(
                '*.c', '*.cpp', '*.h', '*.hpp'))

    if (handle_details.src is not None):
        src_full_path = os.path.join(from_dir, handle_details.src)
        if (os.path.exists(src_full_path) and os.path.isdir(src_full_path)):
            shutil.copytree(src_full_path, os.path.join(to_dir, "src"), dirs_exist_ok=True, ignore=shutil.ignore_patterns(
                '*.h', '*.hpp', '.o', '.a', '.dll', '.lib'))

def handleExe(handle_details: HandleDetails):
    ans = input(f'{AnsiColors.BLUE}Would you like to install {AnsiColors.YELLOW}{handle_details.filename}{AnsiColors.BLUE} now?{AnsiColors.DEFAULT}\n\t\t("y","n") > ')
    if ans.casefold() != 'Y'.casefold() and ans.casefold() != 'YES'.casefold():
        return
    filepath = os.path.join(handle_details.file_working_dir, handle_details.filename)
    os.startfile(filepath)
    print(f'{AnsiColors.CYAN}Please restart setup script when installation of {AnsiColors.YELLOW}{handle_details.filename}{AnsiColors.CYAN} is finished.{AnsiColors.DEFAULT}')
    quit()

def handleOtherFiles(details: HandleDetails):
    filepath = os.path.join(details.file_working_dir, details.filename)
    moveRequiredContent(details.file_working_dir, details.file_working_dir, details)
    os.remove(filepath)
