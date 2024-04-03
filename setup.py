from zipfile import ZipFile
from os.path import join, exists
from os import rename, remove, mkdir
from platform import system
from subprocess import run
from shutil import rmtree


from argparse import ArgumentParser

# --------------------------------------------------------------------------------------------------------------------
# -- Environment Functions


def run_command(command: str) -> bool:
    process = run(command, shell=True, capture_output=True, check=True)

    return True if process.returncode else False


def prepare_venv() -> None:
    if exists("venv"):
        print("/_\ Environment is already created")
        return

    python_exe = f"{join('venv', 'Scripts', 'python.exe')}"

    print("/_\ Creating virtual environment")
    run_command("python -m venv venv")

    print("/_\ Installing required packages")
    run_command(f"{python_exe} -m pip install requests tqdm beautifulsoup4 lxml")

    print("/_\ Creation is completed")


# --------------------------------------------------------------------------------------------------------------------
# -- Downloading ISPC functions


def get_zipfilepath(folder: str, version: str) -> str:
    return join(folder, f"ispc-v{version}-windows.zip")


def get_current_version_ispc() -> str:
    from bs4 import BeautifulSoup
    from requests import get

    doc = BeautifulSoup(
        get("https://github.com/ispc/ispc/tags").content, features="lxml"
    )

    version = doc.find("a", {"class": "Link--primary Link"}).get_text().replace("v", "")

    return version


def download_ispc_zip(version: str, ispc_zip_filename: str) -> None:
    from requests import get
    from tqdm import tqdm

    zip_url = f"https://github.com/ispc/ispc/releases/download/v{version}/ispc-v{version}-windows.zip"

    req = get(zip_url, stream=True)

    assert req.status_code == 200

    total_size = int(req.headers.get("content-length"))

    with open(ispc_zip_filename, "wb") as f:
        with tqdm(
            total=total_size,
            unit="B",
            unit_scale=True,
            unit_divisor=1024,
            desc=f"/_\ Installing ISPC {version}",
        ) as pbar:
            for chunk in req.iter_content(chunk_size=1024):
                if chunk:
                    f.write(chunk)
                    pbar.update(len(chunk))


def prepare_ispc(version: str, ispc_zip_filename: str) -> None:
    download_ispc_zip(version, ispc_zip_filename)

    print("/_\ Extracting ISPC")
    with ZipFile(ispc_zip_filename, "r") as zip_f:
        zip_f.extractall()

    rename(ispc_zip_filename[:-4], "ispc")
    remove(ispc_zip_filename)

    print("/_\ Preparing ISPC is completed")


# --------------------------------------------------------------------------------------------------------------------
# -- Upgrading ISPC functions


def upgrade_ispc(third_party_folder: str, ispc_folder: str) -> None:
    current_version = get_current_version_ispc()
    v_current_version = "v" + current_version

    with open(join(ispc_folder, "ReleaseNotes.txt")) as version_file:
        installed_version = version_file.readline().split(" ")[1]

    if installed_version == v_current_version:
        print("/_\ No need to update")
        return

    print(f"/_\ Removing ISPC {installed_version}")
    rmtree(ispc_folder)

    new_ispc_zip_filename = get_zipfilepath(third_party_folder, current_version)

    prepare_ispc(current_version, new_ispc_zip_filename)


if __name__ == "__main__":
    parser = ArgumentParser(prog="ps_ray_trace setup program")
    parser.add_argument("--setup_env", action="store_true", default=False)
    parser.add_argument("--setup_project", action="store_true", default=False)
    args = parser.parse_args()

    opsys = system()
    third_party_folder = "third-party"

    if exists(third_party_folder) is False:
        mkdir("third-party")
        print("/_\ Third party is created")

    if opsys != "Windows":
        print(
            "Currently supporting Windows OS for the ispc setup. Please install the ispc from the package manager."
        )
        exit(0)

    if args.setup_env:
        prepare_venv()
        exit(0)

    ispc_folder = join(third_party_folder, "ispc")
    ispc_version = get_current_version_ispc()
    ispc_zip_filename = get_zipfilepath(third_party_folder, ispc_version)

    if exists(ispc_folder):
        print("/_\ ISPC is already installed")
        upgrade_ispc(third_party_folder, ispc_folder)
        exit(0)

    prepare_ispc(version=ispc_version, ispc_zip_filename=ispc_zip_filename)
