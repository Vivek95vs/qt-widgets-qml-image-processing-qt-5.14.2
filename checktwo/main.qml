// File name: main.qml (instead of MainWindow.qml)
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

ApplicationWindow {
    id: mainWindow
    width: 1920
    height: 1200
    visible: true
    title: "Primary Window - Drawing Application"
    color: "#1e1e1e"

    property color currentDrawColor: "red"
    property int currentLineWidth: 3
    property bool isDrawing: false
    property point lastPoint: Qt.point(0, 0)

    // File dialog for loading images
    FileDialog {
        id: fileDialog
        title: "Select an image file"
        folder: "D:/"
        nameFilters: ["Image files (*.png *.jpg *.jpeg *.bmp *.gif)"]
        onAccepted: {
            drawingData.imageSource = fileDialog.fileUrl
        }
    }

    // Try to load image from D drive on startup
    Component.onCompleted: {
        console.log("Application started");

        // Try common image paths on D drive
        var testPaths = [
            "file:///D:/test.png",
            "file:///D:/image.png",
            "file:///D:/images/test.png",
            "file:///D:/Pictures/test.png",
            "file:///D:/sample.png"
        ];

        for (var i = 0; i < testPaths.length; i++) {
            console.log("Trying to load:", testPaths[i]);
            // Check if file exists
            var xhr = new XMLHttpRequest();
            xhr.open("GET", testPaths[i], false);
            xhr.send();
            if (xhr.status !== 404) {
                drawingData.imageSource = testPaths[i];
                console.log("Loaded image from:", testPaths[i]);
                break;
            }
        }

        // Create secondary window after a short delay
        timer.start();
    }

    Timer {
        id: timer
        interval: 1000
        onTriggered: windowManager.createSecondaryWindow()
    }

    // Top control panel
    Rectangle {
        id: controlPanel
        width: parent.width
        height: 80
        color: "#2d2d2d"

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 15

            // Load image button
            Button {
                text: "Load Image from D Drive"
                Layout.preferredWidth: 200
                onClicked: fileDialog.open()
            }

            // Clear drawings button
            Button {
                text: "Clear Drawings"
                Layout.preferredWidth: 150
                onClicked: drawingData.clearDrawings()
            }

            // Color selection
            Label {
                text: "Color:"
                color: "white"
                Layout.alignment: Qt.AlignVCenter
            }

            Row {
                spacing: 5
                Layout.alignment: Qt.AlignVCenter

                Repeater {
                    model: ["red", "blue", "green", "yellow", "white", "black"]

                    Rectangle {
                        width: 30
                        height: 30
                        radius: 15
                        color: modelData
                        border.width: currentDrawColor === modelData ? 3 : 1
                        border.color: "white"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: currentDrawColor = modelData
                        }
                    }
                }
            }

            // Line width selection
            Label {
                text: "Width:"
                color: "white"
                Layout.alignment: Qt.AlignVCenter
            }

            ComboBox {
                model: [1, 2, 3, 5, 8, 10]
                currentIndex: 2
                Layout.preferredWidth: 80
                onCurrentTextChanged: currentLineWidth = parseInt(currentText)
            }

            // Image info
            Label {
                text: drawingData.imageLoaded ?
                      "Image: " + drawingData.originalImageSize.width + "x" + drawingData.originalImageSize.height :
                      "No image loaded"
                color: drawingData.imageLoaded ? "lightgreen" : "orange"
                Layout.fillWidth: true
            }

            // Window info
            Label {
                text: "Window: " + mainWindow.width + "x" + mainWindow.height
                color: "white"
            }
        }
    }

    // Main content area
    Rectangle {
        id: contentArea
        anchors.top: controlPanel.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        color: "black"

        // Image display container
        Item {
            id: imageContainer
            anchors.centerIn: parent
            width: Math.min(parent.width - 40, parent.height - 40)
            height: width * (drawingData.originalImageSize.height / Math.max(1, drawingData.originalImageSize.width))

            visible: drawingData.imageLoaded

            // Background pattern for empty areas
            Rectangle {
                anchors.fill: parent
                color: "#333333"
                visible: !drawingData.imageLoaded

                Text {
                    anchors.centerIn: parent
                    text: "No image loaded\nClick 'Load Image from D Drive'"
                    color: "white"
                    font.pixelSize: 24
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            // The actual image
            Image {
                id: displayImage
                anchors.fill: parent
                source: drawingData.imageLoaded ? drawingData.imageSource : ""
                fillMode: Image.PreserveAspectFit
                smooth: true
                mipmap: true
                cache: false

                onStatusChanged: {
                    if (status === Image.Error) {
                        console.log("Image loading error:", source);
                    }
                }

                onPaintedWidthChanged: drawingCanvas.requestPaint()
                onPaintedHeightChanged: drawingCanvas.requestPaint()
            }

            // Canvas for drawings
            Canvas {
                id: drawingCanvas
                anchors.fill: parent
                renderTarget: Canvas.FramebufferObject
                renderStrategy: Canvas.Cooperative

                // Convert image coordinates to canvas coordinates
                function toCanvasX(imageX) {
                    if (!drawingData.imageLoaded) return 0;
                    var scale = displayImage.paintedWidth / drawingData.originalImageSize.width;
                    var offset = (displayImage.width - displayImage.paintedWidth) / 2;
                    return offset + imageX * scale;
                }

                function toCanvasY(imageY) {
                    if (!drawingData.imageLoaded) return 0;
                    var scale = displayImage.paintedHeight / drawingData.originalImageSize.height;
                    var offset = (displayImage.height - displayImage.paintedHeight) / 2;
                    return offset + imageY * scale;
                }

                onPaint: {
                    if (!drawingData.imageLoaded) return;

                    var ctx = getContext("2d");
                    ctx.clearRect(0, 0, width, height);

                    var points = drawingData.getPoints();
                    if (points.length === 0) return;

                    ctx.lineWidth = currentLineWidth;
                    ctx.lineCap = "round";
                    ctx.lineJoin = "round";

                    var started = false;

                    for (var i = 0; i < points.length; i++) {
                        var point = points[i].first;
                        var color = points[i].second;

                        var x = toCanvasX(point.x);
                        var y = toCanvasY(point.y);

                        // Check if coordinates are valid
                        if (isNaN(x) || isNaN(y)) continue;

                        if (!started) {
                            ctx.beginPath();
                            ctx.moveTo(x, y);
                            started = true;
                        } else {
                            ctx.lineTo(x, y);
                        }

                        ctx.strokeStyle = color;
                        ctx.stroke();

                        // Start new path for next segment
                        ctx.beginPath();
                        ctx.moveTo(x, y);
                    }
                }

                Connections {
                    target: drawingData
                    onDrawingChanged: drawingCanvas.requestPaint()
                    onImageSourceChanged: drawingCanvas.requestPaint()
                    onOriginalImageSizeChanged: drawingCanvas.requestPaint()
                }
            }

            // Mouse area for drawing
            MouseArea {
                id: drawArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.CrossCursor

                // Convert mouse coordinates to image coordinates
                function toImageX(mouseX) {
                    if (!drawingData.imageLoaded) return 0;
                    var scale = displayImage.paintedWidth / drawingData.originalImageSize.width;
                    var offset = (displayImage.width - displayImage.paintedWidth) / 2;
                    var imageX = (mouseX - offset) / scale;
                    return Math.max(0, Math.min(drawingData.originalImageSize.width, imageX));
                }

                function toImageY(mouseY) {
                    if (!drawingData.imageLoaded) return 0;
                    var scale = displayImage.paintedHeight / drawingData.originalImageSize.height;
                    var offset = (displayImage.height - displayImage.paintedHeight) / 2;
                    var imageY = (mouseY - offset) / scale;
                    return Math.max(0, Math.min(drawingData.originalImageSize.height, imageY));
                }

                onPressed: {
                    if (!drawingData.imageLoaded) return;

                    isDrawing = true;
                    var imageX = toImageX(mouse.x);
                    var imageY = toImageY(mouse.y);

                    lastPoint = Qt.point(imageX, imageY);
                    drawingData.addPoint(imageX, imageY, currentDrawColor);
                }

                onPositionChanged: {
                    if (!isDrawing || !drawingData.imageLoaded) return;

                    var imageX = toImageX(mouse.x);
                    var imageY = toImageY(mouse.y);

                    // Add intermediate points for smoother drawing
                    var steps = 3;
                    for (var i = 1; i <= steps; i++) {
                        var t = i / steps;
                        var interpX = lastPoint.x + (imageX - lastPoint.x) * t;
                        var interpY = lastPoint.y + (imageY - lastPoint.y) * t;
                        drawingData.addPoint(interpX, interpY, currentDrawColor);
                    }

                    lastPoint = Qt.point(imageX, imageY);
                }

                onReleased: {
                    isDrawing = false;
                }

                // Show cursor position in image coordinates
                Text {
                    id: cursorPosition
                    x: Math.min(parent.width - width - 5, parent.mouseX + 10)
                    y: Math.min(parent.height - height - 5, parent.mouseY + 10)
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                    font.pixelSize: 12

                    text: {
                        if (!drawingData.imageLoaded) return "";
                        var imageX = drawArea.toImageX(drawArea.mouseX);
                        var imageY = drawArea.toImageY(drawArea.mouseY);
                        return "Image: " + Math.round(imageX) + ", " + Math.round(imageY);
                    }
                }
            }
        }
    }

    // Status bar
    Rectangle {
        id: statusBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: 30
        color: "#2d2d2d"

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: drawingData.imageSource ? "Loaded: " + drawingData.imageSource.replace("file:///", "") : "Ready"
            color: "lightgray"
            font.pixelSize: 12
        }

        Text {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: "Drawing synchronized between windows"
            color: "lightgreen"
            font.pixelSize: 12
        }
    }
}
