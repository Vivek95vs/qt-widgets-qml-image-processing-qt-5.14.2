import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Window {
    id: screen1
//    visible: true
//    visibility: "FullScreen"
//    width: 800
//    height: 1200
//    minimumWidth: 800
//    minimumHeight: 1200
    color: "black"
    title: "PrimaryWindow"
    property var primaryLoader: primaryloader

    Loader{
        id: primaryloader
        anchors.fill: parent
        source: "PrimaryLoginScreen.qml"
    }

    // Global function to change source
        function changePrimarySource(newSource) {
             console.log("Changing primary source to:", newSource);
            primaryloader.source = newSource;
        }
}
