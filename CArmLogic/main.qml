import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.14

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "Vascular Stenosis Measurement"

    property var measurementPoints: []
    property bool isMeasuring: false
    property real imageScale: 1.0
    property real imageRotation: 0
    property point imageTranslation: Qt.point(0, 0)
    property point lastPanPoint: Qt.point(0, 0)
    property bool isPanning: false

    RowLayout {
        anchors.fill: parent
        spacing: 10

        // Left panel for controls
        ColumnLayout {
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            spacing: 10

            GroupBox {
                title: "Image Controls"
                Layout.fillWidth: true

                ColumnLayout {
                    width: parent.width

                    Button {
                        text: "Load JPG Image"
                        Layout.fillWidth: true
                        onClicked: fileDialog.open()
                    }

                    Label {
                        text: "Image Status:"
                        font.bold: true
                    }

                    TextArea {
                        id: statusText
                        Layout.fillWidth: true
                        Layout.preferredHeight: 80
                        readOnly: true
                        wrapMode: Text.Wrap
                    }

                    // Zoom and pan controls
                    Label {
                        text: "Touch Controls:"
                        font.bold: true
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Button {
                            text: "Reset View"
                            Layout.fillWidth: true
                            onClicked: {
                                imageScale = 1.0
                                imageRotation = 0
                                imageTranslation = Qt.point(0, 0)
                                canvas.requestPaint()
                            }
                        }
                    }

                    Label {
                        text: "Zoom: " + (imageScale * 100).toFixed(0) + "%"
                        font.bold: true
                    }

                    Slider {
                        id: zoomSlider
                        Layout.fillWidth: true
                        from: 0.1
                        to: 5.0
                        value: imageScale
                        onValueChanged: {
                            imageScale = value
                            canvas.requestPaint()
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Button {
                            text: "Zoom In"
                            Layout.fillWidth: true
                            onClicked: {
                                imageScale = Math.min(5.0, imageScale + 0.1)
                                zoomSlider.value = imageScale
                                canvas.requestPaint()
                            }
                        }
                        Button {
                            text: "Zoom Out"
                            Layout.fillWidth: true
                            onClicked: {
                                imageScale = Math.max(0.1, imageScale - 0.1)
                                zoomSlider.value = imageScale
                                canvas.requestPaint()
                            }
                        }
                    }
                }
            }

            GroupBox {
                title: "Measurement Tools"
                Layout.fillWidth: true

                ColumnLayout {
                    width: parent.width

                    Button {
                        id: measureBtn
                        text: isMeasuring ? "Finish Measurement" : "Start Measurement"
                        Layout.fillWidth: true
                        onClicked: {
                            isMeasuring = !isMeasuring
                            if (!isMeasuring) {
                                analyzeStenosis()
                            } else {
                                measurementPoints = []
                                canvas.requestPaint()
                            }
                        }
                    }

                    Label {
                        text: "Instructions:"
                        font.bold: true
                    }

                    Text {
                        text: "1. Click 'Start Measurement'\n2. Click two points for normal diameter\n3. Click two points for narrowed diameter\n4. Click 'Finish Measurement'\n\nTouch Gestures:\n- Pinch to zoom\n- Two-finger pan to move\n- Double tap to reset view"
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                    }

                    Button {
                        text: "Clear Measurements"
                        Layout.fillWidth: true
                        onClicked: {
                            measurementPoints = []
                            resultText.text = ""
                            canvas.requestPaint()
                        }
                    }
                }
            }

            GroupBox {
                title: "Results"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TextArea {
                        id: resultText
                        width: parent.width
                        readOnly: true
                        wrapMode: Text.Wrap
                    }
                }
            }
        }

        // Right panel for image display
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: "Image Viewer - Use touch gestures: Pinch to zoom, Two-finger pan to move"
                font.bold: true
                Layout.fillWidth: true
            }

            Rectangle {
                id: imageContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.color: "gray"
                border.width: 2
                clip: true

                // Transform the image and canvas
                Item {
                    id: transformedItem
                    anchors.centerIn: parent
                    width: imageDisplay.sourceSize.width * imageScale
                    height: imageDisplay.sourceSize.height * imageScale
                    transform: [
                        Translate {
                            x: imageTranslation.x
                            y: imageTranslation.y
                        },
                        Rotation {
                            origin.x: transformedItem.width / 2
                            origin.y: transformedItem.height / 2
                            angle: imageRotation
                        }
                    ]

                    Image {
                        id: imageDisplay
                        anchors.centerIn: parent
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: ""
                        asynchronous: true

                        onStatusChanged: {
                            if (status === Image.Ready) {
                                statusText.text = "Image loaded successfully"
                                // Reset transformations when new image loads
                                imageScale = 1.0
                                imageRotation = 0
                                imageTranslation = Qt.point(0, 0)
                                zoomSlider.value = 1.0
                            } else if (status === Image.Loading) {
                                statusText.text = "Loading image..."
                            } else if (status === Image.Error) {
                                statusText.text = "Error loading image"
                            }
                        }
                    }

                    Canvas {
                        id: canvas
                        anchors.fill: parent

                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.reset()
                            ctx.scale(imageScale, imageScale)

                            // Draw measurement points and lines
                            if (measurementPoints.length > 0) {
                                ctx.strokeStyle = "red"
                                ctx.lineWidth = 2 / imageScale // Adjust line width for zoom
                                ctx.fillStyle = "red"

                                for (var i = 0; i < measurementPoints.length; i++) {
                                    var point = measurementPoints[i]

                                    // Draw point
                                    ctx.beginPath()
                                    ctx.arc(point.x, point.y, 5 / imageScale, 0, Math.PI * 2)
                                    ctx.fill()

                                    // Draw line between pairs of points
                                    if (i % 2 === 1) {
                                        var prevPoint = measurementPoints[i-1]
                                        ctx.beginPath()
                                        ctx.moveTo(prevPoint.x, prevPoint.y)
                                        ctx.lineTo(point.x, point.y)
                                        ctx.stroke()

                                        // Add label
                                        var midX = (prevPoint.x + point.x) / 2
                                        var midY = (prevPoint.y + point.y) / 2
                                        var length = Math.sqrt(Math.pow(point.x - prevPoint.x, 2) +
                                                              Math.pow(point.y - prevPoint.y, 2))

                                        ctx.fillStyle = "blue"
                                        ctx.font = (12 / imageScale) + "px Arial"
                                        ctx.fillText(length.toFixed(1) + " px", midX + 5, midY - 5)
                                    }
                                }
                            }
                        }
                    }
                }

                // MultiPointTouchArea for pinch-to-zoom and pan gestures
                MultiPointTouchArea {
                    anchors.fill: parent
                    maximumTouchPoints: 2
                    mouseEnabled: true

                    property real initialDistance: 0
                    property real initialScale: 1.0
                    property point initialCenter: Qt.point(0, 0)
                    property bool twoFingerGesture: false

                    onPressed: {
                        if (touchPoints.length === 2) {
                            // Start two-finger gesture (pinch/pan)
                            twoFingerGesture = true
                            var p1 = touchPoints[0].sceneX
                            var p2 = touchPoints[1].sceneX
                            initialDistance = Math.sqrt(Math.pow(touchPoints[1].sceneX - touchPoints[0].sceneX, 2) +
                                                       Math.pow(touchPoints[1].sceneY - touchPoints[0].sceneY, 2))
                            initialScale = imageScale
                            initialCenter = Qt.point((touchPoints[0].sceneX + touchPoints[1].sceneX) / 2,
                                                    (touchPoints[0].sceneY + touchPoints[1].sceneY) / 2)
                            lastPanPoint = initialCenter
                        } else if (touchPoints.length === 1 && isMeasuring) {
                            // Single touch for measurement (only when measuring mode is active)
                            var touch = touchPoints[0]
                            var containerPoint = Qt.point(touch.sceneX, touch.sceneY)
                            var imagePoint = mapToImageCoordinate(containerPoint)

                            if (isMeasuring && measurementPoints.length < 4) {
                                var point = {
                                    x: imagePoint.x,
                                    y: imagePoint.y
                                }
                                measurementPoints.push(point)
                                canvas.requestPaint()

                                statusText.text = "Point " + measurementPoints.length + " placed at (" +
                                                 point.x.toFixed(1) + ", " + point.y.toFixed(1) + ")"
                            }
                        }
                    }

                    onUpdated: {
                        if (touchPoints.length === 2 && twoFingerGesture) {
                            // Calculate current distance between touch points
                            var currentDistance = Math.sqrt(Math.pow(touchPoints[1].sceneX - touchPoints[0].sceneX, 2) +
                                                          Math.pow(touchPoints[1].sceneY - touchPoints[0].sceneY, 2))

                            // Calculate scale factor
                            var scaleFactor = currentDistance / initialDistance
                            imageScale = Math.max(0.1, Math.min(5.0, initialScale * scaleFactor))
                            zoomSlider.value = imageScale

                            // Calculate pan movement
                            var currentCenter = Qt.point((touchPoints[0].sceneX + touchPoints[1].sceneX) / 2,
                                                        (touchPoints[0].sceneY + touchPoints[1].sceneY) / 2)

                            imageTranslation.x += (currentCenter.x - lastPanPoint.x)
                            imageTranslation.y += (currentCenter.y - lastPanPoint.y)

                            lastPanPoint = currentCenter
                        }
                    }

                    onReleased: {
                        if (touchPoints.length === 0) {
                            twoFingerGesture = false
                        }
                    }

                    // Double tap to reset view
                    TapHandler {
                        gesturePolicy: TapHandler.ReleaseWithinBounds
                        onTapped: {
                            if (tapCount === 2) { // Double tap
                                imageScale = 1.0
                                imageRotation = 0
                                imageTranslation = Qt.point(0, 0)
                                zoomSlider.value = 1.0
                                canvas.requestPaint()
                            }
                        }
                    }

                    // Single tap for measurements (when not in two-finger gesture)
                    MouseArea {
                        anchors.fill: parent
                        enabled: isMeasuring && !parent.twoFingerGesture
                        onClicked: {
                            if (isMeasuring && measurementPoints.length < 4) {
                                var imagePoint = mapToImageCoordinate(Qt.point(mouse.x, mouse.y))
                                var point = {
                                    x: imagePoint.x,
                                    y: imagePoint.y
                                }
                                measurementPoints.push(point)
                                canvas.requestPaint()

                                statusText.text = "Point " + measurementPoints.length + " placed at (" +
                                                 point.x.toFixed(1) + ", " + point.y.toFixed(1) + ")"
                            }
                        }
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select JPG Image"
        folder: "file:///D:/C-ARM%20Manual/Images"
        nameFilters: ["Image files (*.jpg *.jpeg *.png *.bmp)"]
        onAccepted: {
            // Extract just the filename from the full path
            var fullPath = fileDialog.fileUrl.toString()
            var fileName = fullPath.substring(fullPath.lastIndexOf("/") + 1)
            statusText.text = StorageInfo.loadImage(fileName)
            imageDisplay.source = fileDialog.fileUrl
        }
    }

    // Function to convert screen coordinates to image coordinates
    function mapToImageCoordinate(screenPoint) {
        var containerRect = imageContainer
        var imageRect = transformedItem

        // Calculate the actual displayed image area (considering PreserveAspectFit)
        var scale = Math.min(imageRect.width / imageDisplay.sourceSize.width,
                           imageRect.height / imageDisplay.sourceSize.height)
        var displayedWidth = imageDisplay.sourceSize.width * scale
        var displayedHeight = imageDisplay.sourceSize.height * scale
        var imageX = (imageRect.width - displayedWidth) / 2
        var imageY = (imageRect.height - displayedHeight) / 2

        // Convert to image coordinates
        var localX = (screenPoint.x - imageContainer.x - imageRect.x - imageTranslation.x - imageX) / (imageScale * scale)
        var localY = (screenPoint.y - imageContainer.y - imageRect.y - imageTranslation.y - imageY) / (imageScale * scale)

        return Qt.point(
            Math.max(0, Math.min(imageDisplay.sourceSize.width, localX)),
            Math.max(0, Math.min(imageDisplay.sourceSize.height, localY))
        )
    }

    function analyzeStenosis() {
        if (measurementPoints.length === 4) {
            var pointsArray = []
            for (var i = 0; i < measurementPoints.length; i++) {
                pointsArray.push(Qt.point(measurementPoints[i].x, measurementPoints[i].y))
            }
            resultText.text = StorageInfo.analyzeVessel(pointsArray)
        } else {
            resultText.text = "Error: Please place exactly 4 measurement points (2 for normal diameter, 2 for narrowed diameter)"
        }
    }
}
