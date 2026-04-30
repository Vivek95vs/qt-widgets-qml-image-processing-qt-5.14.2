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
                        text: "1. Click 'Start Measurement'\n2. Click two points for normal diameter\n3. Click two points for narrowed diameter\n4. Click 'Finish Measurement'"
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
                text: "Image Viewer - Click on image to place measurement points"
                font.bold: true
                Layout.fillWidth: true
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.color: "gray"
                border.width: 2

                Image {
                    id: imageDisplay
                    anchors.fill: parent
                    anchors.margins: 2
                    fillMode: Image.PreserveAspectFit
                    source: ""
                    asynchronous: true

                    onStatusChanged: {
                        if (status === Image.Ready) {
                            statusText.text = "Image loaded successfully"
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

                        // Draw measurement points and lines
                        if (measurementPoints.length > 0) {
                            ctx.strokeStyle = "red"
                            ctx.lineWidth = 2
                            ctx.fillStyle = "red"

                            for (var i = 0; i < measurementPoints.length; i++) {
                                var point = measurementPoints[i]

                                // Draw point
                                ctx.beginPath()
                                ctx.arc(point.x, point.y, 5, 0, Math.PI * 2)
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
                                    ctx.font = "12px Arial"
                                    ctx.fillText(length.toFixed(1) + " px", midX + 5, midY - 5)
                                }
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        enabled: isMeasuring
                        onClicked: {
                            if (isMeasuring && measurementPoints.length < 4) {
                                var point = {
                                    x: mouse.x,
                                    y: mouse.y
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
