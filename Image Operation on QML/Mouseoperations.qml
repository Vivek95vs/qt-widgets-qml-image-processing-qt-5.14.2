import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Shapes 1.12
import QtQuick.Layouts 1.0
import QtQuick.Window 2.12


Item {
    anchors.fill: parent
    focus: true

    property alias polyCanvas: polycanvas  // Expose the canvas to Main.qml


    MouseArea {
        anchors.fill: parent
        onWheel: {
            var factor = wheel.angleDelta.y > 0 ? 1.1 : 0.9;
            imageScale.xScale = Math.max(0.5, Math.min(3.0, imageScale.xScale * factor));
            imageScale.yScale = Math.max(0.5, Math.min(3.0, imageScale.yScale * factor));
        }
    }

    MouseArea {
        id: canvas
        anchors.fill: parent
        hoverEnabled: true

        property point startPos
        property var currentShape

        onPressed: {
            if (drawingrectangle)
            {
                if (drawingrectangle && mouse.x >= 0 && mouse.x <= clipArea.width && mouse.y >= 0 && mouse.y <= clipArea.height) {
                    startPos = Qt.point(mouse.x, mouse.y);
                    currentShape = rectangleComponent.createObject(image1, {x: startPos.x,y: startPos.y,width: 0,height: 0,});
                    rectangles.push(currentShape);
                    console.log("Mouse clicked at:", mouse.x, mouse.y);
                }
            }else if (drawingLine) {
                if(drawingLine&& mouse.x >= 0 && mouse.x <= clipArea.width && mouse.y >= 0 && mouse.y <= clipArea.height)
                {
                startPos = Qt.point(mouse.x, mouse.y);
                currentShape = lineComponent.createObject(canvas, {x1: startPos.x, y1: startPos.y, x2: startPos.x, y2: startPos.y});
                lines.push(currentShape);
                }
            }
            else if (isInsidePolygon(mouse.x, mouse.y)&&isFinalized==false) {
                isPolygonSelected = !isPolygonSelected;
                isDragging = !isDragging;
                dragStartPosition = Qt.point(mouse.x, mouse.y);
                polycanvas.requestPaint();
            }
            else if (measuringCobbAngle && currentCobbLine < 2) {
                startPos = Qt.point(mouse.x, mouse.y);
                currentShape = lineComponent.createObject(canvas, {
                    x1: startPos.x,
                    y1: startPos.y,
                    x2: startPos.x,
                    y2: startPos.y,
                    isCobbLine: true,
                    lineIndex: currentCobbLine  // Make sure this is properly passed
                });
                cobbAngleLines.push(currentShape);
            }
            else if (markingVertebrae) {
                if (mouse.x >= 0 && mouse.x <= clipArea.width && mouse.y >= 0 && mouse.y <= clipArea.height) {
                    var vertebraeMark = vertebraeComponent.createObject(image1, {
                        x: mouse.x - 15,
                        y: mouse.y - 15,
                        label: "C" + vertebraeCount
                    });
                    vertebraeCount++;
                }
            }
            else if (markingNum) {
                if (mouse.x >= 0 && mouse.x <= clipArea.width && mouse.y >= 0 && mouse.y <= clipArea.height) {
                    var NumMark = numComponent.createObject(image1, {
                        x: mouse.x - 15,
                        y: mouse.y - 15,
                        label: numCount
                    });
                    numCount++;
                }
            }
        }




        onClicked: {
            if (!isDragging && isFinalized) {
                polygonPoints.push(Qt.point(mouse.x, mouse.y));
                polycanvas.requestPaint();
            }
            else if(isDragging&&!isFinalized)
            {
                isDragging = !isDragging;
                isPolygonSelected = !isPolygonSelected;

            }


        }

        onPositionChanged: {
            let newWidth, newHeight;
            if (drawingrectangle && currentShape) {
                newWidth = mouse.x - startPos.x;
                newHeight = mouse.y - startPos.y;

                if (startPos.x + newWidth >= 0 && startPos.x + newWidth <= clipArea.width &&
                        startPos.y + newHeight >= 0 && startPos.y + newHeight <= clipArea.height) {
                    currentShape.width = newWidth;
                    currentShape.height = newHeight;

                }

            }
            else if (drawingLine && currentShape) {
                newWidth = mouse.x - startPos.x;
                newHeight = mouse.y - startPos.y;
                if (startPos.x + newWidth >= 0 && startPos.x + newWidth <= clipArea.width &&
                        startPos.y + newHeight >= 0 && startPos.y + newHeight <= clipArea.height) {
                currentShape.x2 = mouse.x;
                currentShape.y2 = mouse.y;
                currentShape.requestPaint();
                }
            }
            else if (isDragging&&isFinalized==false) {
                var dx = mouse.x - dragStartPosition.x;
                var dy = mouse.y - dragStartPosition.y;

                for (var i = 0; i < polygonPoints.length; i++) {
                    polygonPoints[i].x += dx;
                    polygonPoints[i].y += dy;
                }

                dragStartPosition = Qt.point(mouse.x, mouse.y); // Update for continuous movement
                polycanvas.requestPaint();
            } else if (isFinalized) {
                lastMousePosition = Qt.point(mouse.x, mouse.y);
                polycanvas.requestPaint();
            }
            else if (measuringCobbAngle && currentShape && currentCobbLine < 2) {
                currentShape.x2 = mouse.x;
                currentShape.y2 = mouse.y;
                currentShape.requestPaint();

                // Calculate angle when two lines are drawn
                if (cobbAngleLines.length === 2) {
                    var line1 = cobbAngleLines[0];
                    var line2 = cobbAngleLines[1];

                    // Calculate vectors
                    var vec1 = Qt.point(line1.x2 - line1.x1, line1.y2 - line1.y1);
                    var vec2 = Qt.point(line2.x2 - line2.x1, line2.y2 - line2.y1);

                    // Calculate angle between vectors (in degrees)
                    var dot = vec1.x * vec2.x + vec1.y * vec2.y;
                    var mag1 = Math.sqrt(vec1.x * vec1.x + vec1.y * vec1.y);
                    var mag2 = Math.sqrt(vec2.x * vec2.x + vec2.y * vec2.y);
                    var angleRad = Math.acos(Math.min(1, Math.max(-1, dot / (mag1 * mag2))));
                    cobbAngle = angleRad * (180 / Math.PI);

                    // Update the angle display
                    angleDisplay.text = "Cobb Angle: " + cobbAngle.toFixed(1) + "°";
                    console.log("Cob angle:", angleDisplay.text);
                }
            }

        }


        onReleased: {
            if (drawingrectangle && currentShape) {
                if (mouse.x < 0 || mouse.x > clipArea.width || mouse.y < 0 || mouse.y > clipArea.height) {
                    currentShape.destroy(); // Remove rectangle if released outside
                    rectangles.pop();

                }
                currentShape = null;
                drawingrectangle=false;

            }
            else if (drawingLine && currentShape) {
                if (mouse.x < 0 || mouse.x > clipArea.width || mouse.y < 0 || mouse.y > clipArea.height) {
                    currentShape.destroy()
                    lines.pop();
                }
                currentShape = null;
                drawingLine = false;

            }

            else if (measuringCobbAngle && currentShape) {
                if (mouse.x < 0 || mouse.x > clipArea.width || mouse.y < 0 || mouse.y > clipArea.height) {
                    currentShape.destroy();
                    cobbAngleLines.pop();
                } else {
                    currentCobbLine++;
                }
                currentShape = null;

                // Reset measurement mode after two lines are drawn
                if (cobbAngleLines.length == 2) {
                    measuringCobbAngle = false;
                }
            }


             isDragging = false;
        }

        onDoubleClicked: {
            if (isFinalized) {
                isFinalized = false;
                polycanvas.requestPaint();
            }
        }

    }


    Component {
        id: rectangleComponent
        Rectangle {
            id: rect
            width: 50
            height: 50
            color: "transparent"
            border.width: 2
            border.color: "white"

            property bool isSelected: false
            property bool enableDragging: false
            property bool enableResizing: false
            property bool isResizing: false // New flag to prevent dragging during resizing
            property real minSize: 20
            property var dragStartPos: Qt.point(0, 0)

            MouseArea {
                id: dragArea
                anchors.fill: parent
                preventStealing: true
                propagateComposedEvents: true

                drag.target: enableDragging && !isResizing ? parent : null

                onClicked: {
                    mouse.accepted = false;
                    if (selectedShapes.indexOf(parent) === -1) {
                        for (let i = 0; i < selectedShapes.length; i++) {
                            selectedShapes[i].border.color = "white";
                            selectedShapes[i].enableDragging = false;
                            selectedShapes[i].enableResizing = false;
                        }
                        selectedShapes = [];
                        selectedShapes.push(parent);
                        parent.border.color = "red";
                        enableDragging = true;
                        enableResizing = true;
                    } else {
                        selectedShapes.splice(selectedShapes.indexOf(parent), 1);
                        parent.border.color = "white";
                        enableDragging = false;
                        enableResizing = false;
                    }
                }
            }

            // Resize handles at the four corners
            Repeater {
                model: 4
                Rectangle {
                    width: 7
                    height: 7
                    color: "transparent"
                    border.color: "white"
                    border.width: 1
                    radius: 1
                    anchors {
                        left: (index === 0 || index === 3) ? parent.left : undefined
                        right: (index === 1 || index === 2) ? parent.right : undefined
                        top: (index === 0 || index === 1) ? parent.top : undefined
                        bottom: (index === 2 || index === 3) ? parent.bottom : undefined
                    }

                    MouseArea {
                        id: resizeHandle
                        anchors.fill: parent
                        cursorShape: Qt.SizeFDiagCursor

                        onPressed: {
                            rect.dragStartPos = Qt.point(mouse.x, mouse.y);
                            rect.isResizing = true; // Disable dragging while resizing
                        }

                        onReleased: {
                            rect.isResizing = false; // Enable dragging after resizing
                        }

                        onPositionChanged: {
                            if (rect.enableResizing) {
                                let dx = mouse.x - rect.dragStartPos.x;
                                let dy = mouse.y - rect.dragStartPos.y;

                                if (index === 0) { // Top-left
                                    rect.width = Math.max(rect.width - dx, rect.minSize);
                                    rect.height = Math.max(rect.height - dy, rect.minSize);
                                    rect.x += dx;
                                    rect.y += dy;
                                } else if (index === 1) { // Top-right
                                    rect.width = Math.max(rect.width + dx, rect.minSize);
                                    rect.height = Math.max(rect.height - dy, rect.minSize);
                                    rect.y += dy;
                                } else if (index === 2) { // Bottom-right
                                    rect.width = Math.max(rect.width + dx, rect.minSize);
                                    rect.height = Math.max(rect.height + dy, rect.minSize);
                                } else if (index === 3) { // Bottom-left
                                    rect.width = Math.max(rect.width - dx, rect.minSize);
                                    rect.height = Math.max(rect.height + dy, rect.minSize);
                                    rect.x += dx;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: vertebraeComponent
        Item {
            id: vertebraeMark
            width: 30
            height: 30
            property string label: "C1"

            Rectangle {
                anchors.fill: parent
                radius: width / 2
                color: "transparent"
                border.color: "white"
                border.width: 2
            }

            Text {
                anchors.centerIn: parent
                text: parent.label
                color: "white"
                font.bold: true
                font.pixelSize: 14
            }

            MouseArea {
                anchors.fill: parent
                drag.target: parent
            }
        }
    }

    Component {
        id: numComponent
        Item {
            id: numMark
            width: 30
            height: 30
            property string label: "1"

            Rectangle {
                anchors.fill: parent
                radius: width / 2
                color: "transparent"
                border.color: "white"
                border.width: 2
            }

            Text {
                anchors.centerIn: parent
                text: parent.label
                color: "white"
                font.bold: true
                font.pixelSize: 14
            }

            MouseArea {
                anchors.fill: parent
                drag.target: parent
            }
        }
    }


    Component {
        id: lineComponent
        Canvas {
            id: lineCanvas

            property real x1: Math.random() * parent.width
            property real y1: Math.random() * parent.height
            property real x2: Math.random() * parent.width
            property real y2: Math.random() * parent.height

            //cobb measure

            property bool isCobbLine: false
            property int lineIndex: -1


            property bool isSelected: false

            property real linestartX
            property real linestartY

//            width: parent.width
//            height: parent.height
            anchors.fill: parent

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);
                ctx.strokeStyle = isSelected ? "red" : "white";  // Change color when selected
                ctx.lineWidth = 2;
                ctx.beginPath();
                ctx.moveTo(x1, y1);
                ctx.lineTo(x2, y2);
                ctx.stroke();

                if (isCobbLine) {
                     ctx.fillStyle = "white";
                     ctx.font = "bold 14px Arial";
                     ctx.fillText(lineIndex === 0 ? "1" : "2",
                                 (x1 + x2)/2 + 10,
                                 (y1 + y2)/2 + 10);
                 }
            }
            function distanceToLine(x1, y1, x2, y2, px, py) {
                let A = px - x1;
                let B = py - y1;
                let C = x2 - x1;
                let D = y2 - y1;

                let dot = A * C + B * D;
                let len_sq = C * C + D * D;
                let param = (len_sq !== 0) ? (dot / len_sq) : -1;

                let xx, yy;
                if (param < 0) {
                    xx = x1;
                    yy = y1;
                } else if (param > 1) {
                    xx = x2;
                    yy = y2;
                } else {
                    xx = x1 + param * C;
                    yy = y1 + param * D;
                }

                let dx = px - xx;
                let dy = py - yy;
                return Math.sqrt(dx * dx + dy * dy);
            }

            // MouseArea for line selection
            MouseArea {
                id: lineDragArea
                anchors.fill: parent
                drag.target: parent

                property bool online

                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    // Clear previous selections before selecting this line
                    if(drawingLine==false)
                    {
                        let tolerance = 5;  // Click tolerance in pixels

                        let d = parent.distanceToLine(x1, y1, x2, y2, mouse.x, mouse.y);
                        if (d <= tolerance) {
                    for (let i = 0; i <selectedLine.length; i++) {
                        if (selectedLine[i] !== parent) {
                            selectedLine[i].isSelected = false;
                            selectedLine[i].requestPaint();
                        }
                    }
                    selectedLine = [];

                    // Select the clicked line
                    if (!parent.isSelected) {
                        parent.isSelected = true;
                        selectedLine.push(parent);
                    } else {
                        parent.isSelected = false;
                    }
                    parent.requestPaint();  // Update color
                    }
                    }
                }
                onPressed: {
                    //if (parent.isSelected) {
                    if(drawingLine==false)
                    {
                        let tolerance = 5;  // Click tolerance in pixels

                        let d = parent.distanceToLine(x1, y1, x2, y2, mouse.x, mouse.y);
                        if (d <= tolerance) {
                    linestartX = mouse.x;
                    linestartY = mouse.y;
                    //}
                            online=true;
                    }
                    }
                }

                onPositionChanged: {
                    //if (parent.isSelected) {
                    if(drawingLine==false && online==true)
                    {

                    let dx = mouse.x - linestartX;
                    let dy = mouse.y - linestartY;
                    parent.x1 += dx;
                    parent.y1 += dy;
                    parent.x2 += dx;
                    parent.y2 += dy;
                    linestartX = mouse.x;
                    linestartY = mouse.y;
                    parent.requestPaint();
                    //}
                    //var i;
                    //for (i = 0; i < rectangles.length; i++) {
                    //rectangles[i].enabled = true;
                    //}
                    //for (i = 0; i < lines.length; i++) {
                    //lines[i].enabled = true;
                    //}
                    }
                }

                onReleased: {
                    if(drawingLine==false)
                    {
                        online=false;
                    }
                }
            }
        }
    }




    TextInput {
        id: textInput
        visible: false
        anchors.centerIn: parent
        width: 200
        height: 30

        // Set text color
        color: "White"  // Set the text color here (e.g., "blue")

        // Set background color and border
        Rectangle {
            color: "white"  // Background color of the TextInput
            radius: 5           // Optional: rounded corners
            border.color: "white"  // Optional: border color
            border.width: 2
            // Optional: border width
        }

        text: enteredText
        onTextChanged: enteredText = text
        onEditingFinished: {
            // Hide text input after text is entered
            textInput.visible = false
            // Create the Text item with the entered text
            textItem.text = enteredText
            textItem.visible = true
            console.log("hello")
        }
    }


    // Text item that will display the entered text
    Text {
        id: textItem
        visible: false
        text: ""
        font.pixelSize: 20
        color: "white"
        x: textX
        y: textY



        // Dragging logic to move the text
        MouseArea {
            id: dragArea
            anchors.fill: parent
            drag.target: parent
            onReleased: {
                // Update position after moving the text
                textX = parent.x
                textY = parent.y
                console.log("hello2")

            }
        }
    }
    // Canvas to draw the polygon
    Canvas {
        id: polycanvas
        anchors.fill: parent

        onPaint: {

            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            ctx.strokeStyle = isPolygonSelected ? "red" : "blue"; // Highlight if selected
            ctx.lineWidth = 2;

            if (polygonPoints.length > 0) {
                ctx.beginPath();
                ctx.moveTo(polygonPoints[0].x, polygonPoints[0].y);

                for (var i = 1; i < polygonPoints.length; i++) {
                    ctx.lineTo(polygonPoints[i].x, polygonPoints[i].y);
                }

                if (isFinalized) {
                    ctx.lineTo(lastMousePosition.x, lastMousePosition.y);
                }
                ctx.closePath();
                ctx.stroke();
            }
        }
    }

    // Function to check if a point is inside the polygon
    function isInsidePolygon(px, py) {
        var count = 0;
        for (var i = 0; i < polygonPoints.length; i++) {
            var j = (i + 1) % polygonPoints.length;
            var x1 = polygonPoints[i].x;
            var y1 = polygonPoints[i].y;
            var x2 = polygonPoints[j].x;
            var y2 = polygonPoints[j].y;

            if (((y1 > py) !== (y2 > py)) &&
                (px < (x2 - x1) * (py - y1) / (y2 - y1) + x1)) {
                count++;
            }
        }
        return count % 2 === 1;

    }

}
