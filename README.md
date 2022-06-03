<h1 align="center">
  <br>
  <a href="https://cavoke.wlko.me"><img src="https://avatars.githubusercontent.com/u/52053547?s=200&v=4" alt="Cavoke" width="200"></a>
  <br>
  Cavoke
  <br>
</h1>

<h4 align="center">A Platform for creating and hosting multiplayer turn-based board games</h4>

<p align="center">
    <a href="https://github.com/cavoke-project/cavoke/actions/workflows/app.yml">
        <img alt="GitHub Workflow Status" src="https://img.shields.io/github/workflow/status/cavoke-project/cavoke/app?style=flat-square&logo=github">
    </a>
<!--
    <a href="https://github.com/cavoke-project/cavoke/releases">
        <img alt="GitHub release (latest by date including pre-releases)" src="https://img.shields.io/github/v/release/cavoke-project/cavoke?include_prereleases&logo=github&style=flat-square">
    </a>
-->
    <a href="">
        <img alt="GitHub deployments" src="https://img.shields.io/github/deployments/cavoke-project/cavoke/develop?label=deployment&logo=google-cloud&logoColor=white&style=flat-square">
    </a>
    <a href="https://ghcr.io/cavoke-project/cavoke-server:latest">
        <img alt="Docker available" src="https://img.shields.io/badge/docker-available-success?style=flat-square&logo=docker&logoColor=white">
    </a>
    <a href="https://cavoke.docs.apiary.io">
        <img alt="OpenAPI status" src="https://img.shields.io/badge/OpenAPI-passing-success?logo=swagger&logoColor=white&style=flat-square">
    </a>
    <!--<a href="https://gitpod.io/#https://github.com/cavoke-project/cavoke">
        <img alt="Gitpod Ready-to-Code" src="https://img.shields.io/badge/Gitpod-Ready--to--Code-orange?logo=gitpod&style=flat-square">
    </a>-->
    <a href="./LICENSE">
        <img alt="License" src="https://img.shields.io/github/license/cavoke-project/cavoke?style=flat-square&color=6ad">
    </a>
</p>
<p align="center">
<a href="https://github.com/cavoke-project/cavoke/releases">
  <img alt="GitHub release (latest by date including pre-releases)" src="https://img.shields.io/badge/DOWNLOAD-windows%20%7C%20macos%20%7C%20linux-blue?style=for-the-badge">
</a>
<a href="https://console.cavoke.wlko.me">
<img alt="Browse available games" src="https://img.shields.io/badge/browse-available%20games-red?style=for-the-badge">
</a>
  
</p>

<p align="center">
  <a href="#overview">Overview</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#download">Download</a> •
  <a href="#credits">Credits</a> •
  <a href="#license">License</a>
</p>

