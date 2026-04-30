import QtQuick 2.14
import QtQuick.Controls 2.14
import VTK 9.2

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "VTK 9.2 QML Volume Render"

    // 1. Backend: This is a non-visual component that manages the OpenGL context
    VTKRenderWindow {
        id: vtkWindow
    }

    // 2. Frontend: This is the actual visual rectangle where the 3D shows up
    VTKRenderItem {
        id: vtkView
        anchors.fill: parent
        renderWindow: vtkWindow // Link to the backend object above

        Component.onCompleted: {
            dicomController.setRenderer(vtkView)
        }
    }

    Button {
        text: "Start Render"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: dicomController.startRender("D:/DICOM_SERIES")
    }


    Text {
        text: "Files: " + dicomController.fileCount
        color: "red"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        font.pixelSize: 20
    }

}
