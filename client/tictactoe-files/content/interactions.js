function processResponse(response) {
    let res = JSON.parse(response)
    updateBoard(res["state"]);
}

function sendMove(moveString) {
    let move = {}
    move.move = "X" + moveString
    cavoke.getMoveFromQml(JSON.stringify(move))
}

function updateBoard(boardString) {
    for (let i = 0; i < 9; ++i) {
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
