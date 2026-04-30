import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
Item {
    id: mainitem
    property string stupidtext: "stupid"
    property var primaryWindowInstance: null
    property var secondaryWindowInstance: null
QtObject {
    Component.onCompleted: {
        var screens = Qt.application.screens;
        console.log("Detected screens:", screens.length);

        // --- Screen 1 ---
        var comp1 = Qt.createComponent("PrimaryWindow.qml");
        if (comp1.status === Component.Ready) {
            var win1 = comp1.createObject(null);
            win1.screen = screens[0];           // assign BEFORE showing
            win1.showFullScreen();
            primaryWindowInstance = win1; // Store the instance
            console.log("Screen1 loaded on:", win1.screen.name);
        } else {
            console.log("❌ Screen1 load failed:", comp1.errorString());
        }

        // --- Screen 2 ---
        if (screens.length > 1) {
            var comp2 = Qt.createComponent("SecondaryWindow.qml");
            if (comp2.status === Component.Ready) {
                var win2 = comp2.createObject(null);
                win2.screen = screens[1];       // assign BEFORE showing
                win2.showFullScreen();
                secondaryWindowInstance = win2
                console.log("Screen2 loaded on:", win2.screen.name);
            } else {
                console.log("❌ Screen2 load failed:", comp2.errorString());
            }
        } else {
            console.log("⚠️ Only one display detected — Screen2 will not open.");
        }
    }
}
}