<!-- TODO: replace gif -->
![screenshot](https://raw.githubusercontent.com/cavoke-project/cavoke/develop/.github/assets/cavoke-demo.gif)

## Overview

### Problem formulation

We believe that developing a desktop multiplayer game is always a great adventure that has many wonderful opportunities
to learn something new along the way.
However, in our opinion this journey, is often riddled with unnecessary challenges of implementing mechanisms that are
common for many games. This includes:

- Developing a **client application** with many UI components for game creation process
- **Networking**, which in itself includes
  - **Synchronizing the game state** among clients
  - Handling network issues on client's side
  - SSL and Security
- **Game state persistence**
- **Game move validation**
- **Role choosing** (e.g. White/Black in Chess)
- Managing **game sessions** and **shareable invites* for friends to play with you
- **Game and User statistics** (win rate, total time spent, etc.)
- User **Authentication and Authorization**
- Game versioning
- Hosting server logic and scalability

To make these obstacles easier to avoid, we developed a framework for developing multiplayer turn-based board games
that comes with built-in solutions to solve all the above-mentioned problems.

### Our solution
#### Developers side

Our platform provides an easy way for developers to create a desktop game. A developer should provide only two
components:

- **Server logic component** that verifies one's move correctness and updates the game state
- **Client game component**. A [QML](https://doc.qt.io/qt-6/qmlapplications.html) application that communicates with the
  main Cavoke Client application and sends player's moves through this gateway. It also receives updates from other
  players' moves to update its user interface.

<details>
  <summary>Example Cavoke Game Development Story (Tic-Tac-Toe)</summary>

Let's see how a simple Tic-Tac-Toe game using our Cavoke platform may look like. See files for this
example [here](./games/tictactoe) and [here](./client/tictactoe-files)
. <!-- TODO: Put tictactoe into a separate repo -->

We start off with the client-side. We create a simple QML application and design our game UI. Now it's time to connect
our QML component to the main cavoke Qt application:

```qml
...
import "content/interactions.js" as Interact

Rectangle {
    // BEGIN cavoke section
    Connections {
        target: cavoke

        function onReceiveUpdate(jsonUpdate) { // <-- Signal that handles incoming game states
            console.log("Received: " + jsonUpdate);
            Interact.processResponse(jsonUpdate);
        }
    }
    // END cavoke section

    id: game
...
```

And in the `interactions.js`:

```js
function processResponse(response) { // <-- Processes the received state from the server
  let res = JSON.parse(response)
  updateBoard(res["state"]); // Parses the game state UI.
                             // Here res["state"] may look something like "XOO OX X "
}

function sendMove(moveString) {
  let move = {}
  move.move = "X" + moveString
  cavoke.getMoveFromQml(JSON.stringify(move)) // <-- Sends data to the server via Qt
}

function updateBoard(boardString) {
  for (let i = 0; i < 9; ++i) {
    board.children[i].state = boardString[i];
  }
}

// ...
```

Developers may decide for themselves how their states will look. They may use json, plain-string, base64. It just has to
be representable as a string. For example, the developers of this game decided to encode the game state as a json
with `state` field that is 9 characters long representing the tictactoe board, i.e. `XOO OX X ` would represent the
following field.

```
XOO
 OX
 X 
```

> :information_source: Notice that QML game component doesn't include any networking, session management, users
> authentication, etc. All of this is handled by the main cavoke client executable. Developers can focus solely on their
> game.
> That's it with the client side! All the other bits and bobs around QML component have to do with UI and not
> administrating the game process.

Now let's see what awaits us on the server side.

A server component must be able to handle 3 requests.

- `VALIDATE` – Check if received game configuration can be used to start it (e.g. minimum players count reached, at
  least 1 player in every team, etc.)
- `INIT` – Create a game session using given configuration. Generates game state for this session. Now players can play.
- `MOVE` – Process user's move (figure move in Chess, new X/O in Tic-Tac-Toe, etc.)

> :warning: Server components also have to be *stateless*, because they may be shut down and restarted at any point
> without notice. This means that all game session information must be stored in the game state that is passed onto the
> server component with every `MOVE` request.

So for Tic-Tac-Toe we have something like this:

```cpp
bool validate_settings( // <-- Validates game configuration
    const json &settings,
    const std::vector<int> &occupied_positions,
    const std::function<void(std::string)> &message_callback) {
    if (occupied_positions.size() != 2) {
        message_callback("Not enough players");
        return false;
    }
    if (!settings.contains("board_size")) {
        message_callback("No board_size property");
        return false;
    }
    if (settings["board_size"].get<int>() != 3) {
        message_callback("Only  board_size=3 is supported");
        return false;
    }
    return true;
}
// ...
GameState init_state(const json &settings, // <-- Creates a game session
                     const std::vector<int> &occupied_positions) {
    int board_size = settings["board_size"];
    std::string board(board_size * board_size, ' ');
    return GameState{false, board, {board, board}, {}};
}
// ...
GameState apply_move(GameMove &new_move) { /// <-- Handles move request
    std::string &board = new_move.global_state;
    char player = (new_move.player_id == 0 ? 'X' : 'O');
    if (player != current_player(board)) {
        return {false, board, {board, board}, {}};
    }
    int position = extract_position(new_move.move);
    if (!is_valid_move(board, position)) {
        return {false, board, {board, board}, {}};
    }
    board[position] = player;
    bool win = winner(board);
    std::vector<int> winners;
    if (win) {
        winners.push_back(new_move.player_id);
    }
    return {win, board, {board, board}, winners};
}
```

> :information_source: You can read more about all the game logic protocol [here](./GameLogicProtocol.md)
  </details>

#### Players side
Futhermore, we provide many features in our cavoke client for the users. These include:
- **Cross platform**: Available on Windows, macOS and Linux
- **User Authentication** using Email-Password or SSO via Google or GitHub
- **Catalog of available cavoke games**
- **Rooms** with shareable invites for your friends, so you can play multiple games without the need to change the app
- **User's statistics** for every game
- **Developer mode** to test QML components locally

## How To Use

Please see the instructions for how to clone and build server and client components in its subdirectories.

- [Server](./server)
- [Client](./client)

## Download

You can [download](https://github.com/cavoke-project/cavoke/releases/) the latest installable version of Cavoke Client
for Windows, macOS and Linux.

## Credits

This software uses the following open source projects:

- [Qt](https://www.qt.io/)
- [Drogon](https://github.com/drogonframework/drogon)
- [Circle Icons](https://www.iconfinder.com/iconsets/circle-icons-1) by Nick Roach
- [Boost](https://www.boost.org/)
- [KArchive](https://github.com/KDE/karchive)
- [KDE Craft](https://github.com/KDE/Craft)
- [ECM](https://github.com/KDE/extra-cmake-modules)
- [jwt-cpp](https://github.com/Thalhammer/jwt-cpp)
- [qtkeychain](https://github.com/frankosterfeld/qtkeychain)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Quassel](https://github.com/quassel/quassel)

Credit to [Mudlet](https://github.com/Mudlet/Mudlet) and [Markdownify](https://github.com/amitmerchant1990/electron-markdownify) for the README inspiration.

## License

[MIT](./LICENSE)

---

> [@MarkTheHopeful](https://github.com//MarkTheHopeful) &nbsp;&middot;&nbsp;
> [@waleko](https://github.com/waleko) &nbsp;&middot;&nbsp;
> [@petrtsv](https://github.com/petrtsv) 
