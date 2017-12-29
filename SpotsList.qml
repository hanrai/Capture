import QtQuick 2.7
import QtQuick.Controls 2.3
import Qt.labs.settings 1.0
import QtScxml 5.8

Page {
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.top: banner.bottom
    z: banner.z
    opacity: 0.8
    width: 170

    background: Rectangle {
        color: "white"
        border.color: "darkGray"
        radius: 1
    }

    ListView {
        id:listView

        Component{
            id: dataDelegate
            DataDelegate {
                name: display
                titleIcon: "qrc:/img/line-chart.svg"
                background: Rectangle {
                    color: "#00000000"
                    border.color: "darkGray"
                    radius: 1
                }
            }
        }

        model: optionalModel
        delegate: Loader{sourceComponent:dataDelegate}
    }

    footer: ToolBar{
        background: Rectangle {
            color: "#00000000"
            border.color: "darkGray"
            radius: 1
        }
        Row{
            padding: 8
            anchors.fill: parent
            StateButton {
                radius: 24
                source: "qrc:/img/add.svg"
                name: "AddSpot"
                enabled: machine.Ready
            }
        }
    }

}
