import QtQuick 2.14
import QtQuick.Window 2.14
import VTK 1.0

Window {
    width: 900
    height: 700
    visible: true
    title: "VTK Volume Rendering - QML"

    VolumeView {
        id: volume
        anchors.fill: parent

        Component.onCompleted: {
            volume.loadVTI("D:/C-Arm/volume/cbct.vti")
        }
    }

}
