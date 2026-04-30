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
        }

        // Magnified ROI (overlay on the main image)
        Rectangle {
            id: magnifiedROI
            width: roiRect.width * zoomLevel
            height: roiRect.height * zoomLevel
            color: "transparent"
            border.color: "blue"
            border.width: 2
            visible: roiMagnifyEnabled

            // Position the magnified ROI next to the ROI rectangle
            x: roiRect.x + roiRect.width + 10
            y: roiRect.y

            // Magnified image
            Image {
                id: magnifiedImage
                anchors.fill: parent
                source: roiMagnifyEnabled ? "image://imageProvider/roi" : ""
                fillMode: Image.PreserveAspectFit
                smooth: true
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

            // Adjust ROI coordinates
            var roiX = (roiRect.x - mainImage.x) * scaleX;
            var roiY = (roiRect.y - mainImage.y) * scaleY;
            var roiWidth = roiRect.width * scaleX;
            var roiHeight = roiRect.height * scaleY;

            // Debug output
            console.log("ROI Rectangle Coordinates - X:", roiRect.x, "Y:", roiRect.y, "Width:", roiRect.width, "Height:", roiRect.height);
            console.log("Main Image Coordinates - X:", mainImage.x, "Y:", mainImage.y, "Width:", mainImage.width, "Height:", mainImage.height);
            console.log("Adjusted ROI Coordinates - X:", roiX, "Y:", roiY, "Width:", roiWidth, "Height:", roiHeight);
            console.log("zoomLevel:", zoomLevel);

            // Update the ROI image in the ImageMagnifier
            imageMagnifier.updateROI(roiX, roiY, roiWidth, roiHeight, zoomLevel);
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
