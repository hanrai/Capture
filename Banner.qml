import QtQuick 2.7
import QtQuick.Controls 2.2
import QtScxml 5.8

ToolBar {
    id: banner
    opacity: 0.8
    antialiasing: true

    background: Rectangle {
            color: "white"
            border.color: "darkGray"
            radius: 1
        }

    Row {
        leftPadding: 20
        spacing: 20
        anchors.fill: parent

        StateButton {
            id: capture
            radius: 35
            source: "qrc:/img/photo-camera.svg"
            enabled: true
            name: "SnapShot"

            onClicked: {
                lastVisibility = window.visibility
                window.visibility = ApplicationWindow.Minimized
                engine.capture()
            }

            states: [
                State {
                    name: "ENABLED"
                    when: machine.NoSnap
                    PropertyChanges { target: capture; opacity: 0.8 }
                }
            ]

            transitions: Transition {
                from: ""; to: "ENABLED"; reversible: true
                NumberAnimation { properties: "opacity"; duration: 100}
            }
        }

        VSeparator {}

        MousePositionSelector {
            enabled: machine.Ready
            name: "鼠标位置"
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/tag.png"
            name: "合约"
            spotX: contractSpot.spotX
            spotY: contractSpot.spotY
            spotWidth: contractSpot.spotWidth
            spotHeight: contractSpot.spotHeight
            colorName: contractSpot.colorName
            enabled: machine.Ready
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/calendar.png"
            name: "日期"
            spotX: dateSpot.spotX
            spotY: dateSpot.spotY
            spotWidth: dateSpot.spotWidth
            spotHeight: dateSpot.spotHeight
            colorName: dateSpot.colorName
            enabled: machine.Ready || (machine.HotSpotScan && hotSpot.name == name)
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/clock.png"
            name: "时间"
            spotX: timeSpot.spotX
            spotY: timeSpot.spotY
            spotWidth: timeSpot.spotWidth
            spotHeight: timeSpot.spotHeight
            colorName: timeSpot.colorName
            enabled: machine.Ready || (machine.HotSpotScan && hotSpot.name == name)
        }

        VSeparator {}

        DataDelegate {
            titleIcon: "qrc:/img/dollar.png"
            name: "点位"
            enabled: machine.Ready
        }

        VSeparator {}
    }

}
