import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Key Movement Example"

    Rectangle {
        id: rootRect
        width: 400
        height: 300
        color: "lightgray"
        focus: true
        Keys.priority: Keys.BeforeItem

        property bool upPressed: false

        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Up && !event.isAutoRepeat && !upPressed) {
                upPressed = true
                moveTimer.start()
                console.log("Key Up Pressed - Start Moving")
                event.accepted = true
            }
        }

        Keys.onReleased: (event) => {
            if (event.key === Qt.Key_Up && !event.isAutoRepeat) {
                upPressed = false
                moveTimer.stop()
                console.log("Key Up Released------------------------------------")
                event.accepted = true
            }
        }

        Timer {
            id: moveTimer
            interval: 100
            repeat: true
            running: false
            onTriggered: {
                console.log("Key Up Pressed - Moving...")
            }
        }

        Component.onCompleted: rootRect.forceActiveFocus()
    }

}
