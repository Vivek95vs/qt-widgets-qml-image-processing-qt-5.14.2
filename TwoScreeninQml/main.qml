import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.12

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: "Main Window"

    // These flags help with fullscreen stability
    flags: Qt.Window | Qt.FramelessWindowHint

    Component.onCompleted: {
        showFullScreen()
    }

    // Handle visibility changes to maintain fullscreen
    onVisibilityChanged: {
        if (visibility !== Window.FullScreen) {
            visibility = Window.FullScreen
        }
    }

    // Optionally handle escape key to exit
    Shortcut {
        sequence: "Esc"
        onActivated: Qt.quit()
    }

    Loader {
        id: screenLoader
        anchors.fill: parent
        source: "Screen1.qml"
    }
}
