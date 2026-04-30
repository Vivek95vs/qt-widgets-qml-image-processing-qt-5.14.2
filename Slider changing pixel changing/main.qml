import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    width: 1000
    height: 1000
    title: qsTr("Histogram-Based WW/WL Adjustment")

    Rectangle {
        id: imageContainer
        width: 600
        height: 600
        color: "lightgray"
        anchors.left: parent

        Image {
            id: mainImage
            anchors.fill: parent
            anchors.margins: 10
            source: imageMagnifier.imageLoaded ? "image://imageProvider/main" : ""
            smooth: true
            visible: imageMagnifier.imageLoaded
        }

        Button {
            id: loadButton
            text: "Load Image"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                imageMagnifier.loadImage();
            }
        }
    }

    Rectangle {
        id: histogramContainer
        width: 500
        height: 150
        color: "black"
        anchors.top: imageContainer.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20

        Canvas {
            id: histogramCanvas
            width: parent.width
            height: parent.height

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);
                ctx.strokeStyle = "white";
                ctx.lineWidth = 1;

                // Draw the histogram
                if (imageMagnifier.histogramData.length > 0) {
                    var maxVal = Math.max.apply(null, imageMagnifier.histogramData);
                    var scaleX = width / 256;
                    var scaleY = height / maxVal;

                    ctx.beginPath();
                    for (var i = 0; i < 256; i++) {
                        var x = i * scaleX;
                        var y = height - imageMagnifier.histogramData[i] * scaleY;
                        ctx.moveTo(x, height);
                        ctx.lineTo(x, y);
                    }
                    ctx.stroke();
                }

                // Draw the vertical lines for Window Width (WW) and Window Level (WL)
                ctx.strokeStyle = "red";
                ctx.lineWidth = 2;

                // Window Level (WL) line
                var levelX = (imageMagnifier.windowLevel / 255) * width;
                ctx.beginPath();
                ctx.moveTo(levelX, 0);
                ctx.lineTo(levelX, height);
                ctx.stroke();

                // Window Width (WW) lines
                var minX = ((imageMagnifier.windowLevel - imageMagnifier.windowWidth / 2) / 255) * width;
                var maxX = ((imageMagnifier.windowLevel + imageMagnifier.windowWidth / 2) / 255) * width;
                ctx.strokeStyle = "green";
                ctx.beginPath();
                ctx.moveTo(minX, 0);
                ctx.lineTo(minX, height);
                ctx.moveTo(maxX, 0);
                ctx.lineTo(maxX, height);
                ctx.stroke();
            }
        }

        Connections {
            target: imageMagnifier
            function onHistogramUpdated() {
                histogramCanvas.requestPaint();
            }
            function onImageUpdated() {
                mainImage.source = ""; // Force image reload
                mainImage.source = "image://imageProvider/main";
            }
        }
    }

    Rectangle {
        id: controlPanel
        width: 500
        height: 100
        color: "lightblue"
        anchors.top: histogramContainer.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20

        Column {
            anchors.centerIn: parent
            spacing: 10

            Slider {
                id: windowWidthSlider
                width: 400
                from: 1
                to: 255
                value: imageMagnifier.windowWidth
                onValueChanged: {
                    imageMagnifier.setWindowWidth(value);
                }
            }

            Slider {
                id: windowLevelSlider
                width: 400
                from: 0
                to: 255
                value: imageMagnifier.windowLevel
                onValueChanged: {
                    imageMagnifier.setWindowLevel(value);
                }
            }
        }
    }
}
