import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Shapes 1.12
import QtQuick.Layouts 1.0
import QtQuick.Window 2.12

ApplicationWindow {
    visible: true
    width: 800
    height: 800
    title: "Draw and Delete Rectangles"

    Component.onCompleted:
    {
        serverController.startServer()
    }

    property bool drawingrectangle: false
    property bool drawingLine: false
    property bool drawpolygon: false // Indicates if the polygon is finalized
    property var rectangles: []
    property var lines: []

    property var selectedShapes: []
    property var selectedLine:[]

    property real textX: 100
    property real textY: 100
    property string enteredText: ""

    //polygon
    property var polygonPoints: [] // Stores the points of the polygon
    property bool isFinalized: false // Indicates if the polygon is finalized
    property point lastMousePosition: Qt.point(0, 0) // Stores the last mouse position
    property bool isDragging: false // Indicates if the polygon is being dragged
    property point dragStartPosition: Qt.point(0, 0) // Stores the position where drag started
    property bool isPolygonSelected: false // Indicates if the polygon is selected

    //cobb angle:

    property bool measuringCobbAngle: false
    property var cobbAngleLines: []
    property real cobbAngle: 0
    property int currentCobbLine: 0  // To track which line we're drawing (0 or 1)


    //vertebra mark
    property bool markingVertebrae: false
    property int vertebraeCount: 1  // Starts with C1

    //Num mark
    property bool markingNum: false
    property int numCount: 1  // Starts with 1


    signal signsample()
    Connections {
        target: imageController

        onImageUpdated:{
            console.log("Image updated successfully!");
            image1.source = "image://multiImageProvider/img1"
        }
    }


    //    }
    Row {
        anchors.rightMargin: 116
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        spacing: 100


        Keys.onPressed: (event) =>
                        {
                            let i;
                            if (event.key === Qt.Key_Delete && selectedShapes.length > 0) {
                                for (i = 0; i < selectedShapes.length; i++) {
                                    selectedShapes[i].destroy();  // Remove selected item
                                    console.log("deleted")
                                }
                                selectedShapes = [];  // Clear selection
                            }
                            if (event.key === Qt.Key_Delete && selectedLine.length > 0) {
                                for (i = 0; i < selectedLine.length; i++) {
                                    selectedLine[i].destroy();  // Remove selected item
                                    console.log("deleted")
                                }
                                selectedLine = [];  // Clear selection
                            }
                            if (event.key === Qt.Key_Delete && isPolygonSelected ) {
                                console.log("Deleting polygon...");
                                polygonPoints = [];
                                isPolygonSelected = false;
                                isFinalized = false;
                                mouseOperations.polyCanvas.requestPaint();
                            }
                        }

        Rectangle {
            id: clipArea
            width: parent.width/2
            height: parent.parent.height
            anchors.top: parent.top
            color: "white"
            border.color: "black"
            border.width: 2
            clip: true  // Enables clipping


            Item {
                id: imageContainer
                width: parent.parent.width
                height: parent.parent.height
                anchors.centerIn: parent
                // Handle Delete key to remove selected items

                transform: Scale {
                    id: imageScale
                    origin.x: imageContainer.width / 2
                    origin.y: imageContainer.height / 2
                    xScale: 1.0
                    yScale: 1.0
                }

                Image {

                    id: image1
                    anchors.centerIn: parent
                    width: parent.parent.width * 0.97
                    height: parent.parent.height * 0.97
                    source: "image://multiImageProvider/img1"
                    Mouseoperations {
                                    id: mouseOperations
                                    anchors.fill: parent
                    }

                }

                /* ---------- HUD: Cobb angle ---------- */
                Text {
                    id: angleDisplay
                    visible: cobbAngle > 0
                    text: ""
                    font.pixelSize: 16
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    z: 10  // Make sure it's above other elements
                }

            }

        }
        Column
        {
            anchors.top: parent.top

            Row
            {
                spacing: 20
                Column
                {
                    anchors.top: parent.top
                    spacing: 10
                    Button {
                        text: "Load RAW Image"
                        onClicked: {
                            imageController.loadRawImage();
                            image1.source = "";  // Force refresh
                            image1.source = "image://multiImageProvider/img1";

                        }
                    }

                    Button {
                        text: "Draw Retcangle"
                        onClicked: {
                            drawingrectangle = true;
                            drawingLine = false;
                            drawpolygon = false;
                            markingNum = false;
                            measuringCobbAngle = false;
                            markingVertebrae = false;
                        //Button.text= imageController.hello;
                            console.log("Hello, this is a line printed in QML.")
                        }

                    }

                    Button {
                        text: "Draw Line"
                        onClicked: {
                            drawingrectangle = false;
                            drawingLine = true;
                            drawpolygon = false;
                            markingNum = false;
                            markingVertebrae = false;
                            measuringCobbAngle = false;

                        }
                    }
                    Button{
                        text: "Draw polygon"
                        onClicked: {
                            drawingrectangle = false;
                            drawingLine = false;
                            drawpolygon = true;
                            markingNum = false;
                            markingVertebrae = false;
                            measuringCobbAngle = false;

                                polygonPoints = [];
                                isFinalized = true;
                                isPolygonSelected = false;
                        }
                    }


                    Button {
                        text: "Vertical flip"
                        onClicked: {
                            imageController.flipImage_vertically();
                            image1.source = "";  // Force refresh
                            image1.source = "image://multiImageProvider/img1";

                        }
                    }
                    Button {
                        text: "Horizontal flip"
                        onClicked: {
                            imageController.flipImage_horizontally();
                            image1.source = "";  // Force refresh
                            image1.source = "image://multiImageProvider/img1";
                        }
                    }
                    Button{
                        text: "Text"
                        onClicked: {
                            // Trigger to show TextInput
                            enteredText = ""
                            textInput.visible = true
                            textInput.focus = true
                        }
                    }

                    Button {
                        text: "Cobb Angle"
                        onClicked: {
                            drawingrectangle = false;
                            drawingLine = false;
                            drawpolygon = false;
                            markingNum = false;
                            markingVertebrae = false;
                            measuringCobbAngle = true;
                            currentCobbLine = 0;
//                            // Clear previous measurements
                            cobbAngleLines.forEach(line => line.destroy());
                            cobbAngleLines = [];
                            cobbAngle = 0;
                        }
                    }

                    Button {
                        text: "Mark Vertebrae"
                        onClicked: {
                            drawingrectangle = false;
                            drawingLine = false;
                            drawpolygon = false;
                            markingNum = false;
                            measuringCobbAngle = false;
                            markingVertebrae = true;
                            vertebraeCount = 1; // Reset counter when starting
                        }
                    }

                    Button {
                        text: "Number Mark"
                        onClicked: {
                            drawingrectangle = false;
                            drawingLine = false;
                            drawpolygon = false;
                            markingVertebrae = false;
                            measuringCobbAngle = false;
                            markingNum = true;
                            numCount = 1; // Reset counter when starting
                        }
                    }



                    Button{
                        text: "Histogram"
                        onClicked: {
                        }
                    }

                    Button {
                        text: "Delete"

                        onClicked: {
                            // Delete selected rectangles
                            selectedShapes.forEach(function(shape) {
                                if (shape.destroy) {
                                    shape.destroy();
                                }
                            });
                            rectangles = rectangles.filter(shape => !selectedShapes.includes(shape));
                            lines = lines.filter(shape => !selectedShapes.includes(shape));
                            selectedShapes = [];  // Clear selected shapes

                            //Delete selected line
                            selectedLine.forEach(function(shape) {
                                if (shape.destroy) {
                                    shape.destroy();
                                }
                            });
                            rectangles = rectangles.filter(shape => !selectedLine.includes(shape));
                            lines = lines.filter(shape => !selectedLine.includes(shape));
                            selectedLine = [];  // Clear selected shapes

                            //Delete selected poligon
                            isFinalized = false;
                            polygonPoints = [];
                            isPolygonSelected = false;
                            console.log("Before requestPaint()");
                             mouseOperations.polyCanvas.requestPaint();  // Trigger canvas repaint
                             console.log("After requestPaint()");

                            //cobb delete
                            cobbAngleLines.forEach(function(shape) {
                                if (shape.destroy) {
                                    shape.destroy();
                                }
                            });
                            cobbAngleLines = [];
                            cobbAngle = 0;


                        }
                    }
                    Button {
                        text: "Edge Enhancement"
                        onClicked: {
                            imageController.enhanceEdges()
                            image1.source = "";  // Force image reload
                            image1.source = "image://multiImageProvider/img1"
                        }
                    }
                    Button {
                        text: "Filter"
                        onClicked: {
                            imageController.medianFilter()
                            image1.source = "";  // Force image reload
                            image1.source = "image://multiImageProvider/img1"
                        }
                    }
                    Button {
                        text: "Scatter Correction"
                        onClicked: {
                            imageController.scatterCorrection()
                            image1.source = "";  // Force image reload
                            image1.source = "image://multiImageProvider/img1"
                        }
                    }
                    Button{
                        text: "Delete All"
                        onClicked: {
                            // Delete all shapes (rectangles and lines)
                            rectangles.forEach(function(shape) {
                                if (shape.destroy) {
                                    shape.destroy();
                                }
                            });
                            lines.forEach(function(shape) {
                                if (shape.destroy) {
                                    shape.destroy();
                                }
                            });

                            // Clear the arrays
                            rectangles = [];
                            lines = [];

                            //Delete selected poligon
                            isFinalized = false;
                            polygonPoints = [];
                            isPolygonSelected = false;
                            console.log("Before requestPaint()");
                             mouseOperations.polyCanvas.requestPaint();  // Trigger canvas repaint
                             console.log("After requestPaint()");

                            // Delete Cobb angle lines
                            cobbAngleLines.forEach(function(shape) {
                                if (shape.destroy) {
                                    shape.destroy();
                                }
                            });
                            cobbAngleLines = [];
                            cobbAngle = 0;



                        }
                    }
                }
                Column
                {
                    Row
                    {
                        Text { text: "Brightness"; font.bold: true }
                        Slider {
                            id: brightnessSlider
                            from: -100
                            to: 100
                            value: 0
                            //onValueChanged: imageController.updateImage(brightnessSlider.value, contrastSlider.value)
                        }
                    }
                    Row
                    {
                        Text { text: "Contrast"; font.bold: true }
                        Slider {
                            id: contrastSlider
                            from: 0.5
                            to: 3.0
                            value: 1.0
                            stepSize: 0.1
                            //onValueChanged: imageController.updateImage(brightnessSlider.value, contrastSlider.value)


                        }
                    }
                }

            }


        }
    }

}
