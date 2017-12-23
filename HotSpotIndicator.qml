import QtQuick 2.0
import QtQuick.Controls 2.3

Button {
    x: hotSpot.shape.x
    y: hotSpot.shape.y
    width: hotSpot.shape.width
    height: hotSpot.shape.height
    visible: hotSpot.actived
    contentItem: Rectangle {
        anchors.fill: parent
        opacity: down ? 0.4 : 0.7
        color: "lightGray"
        border.color: "darkGray"
        border.width: 1
        radius: 2
    }

    background: null

    onClicked: { machine.submitEvent("HotSpot.Clicked") }
}
