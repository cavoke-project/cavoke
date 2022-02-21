function processResponse(response) {
    updateBoard(response);
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
