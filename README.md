<h1 align="center">
  <br>
  <a href="https://cavoke.wlko.me"><img src="https://avatars.githubusercontent.com/u/52053547?s=200&v=4" alt="Cavoke" width="200"></a>
  <br>
  Cavoke
  <br>
</h1>

<h4 align="center">A Platform for creating and hosting multiplayer turn-based board games</h4>

<div style="text-align: center; padding-bottom: 5px;">
<p align="center">
    <a href="https://github.com/cavoke-project/cavoke/actions/workflows/app.yml">
        <img alt="GitHub Workflow Status" src="https://img.shields.io/github/workflow/status/cavoke-project/cavoke/app?style=flat-square&logo=github">
    </a>
    <a href="">
        <img alt="GitHub deployments" src="https://img.shields.io/github/deployments/cavoke-project/cavoke/develop?label=deployment&logo=google-cloud&logoColor=white&style=flat-square">
    </a>
    <a href="https://ghcr.io/cavoke-project/cavoke-server:latest">
        <img alt="Docker available" src="https://img.shields.io/badge/docker-available-success?style=flat-square&logo=docker&logoColor=white">
    </a>
    <a href="https://cavoke.docs.apiary.io">
        <img alt="OpenAPI status" src="https://img.shields.io/badge/OpenAPI-passing-success?logo=swagger&logoColor=white&style=flat-square">
    </a>
    <a href="https://github.com/cavoke-project/cavoke/blob/master/LICENSE">
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
<p align="center">
<img alt="Demo" src="https://raw.githubusercontent.com/cavoke-project/cavoke/develop/.github/assets/cavoke-demo.gif">
</p>
</div>

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
- Managing **game sessions** and **shareable invites** for friends to play with you
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

You can read more about creating your own cavoke game on
our [Cavoke Game Template](https://github.com/cavoke-project/cavoke-game-template) GitHub page that has been made to
flatten the learning curve.

#### Players side
Furthermore, we provide many features in our cavoke client for the users. These include:
- **Cross-platform**: Available on Windows, macOS and Linux
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

[MIT](https://github.com/cavoke-project/cavoke/blob/master/LICENSE)

---

> [@MarkTheHopeful](https://github.com//MarkTheHopeful) &nbsp;&middot;&nbsp;
> [@waleko](https://github.com/waleko) &nbsp;&middot;&nbsp;
> [@petrtsv](https://github.com/petrtsv) 
