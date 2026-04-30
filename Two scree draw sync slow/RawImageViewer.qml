import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import Qt.labs.platform 1.1
import DrawingManager 1.0

Item {
    id: imageViewer
    property string imageSource: ""
    property var drawings: []
    property bool isDrawing: false
    property point lastPoint
    property color currentColor: DrawingManager.currentColor
    property real currentLineWidth: DrawingManager.currentLineWidth

    // Image display info
    property rect imageBounds: Qt.rect(0, 0, 0, 0)
    property real scaleFactor: 1.0

    // Signal to notify about drawing updates
    signal drawingUpdated(var drawingData)

    // Function to receive drawing from other screen
    function receiveDrawing(drawingData) {
        console.log("Received drawing data for image:", drawingData.imageSource);

        // Only add if it's for the current image
        if (drawingData.imageSource === imageSource) {
            // Check if this drawing already exists
            var exists = false;
            for (var i = 0; i < drawings.length; i++) {
                if (JSON.stringify(drawings[i]) === JSON.stringify(drawingData)) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                drawings.push(drawingData);
                canvas.requestPaint();
            }
        }
    }

    // Function to update drawings from shared manager
    function updateDrawings() {
        if (imageSource) {
            drawings = DrawingManager.getDrawingsForCurrentImage();
            canvas.requestPaint();
        }
    }

    // Function to clear all drawings
    function clearDrawings() {
        drawings = [];
        canvas.requestPaint();
    }

    // Function to load raw image
    function loadRawImage(filePath) {
        imageSource = filePath;
        console.log("Loading raw image:", filePath);

        // Update drawing manager with current image
        DrawingManager.setImageInfo(filePath);

        // Update drawings for this image
        updateDrawings();
    }

    // Function to get normalized coordinates relative to the image
    function getNormalizedPoint(mouseX, mouseY) {
        if (!displayedImage.visible || imageBounds.width === 0) {
            return Qt.point(mouseX / width, mouseY / height);
        }

        // Convert mouse coordinates to image-relative coordinates
        var imgX = (mouseX - imageBounds.x) / imageBounds.width;
        var imgY = (mouseY - imageBounds.y) / imageBounds.height;

        // Clamp to [0, 1]
        imgX = Math.max(0, Math.min(1, imgX));
        imgY = Math.max(0, Math.min(1, imgY));

        return Qt.point(imgX, imgY);
    }

    // Function to calculate image bounds
    function calculateImageBounds() {
        if (!displayedImage.visible || displayedImage.sourceSize.width === 0) {
            imageBounds = Qt.rect(0, 0, width, height);
            return;
        }

        var sourceWidth = displayedImage.sourceSize.width;
        var sourceHeight = displayedImage.sourceSize.height;
        var targetWidth = displayedImage.width;
        var targetHeight = displayedImage.height;

        scaleFactor = Math.min(targetWidth / sourceWidth, targetHeight / sourceHeight);
        var scaledWidth = sourceWidth * scaleFactor;
        var scaledHeight = sourceHeight * scaleFactor;

        var x = displayedImage.x + (targetWidth - scaledWidth) / 2;
        var y = displayedImage.y + (targetHeight - scaledHeight) / 2;

        imageBounds = Qt.rect(x, y, scaledWidth, scaledHeight);
    }

    onWidthChanged: calculateImageBounds()
    onHeightChanged: calculateImageBounds()

    Component.onCompleted: {
        // Initialize with current settings from DrawingManager
        currentColor = DrawingManager.currentColor;
        currentLineWidth = DrawingManager.currentLineWidth;
    }

    Rectangle {
        anchors.fill: parent
        color: "#2c3e50"

        // Toolbar
        Rectangle {
            id: toolbar
            width: parent.width
            height: 50
            color: "#34495e"

            Row {
                anchors.centerIn: parent
                spacing: 10

                Button {
                    text: "Load Image"
                    onClicked: fileDialog.open()
                }

                Button {
                    text: isDrawing ? "Drawing ✓" : "Draw"
                    checked: isDrawing
                    checkable: true
                    onToggled: isDrawing = checked
                }

                Button {
                    text: "Clear"
                    onClicked: {
                        DrawingManager.clearDrawingsForImage(imageSource);
                        clearDrawings();
                    }
                }

                Rectangle {
                    width: 30
                    height: 30
                    color: currentColor
                    border.width: 1
                    border.color: "white"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: colorDialog.open()
                    }
                }

                Slider {
                    id: lineWidthSlider
                    from: 1
                    to: 10
                    value: currentLineWidth
                    onValueChanged: {
                        currentLineWidth = value;
                        DrawingManager.currentLineWidth = value;
                        DrawingManager.settingsChanged();
                    }
                }
            }
        }

        // Main content area
        Rectangle {
            id: contentArea
            anchors.top: toolbar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: "black"

            // Image display
            Image {
                id: displayedImage
                anchors.fill: parent
                source: imageSource
                fillMode: Image.PreserveAspectFit
                smooth: true
                visible: imageSource !== ""

                onStatusChanged: {
                    if (status === Image.Ready) {
                        calculateImageBounds();
                        console.log("Image loaded, bounds:", imageBounds);
                    }
                }

                // Fallback text if no image
                Text {
                    anchors.centerIn: parent
                    text: "No image loaded\nClick 'Load Image' to load a raw image"
                    color: "white"
                    font.pixelSize: 20
                    visible: !displayedImage.visible
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            // Canvas for drawing
            Canvas {
                id: canvas
                anchors.fill: parent

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.clearRect(0, 0, width, height);

                    // Use image bounds for proper positioning
                    var bounds = imageBounds;

                    // Draw all stored drawings
                    for (var i = 0; i < drawings.length; i++) {
                        var drawing = drawings[i];
                        ctx.beginPath();
                        ctx.strokeStyle = drawing.color;
                        ctx.lineWidth = drawing.width;
                        ctx.lineCap = "round";

                        if (drawing.type === "freehand" && drawing.points && drawing.points.length > 0) {
                            // Convert normalized coordinates to canvas coordinates
                            var firstPoint = drawing.points[0];
                            var canvasX = bounds.x + firstPoint.x * bounds.width;
                            var canvasY = bounds.y + firstPoint.y * bounds.height;
                            ctx.moveTo(canvasX, canvasY);

                            for (var j = 1; j < drawing.points.length; j++) {
                                var point = drawing.points[j];
                                canvasX = bounds.x + point.x * bounds.width;
                                canvasY = bounds.y + point.y * bounds.height;
                                ctx.lineTo(canvasX, canvasY);
                            }

                            ctx.stroke();
                        }
                    }
                }

                // Mouse area for drawing
                MouseArea {
                    id: drawingArea
                    anchors.fill: parent
                    enabled: isDrawing && imageSource !== ""
                    hoverEnabled: true

                    onPressed: {
                        if (isDrawing) {
                            var normPoint = getNormalizedPoint(mouse.x, mouse.y);
                            lastPoint = normPoint;
                            var newDrawing = {
                                type: "freehand",
                                color: currentColor,
                                width: currentLineWidth,
                                points: [normPoint],
                                imageSource: imageSource
                            };
                            drawings.push(newDrawing);

                            // Add to shared manager
                            DrawingManager.addDrawing(newDrawing);

                            // Send to other screen
                            drawingUpdated(newDrawing);

                            canvas.requestPaint();
                        }
                    }

                    onPositionChanged: {
                        if (isDrawing && pressed) {
                            var currentPoint = getNormalizedPoint(mouse.x, mouse.y);

                            // Update last drawing
                            if (drawings.length > 0) {
                                var lastDrawing = drawings[drawings.length - 1];
                                lastDrawing.points.push(currentPoint);

                                // Update in shared manager
                                DrawingManager.addDrawing(lastDrawing);

                                // Send update to other screen
                                drawingUpdated(lastDrawing);

                                canvas.requestPaint();
                            }
                        }
                    }

                    onReleased: {
                        if (isDrawing) {
                            console.log("Drawing completed with", drawings[drawings.length - 1].points.length, "points");
                        }
                    }
                }
            }
        }
    }

    // File dialog for loading images
    FileDialog {
        id: fileDialog
        title: "Select Raw Image"
        nameFilters: ["Image files (*.raw *.dcm *.png *.jpg *.bmp)"]
        onAccepted: {
            loadRawImage(fileDialog.file);
        }
    }

    // Color dialog for selecting drawing color
    ColorDialog {
        id: colorDialog
        title: "Select Drawing Color"
        onAccepted: {
            currentColor = colorDialog.color;
            DrawingManager.currentColor = colorDialog.color;
            DrawingManager.settingsChanged();
        }
    }

    // Connect to drawing manager settings changes
    Connections {
        target: DrawingManager
        onSettingsChanged: {
            currentColor = DrawingManager.currentColor;
            currentLineWidth = DrawingManager.currentLineWidth;
        }
    }
}
