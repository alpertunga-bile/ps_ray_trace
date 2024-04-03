from zipfile import ZipFile
from os.path import join, exists
from os import rename, remove
from platform import system
from subprocess import run

from argparse import ArgumentParser


def run_command(command: str) -> bool:
    process = run(command, shell=True, capture_output=True, check=True)

    return True if process.returncode else False


def prepare_venv() -> None:
    if exists("venv"):
        print("Environment is already created")
        return

    python_exe = f"{join('venv', 'Scripts', 'python.exe')}"

    print("Creating virtual environment")
    run_command("python -m venv venv")

    print("Installing required packages")
    run_command(f"{python_exe} -m pip install requests tqdm")

    print("Creation is completed")


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
            desc="Installing ISPC",
        ) as pbar:
            for chunk in req.iter_content(chunk_size=1024):
                if chunk:
                    f.write(chunk)
                    pbar.update(len(chunk))


def prepare_ispc(version: str, ispc_zip_filename: str) -> None:
    download_ispc_zip(version, ispc_zip_filename)

    print("Extracting ISPC")
    with ZipFile(ispc_zip_filename, "r") as zip_f:
        zip_f.extractall()
    zip_f.close()

    rename(ispc_zip_filename[:-4], "ispc")
    remove(ispc_zip_filename)

    print("Preparing ISPC is completed")


if __name__ == "__main__":
    parser = ArgumentParser(prog="ps_ray_trace setup program")
    parser.add_argument("--setup_env", action="store_true", default=False)
    parser.add_argument("--setup_project", action="store_true", default=False)
    args = parser.parse_args()

    ispc_folder = "ispc"
    ispc_version = "1.23.0"
    ispc_zip_filename = f"ispc-v{ispc_version}-windows.zip"
    opsys = system()

    if opsys != "Windows":
        print(
            "Currently supporting Windows OS for the ispc setup. Please install the ispc from the package manager."
        )
        exit(0)

    if args.setup_env:
        prepare_venv()
        exit(0)

    if exists("ispc"):
        print("ISPC is already installed")
        exit(0)

    prepare_ispc(version=ispc_version, ispc_zip_filename=ispc_zip_filename)
