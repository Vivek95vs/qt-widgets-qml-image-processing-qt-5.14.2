import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Window 2.14

ApplicationWindow {
    id: window
    visible: true
    width: 1200
    height: 800
    title: "Medical Volume Viewer - MPR"
    color: "#1a1a2e"

    // Header
    Rectangle {
        id: header
        width: parent.width
        height: 70
        color: "#16213e"

        RowLayout {
            anchors.fill: parent
            anchors.margins: 15

            Text {
                text: "MEDICAL VOLUME VIEWER"
                color: "#ffffff"
                font.bold: true
                font.pixelSize: 22
                Layout.alignment: Qt.AlignVCenter
            }

            Item { Layout.fillWidth: true }

            Button {
                id: loadButton
                text: "📂 Load DICOM Volume"
                font.pixelSize: 14
                padding: 12
                background: Rectangle {
                    color: loadButton.down ? "#0f3460" :
                           (loadButton.hovered ? "#1e4d8c" : "#2d6bc6")
                    radius: 6
                }
                contentItem: Text {
                    text: loadButton.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: dicomHandler.loadDicomSeries()
            }
        }
    }

    // Main content - MPR View only
    Rectangle {
        id: mainContent
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "transparent"

        GridLayout {
            anchors.fill: parent
            anchors.margins: 15
            columns: 2
            rows: 2

            // Axial View
            ViewPanel {
                id: axialView
                Layout.fillWidth: true
                Layout.fillHeight: true
                viewName: "AXIAL VIEW"
                sliceIndex: dicomHandler.currentAxialSlice
                maxSlices: dicomHandler.volumeLoaded ? dicomHandler.volumeDimensions.z - 1 : 0
                imageSource: dicomHandler.volumeLoaded ? "image://axial/" + Date.now() : ""

                onSliceChanged: {
                    dicomHandler.setCurrentAxialSlice(sliceIndex)
                }

                Connections {
                    target: dicomHandler
                    onAxialImageChanged: {
                        axialView.imageSource = ""
                        axialView.imageSource = "image://axial/" + Date.now()
                    }
                    onCurrentAxialSliceChanged: {
                        axialView.sliceIndex = dicomHandler.currentAxialSlice
                    }
                    onVolumeLoaded: {
                        axialView.maxSlices = dicomHandler.volumeDimensions.z - 1
                        axialView.sliceIndex = dicomHandler.currentAxialSlice
                    }
                }
            }

            // Coronal View
            ViewPanel {
                id: coronalView
                Layout.fillWidth: true
                Layout.fillHeight: true
                viewName: "CORONAL VIEW"
                sliceIndex: dicomHandler.currentCoronalSlice
                maxSlices: dicomHandler.volumeLoaded ? dicomHandler.volumeDimensions.y - 1 : 0
                imageSource: dicomHandler.volumeLoaded ? "image://coronal/" + Date.now() : ""

                onSliceChanged: {
                    dicomHandler.setCurrentCoronalSlice(sliceIndex)
                }

                Connections {
                    target: dicomHandler
                    onCoronalImageChanged: {
                        coronalView.imageSource = ""
                        coronalView.imageSource = "image://coronal/" + Date.now()
                    }
                    onCurrentCoronalSliceChanged: {
                        coronalView.sliceIndex = dicomHandler.currentCoronalSlice
                    }
                    onVolumeLoaded: {
                        coronalView.maxSlices = dicomHandler.volumeDimensions.y - 1
                        coronalView.sliceIndex = dicomHandler.currentCoronalSlice
                    }
                }
            }

            // Sagittal View
            ViewPanel {
                id: sagittalView
                Layout.fillWidth: true
                Layout.fillHeight: true
                viewName: "SAGITTAL VIEW"
                sliceIndex: dicomHandler.currentSagittalSlice
                maxSlices: dicomHandler.volumeLoaded ? dicomHandler.volumeDimensions.x - 1 : 0
                imageSource: dicomHandler.volumeLoaded ? "image://sagittal/" + Date.now() : ""

                onSliceChanged: {
                    dicomHandler.setCurrentSagittalSlice(sliceIndex)
                }

                Connections {
                    target: dicomHandler
                    onSagittalImageChanged: {
                        sagittalView.imageSource = ""
                        sagittalView.imageSource = "image://sagittal/" + Date.now()
                    }
                    onCurrentSagittalSliceChanged: {
                        sagittalView.sliceIndex = dicomHandler.currentSagittalSlice
                    }
                    onVolumeLoaded: {
                        sagittalView.maxSlices = dicomHandler.volumeDimensions.x - 1
                        sagittalView.sliceIndex = dicomHandler.currentSagittalSlice
                    }
                }
            }

            // Control Panel
            ControlPanel {
                id: controlPanel
                Layout.fillWidth: true
                Layout.fillHeight: true
                dicomHandler: dicomHandler
            }
        }
    }

    // Status Bar
    Rectangle {
        id: statusBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: 40
        color: "#0f3460"

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10

            Text {
                id: statusText
                color: "#90cdf4"
                font.pixelSize: 12
                text: dicomHandler.volumeLoaded ?
                      "Volume: " + dicomHandler.volumeDimensions.x + "×" +
                      dicomHandler.volumeDimensions.y + "×" +
                      dicomHandler.volumeDimensions.z +
                      " (" + dicomHandler.sliceCount + " slices) | " +
                      "Spacing: " + dicomHandler.volumeSpacing.x.toFixed(2) + "×" +
                      dicomHandler.volumeSpacing.y.toFixed(2) + "×" +
                      dicomHandler.volumeSpacing.z.toFixed(2) + " mm" :
                      "Ready to load DICOM volume"
            }

            Item { Layout.fillWidth: true }

            Text {
                color: "#a0aec0"
                font.pixelSize: 11
                text: "DICOM MPR Viewer v1.0"
            }
        }
    }
}
