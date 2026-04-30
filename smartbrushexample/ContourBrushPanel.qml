import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Rectangle {
    id: controlPanel
    width: 300
    height: 380
    color: "#1e3c72"
    border.color: "#00AAFF"
    border.width: 2
    radius: 8

    property var brushBackend

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 12

        Text {
            text: "CONTOUR CONTROLS"
            color: "white"
            font.bold: true
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#00AAFF"
        }

        // Brush size
        ColumnLayout {
            spacing: 5
            Layout.fillWidth: true

            Text {
                text: "BRUSH SIZE"
                color: "#AAAAFF"
                font.bold: true
                font.pixelSize: 12
            }

            RowLayout {
                Slider {
                    id: brushSizeSlider
                    Layout.fillWidth: true
                    from: 5
                    to: 60
                    value: brushBackend ? brushBackend.brushSize : 20
                    onValueChanged: if (brushBackend) brushBackend.brushSize = value

                    background: Rectangle {
                        color: "#2a5298"
                        radius: 3
                        height: 6
                    }

                    handle: Rectangle {
                        x: brushSizeSlider.leftPadding + brushSizeSlider.visualPosition *
                           (brushSizeSlider.availableWidth - width)
                        y: brushSizeSlider.topPadding + brushSizeSlider.availableHeight / 2 - height / 2
                        width: 22
                        height: 22
                        radius: 11
                        color: "#00AAFF"
                        border.color: "white"
                        border.width: 2
                    }
                }

                Text {
                    text: brushBackend ? brushBackend.brushSize.toFixed(0) : "20"
                    color: "white"
                    font.bold: true
                    Layout.preferredWidth: 30
                }
            }
        }

        // Color selection
        ColumnLayout {
            spacing: 5
            Layout.fillWidth: true

            Text {
                text: "BOUNDARY COLOR"
                color: "#AAAAFF"
                font.bold: true
                font.pixelSize: 12
            }

            GridLayout {
                columns: 3
                rowSpacing: 8
                columnSpacing: 8
                Layout.fillWidth: true

                Repeater {
                    model: [
                        { name: "Blue", color: "#00AAFF" },
                        { name: "Green", color: "#00FF00" },
                        { name: "Red", color: "#FF4444" },
                        { name: "Yellow", color: "#FFFF00" },
                        { name: "Cyan", color: "#00FFFF" },
                        { name: "Purple", color: "#AA00FF" }
                    ]

                    Rectangle {
                        Layout.fillWidth: true
                        height: 40
                        radius: 5
                        color: modelData.color
                        border.color: "white"
                        border.width: brushBackend && brushBackend.brushColor === modelData.color ? 3 : 1

                        Text {
                            anchors.centerIn: parent
                            text: modelData.name
                            color: "white"
                            font.bold: true
                            style: Text.Outline
                            styleColor: "black"
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (brushBackend) {
                                    brushBackend.brushColor = modelData.color
                                }
                            }
                        }
                    }
                }
            }
        }

        // Tools
        ColumnLayout {
            spacing: 5
            Layout.fillWidth: true

            Text {
                text: "TOOLS"
                color: "#AAAAFF"
                font.bold: true
                font.pixelSize: 12
            }

            GridLayout {
                columns: 2
                rowSpacing: 8
                columnSpacing: 8
                Layout.fillWidth: true

                Button {
                    text: "SMOOTH BOUNDARY"
                    Layout.fillWidth: true
                    height: 35
                    onClicked: if (brushBackend) brushBackend.calculateConvexHull()

                    background: Rectangle {
                        color: "#2a5298"
                        radius: 5
                        border.color: "#00AAFF"
                        border.width: 2
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                    }
                }

                Button {
                    text: "CALCULATE AREA"
                    Layout.fillWidth: true
                    height: 35
                    onClicked: {
                        if (brushBackend) {
                            var area = brushBackend.calculateArea()
                            areaText.text = "Area: " + area.toFixed(1) + " px²"
                        }
                    }

                    background: Rectangle {
                        color: "#2a5298"
                        radius: 5
                        border.color: "#00FF00"
                        border.width: 2
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                    }
                }

                Button {
                    text: "CLEAR ALL"
                    Layout.fillWidth: true
                    height: 35
                    onClicked: if (brushBackend) brushBackend.clear()

                    background: Rectangle {
                        color: "#8B0000"
                        radius: 5
                        border.color: "#FF4444"
                        border.width: 2
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                    }
                }

                Button {
                    text: "UNDO"
                    Layout.fillWidth: true
                    height: 35
                    // Implement undo functionality

                    background: Rectangle {
                        color: "#2a5298"
                        radius: 5
                        border.color: "#FFD700"
                        border.width: 2
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                    }
                }
            }
        }

        // Info panel
        Rectangle {
            Layout.fillWidth: true
            height: 70
            color: "white"
            radius: 5
            border.color: "#00AAFF"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 3

                Text {
                    id: areaText
                    text: "Area: Not calculated"
                    color: "#00FF00"
                    font.bold: true
                    font.pixelSize: 12
                }

                Text {
                    text: brushBackend ? "Boundary Points: " + brushBackend.boundaryPoints.length : "Points: 0"
                    color: "#AAAAFF"
                    font.pixelSize: 11
                }

                Text {
                    text: brushBackend && brushBackend.isActive ? "Status: Painting" : "Status: Ready"
                    color: "white"
                    font.pixelSize: 11
                    font.bold: true
                }
            }
        }
    }
}
