import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Item {
    id: primaryworklist
    visible: true
//    width: 800
//    height:1200

    RowLayout {
        anchors.fill: parent
//        anchors.margins: 2
        anchors.leftMargin: parent.width*0.005
        anchors.rightMargin: parent.width*0.005
        anchors.topMargin: parent.height*0.005
        anchors.bottomMargin: parent.height*0.005

        spacing: 4

        // Left Rectangle
        Rectangle {
            id: leftRect
            Layout.fillHeight: true
            Layout.fillWidth: true
//            anchors.centerIn: parent
//            width: parent.width*0.3
//            height: parent.height*0.3

            color: "#1e2a38"
            radius: 5
//            border.color: "#bdc3c7"
//            border.width: 1

            Image {
                id: editstudyimage
                anchors.centerIn: parent
                width: parent.width*0.15
                height: parent.height*0.15
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icon/logo_pmt.png"
                smooth: true
            }
        }

        // Right Rectangle
        Rectangle {
            id: rightRect
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "#1e2a38"
            radius: 10
//            border.color: "#bdc3c7"
            border.width: 1
            anchors.margins: 2


        }

    }
}
