#! /usr/bin/python3

import os
import subprocess
import signal
import pathlib
import sys
import time
import argparse

wemeetapp_pkill_name = "wemeetapp"
safety_kill_time = 30


def kill_wemeetapp():
    os.system(f"pkill -f {wemeetapp_pkill_name}")

def signal_handler(sig, frame):
    print('SIGINT received, now kill wemeetapp and exit')
    kill_wemeetapp()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)


def run_hooked_wemeetapp(
    hook_so_path: pathlib.Path,
    wemeet_startup_script_name = "wemeet-x11",
    
):
    hook_so_path = hook_so_path.absolute()
    _result = subprocess.run(
        ["which", wemeet_startup_script_name],
        capture_output=True, text=True
    )
    wemeet_startup_script_full_path = pathlib.Path(_result.stdout.strip()).absolute()
    
    env = os.environ.copy()
    env["LD_PRELOAD"] = str(hook_so_path)
    p = subprocess.Popen(
        [wemeet_startup_script_full_path],
        env=env
    )

    # wait for 20 secs
    time.sleep(safety_kill_time)

    print("safety time is up, now kill wemeetapp and exit")

    p.terminate()
    kill_wemeetapp()
    

argparser = argparse.ArgumentParser()
argparser.add_argument("hook_so_path", type=pathlib.Path)
args = argparser.parse_args()

hook_so_path = pathlib.Path(args.hook_so_path)
assert hook_so_path.exists(), f"hook_so_path does not exist: {hook_so_path}"

run_hooked_wemeetapp(hook_so_path)
