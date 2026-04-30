import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import ContourBrush 1.0

ApplicationWindow {
    width: 1024
    height: 768
    visible: true
    title: "Multi-Contour Brush"

    SmartContourBrush {
        id: brushBackend
        brushSize: 30.0
        brushColor: "#00AAFF"
    }

    Rectangle {
        anchors.fill: parent
        color: "#1a1a2e"

        SmartContourBrushCanvas {
            id: canvas
            anchors.fill: parent
            brushBackend: brushBackend
        }

        // Control panel
        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 20
            width: 200
            height: 160
            color: "red"
            radius: 8
            border.color: "#00AAFF"
            border.width: 2

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8

                Text {
                    text: "Multi-Contour Brush"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 14
                }

                Row {
                    spacing: 10
                    width: parent.width

                    Text {
                        text: "Size:"
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Slider {
                        width: parent.width - 50
                        from: 10
                        to: 60
                        value: brushBackend.brushSize
                        onValueChanged: brushBackend.brushSize = value
                    }

                    Text {
                        text: brushBackend.brushSize.toFixed(0)
                        color: "white"
                        width: 30
                    }
                }

                Button {
                    text: "Start New Contour"
                    width: parent.width
                    onClicked: {
                        canvas.startingNewContour = true
                    }
                }

                Row {
                    spacing: 5
                    width: parent.width

                    Button {
                        text: "Paint"
                        width: (parent.width - 5) / 2
                        highlighted: !canvas.eraseMode
                        onClicked: canvas.eraseMode = false
                    }

                    Button {
                        text: "Erase"
                        width: (parent.width - 5) / 2
                        highlighted: canvas.eraseMode
                        onClicked: canvas.eraseMode = true
                    }
                }

                Button {
                    text: "Clear All"
                    width: parent.width
                    onClicked: brushBackend.clear()
                }
            }
        }
    }

    // Instructions
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        width: 500
        height: 50
        color: "red"
        radius: 8
        border.color: "#FFD700"
        border.width: 1

        Text {
            anchors.centerIn: parent
            width: parent.width - 20
            horizontalAlignment: Text.AlignHCenter
            color: "white"
            wrapMode: Text.Wrap
            text: "Left Click: Start New Contour | Drag to Paint | Release to Finish | Right Click: Erase"
        }
    }
}
