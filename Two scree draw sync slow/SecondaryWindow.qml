import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Window {
    id: screen2
    color: "black"
    title: "SecondaryWindow"
    property var secondaryLoader: secondaryloader
    property var drawingManager: null

    // Signal to send drawing data to primary screen
    signal drawingUpdated(var drawingData)

    // Function to receive drawing data from primary screen
    function receiveDrawing(data) {
        console.log("Secondary window received drawing data");
        if (secondaryloader.item && secondaryloader.item.receiveDrawing) {
            secondaryloader.item.receiveDrawing(data);
        }
    }

    Loader{
        id: secondaryloader
        anchors.fill: parent
        source: "SecondaryLoginScreen.qml"

        onItemChanged: {
            if (item) {
                // Connect the drawing signal from loaded component
                item.drawingUpdated.connect(screen2.drawingUpdated);
                // Pass drawing manager to loaded component
                if (drawingManager) {
                    item.drawingManager = drawingManager;
                }
            }
        }
    }
}
