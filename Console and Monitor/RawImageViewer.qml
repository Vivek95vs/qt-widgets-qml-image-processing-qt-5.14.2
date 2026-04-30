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
    property color currentColor: DrawingManager.currentColor
    property real currentLineWidth: DrawingManager.currentLineWidth

    // Image display info
    property rect imageBounds: Qt.rect(0, 0, 0, 0)

    // Drawing state
    property var currentDrawing: null
    property int drawingId: 0

    // Performance optimization
    property bool needsRepaint: false

    // Signal to notify about drawing updates
    signal drawingUpdated(var drawingData)

    // Function to receive drawing from other screen
    function receiveDrawing(drawingData) {
        if (drawingData.imageSource === imageSource) {
            // Check if this drawing already exists
            var found = false;
            for (var i = 0; i < drawings.length; i++) {
                if (drawings[i].drawingId === drawingData.drawingId) {
                    drawings[i] = drawingData;
                    found = true;
                    break;
                }
            }

            if (!found) {
                drawings.push(drawingData);
            }

            needsRepaint = true;
            repaintTimer.restart();
        }
    }

    // Function to update drawings from shared manager
    function updateDrawings() {
        if (imageSource) {
            drawings = DrawingManager.getDrawingsForCurrentImage();
            needsRepaint = true;
            repaintTimer.restart();
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
        console.log("Loading image:", filePath);
        DrawingManager.setImageInfo(filePath);
        updateDrawings();
    }

    // Function to get normalized coordinates relative to the image
    function getNormalizedPoint(mouseX, mouseY) {
        if (!displayedImage.visible || imageBounds.width === 0) {
            return Qt.point(mouseX / width, mouseY / height);
        }

        var imgX = (mouseX - imageBounds.x) / imageBounds.width;
        var imgY = (mouseY - imageBounds.y) / imageBounds.height;

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

        var scale = Math.min(targetWidth / sourceWidth, targetHeight / sourceHeight);
        var scaledWidth = sourceWidth * scale;
        var scaledHeight = sourceHeight * scale;

        var x = displayedImage.x + (targetWidth - scaledWidth) / 2;
        var y = displayedImage.y + (targetHeight - scaledHeight) / 2;

        imageBounds = Qt.rect(x, y, scaledWidth, scaledHeight);
    }

    onWidthChanged: calculateImageBounds()
    onHeightChanged: calculateImageBounds()

    Component.onCompleted: {
        currentColor = DrawingManager.currentColor;
        currentLineWidth = DrawingManager.currentLineWidth;
    }

    // Repaint timer for performance
    Timer {
        id: repaintTimer
        interval: 16 // ~60 FPS
        onTriggered: {
            if (needsRepaint) {
                canvas.requestPaint();
                needsRepaint = false;
            }
        }
    }

    // Timer for sending drawing updates
    Timer {
        id: sendTimer
        interval: 32 // ~30 FPS for network updates
        running: false
        repeat: false
        property var pendingData: null
        onTriggered: {
            if (pendingData) {
                drawingUpdated(pendingData);
                pendingData = null;
            }
        }
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
                    }
                }

                // Fallback text if no image
                Text {
                    anchors.centerIn: parent
                    text: "No image loaded\nClick 'Load Image' to load an image"
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

                // Performance optimizations
                renderTarget: Canvas.FramebufferObject
                renderStrategy: Canvas.Cooperative

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.clearRect(0, 0, width, height);

                    if (drawings.length === 0) return;

                    var bounds = imageBounds;

                    for (var i = 0; i < drawings.length; i++) {
                        var drawing = drawings[i];
                        if (!drawing.points || drawing.points.length < 2) continue;

                        ctx.beginPath();
                        ctx.strokeStyle = drawing.color;
                        ctx.lineWidth = drawing.width;
                        ctx.lineCap = "round";
                        ctx.lineJoin = "round";

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

                // Mouse area for drawing
                MouseArea {
                    id: drawingArea
                    anchors.fill: parent
                    enabled: isDrawing && imageSource !== ""
                    hoverEnabled: true

                    onPressed: {
                        if (isDrawing) {
                            drawingId++;
                            var normPoint = getNormalizedPoint(mouse.x, mouse.y);

                            currentDrawing = {
                                type: "freehand",
                                color: /*currentColor*/"white",
                                width: currentLineWidth,
                                points: [normPoint],
                                imageSource: imageSource,
                                drawingId: drawingId
                            };

                            drawings.push(currentDrawing);
                            canvas.requestPaint();
                        }
                    }

                    onPositionChanged: {
                        if (isDrawing && pressed && currentDrawing) {
                            var currentPoint = getNormalizedPoint(mouse.x, mouse.y);
                            currentDrawing.points.push(currentPoint);

                            // Update local display
                            canvas.requestPaint();

                            // Send update (throttled)
                            sendTimer.pendingData = currentDrawing;
                            if (!sendTimer.running) {
                                sendTimer.start();
                            }
                        }
                    }

                    onReleased: {
                        if (isDrawing && currentDrawing) {
                            // Send final drawing
                            DrawingManager.addDrawing(currentDrawing);
                            drawingUpdated(currentDrawing);
                            currentDrawing = null;
                        }
                    }
                }
            }
        }
    }

    // File dialog for loading images
    FileDialog {
        id: fileDialog
        title: "Select Image"
        nameFilters: ["Image files (*.png *.jpg *.bmp)"]
        onAccepted: {
            loadRawImage(fileDialog.file);
        }
    }

    // Color dialog for selecting drawing color
    ColorDialog {
        id: colorDialog
        title: "Select Drawing Color"
        onAccepted: {
            currentColor = /*colorDialog.color*/"white";
            DrawingManager.currentColor = /*colorDialog.color*/"white";
        }
    }

    // Connect to drawing manager updates
    Connections {
        target: DrawingManager
        onDrawingsUpdated: {
            updateDrawings();
        }
    }
}
