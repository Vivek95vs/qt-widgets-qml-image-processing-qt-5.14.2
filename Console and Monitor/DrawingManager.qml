pragma Singleton
import QtQuick 2.12

QtObject {
    id: drawingManager

    // Shared drawings storage
    property var drawings: []

    // Current drawing settings
    property color currentColor: "red"
    property real currentLineWidth: 2
    property string currentImageSource: ""

    // Signal when drawings are updated
    signal drawingsUpdated()

    // Timer for debouncing updates
    property Timer updateTimer: Timer {
        id: updateTimer
        interval: 16 // ~60 FPS
        onTriggered: drawingsUpdated()
    }

    // Add a new drawing
    function addDrawing(drawingData) {
        if (!drawingData.imageSource) {
            drawingData.imageSource = currentImageSource;
        }

        // Check if we're updating an existing drawing
        if (drawingData.drawingId) {
            for (var i = 0; i < drawings.length; i++) {
                if (drawings[i].drawingId === drawingData.drawingId) {
                    drawings[i] = drawingData;
                    scheduleUpdate();
                    return;
                }
            }
        }

        // Add new drawing
        drawings.push(drawingData);
        scheduleUpdate();
    }

    // Clear all drawings for current image
    function clearDrawings() {
        var newDrawings = [];
        for (var i = 0; i < drawings.length; i++) {
            if (drawings[i].imageSource !== currentImageSource) {
                newDrawings.push(drawings[i]);
            }
        }
        drawings = newDrawings;
        scheduleUpdate();
    }

    // Clear drawings for specific image
    function clearDrawingsForImage(imageSource) {
        var newDrawings = [];
        for (var i = 0; i < drawings.length; i++) {
            if (drawings[i].imageSource !== imageSource) {
                newDrawings.push(drawings[i]);
            }
        }
        drawings = newDrawings;
        scheduleUpdate();
    }

    // Update image info
    function setImageInfo(source) {
        if (currentImageSource !== source) {
            currentImageSource = source;
        }
    }

    // Get all drawings for current image
    function getDrawingsForCurrentImage() {
        var result = [];
        for (var i = 0; i < drawings.length; i++) {
            if (drawings[i].imageSource === currentImageSource) {
                result.push(drawings[i]);
            }
        }
        return result;
    }

    // Schedule an update (debounced)
    function scheduleUpdate() {
        if (!updateTimer.running) {
            updateTimer.start();
        }
    }
}
