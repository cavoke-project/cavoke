import logging
import threading
import time
import uuid
from typing import List

import cavoke_openapi_client
import pytest
from cavoke_openapi_client.api import default_api

tictactoe_game_id = "tictactoe"


def test_check_tictactoe_available():
    """
    Checks whether tictactoe game is available
    """
    with cavoke_openapi_client.ApiClient(pytest.server_config) as api_client:
        api_instance = default_api.DefaultApi(api_client)
        games: List[default_api.GameInfo] = api_instance.list_games()
        assert any(e.id == tictactoe_game_id for e in
                   games), f"Expected {tictactoe_game_id} in games. All other f{tictactoe_game_id} tests will fail as well."


def test_simple_game():
    """
    Creates a tictactoe session and plays a simple round with itself, checking the state after every move.
    """
    with cavoke_openapi_client.ApiClient(pytest.server_config) as api_client:
        api_instance = default_api.DefaultApi(api_client)
        alice_id = str(uuid.uuid4())
        bob_id = str(uuid.uuid4())

        session: default_api.SessionInfo = api_instance.create_session(tictactoe_game_id, user_id=alice_id)
        assert session.game_id == tictactoe_game_id

        time.sleep(0.2)

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


@pytest.mark.parametrize('execution_number', range(10))
def test_transaction_isolation_on_game_end(execution_number):
    """
    Test for correct transaction isolation. There was a bug, when transactions were not isolated properly,
    therefore producing invalid a game state with broken invariant
    """
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
            (1, 'O 5')
        ]
        for player_id, move in moves:
            assert api_instance.session_info(session.session_id, user_id=bob_id).status == 1
            api_instance.send_move(session.session_id, user_id=bob_id if player_id else alice_id,
                                   game_move=default_api.GameMove(move))
            time.sleep(0.1)  # for synchronizing moves

        do_spamming = True

        def spam_x6_player_1():
            logging.debug('Commencing spamming')
            for i in range(50):
                logging.debug(f'req {i}')
                api_instance.send_move(session.session_id, user_id=bob_id, game_move=default_api.GameMove('X 6'), async_req=True)
                time.sleep(0.1)
                if not do_spamming:
                    break
            logging.debug('Spamming stopped')

        t_spam = threading.Thread(target=spam_x6_player_1)
        t_spam.start()

        time.sleep(1)

        api_instance.send_move(session.session_id, user_id=alice_id, game_move=default_api.GameMove('X 6'), _request_timeout=10)

        do_spamming = False
        logging.debug('Spamming stopped. Checking final.')

        final_session: default_api.SessionInfo = api_instance.session_info(session.session_id, user_id=alice_id)
        final_state: default_api.GameState = api_instance.get_update(session.session_id, user_id=alice_id)
        logging.debug(str(final_state))

        time.sleep(1)

        assert final_session.session_id == session.session_id
        if not final_state.is_terminal:
            logging.warning("Session is not finished! Skipping.")
        else:
            assert final_state.state == 'OXOXXOXOX'


@pytest.mark.parametrize('execution_number', range(5))
def test_requests_flood(execution_number):
    """
    Makes many requests to internal game (via boost process) and checks that the server is alive.
    Previously there was a server segmentation fault caused here.
    """
    with cavoke_openapi_client.ApiClient(pytest.server_config) as api_client:
        api_instance = default_api.DefaultApi(api_client)
        alice_id = str(uuid.uuid4())

        session: default_api.SessionInfo = api_instance.create_session(tictactoe_game_id, user_id=alice_id)
        assert session.game_id == tictactoe_game_id
        logging.debug('Starting spamming...')
        for i in range(50):
            logging.debug(f'req {i}')
            api_instance.validate_session(session.session_id, user_id=alice_id, async_req=True)
        logging.debug('Spamming stopped.')
        assert api_instance.health() == "OK"
