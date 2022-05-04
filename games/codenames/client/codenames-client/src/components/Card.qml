import QtQuick 2.0

import "../interactions/interactions.js" as Interact
import "../style/colors.js" as Colors

Item {
    id: card;

    property string word: ""
    property int state: 0
    property bool opened: false
    property int player: 0
    property int card_index: 0
    width: 180
    height: 100

    function getColor(state, opened, player) {
        if (state === 0) {
            if (player >= 2 || !opened) {
                return Colors.BLUE_AGENT;
            }
            return Colors.BLUE_AGENT_OPENED;
        } else if (state === 1) {
            if (player >= 2 || !opened) {
                return Colors.RED_AGENT;
            }
            return Colors.RED_AGENT_OPENED;
        } else if (state === 2) {
            return Colors.BLACK_CARD;
        } else if (state === 3) {
            if (player >= 2 || !opened) {
                return Colors.NEUTRAL;
            }
            return Colors.NEUTRAL_OPENED;
        } else {
            return Colors.CLOSED;
        }
    }

    function getTextColor(state) {
        return "white";
    }

    function getText(state, player, word) {
        if (state === 4 || player === 0 || player === 1) {
            return word;
        } else {
            return "";
        }
    }

    Rectangle {
        id: card_content
        anchors.fill: parent
//        radius: 10

        color: getColor(card.state, card.opened, card.player);

        Text {
            id: card_hint;

            anchors.centerIn: parent;
            text: getText(card.state, card.player, card.word);
            color: getTextColor(card.state);
        }

        MouseArea {
            anchors.fill: parent
            onClicked: Interact.openCard(parent.parent.card_index);
        }
    }
}
