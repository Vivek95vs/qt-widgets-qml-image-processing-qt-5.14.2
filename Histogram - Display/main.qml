import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    width: 1000
    height: 1000
    title: qsTr("Raw Image Load with Histogram")

    Rectangle {
        id: imageContainer
        width: 600
        height: 600
        color: "lightgray"
        anchors.centerIn: parent

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
            }
        }

        Connections {
            target: imageMagnifier
            function onHistogramUpdated() {
                histogramCanvas.requestPaint();
            }
        }
    }
}
