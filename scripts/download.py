from shared import AnsiColors
from libfile import getFileName
import os
import pip._vendor.requests as requests
import sys
import time

# Code for "handleDownload" from @TheCherno's Hazel Utils.py
# https://github.com/TheCherno/Hazel/tree/master
def handleDownload(out_file: str, lib_url: str):
    with open(out_file, 'wb') as f:
        headers = {'User-Agent': "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36"}
        response = requests.get(lib_url, headers=headers, stream=True)
        total = response.headers.get('content-length')
        if total is None:
            f.write(response.content)
        else:
            download = 0
            total = int(total)
            start_time = time.time()
            for data in response.iter_content(chunk_size=max(int(total/1000), 1024 * 1024)):
                download += len(data)
                f.write(data)
                try:
                    done = int(50*download/total) if download < total else 50
                    percentage = (download / total) * 100 if download < total else 100
                except ZeroDivisionError:
                    done = 50
                    percentage = 100
                elapsed_time = time.time() - start_time
                kb_per_sec = 0.0
                try:
                    kb_per_sec = (download / 1024) / elapsed_time
                except ZeroDivisionError:
                    kb_per_sec = 0.0
                avg_speed_str = '{:.2f} kB/s'.format(kb_per_sec)
                if (kb_per_sec > 1024):
                    mb_per_sec = kb_per_sec / 1024
                    avg_speed_str = '{:.2f} MB/s'.format(mb_per_sec)
                sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50-done), percentage, avg_speed_str))
                sys.stdout.flush()
    sys.stdout.write('\n')


def downloadDependency(lib_name: str, lib_url: str, lib_dir: str):
    install_app = input(f'{AnsiColors.BLUE}Would you like to install {AnsiColors.YELLOW}{lib_name}{AnsiColors.BLUE} from {AnsiColors.YELLOW}{lib_url}{AnsiColors.BLUE}?{AnsiColors.DEFAULT}\n\t("y", "n") > ')
    filename = getFileName(lib_url)
    if (install_app.casefold() != 'y' and install_app.casefold() != 'yes'):
        return False
    try:
        os.makedirs(lib_dir, exist_ok=True)
        handleDownload(os.path.join(lib_dir, filename), lib_url)
        return True
    except OSError:
        print(f"{AnsiColors.RED}Failed to create [{lib_dir}].{AnsiColors.DEFAULT}")
        exit()

def isVulkanPresent(lib_version):
    vulkan_sdk = os.environ.get("VULKAN_SDK")
    if vulkan_sdk is None:
        return False
    print(f"{AnsiColors.GREEN}Vulkan install detected.{AnsiColors.DEFAULT}")
    if lib_version not in vulkan_sdk:
        print(f"{AnsiColors.CYAN}Vulkan version mismatch. Proceeding.{AnsiColors.DEFAULT}")
        return False
    return True
