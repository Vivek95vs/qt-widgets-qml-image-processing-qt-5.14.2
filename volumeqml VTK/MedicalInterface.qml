import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

// Advanced medical interface with multiple viewports
Rectangle {
    id: medicalInterface
    color: "#1a1a2e"

    property int currentLayout: 0 // 0: Single, 1: 2x2, 2: 3D+2D

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: currentLayout === 1 ? 2 : 1
        rows: currentLayout === 1 ? 2 : 1

        // Viewport 1 (Always visible)
        Viewport {
            id: viewport1
            title: "Axial View"
            Layout.fillWidth: true
            Layout.fillHeight: true
            sliceIndex: sliceSlider.value
            orientation: 0 // Axial
        }

        // Viewport 2 (Visible in 2x2 layout)
        Viewport {
            id: viewport2
            title: "Coronal View"
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: currentLayout === 1
            sliceIndex: sliceSlider.value * 0.5
            orientation: 1 // Coronal
        }

        // Viewport 3 (Visible in 2x2 layout)
        Viewport {
            id: viewport3
            title: "Sagittal View"
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: currentLayout === 1
            sliceIndex: sliceSlider.value * 0.5
            orientation: 2 // Sagittal
        }

        // Viewport 4 (3D volume)
        Viewport3D {
            id: viewport4
            title: "3D Volume"
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: currentLayout === 1 && vtkRenderer.volumeLoaded
        }
    }

    // Layout selector
    Row {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 5

        Button {
            text: "1x"
            onClicked: medicalInterface.currentLayout = 0
            highlighted: medicalInterface.currentLayout === 0
        }

        Button {
            text: "2x2"
            onClicked: medicalInterface.currentLayout = 1
            highlighted: medicalInterface.currentLayout === 1
        }

        Button {
            text: "3D"
            onClicked: medicalInterface.currentLayout = 2
            highlighted: medicalInterface.currentLayout === 2
            visible: vtkRenderer.volumeLoaded
        }
    }
}

// Viewport component
component Viewport: Rectangle {
    property string title
    property real sliceIndex
    property int orientation

    color: "#2d2d3d"
    radius: 6

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Text {
            text: title
            color: "white"
            font.pixelSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"
            radius: 4

            Image {
                anchors.fill: parent
                anchors.margins: 2
                source: "image://dicom/slice_" + sliceIndex + "_" + orientation
                fillMode: Image.PreserveAspectFit
            }
        }
    }
}

// 3D Viewport component
component Viewport3D: Rectangle {
    property string title

    color: "#2d2d3d"
    radius: 6

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Text {
            text: title
            color: "white"
            font.pixelSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"
            radius: 4

            VtkRenderer {
                anchors.fill: parent
                anchors.margins: 2
            }
        }
    }
}
