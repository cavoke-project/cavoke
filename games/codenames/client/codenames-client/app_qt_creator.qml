import QtQuick 2.15
import QtQuick.Window 2.15
import "src/components"
import "src/screens"

Window {
    width: 1280
    height: 768
    visible: true
    title: qsTr("Codenames")

    Game {
        anchors.fill: parent
    }
}
