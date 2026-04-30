import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: secondaryWindow
    width: 1280
    height: 1024
    visible: true
    title: "Secondary Window (Read-only)"
    color: "#1e1e1e"

    // Image display container
    Item {
        id: imageContainer
        anchors.centerIn: parent
        width: Math.min(parent.width - 20, parent.height - 20)
        height: width * (drawingData.originalImageSize.height / Math.max(1, drawingData.originalImageSize.width))

        visible: drawingData.imageLoaded

        // Background for empty state
        Rectangle {
            anchors.fill: parent
            color: "#333333"
            visible: !drawingData.imageLoaded

            Text {
                anchors.centerIn: parent
                text: "No image loaded in primary window"
                color: "white"
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
            }
        }

        // Display image
        Image {
            id: displayImage
            anchors.fill: parent
            source: drawingData.imageLoaded ? drawingData.imageSource : ""
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
            cache: false
        }

        // Canvas for synchronized drawings
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

                ctx.lineWidth = 3; // Fixed width for secondary window
                ctx.lineCap = "round";
                ctx.lineJoin = "round";

                for (var i = 0; i < points.length; i++) {
                    var point = points[i].first;
                    var color = points[i].second;

                    var x = toCanvasX(point.x);
                    var y = toCanvasY(point.y);

                    if (i === 0) {
                        ctx.beginPath();
                        ctx.moveTo(x, y);
                    } else {
                        var prevPoint = points[i-1].first;
                        var prevX = toCanvasX(prevPoint.x);
                        var prevY = toCanvasY(prevPoint.y);

                        // Only draw line if points are reasonably close
                        if (Math.sqrt(Math.pow(x - prevX, 2) + Math.pow(y - prevY, 2)) < 100) {
                            ctx.lineTo(x, y);
                        } else {
                            ctx.beginPath();
                            ctx.moveTo(x, y);
                        }
                    }

                    ctx.strokeStyle = color;
                    ctx.stroke();
                }
            }

            Connections {
                target: drawingData
                onDrawingChanged: drawingCanvas.requestPaint()
                onImageSourceChanged: drawingCanvas.requestPaint()
                onOriginalImageSizeChanged: drawingCanvas.requestPaint()
            }
        }
    }

    // Status info
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 40
        color: "#2d2d2d"

        Row {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "Secondary Window - " + secondaryWindow.width + "x" + secondaryWindow.height
                color: "white"
                font.pixelSize: 14
            }

            Text {
                text: drawingData.imageLoaded ?
                      "Image: " + drawingData.originalImageSize.width + "x" + drawingData.originalImageSize.height :
                      "No image"
                color: drawingData.imageLoaded ? "lightgreen" : "orange"
                font.pixelSize: 14
            }

            Text {
                text: "Read-only view"
                color: "yellow"
                font.pixelSize: 14
            }
        }
    }
}
