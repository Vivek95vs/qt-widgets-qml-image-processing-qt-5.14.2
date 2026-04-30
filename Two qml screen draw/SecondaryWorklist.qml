import QtQuick 2.12

Item {
    id: secondaryworklist

    // Signal to notify about drawing updates
    signal drawingUpdated(var drawingData)

    // Function to receive drawing from primary screen
    function receiveDrawing(drawingData) {
        console.log("Secondary screen received drawing:", drawingData);
        if (drawingData.type === "clear") {
            if (imageViewerLoader.item) {
                imageViewerLoader.item.clearDrawings();
            }
        } else if (imageViewerLoader.item) {
            imageViewerLoader.item.receiveDrawing(drawingData);
        }
    }

    property string currentTime: Qt.formatDateTime(new Date(), "dd-MM-yyyy hh:mm:ss")
    property string currentDate: Qt.formatDateTime(new Date(), "yyyyMMdd")

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            currentTime = Qt.formatDateTime(new Date(), "dd-MM-yyyy hh:mm:ss")
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        Column {
            anchors.fill: parent
            spacing: 5

            // Header
            Rectangle {
                id: header
                width: parent.width
                height: parent.height * 0.07
                color: "#1e2a38"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 5

                    // Storage info
                    Rectangle {
                        width: parent.width * 0.2
                        height: parent.height
                        color: "transparent"
                        border.color: "#f5f5f5"
                        border.width: 1
                        radius: 10

                        Row {
                            anchors.centerIn: parent
                            spacing: 5

                            Image {
                                width: parent.height * 0.6
                                height: parent.height * 0.6
                                source: "qrc:/icon/13'/Storage.png"
                                fillMode: Image.PreserveAspectFit
                            }

                            Text {
                                text: "dummy/dummy"
                                color: "white"
                                font.pixelSize: parent.height * 0.3
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }

                    // Live data
                    Rectangle {
                        width: parent.width * 0.43
                        height: parent.height
                        color: "#5a6e80"
                        radius: 10

                        Row {
                            anchors.centerIn: parent
                            spacing: 10

                            Repeater {
                                model: [
                                    {icon: "qrc:/icon/13'/C-arm Motions/Orbital.png", value: "90"},
                                    {icon: "qrc:/icon/13'/C-arm Motions/Angular.png", value: "0"},
                                    {icon: "qrc:/icon/13'/C-arm Motions/Vertical.png", value: "20cm"},
                                    {icon: "qrc:/icon/13'/C-arm Motions/Horizontal.png", value: "20cm"},
                                    {icon: "qrc:/icon/13'/C-arm Motions/Detector Vertical.png", value: "25cm"}
                                ]

                                Row {
                                    spacing: 5
                                    anchors.verticalCenter: parent.verticalCenter

                                    Image {
                                        width: 30
                                        height: 30
                                        source: modelData.icon
                                        fillMode: Image.PreserveAspectFit
                                    }

                                    Text {
                                        text: modelData.value
                                        color: "white"
                                        font.pixelSize: 14
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                        }
                    }

                    // Tool icons
                    Rectangle {
                        width: parent.width * 0.35
                        height: parent.height
                        color: "transparent"

                        Row {
                            anchors.centerIn: parent
                            spacing: 15

                            Repeater {
                                model: [
                                    "qrc:/icon/13'/Home Disabled.png",
                                    "qrc:/icon/13'/Modes Menu.png",
                                    "qrc:/icon/13'/Images Disabled.png",
                                    "qrc:/icon/13'/Processing.png",
                                    "qrc:/icon/13'/CBCT.png",
                                    "qrc:/icon/13'/Settings.png"
                                ]

                                Image {
                                    width: 30
                                    height: 30
                                    source: modelData
                                    fillMode: Image.PreserveAspectFit
                                }
                            }
                        }
                    }
                }
            }

            // Main content area
            Rectangle {
                id: contentArea
                width: parent.width
                height: parent.height * 0.88
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
                            secondaryworklist.drawingUpdated(drawingData);
                        });
                    }
                }
            }

            // Footer
            Rectangle {
                width: parent.width
                height: parent.height * 0.025
                color: "transparent"

                Row {
                    anchors.fill: parent

                    Text {
                        text: "Software Version: " + appVersion
                        font.pixelSize: 12
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: "Licensed To: " + hospitalname
                        font.pixelSize: 12
                        color: "white"
                        anchors.centerIn: parent
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        id: currenttime
                        text: currentTime
                        font.pixelSize: 12
                        color: "white"
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}
