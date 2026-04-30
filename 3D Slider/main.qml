import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

ApplicationWindow {
    id: window
    visible: true
//    visibility: "FullScreen"
    minimumWidth: 1000
    minimumHeight: 600
//    title: qsTr("DICOM Series Viewer")
//    color: "black"


    Button {
        x: 647
        y: 478
        text: "Load DICOM Folder"
        onClicked: dicomHandler.loadDicomSeries()
        }

    Rectangle{
        y: 22
        width: 512
        height: 512
        anchors.horizontalCenterOffset: -122
        anchors.horizontalCenter: parent.horizontalCenter
        color: "black"
        Image {
            id: dicomImage
            anchors.fill: parent
            source: ""
            fillMode: Image.PreserveAspectFit
        }
    }

        Slider {
            x: 586
            y: 11
            from: 0
            to: 512  // You can adjust dynamically in the future
            stepSize: 1
            width: 24
            height: 535
            orientation: Qt.Vertical
            onValueChanged:
            {

                dicomHandler.showSlice(value)
                // Force image reload (with delay so C++ finishes setting it)
                Qt.callLater(() => {
                                 dicomImage.source = "image://dicom/"+value
                             })
            }
        }



//        VerticalSliceSlider {
//            id: sliceSlider
//            from: 0
//            to: 200
//            onValueChanged: console.log("slice:", value, "step:", sliceStepSize)
//        }







//        /*
//             Medical-grade DPI normalization
//             Reference DPI = 96 (industry standard)
//           */
//           readonly property real referenceDpi: 96.0
//           readonly property real logicalDpi: Screen.logicalPixelDensity * 25.4
//           readonly property real dpiScale: logicalDpi / referenceDpi

//           /*
//             Medically safe base font sizes (points)
//             Based on IEC 62366 / human factors guidance
//           */
//           readonly property real fontLabel: 9
//           readonly property real fontBody: 10
//           readonly property real fontHeader: 12
//           readonly property real fontCritical: 16


//           Column {
//                   anchors.centerIn: parent
//                   spacing: 12

//                   // Header text
//                   Text {
//                       text: "CT Image Viewer"
//                       font.family: Qt.application.font.family
//                       font.bold: true
//                       font.pointSize: Math.max(fontHeader * dpiScale, 11)
//                   }

//                   // Normal body text
//                   Text {
//                       text: "Patient Name: John Doe"
//                       font.family: Qt.application.font.family
//                       font.pointSize: Math.max(fontBody * dpiScale, 9)
//                   }

//                   // Label text
//                   Text {
//                       text: "Slice Thickness: 1.25 mm"
//                       font.family: Qt.application.font.family
//                       font.pointSize: Math.max(fontLabel * dpiScale, 8)
//                   }

//                   // Critical medical value
//                   Text {
//                       text: "WARNING: Abnormal Finding"
//                       color: "red"
//                       font.family: Qt.application.font.family
//                       font.bold: true
//                       font.pointSize: Math.max(fontCritical * dpiScale, 14)
//                   }

//                   // Debug info (optional)
//                   Text {
//                       text: "DPI: " + logicalDpi.toFixed(1) +
//                             " | Scale: " + dpiScale.toFixed(2)
//                       font.pointSize: Math.max(8 * dpiScale, 8)
//                       color: "#666"
//                   }
//               }




}


