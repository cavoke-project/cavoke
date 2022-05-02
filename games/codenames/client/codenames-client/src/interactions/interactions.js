let previousState = "";

function sendMove(move) {
    let moveObj = {}
    moveObj.move = move;
    cavoke.getMoveFromQml(JSON.stringify(moveObj));
}

function makeHint(hint) {
    console.log("Make hint", hint);
    sendMove(hint);
}

function openCard(index) {
    console.log("Open card", index);
    sendMove(index.toString());
}


function updateInterface(model) {
    let board = findItemById('board');
    board.rows = model.m_height;
    board.columns = model.m_width;
    let stage = model.m_stage;
    let player = model.role;
    let hintControls = findItemById("hintControls");
    hintControls.visible = ((stage === 0 && player === 0) || (stage === 2 && player === 1));
    findItemById("currentHint").text = model.m_last_hint;

    for (let i = 0; i < model.m_height; ++i) {
        for (let j = 0; j < model.m_width; ++j) {
            if (board.children.length < (i * model.m_width + j) + 1) {
                let component = Qt.createComponent("../components/Card.qml");
                component.createObject(board);
            }
            let card = board.children[(i * model.m_width + j)];
            card.word = model.m_words[(i * model.m_width + j)];
            card.state = model.m_card_states[(i * model.m_width + j)];
            card.player = player;
            card.card_index = (i * model.m_width + j);
        }
    }
}

function processUpdate(update_str) {
    let update = JSON.parse(update_str);
    let model = JSON.parse(update.state);
    if (model !== previousState) {
        previousState = model;
        updateInterface(model);
    }
}
