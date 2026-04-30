import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import VTK 9.2

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "VTK 9.2 & Image Viewer"

    VTKRenderWindow {
        id: vtkWindow
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // LEFT SIDE
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // 3D VOLUME
            Rectangle {
                id: vtkContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"

                VTKRenderItem {
                    id: vtkView3D
                    anchors.fill: parent
                    renderWindow: vtkWindow
                    Component.onCompleted: dicomController.setRenderer(vtkWindow)
                }
                Button {
                    text: "Start Render"
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 15
                    onClicked: dicomController.startRender("D:/DICOM_SERIES")
                }

                Text {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 10
                    text: "3d rendering of the dicom .dcm"
                    color: "white"
                    font.pixelSize: 14
                }
            }

            // 2D IMAGE [secondary]
            Rectangle {
                id: imageContainerLeftBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#111111"
                clip: true

                Flickable {
                    id: flickLeft
                    anchors.fill: parent
                    boundsBehavior: Flickable.StopAtBounds

                    Image {
                        id: viewerLeft
                        width: imageContainerLeftBottom.width
                        height: imageContainerLeftBottom.height
                        fillMode: Image.PreserveAspectFit
                        source: "image://raw/dr_image"
                        cache: false
                        transformOrigin: Item.Center
                    }
                }
                Text {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 10
                    text: "2D Image"
                    color: "white"
                    font.pixelSize: 14
                }
            }
        }

        // RIGHT SIDE
        Rectangle {
            id: imageContainerRight
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#111111"
            clip: true

            Flickable {
                id: flickRight
                anchors.fill: parent
                boundsBehavior: Flickable.StopAtBounds

                Image {
                    id: viewerRight
                    width: imageContainerRight.width
                    height: imageContainerRight.height
                    fillMode: Image.PreserveAspectFit
                    source: "image://raw/dr_image"
                    cache: false
                    transformOrigin: Item.Center
                }
            }
            Text {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 10
                text: "2D Image"
                color: "white"
                font.pixelSize: 14
            }
        }
    }
}
