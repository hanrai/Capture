import QtQuick 2.0
import QtQuick.Controls 2.2

Pane {
    property string titleIcon
    property string titleText
    padding: 8
    background: null

    Row {
        spacing: 8
        Image {
            width: 24
            height: 24
            source: titleIcon
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            font.pixelSize: 18
            text: titleText
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
