import QtQuick.Controls 2.3
import QtQuick 2.7
import QtScxml 5.8

Dialog {
    title: "添加指标"
    modal: true
    visible: machine.AddSpot
    opacity: 0.8
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    Column{
        spacing: 20
        Row {
            Text {
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                text: "指标名："}

            TextField {
                id: nameField
                EventConnection {
                    stateMachine: machine
                    events: ["AddSpot.Duplicated"]
                    onOccurred: nameField.selectAll()
                }
            }
        }

        Label {
            color: "red"
            text: "错误：指标名重复"
            visible: machine.AddSpotDuplicated
        }

        DialogButtonBox {
            width: parent.width
            background: Rectangle {
                    color: "#00000000"
                    border.color: "darkGray"
                    radius: 1
            }

            Button {
                text: qsTr("添加")
                DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                enabled: nameField.length>0
                onClicked: machine.submitEvent("AddSpot.Ok",
                                               {"name": nameField.text})
            }

            Button {
                text: qsTr("取消")
                DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
                onClicked: close()
            }
        }
    }

    onClosed: {
        machine.submitEvent("AddSpot.Done")
    }
}
