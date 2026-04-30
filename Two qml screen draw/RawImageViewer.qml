import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import Qt.labs.platform 1.1

Item {
    id: imageViewer
    property string imageSource: ""
    property var drawings: []
    property bool isDrawing: false
    property point lastPoint
    property color currentColor: "red"
    property real currentLineWidth: 2

    // Signal to notify about drawing updates
    signal drawingUpdated(var drawingData)

    // Function to receive drawing from other screen
    function receiveDrawing(drawingData) {
        console.log("Received drawing data:", drawingData);
        drawings.push(drawingData);
        canvas.requestPaint();
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
                    text: "Draw"
                    checked: isDrawing
                    checkable: true
                    onToggled: isDrawing = checked
                }

                Button {
                    text: "Clear"
                    onClicked: clearDrawings()
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
                    onValueChanged: currentLineWidth = value
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

                    // Draw all stored drawings
                    for (var i = 0; i < drawings.length; i++) {
                        var drawing = drawings[i];
                        ctx.beginPath();
                        ctx.strokeStyle = drawing.color;
                        ctx.lineWidth = drawing.width;
                        ctx.lineCap = "round";

                        if (drawing.type === "line") {
                            ctx.moveTo(drawing.startX * width, drawing.startY * height);
                            ctx.lineTo(drawing.endX * width, drawing.endY * height);
                        } else if (drawing.type === "freehand") {
                            ctx.moveTo(drawing.points[0].x * width, drawing.points[0].y * height);
                            for (var j = 1; j < drawing.points.length; j++) {
                                ctx.lineTo(drawing.points[j].x * width, drawing.points[j].y * height);
                            }
                        }

                        ctx.stroke();
                    }
                }

                // Mouse area for drawing
                MouseArea {
                    anchors.fill: parent
                    enabled: isDrawing
                    hoverEnabled: true

                    onPressed: {
                        if (isDrawing) {
                            lastPoint = Qt.point(mouse.x / width, mouse.y / height);
                            var newDrawing = {
                                type: "freehand",
                                color: currentColor,
                                width: currentLineWidth,
                                points: [Qt.point(mouse.x / width, mouse.y / height)]
                            };
                            drawings.push(newDrawing);
                        }
                    }

                    onPositionChanged: {
                        if (isDrawing && pressed) {
                            var currentPoint = Qt.point(mouse.x / width, mouse.y / height);

                            // Update last drawing
                            var lastDrawing = drawings[drawings.length - 1];
                            lastDrawing.points.push(currentPoint);

                            // Send drawing update
                            var drawingData = {
                                type: "freehand",
                                color: currentColor,
                                width: currentLineWidth,
                                points: lastDrawing.points
                            };

                            // Send to other screen
                            drawingUpdated(drawingData);

                            canvas.requestPaint();
                        }
                    }

                    onReleased: {
                        if (isDrawing) {
                            // Finalize the drawing
                            var drawingData = {
                                type: "freehand",
                                color: currentColor,
                                width: currentLineWidth,
                                points: drawings[drawings.length - 1].points
                            };

                            // Send final drawing
                            drawingUpdated(drawingData);
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
            imageSource = fileDialog.file;
            console.log("Image loaded from:", fileDialog.file);
        }
    }

    // Color dialog for selecting drawing color
    ColorDialog {
        id: colorDialog
        title: "Select Drawing Color"
        onAccepted: {
            currentColor = colorDialog.color;
        }
    }
}
