import QtQuick 2.0
import QtQuick.Controls 2.3
Item {
    x: obj.spotX
    y: obj.spotY - 16
    width: obj.spotWidth
    height: obj.spotHeight + 16
    visible: obj.actived && !machine.NoSnap
    property var obj
    property alias source: icon.source

    Image {
        id: icon
        width:16
        height:16
        x: 0
        y: 0
    }

    Button {
        id: button
        x: 0
        y: 16
        width: obj.spotWidth
        height: obj.spotHeight
        contentItem: Rectangle {
            anchors.fill: parent
            opacity: button.down ? 0.4 : 0.7
            color: "lightGray"
            border.color: "darkGray"
            border.width: 1
            radius: 2
        }
        background: null
    }
}
