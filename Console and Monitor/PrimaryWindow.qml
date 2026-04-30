import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Window {
    id: screen1
    color: "black"
    title: "PrimaryWindow"
    property var primaryLoader: primaryloader

    // Signal to send drawing data to secondary screen
    signal drawingUpdated(var drawingData)

    // Function to receive drawing data from secondary screen
    function receiveDrawing(data) {
        console.log("Primary window received drawing");
        if (primaryloader.item && primaryloader.item.receiveDrawing) {
            primaryloader.item.receiveDrawing(data);
        }
    }

    Loader{
        id: primaryloader
        anchors.fill: parent
        source: "PrimaryLoginScreen.qml"

        onItemChanged: {
            if (item) {
                // Connect the drawing signal from loaded component
                item.drawingUpdated.connect(screen1.drawingUpdated);
            }
        }
    }
}
