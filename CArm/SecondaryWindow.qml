import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Window {
    id: screen2
//    visible: true
//    visibility: "FullScreen"
//    width: 800
//    height: 1200
//    minimumWidth: 800
//    minimumHeight: 1200
    color: "black"
    title: "SecondaryWindow"
    property var secondaryLoader: secondaryloader

    Loader{
        id: secondaryloader
        anchors.fill: parent
        source: "SecondaryLoginScreen.qml"
    }
}
