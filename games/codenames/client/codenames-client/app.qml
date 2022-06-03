import QtQuick 2.15
import QtQuick.Window 2.15
import "src/components"
import "src/screens"

Rectangle {
    width: 1280
    height: 768
    //anchors.fill: parent
    visible: true

    Game {
        anchors.fill: parent
    }
}
