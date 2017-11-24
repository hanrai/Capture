import QtQuick 2.7
import QtQuick.Controls 2.2

Pane {
    property string labelText: "X:"
    property string labelValue: "8888"
    property int labelWidth: 30
    padding: 0
    Row {
        Text {
            width: labelWidth
            leftPadding: 10
            font.pixelSize: 12
            text: labelText
            color: "darkGray"
        }
        Column {
            Text {
                width: 30
                font.pixelSize: 10
                leftPadding: 4
                rightPadding: 4
                horizontalAlignment: Text.AlignRight
                text: labelValue
            }
            Rectangle {
                height: 1
                width: parent.width
                color: "lightGray"
            }
        }
    }
}
