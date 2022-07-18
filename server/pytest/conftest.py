import subprocess

import cavoke_openapi_client
import pytest


def pytest_addoption(parser: pytest.Parser):
    parser.addoption("--endpoint", action="store", default="https://develop.api.cavoke.wlko.me", help="API endpoint "
                                                                                                      "to test, "
                                                                                                      "usually your "
                                                                                                      "'localhost:8080'")
    parser.addoption("--servercmd", action="store", default=None, help="command to start the server process")


def pytest_configure(config: pytest.Config):
    """
    Allows plugins and conftest files to perform initial configuration.
    This hook is called for every plugin and initial conftest
    file after command line options have been parsed.
    """
    host = config.getoption("endpoint")
    print(f"Using this endpoint for testing: '{host}'", flush=True)
    pytest.server_config = cavoke_openapi_client.Configuration(
        host=host
    )


def pytest_sessionstart(session: pytest.Session):
    """
    Called after the Session object has been created and
    before performing collection and entering the run test loop.
    """
    server_cmd = session.config.getoption("servercmd")
    if server_cmd is None:
        print("No process started!!!", flush=True)
    else:
        print(f"Starting server process with '{server_cmd}'...", flush=True)
        pytest.server_process = subprocess.Popen(args=server_cmd, shell=True)


def pytest_sessionfinish(session, exitstatus):
    """
    Called after whole test run finished, right before
    returning the exit status to the system.
    """
    if session.config.getoption("servercmd") is None:
        return
    try:
        print('Waiting for server to finish...', flush=True)
        pytest.server_process.wait(timeout=0.5)
    except subprocess.TimeoutExpired:
        print('Server terminating by force...', flush=True)
        pytest.server_process.kill()
    print('Server process stopped', flush=True)


def pytest_unconfigure(config):
    """
    called before test process is exited.
    """
