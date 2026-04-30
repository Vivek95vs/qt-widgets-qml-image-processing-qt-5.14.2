import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Window {
    id: mainWindow
    width: 1400
    height: 900
    visible: true
    title: "Medical Image Collimation Control"

    Rectangle {
        anchors.fill: parent
        color: "#1e1e1e"

        RowLayout {
            anchors.fill: parent
            spacing: 10
            anchors.margins: 10

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
                    color: "#000000"  // Black background

                    Item {
                        id: imageWrapper
                        anchors.centerIn: parent
                        width: Math.min(parent.width, parent.height) * 0.9
                        height: width

                        // Only show the cropped area
                        Rectangle {
                            id: visibleArea
                            x: (collimationManager.cropRect.x / 42.7) * parent.width
                            y: (collimationManager.cropRect.y / 42.7) * parent.height
                            width: (collimationManager.cropRect.width / 42.7) * parent.width
                            height: (collimationManager.cropRect.height / 42.7) * parent.height
                            clip: true
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

                            Image {
                                x: -visibleArea.x
                                y: -visibleArea.y
                                width: imageWrapper.width
                                height: imageWrapper.height
                                source: collimationManager.hasImage ? collimationManager.imageData : ""
                                fillMode: Image.PreserveAspectFit
                            }

                            // Green border
                            Rectangle {
                                anchors.fill: parent
                                color: "#00000000"
                                border.color: "#00ff00"
                                border.width: 3
                            }
                        }

                        // Resize Handles (visible on top of black area)
                        // Top-Left Handle
//                        Rectangle {
//                            id: h0
//                            x: (collimationManager.cropRect.x / 42.7) * parent.width - 8
//                            y: (collimationManager.cropRect.y / 42.7) * parent.height - 8
//                            width: 16; height: 16; radius: 8
//                            color: "#00ff00"; border.color: "white"; border.width: 2
//                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

//                            MouseArea {
//                                anchors.fill: parent; anchors.margins: -4
//                                cursorShape: Qt.SizeFDiagCursor
//                                property point startPos: Qt.point(0,0)
//                                onPressed: {
//                                    startPos = Qt.point(mouseX, mouseY)
//                                    collimationManager.startResize(0, parent.x + startPos.x, parent.y + startPos.y,
//                                        imageWrapper.width, imageWrapper.height)
//                                }
//                                onPositionChanged: {
//                                    if (pressed) {
//                                        collimationManager.updateResize(parent.x + mouseX, parent.y + mouseY)
//                                    }
//                                }
//                                onReleased: { collimationManager.endResize() }
//                            }
//                        }

                        // Top Handle
                        Rectangle {
                            id: h1
                            x: (collimationManager.cropRect.x / 42.7) * parent.width + (collimationManager.cropRect.width / 42.7) * parent.width/2 - 8
                            y: (collimationManager.cropRect.y / 42.7) * parent.height - 8
                            width: 16; height: 16; radius: 8
                            color: "#00ff00"; border.color: "white"; border.width: 2
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

                            MouseArea {
                                anchors.fill: parent; anchors.margins: -4
                                cursorShape: Qt.SizeVerCursor
                                property point startPos: Qt.point(0,0)
                                onPressed: {
                                    startPos = Qt.point(mouseX, mouseY)
                                    collimationManager.startResize(1, parent.x + startPos.x, parent.y + startPos.y,
                                        imageWrapper.width, imageWrapper.height)
                                }
                                onPositionChanged: {
                                    if (pressed) {
                                        collimationManager.updateResize(parent.x + mouseX, parent.y + mouseY)
                                    }
                                }
                                onReleased: { collimationManager.endResize() }
                            }
                        }

                        // Top-Right Handle
//                        Rectangle {
//                            id: h2
//                            x: (collimationManager.cropRect.x / 42.7) * parent.width + (collimationManager.cropRect.width / 42.7) * parent.width - 8
//                            y: (collimationManager.cropRect.y / 42.7) * parent.height - 8
//                            width: 16; height: 16; radius: 8
//                            color: "#00ff00"; border.color: "white"; border.width: 2
//                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

//                            MouseArea {
//                                anchors.fill: parent; anchors.margins: -4
//                                cursorShape: Qt.SizeFDiagCursor
//                                property point startPos: Qt.point(0,0)
//                                onPressed: {
//                                    startPos = Qt.point(mouseX, mouseY)
//                                    collimationManager.startResize(2, parent.x + startPos.x, parent.y + startPos.y,
//                                        imageWrapper.width, imageWrapper.height)
//                                }
//                                onPositionChanged: {
//                                    if (pressed) {
//                                        collimationManager.updateResize(parent.x + mouseX, parent.y + mouseY)
//                                    }
//                                }
//                                onReleased: { collimationManager.endResize() }
//                            }
//                        }

                        // Right Handle
                        Rectangle {
                            id: h3
                            x: (collimationManager.cropRect.x / 42.7) * parent.width + (collimationManager.cropRect.width / 42.7) * parent.width - 8
                            y: (collimationManager.cropRect.y / 42.7) * parent.height + (collimationManager.cropRect.height / 42.7) * parent.height/2 - 8
                            width: 16; height: 16; radius: 8
                            color: "#00ff00"; border.color: "white"; border.width: 2
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

                            MouseArea {
                                anchors.fill: parent; anchors.margins: -4
                                cursorShape: Qt.SizeHorCursor
                                property point startPos: Qt.point(0,0)
                                onPressed: {
                                    startPos = Qt.point(mouseX, mouseY)
                                    collimationManager.startResize(3, parent.x + startPos.x, parent.y + startPos.y,
                                        imageWrapper.width, imageWrapper.height)
                                }
                                onPositionChanged: {
                                    if (pressed) {
                                        collimationManager.updateResize(parent.x + mouseX, parent.y + mouseY)
                                    }
                                }
                                onReleased: { collimationManager.endResize() }
                            }
                        }

                        // Bottom-Right Handle
//                        Rectangle {
//                            id: h4
//                            x: (collimationManager.cropRect.x / 42.7) * parent.width + (collimationManager.cropRect.width / 42.7) * parent.width - 8
//                            y: (collimationManager.cropRect.y / 42.7) * parent.height + (collimationManager.cropRect.height / 42.7) * parent.height - 8
//                            width: 16; height: 16; radius: 8
//                            color: "#00ff00"; border.color: "white"; border.width: 2
//                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

//                            MouseArea {
//                                anchors.fill: parent; anchors.margins: -4
//                                cursorShape: Qt.SizeFDiagCursor
//                                property point startPos: Qt.point(0,0)
//                                onPressed: {
//                                    startPos = Qt.point(mouseX, mouseY)
//                                    collimationManager.startResize(4, parent.x + startPos.x, parent.y + startPos.y,
//                                        imageWrapper.width, imageWrapper.height)
//                                }
//                                onPositionChanged: {
//                                    if (pressed) {
//                                        collimationManager.updateResize(parent.x + mouseX, parent.y + mouseY)
//                                    }
//                                }
//                                onReleased: { collimationManager.endResize() }
//                            }
//                        }

                        // Bottom Handle
                        Rectangle {
                            id: h5
                            x: (collimationManager.cropRect.x / 42.7) * parent.width + (collimationManager.cropRect.width / 42.7) * parent.width/2 - 8
                            y: (collimationManager.cropRect.y / 42.7) * parent.height + (collimationManager.cropRect.height / 42.7) * parent.height - 8
                            width: 16; height: 16; radius: 8
                            color: "#00ff00"; border.color: "white"; border.width: 2
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

                            MouseArea {
                                anchors.fill: parent; anchors.margins: -4
                                cursorShape: Qt.SizeVerCursor
                                property point startPos: Qt.point(0,0)
                                onPressed: {
                                    startPos = Qt.point(mouseX, mouseY)
                                    collimationManager.startResize(5, parent.x + startPos.x, parent.y + startPos.y,
                                        imageWrapper.width, imageWrapper.height)
                                }
                                onPositionChanged: {
                                    if (pressed) {
                                        collimationManager.updateResize(parent.x + mouseX, parent.y + mouseY)
                                    }
                                }
                                onReleased: { collimationManager.endResize() }
                            }
                        }

                        // Bottom-Left Handle
//                        Rectangle {
//                            id: h6
//                            x: (collimationManager.cropRect.x / 42.7) * parent.width - 8
//                            y: (collimationManager.cropRect.y / 42.7) * parent.height + (collimationManager.cropRect.height / 42.7) * parent.height - 8
//                            width: 16; height: 16; radius: 8
//                            color: "#00ff00"; border.color: "white"; border.width: 2
//                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

//                            MouseArea {
//                                anchors.fill: parent; anchors.margins: -4
//                                cursorShape: Qt.SizeFDiagCursor
//                                property point startPos: Qt.point(0,0)
//                                onPressed: {
//                                    startPos = Qt.point(mouseX, mouseY)
//                                    collimationManager.startResize(6, parent.x + startPos.x, parent.y + startPos.y,
//                                        imageWrapper.width, imageWrapper.height)
//                                }
//                                onPositionChanged: {
//                                    if (pressed) {
//                                        collimationManager.updateResize(parent.x + mouseX, parent.y + mouseY)
//                                    }
//                                }
//                                onReleased: { collimationManager.endResize() }
//                            }
//                        }

                        // Left Handle
                        Rectangle {
                            id: h7
                            x: (collimationManager.cropRect.x / 42.7) * parent.width - 8
                            y: (collimationManager.cropRect.y / 42.7) * parent.height + (collimationManager.cropRect.height / 42.7) * parent.height/2 - 8
                            width: 16; height: 16; radius: 8
                            color: "#00ff00"; border.color: "white"; border.width: 2
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0

                            MouseArea {
                                anchors.fill: parent; anchors.margins: -4
                                cursorShape: Qt.SizeHorCursor
                                property point startPos: Qt.point(0,0)
                                onPressed: {
                                    startPos = Qt.point(mouseX, mouseY)
                                    collimationManager.startResize(7, parent.x + startPos.x, parent.y + startPos.y,
                                        imageWrapper.width, imageWrapper.height)
                                }
                                onPositionChanged: {
                                    if (pressed) {
                                        collimationManager.updateResize(parent.x + mouseX, parent.y + mouseY)
                                    }
                                }
                                onReleased: { collimationManager.endResize() }
                            }
                        }

                        // Move area (center)
                        MouseArea {
                            id: moveArea
                            x: (collimationManager.cropRect.x / 42.7) * parent.width
                            y: (collimationManager.cropRect.y / 42.7) * parent.height
                            width: (collimationManager.cropRect.width / 42.7) * parent.width
                            height: (collimationManager.cropRect.height / 42.7) * parent.height
                            visible: collimationManager.hasImage && collimationManager.cropRect.width > 0
                            cursorShape: Qt.SizeAllCursor

                            property point startPos: Qt.point(0,0)
                            onPressed: {
                                startPos = Qt.point(mouseX, mouseY)
                                collimationManager.startMove(x + startPos.x, y + startPos.y,
                                    imageWrapper.width, imageWrapper.height)
                            }
                            onPositionChanged: {
                                if (pressed) {
                                    collimationManager.updateMove(x + mouseX, y + mouseY)
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
//                              "Drag GREEN handles → Resize collimation\n" +
//                              "Drag CENTER → Move collimation\n\n" +
//                              "Only the area inside green rectangle is visible\n" +
//                              "Size: " + collimationManager.cropRect.width.toFixed(1) + " x " +
//                              collimationManager.cropRect.height.toFixed(1) + " cm" :
//                              "Loading D:/DR.raw..."
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 14
                    }
                }
            }

            // Control Panel
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
                        text: "Reset (100%)"
                        onClicked: collimationManager.resetCollimation()
                        Layout.fillWidth: true
                    }


                    GroupBox {
                        title: "Current Values"
                        Layout.fillWidth: true

                        ColumnLayout {
                            Text {
                                text: "Crop Position: (" + collimationManager.cropRect.x.toFixed(1) + ", " +
                                      collimationManager.cropRect.y.toFixed(1) + ") cm"
                                color: "#00ff00"
                            }
                            Text {
                                text: "Crop Size: " + collimationManager.cropRect.width.toFixed(1) + " x " +
                                      collimationManager.cropRect.height.toFixed(1) + " cm"
                                color: "#00ff00"
                                font.bold: true
                            }
                            Text {
                                text: "Collimation X: " + collimationManager.collimationX.toFixed(2) + " cm"
                                color: "#ffff00"
                            }
                            Text {
                                text: "Collimation Y: " + collimationManager.collimationY.toFixed(2) + " cm"
                                color: "#ffff00"
                            }

                        }
                    }
                }
            }
        }
    }
}
