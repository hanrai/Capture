import QtQuick 2.7
import QtQuick.Controls 2.2
import Qt.labs.settings 1.0
import QtScxml 5.8

ApplicationWindow {
    id: window
    title: qsTr("Capture")
    visible: true
    property var lastVisibility
    property StateMachine stateMachine

    StateMachineLoader {
        id: loader
        source: "qrc:/statemachine.scxml"
    }

    stateMachine: loader.stateMachine

    header: Banner {
        id: banner
        stateMachine: window.stateMachine
    }

    WindowStateSaver{
        window: window
        defaultWidth: 640
        defaultHeight: 480
    }

    Drawer {
        id: drawer
        clip: true
        opacity: 0.8

        y: banner.height
        width: 186
        height: window.height - banner.height

        modal: false
        interactive: false
        visible: true
        edge: Qt.RightEdge

        ListView {
            id: listView
            anchors.fill: parent

            headerPositioning: ListView.OverlayHeader
            header: Pane {
                id: header
                z: 2
                width: parent.width

                contentHeight: logo.height

                Image {
                    id: logo
                    width: parent.width
                    source: "qrc:/img/photo-camera.svg"
                    fillMode: implicitWidth > width ? Image.PreserveAspectFit : Image.Pad
                }

                MenuSeparator {
                    parent: header
                    width: parent.width
                    anchors.verticalCenter: parent.bottom
                    visible: !listView.atYBeginning
                }
            }

            footer: ItemDelegate {
                id: footer
                text: qsTr("Footer")
                width: parent.width

                MenuSeparator {
                    parent: footer
                    width: parent.width
                    anchors.verticalCenter: parent.top
                }
            }

            model: 10

            delegate: SwipeDelegate {
                height: 130
                DataDelegate {
                    leftPadding: 8
                    titleIcon: "qrc:/img/tag.png"
                    titleText: "合约"
                    enabled: stateMachine.Ready
                }
                ToolSeparator{
                    orientation: Qt.Horizontal
                    width: 186
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    Flickable {
        anchors.top: banner.bottom
        x: 0
        width: window.width - drawer.width
        height: window.height - banner.height
        contentWidth: image.width;
        contentHeight: image.height

        Image {
            id: image
            source: "image://snapshot/"+ocr.snapshot
            onSourceChanged: {window.visibility = lastVisibility}
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.AllButtons
                onPositionChanged: {
                    stateMachine.submitEvent("Mouse.Moved",
                        {"X": mouseX, "Y": mouseY})
                }
                onClicked: {
                    if(mouse.button & Qt.LeftButton) {
                        stateMachine.submitEvent("Mouse.LeftClicked",
                            {"X": mouseX, "Y": mouseY})
                    }
                    if(mouse.button & Qt.RightButton) {
                        stateMachine.submitEvent("Mouse.RightClicked",
                            {"X": mouseX, "Y": mouseY})
                    }
                    if(mouse.button & Qt.MiddleButton) {
                        stateMachine.submitEvent("Mouse.MiddleClicked",
                            {"X": mouseX, "Y": mouseY})
                    }
                }
                onDoubleClicked: {
                    if(mouse.button & Qt.LeftButton) {
                        stateMachine.submitEvent("Mouse.LeftDoubleClicked",
                            {"X": mouseX, "Y": mouseY})
                    }
                    if(mouse.button & Qt.RightButton) {
                        stateMachine.submitEvent("Mouse.RightDoubleClicked",
                            {"X": mouseX, "Y": mouseY})
                    }
                    if(mouse.button & Qt.MiddleButton) {
                        stateMachine.submitEvent("Mouse.MiddleDoubleClicked",
                            {"X": mouseX, "Y": mouseY})
                    }
                }
            }
        }
        ScrollIndicator.vertical: ScrollIndicator { }
        ScrollIndicator.horizontal: ScrollIndicator { }
    }

    StateGroup{
        states: State {
            name: "READY"
            when: ocr.loaded
            StateChangeScript {
                name: "myScript"
                script: stateMachine.submitEvent("Loaded")
            }
        }
    }
}
