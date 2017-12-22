import QtQuick 2.0

Image {
    id: image
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.AllButtons
        onPositionChanged: {
            machine.submitEvent("Mouse.Moved",
                {"pos": Qt.point(mouseX, mouseY)})
        }
        onClicked: {
            if(mouse.button & Qt.LeftButton) {
                machine.submitEvent("Mouse.LeftClicked",
                    {"pos": Qt.point(mouseX, mouseY)})
            }
            if(mouse.button & Qt.RightButton) {
                machine.submitEvent("Mouse.RightClicked",
                    {"pos": Qt.point(mouseX, mouseY)})
            }
            if(mouse.button & Qt.MiddleButton) {
                machine.submitEvent("Mouse.MiddleClicked",
                    {"pos": Qt.point(mouseX, mouseY)})
            }
        }
        onDoubleClicked: {
            if(mouse.button & Qt.LeftButton) {
                machine.submitEvent("Mouse.LeftDoubleClicked",
                    {"pos": Qt.point(mouseX, mouseY)})
            }
            if(mouse.button & Qt.RightButton) {
                machine.submitEvent("Mouse.RightDoubleClicked",
                    {"pos": Qt.point(mouseX, mouseY)})
            }
            if(mouse.button & Qt.MiddleButton) {
                machine.submitEvent("Mouse.MiddleDoubleClicked",
                    {"pos": Qt.point(mouseX, mouseY)})
            }
        }
    }
}
