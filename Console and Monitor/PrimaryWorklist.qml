import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import DrawingManager 1.0

Item {
    id: primaryworklist
    visible: true

    // Signal to notify about drawing updates
    signal drawingUpdated(var drawingData)

    // Function to receive drawing from secondary screen
    function receiveDrawing(drawingData) {
        console.log("Primary screen received drawing");
        if (imageViewerLoader.item) {
            imageViewerLoader.item.receiveDrawing(drawingData);
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: parent.width*0.005
        anchors.rightMargin: parent.width*0.005
        anchors.topMargin: parent.height*0.005
        anchors.bottomMargin: parent.height*0.005
        spacing: 4

        // Left Rectangle - Controls
        Rectangle {
            id: leftRect
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.3
            color: "#1e2a38"
            radius: 5

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                // Logo
                Image {
                    id: logo
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width * 0.6
                    height: width
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/icon/logo_pmt.png"
                }

                // Patient Info
                Rectangle {
                    width: parent.width
                    height: 100
                    color: "#2c3e50"
                    radius: 5

                    Column {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 5

                        Text {
                            text: "Patient: John Doe"
                            color: "white"
                            font.pixelSize: 14
                        }

                        Text {
                            text: "ID: 123456"
                            color: "white"
                            font.pixelSize: 14
                        }

                        Text {
                            text: "Study: C-Arm Scan"
                            color: "white"
                            font.pixelSize: 14
                        }
                    }
                }

                // Drawing Tools
                Rectangle {
                    width: parent.width
                    height: 200
                    color: "#2c3e50"
                    radius: 5

                    Column {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Text {
                            text: "Drawing Tools"
                            color: "white"
                            font.bold: true
                            font.pixelSize: 16
                        }

                        Row {
                            spacing: 5
                            anchors.horizontalCenter: parent.horizontalCenter

                            Repeater {
                                model: ["red", "green", "blue", "yellow", "white"]

                                Rectangle {
                                    width: 30
                                    height: 30
                                    radius: 15
                                    color: modelData
                                    border.width: 2
                                    border.color: DrawingManager.currentColor === modelData ? "white" : "gray"

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            DrawingManager.currentColor = modelData;
                                        }
                                    }
                                }
                            }
                        }

                        // Line width controls
                        Row {
                            spacing: 10
                            anchors.horizontalCenter: parent.horizontalCenter

                            Text {
                                text: "Width:"
                                color: "white"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Slider {
                                id: lineWidthSlider
                                width: 100
                                from: 1
                                to: 10
                                value: DrawingManager.currentLineWidth
                                onValueChanged: {
                                    DrawingManager.currentLineWidth = value;
                                }
                            }

                            Text {
                                text: Math.round(lineWidthSlider.value)
                                color: "white"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        Button {
                            text: "Clear Drawings"
                            width: parent.width
                            onClicked: {
                                if (imageViewerLoader.item) {
                                    var imageSource = imageViewerLoader.item.imageSource;
                                    DrawingManager.clearDrawingsForImage(imageSource);
                                }
                            }
                        }
                    }
                }

                // Status
                Rectangle {
                    width: parent.width
                    height: 80
                    color: "#2c3e50"
                    radius: 5

                    Column {
                        anchors.fill: parent
                        anchors.margins: 10

                        Text {
                            text: "Status: Connected"
                            color: "lightgreen"
                            font.pixelSize: 14
                        }

                        Text {
                            text: "Screen: Primary"
                            color: "lightblue"
                            font.pixelSize: 12
                        }
                    }
                }
            }
        }

        // Right Rectangle - Image Viewer
        Rectangle {
            id: rightRect
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "#1e2a38"
            radius: 10

            Loader {
                id: imageViewerLoader
                anchors.fill: parent
                anchors.margins: 10
                source: "RawImageViewer.qml"

                onLoaded: {
                    // Connect the drawing signal
                    item.drawingUpdated.connect(function(drawingData) {
                        primaryworklist.drawingUpdated(drawingData);
                    });
                }
            }
        }
    }
}
