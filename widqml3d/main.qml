import QtQuick 2.14
import QtQuick.Controls 2.14

ApplicationWindow {
    width: 900
    height: 900
    visible: true
    title: "VTK Render Viewer"

    Column {
        anchors.centerIn: parent
        spacing: 10

        Button {
            text: "Render 3D"
            onClicked: vtkRenderer.renderAndSave()
        }

        Image {
            width: 800
            height: 800
            fillMode: Image.PreserveAspectFit
            source: vtkRenderer.imagePath !== ""
                    ? "file:///" + vtkRenderer.imagePath
                    : ""
        }
    }
}
