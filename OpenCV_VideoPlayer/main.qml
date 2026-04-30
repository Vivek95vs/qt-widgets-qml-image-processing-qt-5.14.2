import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls.Material 2.12

Window {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("OpenCV Video Player")
    Material.theme: Material.Dark

    HomePage{
        id: homePage
        anchors.fill: parent
    }
}
