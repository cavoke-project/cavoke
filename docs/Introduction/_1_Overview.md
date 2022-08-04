# Overview
**Cavoke** is a platform for creating and hosting multiplayer turn-based board games.

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