#!/usr/bin/env python3
import atexit
import time

import requests
import subprocess
import sys


def check_eq(expected, actual):
    if expected != actual:
        print(f'Check failed: expected {repr(expected)}, got {repr(actual)}')
        sys.exit(1)


def main():
    server_cmds = sys.argv[1:]
    assert server_cmds, 'Expected usage: ./server-test-health.py <command-to-run> <args>'

    port = 8080
    print(f'Booting server... at {server_cmds[0]}', flush=True)

    server = subprocess.Popen(args=server_cmds + ['-p', str(port)])
    def kill_server():
        try:
            server.wait(timeout=0.1)
        except subprocess.TimeoutExpired:
            print('Server terminating...', flush=True)
            server.kill()
    atexit.register(kill_server)
    time.sleep(2)  # FIXME
    print('Checks starting...', flush=True)

    with requests.get(f'http://localhost:{port}/health') as r:
        check_eq(200, r.status_code)
        check_eq("OK", r.text)
        print("Health ok.")

    print('All ok.')


if __name__ == '__main__':
    main()
