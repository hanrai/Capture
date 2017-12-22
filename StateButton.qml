import QtQuick 2.0
import QtQuick.Controls 2.2
import QtScxml 5.8

RoundButton {
    id: button
    width: radius*2
    height: radius*2
    opacity: 0.1
    enabled: false

    property string source
    property string name

    anchors.verticalCenter: parent.verticalCenter

    Image{
        source: button.source
        width: button.width * 0.6
        height: button.height * 0.6
        anchors.centerIn: parent;
    }

    states: [
        State {
            name: "ENABLED"
            when: machine.Ready
            PropertyChanges { target: button; opacity: 0.8 }
        }
    ]

    transitions: Transition {
        from: ""; to: "ENABLED"; reversible: true
        NumberAnimation { properties: "opacity"; duration: 100}
    }

    onClicked: {
        machine.submitEvent("Button.Clicked",
                                 {"name": name})
        console.log(engine.action);
    }
}
