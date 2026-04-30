import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 800
    height: 800
    title: qsTr("ROI Magnifier")

    property bool imageLoaded: false // Track if the image is loaded
    property real zoomLevel: 2.0 // Initial zoom level for magnification
    property bool roiMagnifyEnabled: false // Track if ROI magnifying is enabled

    // Main image container
    Rectangle {
        id: imageContainer
        width: 600
        height: 600
        color: "lightgray"
        anchors.centerIn: parent

        // Main image
        Image {
            id: mainImage
            anchors.fill: parent
            anchors.margins: 10
            source: imageLoaded ? "image://imageProvider/main" : ""
            smooth: true
            visible: imageLoaded
            fillMode: Image.PreserveAspectFit

            // Debug: Display main image coordinates
            onXChanged: console.log("Main Image X:", x, "Y:", y, "Width:", width, "Height:", height);
        }

        // ROI Rectangle (movable and resizable)
        Rectangle {
            id: roiRect
            width: 100
            height: 100
            color: "transparent"
            border.color: "blue"
            border.width: 2
            visible: roiMagnifyEnabled

            // Ensure the ROI rectangle is rendered above the main image
            z: 1

            // MouseArea for dragging the ROI
            MouseArea {
                id: dragArea
                anchors.fill: parent
                drag.target: parent
                drag.axis: Drag.XAndYAxis
                drag.minimumX: 0
                drag.maximumX: imageContainer.width - roiRect.width
                drag.minimumY: 0
                drag.maximumY: imageContainer.height - roiRect.height
                onPositionChanged: updateMagnifiedROI() // Call this when the rectangle is moved
            }

            // Resize handles
            Rectangle {
                id: resizeHandle
                width: 10
                height: 10
                color: "blue"
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.SizeFDiagCursor
                    onPressed: resizeHandle.visible = true
                    onReleased: resizeHandle.visible = false
                    onPositionChanged: {
                        if (pressed) {
                            roiRect.width = Math.max(50, roiRect.width + mouseX)
                            roiRect.height = Math.max(50, roiRect.height + mouseY)
                            updateMagnifiedROI() // Call this when the rectangle is resized
                        }
                    }
                }
            }

            // Magnified ROI image (inside the ROI rectangle)
            Image {
                id: magnifiedImage
                anchors.fill: parent
                source: roiMagnifyEnabled ? "image://imageProvider/roi" : ""
                fillMode: Image.PreserveAspectFit
                smooth: true
                visible: roiMagnifyEnabled
            }
        }
    }

    // Control buttons
    Row {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        // Button to load the image
        Button {
            text: "Load Image"
            onClicked: {
                imageLoaded = true
            }
        }

        // Button to enable/disable ROI magnifying
        Button {
            text: roiMagnifyEnabled ? "Disable ROI Magnify" : "Enable ROI Magnify"
            onClicked: {
                roiMagnifyEnabled = !roiMagnifyEnabled
                if (roiMagnifyEnabled) {
                    updateMagnifiedROI()
                } else {
                    magnifiedImage.source = ""
                }
            }
        }

        // Button to zoom in
        Button {
            text: "Zoom In"
            onClicked: {
                zoomLevel += 0.5
                updateMagnifiedROI()
            }
        }

        // Button to zoom out
        Button {
            text: "Zoom Out"
            onClicked: {
                zoomLevel = Math.max(1.0, zoomLevel - 0.5)
                updateMagnifiedROI()
            }
        }
    }

    // Function to update the magnified ROI
    function updateMagnifiedROI() {
        if (roiMagnifyEnabled) {
            // Calculate scaling factor
            var scaleX = mainImage.sourceSize.width / mainImage.width;
            var scaleY = mainImage.sourceSize.height / mainImage.height;

            // Calculate the center of the ROI rectangle
            var roiCenterX = roiRect.x + roiRect.width / 2;
            var roiCenterY = roiRect.y + roiRect.height / 2;

            // Adjust ROI coordinates relative to the main image
            var roiX = (roiCenterX - mainImage.x - (roiRect.width / 2 / zoomLevel)) * scaleX;
            var roiY = (roiCenterY - mainImage.y - (roiRect.height / 2 / zoomLevel)) * scaleY;
            var roiWidth = roiRect.width * scaleX / zoomLevel;
            var roiHeight = roiRect.height * scaleY / zoomLevel;

            // Debug output
            console.log("ROI Rectangle Center - X:", roiCenterX, "Y:", roiCenterY);
            console.log("Adjusted ROI Coordinates - X:", roiX, "Y:", roiY, "Width:", roiWidth, "Height:", roiHeight);
            console.log("zoomLevel:", zoomLevel);

            // Update the ROI image in the ImageMagnifier
            imageMagnifier.updateROI(roiX, roiY, roiWidth, roiHeight, zoomLevel);

            // Force the magnifiedImage to reload the ROI image
            magnifiedImage.source = "";
            magnifiedImage.source = "image://imageProvider/roi";
        }
    }

    // Connections to listen for the roiImageUpdated signal
    Connections {
        target: imageMagnifier
        onRoiImageUpdated: {
            console.log("ROI image updated!");
            // Force the magnifiedImage to reload the ROI image
            magnifiedImage.source = "";
            magnifiedImage.source = "image://imageProvider/roi";
        }
    }
}
