import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Dialogs 1.3

ApplicationWindow {
    id: window
    width: 1400
    height: 900
    visible: true
    title: "DICOM 3D Viewer - VTK 8.2 / Qt 5.14.1"

    // Custom volume renderer
    DicomVolumeItem {
        id: volumeRenderer
        anchors.fill: parent
        anchors.margins: 10
    }

    // Control Panel
    Rectangle {
        id: controlPanel
        width: 300
        height: parent.height
        color: "#2c3e50"
        opacity: 0.9

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Label {
                text: "DICOM 3D Viewer"
                font.bold: true
                font.pointSize: 14
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            // File selection
            Button {
                text: "Open DICOM Folder"
                Layout.fillWidth: true
                onClicked: fileDialog.open()

                FileDialog {
                    id: fileDialog
                    title: "Select DICOM Folder"
                    selectFolder: true
                    onAccepted: {
                        volumeRenderer.dicomDirectory = fileDialog.folder
                    }
                }
            }

            // Separator
            Rectangle {
                height: 1
                Layout.fillWidth: true
                color: "gray"
            }

            // Window/Level controls
            Label {
                text: "Window/Level"
                color: "white"
                font.bold: true
            }

            RowLayout {
                Label {
                    text: "Window:"
                    color: "white"
                    Layout.preferredWidth: 80
                }
                Slider {
                    id: windowSlider
                    from: 100
                    to: 4000
                    value: 2000
                    stepSize: 10
                    Layout.fillWidth: true
                    onValueChanged: volumeRenderer.window = value
                }
                Label {
                    text: Math.round(windowSlider.value)
                    color: "white"
                    Layout.preferredWidth: 50
                }
            }

            RowLayout {
                Label {
                    text: "Level:"
                    color: "white"
                    Layout.preferredWidth: 80
                }
                Slider {
                    id: levelSlider
                    from: -1000
                    to: 3000
                    value: 500
                    stepSize: 10
                    Layout.fillWidth: true
                    onValueChanged: volumeRenderer.level = value
                }
                Label {
                    text: Math.round(levelSlider.value)
                    color: "white"
                    Layout.preferredWidth: 50
                }
            }

            // Opacity control
            Label {
                text: "Opacity"
                color: "white"
                font.bold: true
            }

            RowLayout {
                Slider {
                    id: opacitySlider
                    from: 0
                    to: 1
                    value: 1
                    stepSize: 0.01
                    Layout.fillWidth: true
                    onValueChanged: volumeRenderer.opacity = value
                }
                Label {
                    text: opacitySlider.value.toFixed(2)
                    color: "white"
                    Layout.preferredWidth: 50
                }
            }

            // Render mode
            Label {
                text: "Render Mode"
                color: "white"
                font.bold: true
            }

            ComboBox {
                id: renderModeCombo
                model: ["Composite", "Maximum IP", "Minimum IP"]
                Layout.fillWidth: true
                onCurrentIndexChanged: volumeRenderer.renderMode = currentIndex
            }

            // Camera controls
            Label {
                text: "Camera"
                color: "white"
                font.bold: true
            }

            Button {
                text: "Reset Camera"
                Layout.fillWidth: true
                onClicked: volumeRenderer.resetCamera()
            }

            // View controls
            RowLayout {
                Button {
                    text: "Axial"
                    Layout.fillWidth: true
                    onClicked: {
                        // Could implement view preset
                    }
                }
                Button {
                    text: "Coronal"
                    Layout.fillWidth: true
                    onClicked: {
                        // Could implement view preset
                    }
                }
                Button {
                    text: "Sagittal"
                    Layout.fillWidth: true
                    onClicked: {
                        // Could implement view preset
                    }
                }
            }

            // Status info
            Rectangle {
                height: 1
                Layout.fillWidth: true
                color: "gray"
            }

            Text {
                text: "Status: Ready"
                color: "lightgreen"
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }

            Item { Layout.fillHeight: true }
        }
    }

    // Status bar
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 30
        color: "#34495e"

        RowLayout {
            anchors.fill: parent
            anchors.margins: 5

            Label {
                text: "VTK 8.2 | Qt 5.14.1 | Visual Studio 2017"
                color: "white"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "FPS: 60"
                color: "white"
            }
        }
    }
}
