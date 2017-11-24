import QtQuick 2.7

Column {
    property string txt
    Text {
        width: 25
        text: txt
        font.pixelSize: 10
        horizontalAlignment: Text.AlignRight
    }
    HSeparator{}
}
