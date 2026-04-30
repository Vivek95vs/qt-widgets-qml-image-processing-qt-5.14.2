import QtQuick 2.14
import QtQuick.Controls 2.14

Canvas {
    id: canvas
    anchors.fill: parent

    property var brushBackend
    property real scaleFactor: 1.0
    property point viewOffset: Qt.point(0, 0)
    property bool showGrid: false

    property point mousePosition: Qt.point(0, 0)
    property bool mousePressed: false
    property bool eraseMode: false
    property bool startingNewContour: false

    // Cache for better performance
    renderTarget: Canvas.FramebufferObject
    renderStrategy: Canvas.Threaded

    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, width, height)

        // Save context
        ctx.save()
        ctx.scale(scaleFactor, scaleFactor)
        ctx.translate(viewOffset.x, viewOffset.y)

        // Draw all contours separately
        drawAllContours(ctx)

        // Draw brush preview
        drawBrushPreview(ctx)

        ctx.restore()
    }

    function drawAllContours(ctx) {
        if (!brushBackend)
            return

        var points = brushBackend.contourPoints
        if (!points || points.length === 0)
            return

        // Set fill and stroke style
        ctx.fillStyle = Qt.rgba(
            brushBackend.brushColor.r,
            brushBackend.brushColor.g,
            brushBackend.brushColor.b,
            0.3
        )

        ctx.strokeStyle = brushBackend.brushColor
        ctx.lineWidth = 2
        ctx.lineJoin = "round"
        ctx.lineCap = "round"

        var currentContourPoints = []
        var separatorMarker = -1000.0

        // Process all points, separating by contours
        for (var i = 0; i < points.length; i++) {
            var point = points[i]

            // Check if this is a separator marker
            if (Math.abs(point.x - separatorMarker) < 0.1 &&
                Math.abs(point.y - separatorMarker) < 0.1) {

                // Draw the current contour
                if (currentContourPoints.length >= 3) {
                    drawSingleContour(ctx, currentContourPoints)
                }

                // Start new contour
                currentContourPoints = []
            } else {
                // Add point to current contour
                currentContourPoints.push(point)
            }
        }

        // Draw the last contour
        if (currentContourPoints.length >= 3) {
            drawSingleContour(ctx, currentContourPoints)
        }
    }

    function drawSingleContour(ctx, points) {
        // Draw filled area
        ctx.beginPath()
        ctx.moveTo(points[0].x, points[0].y)

        // Use simple line connections
        for (var i = 1; i < points.length; i++) {
            ctx.lineTo(points[i].x, points[i].y)
        }

        ctx.closePath()
        ctx.fill()
        ctx.stroke()
    }

    function drawBrushPreview(ctx) {
        if (!brushBackend)
            return

        var pos = mousePosition
        var brushSize = brushBackend.brushSize

        // Draw brush circle
        if (mousePressed) {
            // Active brush
            if (eraseMode) {
                // Erase mode - red with minus
                ctx.fillStyle = "rgba(255, 50, 50, 0.4)"
                ctx.strokeStyle = "red"

                // Draw minus sign
                ctx.beginPath()
                ctx.moveTo(pos.x - brushSize * 0.6, pos.y)
                ctx.lineTo(pos.x + brushSize * 0.6, pos.y)
                ctx.lineWidth = 3
                ctx.stroke()
            } else {
                // Paint mode - brush color with plus
                ctx.fillStyle = Qt.rgba(
                    brushBackend.brushColor.r,
                    brushBackend.brushColor.g,
                    brushBackend.brushColor.b,
                    0.4
                )
                ctx.strokeStyle = brushBackend.brushColor

                // Draw plus sign for new contour
                ctx.beginPath()
                if (startingNewContour) {
                    // Show "new contour" indicator
                    ctx.moveTo(pos.x - brushSize * 0.6, pos.y)
                    ctx.lineTo(pos.x + brushSize * 0.6, pos.y)
                    ctx.moveTo(pos.x, pos.y - brushSize * 0.6)
                    ctx.lineTo(pos.x, pos.y + brushSize * 0.6)
                } else {
                    // Show continuation indicator
                    ctx.arc(pos.x, pos.y, brushSize * 0.3, 0, Math.PI * 2)
                }
                ctx.lineWidth = 3
                ctx.stroke()
            }

            ctx.lineWidth = 2
        } else {
            // Inactive brush preview
            if (eraseMode) {
                ctx.fillStyle = "rgba(255, 50, 50, 0.2)"
                ctx.strokeStyle = "rgba(255, 0, 0, 0.5)"
            } else {
                ctx.fillStyle = Qt.rgba(
                    brushBackend.brushColor.r,
                    brushBackend.brushColor.g,
                    brushBackend.brushColor.b,
                    0.2
                )
                ctx.strokeStyle = brushBackend.brushColor
            }
            ctx.lineWidth = 1.5
        }

        // Draw brush circle
        ctx.beginPath()
        ctx.arc(pos.x, pos.y, brushSize, 0, Math.PI * 2)
        ctx.fill()
        ctx.stroke()

        // Draw brush center dot
        ctx.fillStyle = "white"
        ctx.beginPath()
        ctx.arc(pos.x, pos.y, 2, 0, Math.PI * 2)
        ctx.fill()
    }

    function getCanvasPos(mouseX, mouseY) {
        return Qt.point(
            (mouseX / scaleFactor) - viewOffset.x,
            (mouseY / scaleFactor) - viewOffset.y
        )
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        propagateComposedEvents: true

        onPressed: function(mouse) {
            if (!brushBackend)
                return

            mousePressed = true
            var pos = getCanvasPos(mouse.x, mouse.y)
            mousePosition = pos

            // Right click for erase, left click for paint
            eraseMode = (mouse.button === Qt.RightButton)

            if (eraseMode) {
                brushBackend.startErase(pos.x, pos.y)
                startingNewContour = false
            } else {
                // Check if we should start a new contour
                // You can add logic here to detect if clicking far from existing contours
                startingNewContour = true
                brushBackend.startNewContour(pos.x, pos.y)
            }

            canvas.requestPaint()
        }

        onPositionChanged: function(mouse) {
            var pos = getCanvasPos(mouse.x, mouse.y)
            mousePosition = pos

            if (mousePressed && brushBackend && brushBackend.isActive) {
                if (eraseMode) {
                    brushBackend.updateErase(pos.x, pos.y)
                } else {
                    // Once we start moving, we're continuing the current contour
                    startingNewContour = false
                    brushBackend.updateCurrentContour(pos.x, pos.y)
                }
                canvas.requestPaint()
            }
        }

        onReleased: function(mouse) {
            mousePressed = false

            if (!brushBackend || !brushBackend.isActive)
                return

            if (eraseMode) {
                brushBackend.finishErase()
            } else {
                brushBackend.finishCurrentContour()
            }

            canvas.requestPaint()
        }
    }

    // Mouse wheel for zoom
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        propagateComposedEvents: true

        onWheel: function(wheel) {
            var scaleChange = wheel.angleDelta.y > 0 ? 1.1 : 0.9
            scaleFactor = Math.max(0.1, Math.min(5, scaleFactor * scaleChange))

            var mousePos = Qt.point(wheel.x, wheel.y)
            var canvasPos = getCanvasPos(mousePos.x, mousePos.y)

            viewOffset.x = mousePos.x / scaleFactor - canvasPos.x
            viewOffset.y = mousePos.y / scaleFactor - canvasPos.y

            canvas.requestPaint()
            wheel.accepted = true
        }
    }

    // Instructions
    Text {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 10
        color: "white"
        style: Text.Outline
        styleColor: "black"
        font.pixelSize: 12

        text: {
            if (!brushBackend) return ""

            var mode = eraseMode ? "ERASE (Right Click)" : "PAINT (Left Click)"
            var action = startingNewContour ? "Start New Contour" : "Continue Drawing"
            return mode + " | " + action + " | Brush: " + brushBackend.brushSize.toFixed(0) + "px"
        }
    }
}
