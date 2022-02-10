function processResponse(response) {
    var parts = response.split("\n"); // Result, Board
    updateBoard(parts[1]);
    if (parts[0].endsWith("wins")) {
        gameFinished(parts[0]);
    }
    
}

function updateBoard(boardString) {
    for (var i = 0; i < 9; ++i) {
        board.children[i].state = boardString[i];
    }
}

function gameFinished(message) {
    messageDisplay.text = message
    messageDisplay.visible = true
}

function resetField() {
    for (var i = 0; i < 9; ++i)
        board.children[i].state = ""
}
