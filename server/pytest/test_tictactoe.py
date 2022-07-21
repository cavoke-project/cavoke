import time
import uuid
from typing import List

import cavoke_openapi_client
import pytest
from cavoke_openapi_client.api import default_api

tictactoe_game_id = "tictactoe"


def test_check_tictactoe_available():
    with cavoke_openapi_client.ApiClient(pytest.server_config) as api_client:
        api_instance = default_api.DefaultApi(api_client)
        games: List[default_api.GameInfo] = api_instance.list_games()
        assert any(e.id == tictactoe_game_id for e in
                   games), f"Expected {tictactoe_game_id} in games. All other f{tictactoe_game_id} tests will fail as well."


def test_simple_game():
    with cavoke_openapi_client.ApiClient(pytest.server_config) as api_client:
        api_instance = default_api.DefaultApi(api_client)
        alice_id = str(uuid.uuid4())
        bob_id = str(uuid.uuid4())

        session: default_api.SessionInfo = api_instance.create_session(tictactoe_game_id, user_id=alice_id)
        assert session.game_id == tictactoe_game_id

        s2 = api_instance.join_session(session.session_id, user_id=bob_id)
        assert s2.status == 0

        api_instance.start_session(session.session_id, user_id=alice_id)

        # Move
        moves = [
            (0, 'X 4'),
            (1, 'O 2'),
            (0, 'X 8'),
            (1, 'O 0'),
            (0, 'X 1'),
            (1, 'O 7'),
            (0, 'X 3'),
            (1, 'O 5'),
            (0, 'X 6'),
        ]
        for player_id, move in moves:
            assert api_instance.session_info(session.session_id, user_id=bob_id).status == 1
            api_instance.send_move(session.session_id, user_id=bob_id if player_id else alice_id,
                                   game_move=default_api.GameMove(move))
            time.sleep(0.1)  # for synchronizing moves

        final_session: default_api.SessionInfo = api_instance.session_info(session.session_id, user_id=bob_id)
        final_state: default_api.GameState = api_instance.get_update(session.session_id, user_id=alice_id)
        assert final_session.session_id == session.session_id
        assert final_session.status == 2
        assert final_state.state == 'OXOXXOXOX'
