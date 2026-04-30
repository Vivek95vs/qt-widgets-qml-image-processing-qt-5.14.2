import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Window {
    id: window
    visible: true
    visibility: "FullScreen"
    minimumWidth: 1200
    minimumHeight: 800
    color: "#668080"
    title: qsTr("Hello World")
    Loader {
        id: mainLoader
        anchors.fill: parent
        source: "worklist.qml" // initial view
    }
}
