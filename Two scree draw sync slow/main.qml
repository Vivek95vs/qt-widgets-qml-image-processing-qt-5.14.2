import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import DrawingManager 1.0

Item {
    id: mainitem
    property string stupidtext: "stupid"
    property var primaryWindowInstance: null
    property var secondaryWindowInstance: null

    // Access the singleton directly
    property var drawingManager: DrawingManager

    QtObject {
        Component.onCompleted: {
            var screens = Qt.application.screens;
            console.log("Detected screens:", screens.length);
            console.log("Screen 1 resolution:", screens[0].width + "x" + screens[0].height);
            if (screens.length > 1) {
                console.log("Screen 2 resolution:", screens[1].width + "x" + screens[1].height);
            }

            // --- Screen 1 ---
            var comp1 = Qt.createComponent("PrimaryWindow.qml");
            if (comp1.status === Component.Ready) {
                var win1 = comp1.createObject(null);
                win1.screen = screens[0];
                win1.showFullScreen();
                primaryWindowInstance = win1;
                console.log("Screen1 loaded on:", win1.screen.name, "Size:", win1.width + "x" + win1.height);

                // Connect drawing manager updates to primary window
                drawingManager.drawingsUpdated.connect(function() {
                    if (win1.primaryLoader && win1.primaryLoader.item && win1.primaryLoader.item.updateDrawings) {
                        win1.primaryLoader.item.updateDrawings();
                    }
                });
            } else {
                console.log("❌ Screen1 load failed:", comp1.errorString());
            }

            // --- Screen 2 ---
            if (screens.length > 1) {
                var comp2 = Qt.createComponent("SecondaryWindow.qml");
                if (comp2.status === Component.Ready) {
                    var win2 = comp2.createObject(null);
                    win2.screen = screens[1];
                    win2.showFullScreen();
                    secondaryWindowInstance = win2;
                    console.log("Screen2 loaded on:", win2.screen.name, "Size:", win2.width + "x" + win2.height);

                    // Connect drawing manager updates to secondary window
                    drawingManager.drawingsUpdated.connect(function() {
                        if (win2.secondaryLoader && win2.secondaryLoader.item && win2.secondaryLoader.item.updateDrawings) {
                            win2.secondaryLoader.item.updateDrawings();
                        }
                    });

                    // Connect the drawing signals between windows
                    if (primaryWindowInstance && win2) {
                        primaryWindowInstance.drawingUpdated.connect(win2.receiveDrawing);
                        win2.drawingUpdated.connect(primaryWindowInstance.receiveDrawing);
                        console.log("Drawing signals connected between screens");
                    }
                } else {
                    console.log("❌ Screen2 load failed:", comp2.errorString());
                }
            } else {
                console.log("⚠️ Only one display detected — Screen2 will not open.");
            }
        }
    }
}
