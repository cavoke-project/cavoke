let previousState = "";

function sendMove(move) {
    let moveObj = {}
    moveObj.move = JSON.stringify(move);
    cavoke.getMoveFromQml(JSON.stringify(moveObj));
}

function makeHint(hint, attempts = 1) {
    if (!hint || !attempts || attempts <= 0) {
        return;
    }
    console.log("Make hint", hint);
    sendMove({type: "hint", hint: hint, attempts: attempts});
}

function openCard(index) {
    console.log("Open card", index);
    sendMove({type: "open", position: index});
}

function skip() {
    console.log("Skip move");
    sendMove({type: "skip"});
}

function getRoleName(role) {
    if (role === 0) {
        return "blue captain";
    } else if (role === 1) {
        return "red captain";
    } else if (role % 2 === 0) {
        return "blue player";
    } else {
        return "red player";
    }
}

function isYourTurn(role, stage) {
    if (stage === 0) {
        return role === 0;
    } else if (stage === 2) {
        return role === 1;
    } else if (stage === 1) {
        return role === 2 || role === 4;
    } else if (stage === 3) {
        return role === 3 || role === 5;
    }

    return false;
}


function updateInterface(model) {
    board.rows = model.m_height;
    board.columns = model.m_width;
    let stage = model.m_stage;
    let player = model.role;
    hintControls.visible = ((stage === 0 && player === 0) || (stage === 2 && player === 1));
    if (model.m_last_hint) {
        currentHint.text = "\"" + model.m_last_hint + "\"";
    } else {
        currentHint.text = "none";
    }
    currentAttempts.text = model.m_attempts_left.toString();
    yourRole.text = getRoleName(player);

    yourTurnLabel.visible = isYourTurn(player, model.m_stage);
    skipControls.visible = yourTurnLabel.visible && (player >= 2);

    blueCardsLeft.text = model.m_blue_closed.toString();
    redCardsLeft.text = model.m_red_closed.toString();

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
            if (player <= 1) {
                card.opened = model.m_opened[(i * model.m_width + j)];
            }
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
