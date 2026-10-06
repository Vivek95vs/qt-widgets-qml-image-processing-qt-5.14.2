import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Rectangle {
    id: root

    color: "#000000"
    border.color: "#29384B"
    border.width: 1
    clip: true

    property int imageRevision: 0

    Connections {
        target: dicomLoader

        function onImageChanged() {
            root.imageRevision++

            console.log(
                "ImageViewer: image updated. Revision:",
                root.imageRevision
            )
        }
    }

    // =========================================================
    // EMPTY STATE
    // =========================================================

    Column {
        anchors.centerIn: parent

        spacing: 8

        visible: !dicomLoader.loaded

        Text {
            anchors.horizontalCenter: parent.horizontalCenter

            text: "NO IMAGE"

            color: "#64748B"

            font.pixelSize: 18
            font.bold: true
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter

            text: "Open a DICOM image to start viewing"

            color: "#64748B"

            font.pixelSize: 12
        }
    }

    // =========================================================
    // SINGLE IMAGE VIEWPORT
    // =========================================================

    Item {
        id: imageViewport

        anchors.fill: parent

        visible: dicomLoader.loaded

        clip: true

        // =====================================================
        // DICOM IMAGE
        // =====================================================

        Image {
            id: dicomImage

            anchors.fill: parent

            anchors.leftMargin: 8
            anchors.rightMargin: 8
            anchors.topMargin: 8
            anchors.bottomMargin: 8

            source: dicomLoader.loaded
                    ? "image://dicom/image?revision="
                      + root.imageRevision
                    : ""

            fillMode: Image.PreserveAspectFit

            smooth: false
            mipmap: false
            cache: false
            asynchronous: false

            onStatusChanged: {
                if (status === Image.Ready) {
                    console.log(
                        "ImageViewer: image loaded successfully"
                    )
                }

                if (status === Image.Error) {
                    console.log(
                        "ImageViewer: image loading error"
                    )
                }
            }
        }

        // =====================================================
        // TOP LEFT
        // DICOM IMAGE INFORMATION
        // =====================================================

        Column {
            id: topLeftInfo

            anchors.left: parent.left
            anchors.top: parent.top

            anchors.leftMargin: 16
            anchors.topMargin: 12

            spacing: 3

            Text {
                text: "DICOM IMAGE"

                color: "#FFFFFF"

                font.pixelSize: 13
                font.bold: true
            }

            Text {
                text: "Image: " +
                      dicomLoader.imageWidth +
                      " × " +
                      dicomLoader.imageHeight

                color: "#B8C4D1"

                font.pixelSize: 11
            }
        }

        // =====================================================
        // TOP RIGHT
        // WINDOW / LEVEL INFORMATION
        // =====================================================

        Column {
            id: topRightInfo

            anchors.right: parent.right
            anchors.top: parent.top

            anchors.rightMargin: 16
            anchors.topMargin: 12

            spacing: 3

            Text {
                text: "IMAGE"

                color: "#69E58C"

                font.pixelSize: 11
                font.bold: true

                horizontalAlignment: Text.AlignRight

                anchors.right: parent.right
            }

            Text {
                text: "WL: 65535 / 65535"

                color: "#69E58C"

                font.pixelSize: 11

                horizontalAlignment: Text.AlignRight

                anchors.right: parent.right
            }

            Text {
                text: "Zoom: 100%"

                color: "#69E58C"

                font.pixelSize: 11

                horizontalAlignment: Text.AlignRight

                anchors.right: parent.right
            }
        }

        // =====================================================
        // LEFT ORIENTATION MARKER
        // =====================================================

        Text {
            id: leftMarker

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            anchors.leftMargin: 18

            text: "L"

            color: "#FFFFFF"

            font.pixelSize: 16
            font.bold: true
        }

        // =====================================================
        // RIGHT ORIENTATION MARKER
        // =====================================================

        Text {
            id: rightMarker

            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            anchors.rightMargin: 18

            text: "R"

            color: "#FFFFFF"

            font.pixelSize: 16
            font.bold: true
        }

        // =====================================================
        // LEFT SIDE SCALE
        // =====================================================

        Rectangle {
            id: leftScale

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            anchors.leftMargin: 10

            width: 1
            height: 70

            color: "#7A8795"

            opacity: 0.8
        }

        // =====================================================
        // RIGHT SIDE SCALE
        // =====================================================

        Rectangle {
            id: rightScale

            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            anchors.rightMargin: 10

            width: 1
            height: 70

            color: "#7A8795"

            opacity: 0.8
        }

        // =====================================================
        // BOTTOM RIGHT TOOLBAR
        // =====================================================

        Row {
            id: toolBar

            anchors.right: parent.right
            anchors.bottom: parent.bottom

            anchors.rightMargin: 14
            anchors.bottomMargin: 14

            spacing: 5

            // -------------------------------------------------
            // WINDOW / LEVEL
            // -------------------------------------------------

            Rectangle {
                width: 34
                height: 28

                radius: 3

                color: "#18222D"

                border.color: "#344454"
                border.width: 1

                Text {
                    anchors.centerIn: parent

                    text: "☼"

                    color: "#D6DEE8"

                    font.pixelSize: 15
                }
            }

            // -------------------------------------------------
            // ZOOM
            // -------------------------------------------------

            Rectangle {
                width: 42
                height: 28

                radius: 3

                color: "#18222D"

                border.color: "#344454"
                border.width: 1

                Text {
                    anchors.centerIn: parent

                    text: "100%"

                    color: "#D6DEE8"

                    font.pixelSize: 10
                }
            }

            // -------------------------------------------------
            // 1:1
            // -------------------------------------------------

            Rectangle {
                width: 38
                height: 28

                radius: 3

                color: "#18222D"

                border.color: "#344454"
                border.width: 1

                Text {
                    anchors.centerIn: parent

                    text: "1:1"

                    color: "#D6DEE8"

                    font.pixelSize: 10
                }
            }

            // -------------------------------------------------
            // W/L
            // -------------------------------------------------

            Rectangle {
                width: 42
                height: 28

                radius: 3

                color: "#18222D"

                border.color: "#344454"
                border.width: 1

                Text {
                    anchors.centerIn: parent

                    text: "W/L"

                    color: "#D6DEE8"

                    font.pixelSize: 10
                }
            }

            // -------------------------------------------------
            // CINE
            // -------------------------------------------------

            Rectangle {
                width: 46
                height: 28

                radius: 3

                color: "#18222D"

                border.color: "#344454"
                border.width: 1

                Text {
                    anchors.centerIn: parent

                    text: "CINE"

                    color: "#D6DEE8"

                    font.pixelSize: 10
                }
            }
        }


 }
}
