import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import Medical3D 1.0

Item {
    id: root
    anchors.fill: parent

    // Rotation properties
    property real rotationX: 0
    property real rotationY: 0
    property real zoom: 1.0

    // Background
    Rectangle {
        anchors.fill: parent
        color: "#0f3460"
    }

    // Volume renderer using custom OpenGL
    VolumeTexture {
        id: volumeRenderer
        anchors.fill: parent
        volumeData: dicomHandler.volumeData
        dimensions: dicomHandler.volumeDimensions
        stepSize: stepSizeSlider.value
        brightness: brightnessSlider.value
        contrast: contrastSlider.value
        opacity: opacitySlider.value

        // Apply 3D transformations
        transform: [
            Scale {
                origin.x: root.width / 2
                origin.y: root.height / 2
                xScale: root.zoom
                yScale: root.zoom
            },
            Rotation {
                origin.x: root.width / 2
                origin.y: root.height / 2
                axis { x: 1; y: 0; z: 0 }
                angle: root.rotationX
            },
            Rotation {
                origin.x: root.width / 2
                origin.y: root.height / 2
                axis { x: 0; y: 1; z: 0 }
                angle: root.rotationY
            }
        ]
    }

    // Controls overlay
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 140
        color: "#80000000"
        opacity: 0.8

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 5

            // Step size control
            RowLayout {
                Layout.fillWidth: true

                Label {
                    text: "Step Size:"
                    color: "white"
                    font.pixelSize: 12
                    Layout.preferredWidth: 100
                }

                Slider {
                    id: stepSizeSlider
                    Layout.fillWidth: true
                    from: 0.0001
                    to: 0.01
                    value: 0.001
                    stepSize: 0.0001
                }

                Label {
                    text: stepSizeSlider.value.toFixed(4)
                    color: "white"
                    font.pixelSize: 12
                    Layout.preferredWidth: 60
                }
            }

            // Brightness control
            RowLayout {
                Layout.fillWidth: true

                Label {
                    text: "Brightness:"
                    color: "white"
                    font.pixelSize: 12
                    Layout.preferredWidth: 100
                }

                Slider {
                    id: brightnessSlider
                    Layout.fillWidth: true
                    from: 0.1
                    to: 3.0
                    value: 1.0
                }

                Label {
                    text: brightnessSlider.value.toFixed(2)
                    color: "white"
                    font.pixelSize: 12
                    Layout.preferredWidth: 60
                }
            }

            // Contrast control
            RowLayout {
                Layout.fillWidth: true

                Label {
                    text: "Contrast:"
                    color: "white"
                    font.pixelSize: 12
                    Layout.preferredWidth: 100
                }

                Slider {
                    id: contrastSlider
                    Layout.fillWidth: true
                    from: 0.1
                    to: 3.0
                    value: 1.0
                }

                Label {
                    text: contrastSlider.value.toFixed(2)
                    color: "white"
                    font.pixelSize: 12
                    Layout.preferredWidth: 60
                }
            }

            // Opacity control
            RowLayout {
                Layout.fillWidth: true

                Label {
                    text: "Opacity:"
                    color: "white"
                    font.pixelSize: 12
                    Layout.preferredWidth: 100
                }

                Slider {
                    id: opacitySlider
                    Layout.fillWidth: true
                    from: 0.0
                    to: 1.0
                    value: 0.8
                }

                Label {
                    text: opacitySlider.value.toFixed(2)
                    color: "white"
                    font.pixelSize: 12
                    Layout.preferredWidth: 60
                }
            }
        }
    }

    // Top info panel
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        width: 250
        height: 100
        color: "#80000000"
        radius: 5
        opacity: 0.8

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 5

            Text {
                text: "Volume Dimensions:"
                color: "white"
                font.pixelSize: 12
                font.bold: true
            }

            Text {
                text: {
                    var dim = dicomHandler.volumeDimensions;
                    if (dim.x === 0) return "No volume loaded";
                    return dim.x.toFixed(0) + " × " +
                           dim.y.toFixed(0) + " × " +
                           dim.z.toFixed(0);
                }
                color: "#e0e0e0"
                font.pixelSize: 11
            }

            Text {
                text: {
                    var dataSize = dicomHandler.volumeData.length;
                    if (dataSize === 0) return "";
                    return "Data: " + (dataSize / 1024 / 1024).toFixed(2) + " MB";
                }
                color: "#e0e0e0"
                font.pixelSize: 11
            }

            Text {
                text: "Zoom: " + root.zoom.toFixed(2) + "x"
                color: "#e0e0e0"
                font.pixelSize: 11
            }
        }
    }

    // Mouse interaction for rotation and zoom
    MouseArea {
        id: interactionArea
        anchors.fill: parent
        hoverEnabled: true
        property point lastMousePos

        onPressed: {
            lastMousePos = Qt.point(mouse.x, mouse.y);
        }

        onPositionChanged: {
            if (pressed) {
                var dx = mouse.x - lastMousePos.x;
                var dy = mouse.y - lastMousePos.y;

                // Update rotation
                root.rotationX += dy * 0.5;
                root.rotationY += dx * 0.5;

                // Normalize angles
                root.rotationX = root.rotationX % 360;
                root.rotationY = root.rotationY % 360;

                lastMousePos = Qt.point(mouse.x, mouse.y);
            }
        }

        onWheel: {
            // Zoom in/out
            var zoomFactor = 1.0 + wheel.angleDelta.y * 0.001;
            root.zoom = Math.max(0.1, Math.min(10.0, root.zoom * zoomFactor));
        }

        // Double click to reset view
        onDoubleClicked: {
            root.rotationX = 0;
            root.rotationY = 0;
            root.zoom = 1.0;
        }
    }

    // Reset view button
    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        width: 40
        height: 40
        color: "#80000000"
        radius: 20
        opacity: 0.8

        Text {
            anchors.centerIn: parent
            text: "↻"
            color: "white"
            font.pixelSize: 20
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.rotationX = 0;
                root.rotationY = 0;
                root.zoom = 1.0;
            }
        }
    }

    // Connections to update when volume loads
    Connections {
        target: dicomHandler

        onVolumeLoaded: {
            // Reset view when new volume loads
            root.rotationX = 0;
            root.rotationY = 0;
            root.zoom = 1.0;
        }

        onVolumeDataChanged: {
            console.log("Volume data updated, size:", dicomHandler.volumeData.length);
        }
    }
}
