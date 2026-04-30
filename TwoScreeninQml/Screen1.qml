import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
    width: parent ? parent.width : 800
    height: parent ? parent.height : 600

    Button {
        text: "Go to Screen 2"
        anchors.centerIn: parent
        onClicked: {
            screenLoader.source = "Screen2.qml"
        }
    }
}
