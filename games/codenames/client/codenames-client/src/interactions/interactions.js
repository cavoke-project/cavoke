let previousState = "";

function makeHint(hint) {
    console.log("Make hint", index);
    cavoke.getMoveFromQml(hint);
}

function openCard(index) {
    console.log("Open card", index);
    cavoke.getMoveFromQml(index.toString());
}

function clearChildren(element) {
    while (element.children.length > 0) {
        element.children[element.children.length - 1].destroy()
    }
}

function drawBoard(h, w, card_words, card_states, player) {
    let board = findItemById('board');
    clearChildren(board);
    board.rows = h;
    board.columns = w;
    for (let i = 0; i < h; ++i) {
        for (let j = 0; j < w; ++j) {
            let component = Qt.createComponent("../components/Card.qml");
            let card = component.createObject(board);
            card.word = card_words[(i * w + j)];
            card.state = card_states[(i * w + j)];
            card.player = player;
            card.card_index = (i * w + j);
        }
    }
}

function processUpdate(update_str) {
    let update = JSON.parse(update_str);
    let model = JSON.parse(update.state);
    if (model !== previousState) {
        previousState = model;
        drawBoard(model["m_height"], model["m_width"], model["m_words"], model["m_card_states"], model["role"]);
    }
}
