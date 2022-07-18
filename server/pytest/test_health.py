import pytest
import cavoke_openapi_client
from cavoke_openapi_client.api import default_api


def test_health():
    with cavoke_openapi_client.ApiClient(pytest.server_config) as api_client:
        api_instance = default_api.DefaultApi(api_client)
        # Check health
        assert api_instance.health() == 'OK'
