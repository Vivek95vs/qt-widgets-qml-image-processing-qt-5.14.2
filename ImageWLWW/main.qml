import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    width: 1000
    height: 1000
    title: qsTr("Raw Image Load with Window Level/Width Adjustment")

    Rectangle {
        id: imageContainer
        width: 600
        height: 600
        color: "lightgray"
        anchors.centerIn: parent

        // Main image
        Image {
            id: mainImage
            anchors.fill: parent
            anchors.margins: 10
            source: imageMagnifier.imageLoaded ? "image://imageProvider/main" : ""
            smooth: true
            visible: imageMagnifier.imageLoaded
        }

        // Mouse area for handling mouse events
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
                imageMagnifier.startWindowLevel(mouseX, mouseY);
            }
            onPositionChanged: {
                imageMagnifier.adjustWindowLevel(mouseX, mouseY);
            }
            onReleased: {
                imageMagnifier.endWindowLevel();
            }
        }

        // Button to load the image
        Button {
            id: loadButton
            text: "Load Image"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                // Call the C++ function to load the image
                imageMagnifier.loadImage();
            }
        }

        // Listen for the imageUpdated signal
        Connections {
            target: imageMagnifier
            onImageUpdated: {
                // Force refresh the image
                console.log("Image updated signal received. Refreshing image...");
                mainImage.source = "";
                mainImage.source = "image://imageProvider/main";
            }
        }
    }
}
