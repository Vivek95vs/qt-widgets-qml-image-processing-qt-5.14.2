import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Window {
    id: mainWindow
    width: 1400
    height: 900
    visible: true
    title: "Image Collimation"

    Rectangle {
        anchors.fill: parent
        color: "#1e1e1e"

        RowLayout {
            anchors.fill: parent
            spacing: 10
            anchors.margins: 10

            // Image Display Area
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#2c2c2c"
                border.color: "#555"
                border.width: 2

                Rectangle {
                    id: imageContainer
                    anchors.fill: parent
                    anchors.margins: 10
                    color: "#3c3c3c"

                    // Image Display
                    Image {
                        id: imageDisplay
                        anchors.centerIn: parent
                        width: Math.min(parent.width, parent.height) * 0.9
                        height: width
                        fillMode: Image.PreserveAspectFit
                        source: collimationManager.hasImage ? collimationManager.imageData : ""
                        visible: collimationManager.hasImage

                        // Crop rectangle
                        Rectangle {
                            id: cropRect
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

                            x: (collimationManager.cropRect.x / collimationManager.originalSize.width) * parent.width
                            y: (collimationManager.cropRect.y / collimationManager.originalSize.height) * parent.height
                            width: (collimationManager.cropRect.width / collimationManager.originalSize.width) * parent.width
                            height: (collimationManager.cropRect.height / collimationManager.originalSize.height) * parent.height
                            color: "#00000000"
                            border.color: "#00ff00"
                            border.width: 3
                        }

                        // Dark overlay outside crop
                        Rectangle {
                            anchors.fill: parent
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0
                            color: "#80000000"

                            Rectangle {
                                x: cropRect.x
                                y: cropRect.y
                                width: cropRect.width
                                height: cropRect.height
                                color: "#00000000"
                            }
                        }

                        // Resize handles - Fixed with proper mouse tracking
                        // Top-Left Handle
//                        Rectangle {
//                            id: handleTL
//                            x: cropRect.x - 7
//                            y: cropRect.y - 7
//                            width: 14; height: 14; radius: 7
//                            color: "#00ff00"
//                            border.color: "white"
//                            border.width: 2

//                            MouseArea {
//                                id: mouseTL
//                                anchors.fill: parent
//                                cursorShape: Qt.SizeFDiagCursor
//                                property point pressPoint: Qt.point(0, 0)

//                                onPressed: {
//                                    pressPoint = Qt.point(mouseX, mouseY)
//                                    var globalX = handleTL.x + pressPoint.x
//                                    var globalY = handleTL.y + pressPoint.y
//                                    collimationManager.startResize(0, globalX, globalY,
//                                        imageDisplay.width, imageDisplay.height)
//                                }
//                                onPositionChanged: {
//                                    if (pressed) {
//                                        var globalX = handleTL.x + mouseX
//                                        var globalY = handleTL.y + mouseY
//                                        collimationManager.updateResize(globalX, globalY)
//                                    }
//                                }
//                                onReleased: {
//                                    collimationManager.endResize()
//                                }
//                            }
//                        }

                        // Top Handle
                        Rectangle {
                            id: handleT
                            x: cropRect.x + cropRect.width/2 - 7
                            y: cropRect.y - 7
                            width: 14; height: 14; radius: 7
                            color: "#00ff00"
                            border.color: "white"
                            border.width: 2

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.SizeVerCursor
                                property point pressPoint: Qt.point(0, 0)

                                onPressed: {
                                    pressPoint = Qt.point(mouseX, mouseY)
                                    collimationManager.startResize(1, handleT.x + mouseX, handleT.y + mouseY,
                                        imageDisplay.width, imageDisplay.height)
                                }
                                onPositionChanged: {
                                    if (pressed) {
                                        collimationManager.updateResize(handleT.x + mouseX, handleT.y + mouseY)
                                    }
                                }
                                onReleased: {
                                    collimationManager.endResize()
                                }
                            }
                        }

                        // Top-Right Handle
//                        Rectangle {
//                            id: handleTR
//                            x: cropRect.x + cropRect.width - 7
//                            y: cropRect.y - 7
//                            width: 14; height: 14; radius: 7
//                            color: "#00ff00"
//                            border.color: "white"
//                            border.width: 2

//                            MouseArea {
//                                anchors.fill: parent
//                                cursorShape: Qt.SizeFDiagCursor
//                                property point pressPoint: Qt.point(0, 0)

//                                onPressed: {
//                                    pressPoint = Qt.point(mouseX, mouseY)
//                                    collimationManager.startResize(2, handleTR.x + mouseX, handleTR.y + mouseY,
//                                        imageDisplay.width, imageDisplay.height)
//                                }
//                                onPositionChanged: {
//                                    if (pressed) {
//                                        collimationManager.updateResize(handleTR.x + mouseX, handleTR.y + mouseY)
//                                    }
//                                }
//                                onReleased: {
//                                    collimationManager.endResize()
//                                }
//                            }
//                        }

                        // Right Handle
                        Rectangle {
                            id: handleR
                            x: cropRect.x + cropRect.width - 7
                            y: cropRect.y + cropRect.height/2 - 7
                            width: 14; height: 14; radius: 7
                            color: "#00ff00"
                            border.color: "white"
                            border.width: 2

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.SizeHorCursor
                                property point pressPoint: Qt.point(0, 0)

                                onPressed: {
                                    pressPoint = Qt.point(mouseX, mouseY)
                                    collimationManager.startResize(3, handleR.x + mouseX, handleR.y + mouseY,
                                        imageDisplay.width, imageDisplay.height)
                                }
                                onPositionChanged: {
                                    if (pressed) {
                                        collimationManager.updateResize(handleR.x + mouseX, handleR.y + mouseY)
                                    }
                                }
                                onReleased: {
                                    collimationManager.endResize()
                                }
                            }
                        }

                        // Bottom-Right Handle
//                        Rectangle {
//                            id: handleBR
//                            x: cropRect.x + cropRect.width - 7
//                            y: cropRect.y + cropRect.height - 7
//                            width: 14; height: 14; radius: 7
//                            color: "#00ff00"
//                            border.color: "white"
//                            border.width: 2

//                            MouseArea {
//                                anchors.fill: parent
//                                cursorShape: Qt.SizeFDiagCursor
//                                property point pressPoint: Qt.point(0, 0)

//                                onPressed: {
//                                    pressPoint = Qt.point(mouseX, mouseY)
//                                    console.log("BR Handle pressed at:", handleBR.x + mouseX, handleBR.y + mouseY)
//                                    collimationManager.startResize(4, handleBR.x + mouseX, handleBR.y + mouseY,
//                                        imageDisplay.width, imageDisplay.height)
//                                }
//                                onPositionChanged: {
//                                    if (pressed) {
//                                        console.log("BR Handle moving to:", handleBR.x + mouseX, handleBR.y + mouseY)
//                                        collimationManager.updateResize(handleBR.x + mouseX, handleBR.y + mouseY)
//                                    }
//                                }
//                                onReleased: {
//                                    collimationManager.endResize()
//                                }
//                            }
//                        }

                        // Bottom Handle
                        Rectangle {
                            id: handleB
                            x: cropRect.x + cropRect.width/2 - 7
                            y: cropRect.y + cropRect.height - 7
                            width: 14; height: 14; radius: 7
                            color: "#00ff00"
                            border.color: "white"
                            border.width: 2

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.SizeVerCursor
                                property point pressPoint: Qt.point(0, 0)

                                onPressed: {
                                    pressPoint = Qt.point(mouseX, mouseY)
                                    collimationManager.startResize(5, handleB.x + mouseX, handleB.y + mouseY,
                                        imageDisplay.width, imageDisplay.height)
                                }
                                onPositionChanged: {
                                    if (pressed) {
                                        collimationManager.updateResize(handleB.x + mouseX, handleB.y + mouseY)
                                    }
                                }
                                onReleased: {
                                    collimationManager.endResize()
                                }
                            }
                        }

                        // Bottom-Left Handle
//                        Rectangle {
//                            id: handleBL
//                            x: cropRect.x - 7
//                            y: cropRect.y + cropRect.height - 7
//                            width: 14; height: 14; radius: 7
//                            color: "#00ff00"
//                            border.color: "white"
//                            border.width: 2

//                            MouseArea {
//                                anchors.fill: parent
//                                cursorShape: Qt.SizeFDiagCursor
//                                property point pressPoint: Qt.point(0, 0)

//                                onPressed: {
//                                    pressPoint = Qt.point(mouseX, mouseY)
//                                    collimationManager.startResize(6, handleBL.x + mouseX, handleBL.y + mouseY,
//                                        imageDisplay.width, imageDisplay.height)
//                                }
//                                onPositionChanged: {
//                                    if (pressed) {
//                                        collimationManager.updateResize(handleBL.x + mouseX, handleBL.y + mouseY)
//                                    }
//                                }
//                                onReleased: {
//                                    collimationManager.endResize()
//                                }
//                            }
//                        }

                        // Left Handle
                        Rectangle {
                            id: handleL
                            x: cropRect.x - 7
                            y: cropRect.y + cropRect.height/2 - 7
                            width: 14; height: 14; radius: 7
                            color: "#00ff00"
                            border.color: "white"
                            border.width: 2

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.SizeHorCursor
                                property point pressPoint: Qt.point(0, 0)

                                onPressed: {
                                    pressPoint = Qt.point(mouseX, mouseY)
                                    collimationManager.startResize(7, handleL.x + mouseX, handleL.y + mouseY,
                                        imageDisplay.width, imageDisplay.height)
                                }
                                onPositionChanged: {
                                    if (pressed) {
                                        collimationManager.updateResize(handleL.x + mouseX, handleL.y + mouseY)
                                    }
                                }
                                onReleased: {
                                    collimationManager.endResize()
                                }
                            }
                        }

                        // Move area (center of rectangle)
                        MouseArea {
                            id: moveArea
                            anchors.fill: cropRect
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0
                            cursorShape: Qt.SizeAllCursor

                            property point startPos: Qt.point(0, 0)

                            onPressed: {
                                startPos = Qt.point(mouseX, mouseY)
                                collimationManager.startMove(cropRect.x + mouseX, cropRect.y + mouseY,
                                                            imageDisplay.width, imageDisplay.height)
                            }
                            onPositionChanged: {
                                if (pressed) {
                                    collimationManager.updateMove(cropRect.x + mouseX, cropRect.y + mouseY)
                                }
                            }
                            onReleased: {
                                collimationManager.endMove()
                            }
                        }
                    }

                    // Instructions
                    Text {
                        anchors.centerIn: parent
//                        text: collimationManager.hasImage ?
//                              " DRAG green handles → Resize collimation\n" +
//                              "DRAG center → Move collimation\n\n" +
//                              "Current Size: " + collimationManager.cropRect.width.toFixed(1) + " × " +
//                              collimationManager.cropRect.height.toFixed(1) + " cm" :
//                              "Loading D:/DR.raw...\n3072×3072 16-bit image"
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 16
                    }
                }
            }

            // Control Panel (same as before)
            Rectangle {
                Layout.preferredWidth: 350
                Layout.fillHeight: true
                color: "#2c2c2c"
                border.color: "#555"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15

                    Text {
                        text: "Collimation Controls"
                        font.pixelSize: 20
                        font.bold: true
                        color: "white"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Button {
                        text: "Reset Collimation"
                        onClicked: collimationManager.resetCollimation()
                        Layout.fillWidth: true
                        enabled: collimationManager.hasImage
                    }

                    GroupBox {
                        title: "Image Information"
                        Layout.fillWidth: true

                        ColumnLayout {
                            Text { text: "File: D:/DR.raw"; color: "white" }
                            Text { text: "Size: 3072 × 3072 pixels"; color: "white" }
                            Text { text: "Physical: 42.7 × 42.7 cm"; color: "white" }
                            Text { text: "FID: 1200 mm"; color: "white" }
                        }
                    }

                    GroupBox {
                        title: "Current Collimation"
                        Layout.fillWidth: true

                        ColumnLayout {
                            Text {
                                text: "Crop Position: (" + collimationManager.cropRect.x.toFixed(1) + ", " +
                                      collimationManager.cropRect.y.toFixed(1) + ") cm"
                                color: "#00ff00"
                            }
                            Text {
                                text: "Crop Size: " + collimationManager.cropRect.width.toFixed(1) + " × " +
                                      collimationManager.cropRect.height.toFixed(1) + " cm"
                                color: "#00ff00"
                                font.bold: true
                            }
                            Text {
                                text: "Collimation X: " + collimationManager.collimationX.toFixed(2) + " cm"
                                color: "#ffff00"
                                font.bold: true
                            }
                            Text {
                                text: "Collimation Y: " + collimationManager.collimationY.toFixed(2) + " cm"
                                color: "#ffff00"
                                font.bold: true
                            }
                        }
                    }

                }
            }
        }
    }
}
