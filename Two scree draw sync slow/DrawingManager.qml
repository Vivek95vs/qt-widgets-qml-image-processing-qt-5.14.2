pragma Singleton
import QtQuick 2.12

QtObject {
    id: drawingManager

    // Shared drawings array
    property var drawings: []

    // Current drawing settings
    property color currentColor: "red"
    property real currentLineWidth: 2
    property string currentImageSource: ""

    // Signal when drawings are updated
    signal drawingsUpdated()

    // Signal when drawing settings change
    signal settingsChanged()

    // Add a new drawing
    function addDrawing(drawingData) {
        // Ensure drawingData has the current image source
        if (!drawingData.imageSource) {
            drawingData.imageSource = currentImageSource;
        }
        drawings.push(drawingData);
        drawingsUpdated();
    }

    // Clear all drawings
    function clearDrawings() {
        drawings = [];
        drawingsUpdated();
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

    // Remove drawings for a specific image
    function clearDrawingsForImage(imageSource) {
        var newDrawings = [];
        for (var i = 0; i < drawings.length; i++) {
            if (drawings[i].imageSource !== imageSource) {
                newDrawings.push(drawings[i]);
            }
        }
        drawings = newDrawings;
        drawingsUpdated();
    }
}
