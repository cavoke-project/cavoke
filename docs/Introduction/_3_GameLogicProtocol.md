# Game Logic Protocol {#GameLogicProtocol}

## Main Structures

A more hands-on explanation is available on the Cavoke Developer OpenAPI website:

[![openapi](https://img.shields.io/badge/Cavoke%20DEV-passing-success?logo=swagger&logoColor=white&style=for-the-badge)](https://cavokedev.docs.apiary.io)

### GameState

Game state that is returned by the **server component**.

This object has four fields:

- `is_terminal: bool`: True if the game is over. **It means that no more [moves](#GameMove) will be accepted** from
  the
  players and winners must be announced.
- `global_state: str`: Global state of the game containing **all** information about the current state of the game
  session. This is required, as
  components are stateless (see [Components](#Components)). This state is **never shared with players**. This state is
  received and
  stored in the database for the next [GameMove](#GameMove) processing. This string-state can be **of any format**:
  json, base64, xml,
  etc. It just must be encodable using UTF-8.
- `players_state: str[]`: An array of player states. Only **this state is shared with players**. They are
  numbered
  according to `player_id`. For instance, upon request `player_id=2` will receive the `player_state[2]` string. As with
  the global state, `players_state` can be of any format that is encodable in UTF-8.
- `winners: int[]`: An array of `player_id`'s have won in this round. Should be an empty
  when `is_terminal=false` and set
  once when `is_terminal=true`. Future plans to use a points-based system.

Example:

```json
{
  "is_terminal": true,
  "global_state": "OOXXXOXOX",
  "players_state": [
    "OOXXXOXOX",
    "OOXXXOXOX"
  ],
  "winners": [
    0
  ]
}
```

### GameMove

Player's move that is delivered to the **server component**.

This object 3 fields.

- `player_id: int`: Player int in the session (see [Components](#Components)). This id auto-determined by the cavoke
  server and can be trusted ([#TODO]).
- `move: str`: Player's move transported from the **client component**. As with states, this string can be any UTF-8
  string. **Important:** This move **cannot be trusted** to be delivered from the **client component**, therefore move
  correctness validation is required.
- `global_state: str`: Current `global_state` of the game session. It is also auto-added to the move by the cavoke
  server.

Example:

```json
{
  "player_id": 0,
  "move": "X1",
  "global_state": "O OXXOXOX"
}
```

## Interacting with the server component

Developer must provide 3 methods:

- `validate: (json, int[]) -> ValidationResult`: Checks whether a session is valid and is ready to be started, by
  getting game json settings and array of occupied position.
- `init_state: (json, int[]) -> cavoke::GameState`: Initializes game state with *valid* (checked via the previous
  method) game config.
- `apply_move: (GameMove) -> cavoke::GameState`: Updates current game state using the supplied GameMove.

### Local game component

For C++ development please use provided header-only library `cavoke-dev-lib/cavoke.h` and implement the following
functions:

- `bool validate_settings()`
- `cavoke::GameState init_state()`
- `cavoke::GameState apply_move()`

### Remote game component

Please consult the Cavoke Developer OpenAPI Schema Docs:

[![openapi](https://img.shields.io/badge/Cavoke%20DEV-passing-success?logo=swagger&logoColor=white&style=for-the-badge)](https://cavokedev.docs.apiary.io)