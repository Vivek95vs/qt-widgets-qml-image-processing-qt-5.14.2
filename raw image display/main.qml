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
            source: imageLoaded ? "image://rawImage" : ""
            fillMode: Image.PreserveAspectFit
            smooth: true
            visible: imageLoaded
        }

        // ROI Rectangle (movable and resizable)
        Rectangle {
            id: roiRect
            width: 100
            height: 100
            color: "transparent"
            border.color: "red"
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
                onPositionChanged: updateMagnifiedROI()
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
                    onMouseXChanged: {
                        roiRect.width = Math.max(50, roiRect.width + mouseX)
                        roiRect.height = Math.max(50, roiRect.height + mouseY)
                        updateMagnifiedROI()
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
                source: ""
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
            // Calculate the ROI coordinates relative to the main image
            var roiX = roiRect.x
            var roiY = roiRect.y
            var roiWidth = roiRect.width
            var roiHeight = roiRect.height

            // Update the magnified image source
            magnifiedImage.source = "image://rawImage/roi?" + roiX + "," + roiY + "," + roiWidth + "," + roiHeight
            console.log("ROI Updated:", roiX, roiY, roiWidth, roiHeight)
        }
    }
}
