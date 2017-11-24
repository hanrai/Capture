import QtQuick 2.7
import QtQuick.Controls 2.2
import Qt.labs.settings 1.0
import QtScxml 5.8

ApplicationWindow {
    id: window
    title: qsTr("Capture")
    visible: true
    property int lastVisibility

    header: Banner {
        id: banner
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

//        ListView {
//            id: listView
//            anchors.fill: parent

//            headerPositioning: ListView.OverlayHeader
//            header: Pane {
//                id: header
//                z: 2
//                width: parent.width

//                contentHeight: logo.height

//                Image {
//                    id: logo
//                    width: parent.width
//                    source: "qrc:/img/photo-camera.svg"
//                    fillMode: implicitWidth > width ? Image.PreserveAspectFit : Image.Pad
//                }

//                MenuSeparator {
//                    parent: header
//                    width: parent.width
//                    anchors.verticalCenter: parent.bottom
//                }
//            }

//            footer: ItemDelegate {
//                id: footer
//                text: qsTr("Footer")
//                width: parent.width

//                MenuSeparator {
//                    parent: footer
//                    width: parent.width
//                    anchors.verticalCenter: parent.top
//                }
//            }

//            model: 10

//            delegate: SwipeDelegate {
//                height: 130
//                DataDelegate {
//                    leftPadding: 8
//                    titleIcon: "qrc:/img/tag.png"
//                    titleText: "合约"
//                    enabled: machine.Ready
//                }
//                ToolSeparator{
//                    orientation: Qt.Horizontal
//                    width: 186
//                }
//            }

//            ScrollIndicator.vertical: ScrollIndicator { }
//        }
    }

    Flickable {
        x: 0
        anchors.top: banner.bottom
        width: window.width - drawer.width
        height: drawer.height
        contentWidth: workspace.width;
        contentHeight: workspace.height

        Workspace {
            id: workspace
            source: engine.snapshotLoaded ?
                        "image://snapshot/"+engine.snapshot : ""
            onSourceChanged: {window.visibility = lastVisibility}
            MousePositionIndicator {}
            HotSpotIndicator {}
        }

        ScrollIndicator.vertical: ScrollIndicator { }
        ScrollIndicator.horizontal: ScrollIndicator { }
    }

    StateGroup{
        states: State {
            name: "READY"
            when: engine.snapshotLoaded
            StateChangeScript {
                name: "myScript"
                script: machine.submitEvent("SnapshotLoaded")
            }
        }
    }
}
