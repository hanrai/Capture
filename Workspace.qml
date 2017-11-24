import QtQuick 2.0

Image {
    id: image
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.AllButtons
        onPositionChanged: {
            machine.submitEvent("Mouse.Moved",
                {"X": mouseX, "Y": mouseY})
        }
        onClicked: {
            if(mouse.button & Qt.LeftButton) {
                machine.submitEvent("Mouse.LeftClicked",
                    {"X": mouseX, "Y": mouseY})
            }
            if(mouse.button & Qt.RightButton) {
                machine.submitEvent("Mouse.RightClicked",
                    {"X": mouseX, "Y": mouseY})
            }
            if(mouse.button & Qt.MiddleButton) {
                machine.submitEvent("Mouse.MiddleClicked",
                    {"X": mouseX, "Y": mouseY})
            }
        }
        onDoubleClicked: {
            if(mouse.button & Qt.LeftButton) {
                machine.submitEvent("Mouse.LeftDoubleClicked",
                    {"X": mouseX, "Y": mouseY})
            }
            if(mouse.button & Qt.RightButton) {
                machine.submitEvent("Mouse.RightDoubleClicked",
                    {"X": mouseX, "Y": mouseY})
            }
            if(mouse.button & Qt.MiddleButton) {
                machine.submitEvent("Mouse.MiddleDoubleClicked",
                    {"X": mouseX, "Y": mouseY})
            }
        }
    }
}
