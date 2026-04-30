import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.15
import Contour.Backend 1.0

ApplicationWindow {
    id: mainWindow
    width: 1400
    height: 900
    visible: true
    title: "Contour Drawing Application - C++/Python/QML"

    // Contour Backend
    ContourBackend {
        id: contourBackend
        contourType: "circle"
        contourColor: "#FF5733"

        onContourGenerated: {
            canvas.requestPaint()
            areaText.text = "Area: " + contourBackend.contourArea.toFixed(2)
        }

        onStatusMessageChanged: {
            statusLabel.text = contourBackend.statusMessage
        }
    }

    // Main layout
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Left panel - Controls
        Rectangle {
            id: controlPanel
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#2c3e50"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                // Header
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: "transparent"

                    Text {
                        anchors.centerIn: parent
                        text: "CONTOUR CONTROLS"
                        font.pixelSize: 24
                        font.bold: true
                        color: "white"
                    }

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 2
                        color: "#3498db"
                    }
                }

                // Contour type selection
                GroupBox {
                    Layout.fillWidth: true
                    title: "Contour Type"
                    label: Label {
                        text: parent.title
                        color: "white"
                        font.bold: true
                        font.pixelSize: 14
                    }
                    background: Rectangle {
                        color: "#34495e"
                        radius: 8
                        border.color: "#7f8c8d"
                    }

                    Flow {
                        spacing: 10
                        width: parent.width

                        RadioButton {
                            id: circleBtn
                            text: "● Circle"
                            checked: true
                            onCheckedChanged: if (checked) contourBackend.contourType = "circle"
                        }
                        RadioButton {
                            id: starBtn
                            text: "★ Star"
                            onCheckedChanged: if (checked) contourBackend.contourType = "star"
                        }
                        RadioButton {
                            id: waveBtn
                            text: "〰 Wave"
                            onCheckedChanged: if (checked) contourBackend.contourType = "wave"
                        }
                        RadioButton {
                            id: heartBtn
                            text: "❤ Heart"
                            onCheckedChanged: if (checked) contourBackend.contourType = "heart"
                        }
                    }
                }

                // Color selection
                GroupBox {
                    Layout.fillWidth: true
                    title: "Color"
                    label: Label {
                        text: parent.title
                        color: "white"
                        font.bold: true
                        font.pixelSize: 14
                    }
                    background: Rectangle {
                        color: "#34495e"
                        radius: 8
                        border.color: "#7f8c8d"
                    }

                    RowLayout {
                        width: parent.width

                        Rectangle {
                            id: colorDisplay
                            width: 40
                            height: 40
                            radius: 5
                            color: contourBackend.contourColor
                            border.color: "white"
                            border.width: 2
                        }

                        Slider {
                            id: hueSlider
                            Layout.fillWidth: true
                            from: 0
                            to: 360
                            value: 0
                            onValueChanged: {
                                contourBackend.contourColor = Qt.hsla(value/360, 0.8, 0.6, 1.0)
                            }
                        }
                    }
                }

                // Parameters
                GroupBox {
                    Layout.fillWidth: true
                    title: "Parameters"
                    label: Label {
                        text: parent.title
                        color: "white"
                        font.bold: true
                        font.pixelSize: 14
                    }
                    background: Rectangle {
                        color: "#34495e"
                        radius: 8
                        border.color: "#7f8c8d"
                    }

                    GridLayout {
                        columns: 2
                        rowSpacing: 10
                        columnSpacing: 10

                        Label { text: "Size:"; color: "white" }
                        Slider {
                            id: sizeSlider
                            Layout.fillWidth: true
                            from: 20
                            to: 200
                            value: 100
                        }

                        Label { text: "X Pos:"; color: "white" }
                        Slider {
                            id: xPosSlider
                            Layout.fillWidth: true
                            from: 100
                            to: 700
                            value: 400
                        }

                        Label { text: "Y Pos:"; color: "white" }
                        Slider {
                            id: yPosSlider
                            Layout.fillWidth: true
                            from: 100
                            to: 500
                            value: 300
                        }

                        Label { text: "Smooth:"; color: "white" }
                        Slider {
                            id: smoothSlider
                            Layout.fillWidth: true
                            from: 0
                            to: 1
                            value: 0.1
                            stepSize: 0.05
                        }
                    }
                }

                // Action buttons
                Column {
                    Layout.fillWidth: true
                    spacing: 10

                    Button {
                        width: parent.width
                        height: 50
                        text: "Generate Contour"
                        font.pixelSize: 16
                        font.bold: true
                        background: Rectangle {
                            color: "#27ae60"
                            radius: 8
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: {
                            contourBackend.generateContour(
                                sizeSlider.value,
                                xPosSlider.value,
                                yPosSlider.value,
                                smoothSlider.value
                            )
                        }
                    }

                    Button {
                        width: parent.width
                        height: 50
                        text: "Clear"
                        font.pixelSize: 16
                        background: Rectangle {
                            color: "#e74c3c"
                            radius: 8
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: contourBackend.clearContour()
                    }
                }

                // Status area
                GroupBox {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: "Status"
                    label: Label {
                        text: parent.title
                        color: "white"
                        font.bold: true
                        font.pixelSize: 14
                    }
                    background: Rectangle {
                        color: "#34495e"
                        radius: 8
                        border.color: "#7f8c8d"
                    }

                    ColumnLayout {
                        anchors.fill: parent

                        Label {
                            id: areaText
                            text: "Area: 0.00"
                            color: "#2ecc71"
                            font.pixelSize: 16
                            font.bold: true
                        }

                        Text {
                            id: statusLabel
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            text: "Ready"
                            color: "white"
                            font.pixelSize: 14
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }

        // Right panel - Canvas
        Rectangle {
            id: canvasPanel
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1a1a2e"

            // Canvas for drawing
            Canvas {
                id: canvas
                anchors.fill: parent
                antialiasing: true

                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0, 0, width, height)

                    // Draw grid
                    ctx.strokeStyle = "#2c3e50"
                    ctx.lineWidth = 1

                    // Vertical lines
                    for (var x = 50; x < width; x += 50) {
                        ctx.beginPath()
                        ctx.moveTo(x, 0)
                        ctx.lineTo(x, height)
                        ctx.stroke()
                    }

                    // Horizontal lines
                    for (var y = 50; y < height; y += 50) {
                        ctx.beginPath()
                        ctx.moveTo(0, y)
                        ctx.lineTo(width, y)
                        ctx.stroke()
                    }

                    // Draw axes
                    ctx.strokeStyle = "#ecf0f1"
                    ctx.lineWidth = 2
                    ctx.beginPath()
                    ctx.moveTo(width/2, 0)
                    ctx.lineTo(width/2, height)
                    ctx.moveTo(0, height/2)
                    ctx.lineTo(width, height/2)
                    ctx.stroke()

                    // Draw contour if exists
                    if (contourBackend.contourPoints.length > 0) {
                        ctx.strokeStyle = contourBackend.contourColor
                        ctx.fillStyle = Qt.rgba(
                            contourBackend.contourColor.r,
                            contourBackend.contourColor.g,
                            contourBackend.contourColor.b,
                            0.3
                        )
                        ctx.lineWidth = 3
                        ctx.lineJoin = "round"

                        ctx.beginPath()

                        var firstPoint = true
                        for (var i = 0; i < contourBackend.contourPoints.length; i++) {
                            var point = contourBackend.contourPoints[i]
                            var x = point[0]
                            var y = point[1]

                            if (firstPoint) {
                                ctx.moveTo(x, y)
                                firstPoint = false
                            } else {
                                ctx.lineTo(x, y)
                            }
                        }

                        // Close the path
                        var firstPointData = contourBackend.contourPoints[0]
                        ctx.lineTo(firstPointData[0], firstPointData[1])

                        ctx.closePath()
                        ctx.fill()
                        ctx.stroke()

                        // Draw center point
                        ctx.fillStyle = "#f1c40f"
                        ctx.beginPath()
                        ctx.arc(xPosSlider.value, yPosSlider.value, 8, 0, Math.PI * 2)
                        ctx.fill()

                        // Draw control points
                        ctx.fillStyle = "#3498db"
                        for (var j = 0; j < contourBackend.contourPoints.length; j += 10) {
                            var ctrlPoint = contourBackend.contourPoints[j]
                            ctx.beginPath()
                            ctx.arc(ctrlPoint[0], ctrlPoint[1], 4, 0, Math.PI * 2)
                            ctx.fill()
                        }
                    }
                }
            }

            // Canvas controls overlay
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 20
                width: 200
                height: 100
                color: Qt.rgba(0.2, 0.2, 0.2, 0.8)
                radius: 10

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Button {
                        Layout.fillWidth: true
                        text: "Save Contour"
                        onClicked: fileDialog.save()
                    }

                    Button {
                        Layout.fillWidth: true
                        text: "Load Contour"
                        onClicked: fileDialog.open()
                    }
                }
            }
        }
    }

    // File dialog
    FileDialog {
        id: fileDialog
        title: "Save Contour"
        folder: shortcuts.home
        selectExisting: false
        onAccepted: {
            contourBackend.saveContourToFile(fileDialog.fileUrl.toString().replace("file://", ""))
        }
    }
}
