import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import DrawingManager 1.0

Item {
    id: secondaryworklist
    visible: true

    // Signal to notify about drawing updates
    signal drawingUpdated(var drawingData)

    // Function to receive drawing from primary screen
    function receiveDrawing(drawingData) {
        console.log("Secondary screen received drawing");
        if (imageViewerLoader.item) {
            imageViewerLoader.item.receiveDrawing(drawingData);
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#1e2a38"

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

        // Status overlay
        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            width: 200
            height: 60
            color: "#2c3e50"
            radius: 5
            opacity: 0.8

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 5

                Text {
                    text: "Status: Connected"
                    color: "lightgreen"
                    font.pixelSize: 14
                }

                Text {
                    text: "Screen: Secondary"
                    color: "lightblue"
                    font.pixelSize: 12
                }
            }
        }
    }
}
