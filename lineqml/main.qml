import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Draw and Delete Lines"

    property var lines: [] // Store all lines as objects
    property var selectedLineIndex: -1 // Track the selected line

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height) // Clear the canvas

            // Draw all lines
            for (var i = 0; i < lines.length; i++) {
                var line = lines[i]
                ctx.beginPath()
                ctx.moveTo(line.startX, line.startY)
                ctx.lineTo(line.endX, line.endY)
                ctx.strokeStyle = selectedLineIndex === i ? "red" : "black" // Highlight selected line
                ctx.stroke()

                // Display length and angle
                ctx.fillStyle = "blue"
                ctx.fillText(`Length: ${line.length.toFixed(2)}`, (line.startX + line.endX) / 2, (line.startY + line.endY) / 2 - 10)
                ctx.fillText(`Angle: ${line.angle.toFixed(2)}°`, (line.startX + line.endX) / 2, (line.startY + line.endY) / 2 + 10)
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            var startX = mouse.x
            var startY = mouse.y
            lines.push({ startX: startX, startY: startY, endX: startX, endY: startY, length: 0, angle: 0 })
            selectedLineIndex = lines.length - 1
        }
        onPositionChanged: {
            if (selectedLineIndex >= 0) {
                var line = lines[selectedLineIndex]
                line.endX = mouse.x
                line.endY = mouse.y

                // Calculate length and angle
                var dx = line.endX - line.startX
                var dy = line.endY - line.startY
                line.length = Math.sqrt(dx * dx + dy * dy)
                line.angle = Math.atan2(dy, dx) * (180 / Math.PI)

                canvas.requestPaint()
            }
        }
        onReleased: {
            if (selectedLineIndex >= 0) {
                var line = lines[selectedLineIndex]
                line.endX = mouse.x
                line.endY = mouse.y

                // Finalize length and angle
                var dx = line.endX - line.startX
                var dy = line.endY - line.startY
                line.length = Math.sqrt(dx * dx + dy * dy)
                line.angle = Math.atan2(dy, dx) * (180 / Math.PI)

                canvas.requestPaint()
            }
        }
    }

    Row {
        anchors.bottom: parent.bottom
        spacing: 10
        Button {
            text: "Delete Selected Line"
            onClicked: {
                if (selectedLineIndex >= 0) {
                    lines.splice(selectedLineIndex, 1) // Remove the selected line
                    selectedLineIndex = -1
                    canvas.requestPaint()
                }
            }
        }
    }
}
