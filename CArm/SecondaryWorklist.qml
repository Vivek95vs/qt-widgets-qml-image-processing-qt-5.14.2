import QtQuick 2.12

Item {
    id: secondaryworklist

    //------------------------------------Time and Date start------------------------
        // ✅ Correct declaration
            property string currentTime: Qt.formatDateTime(new Date(), "dd-MM-yyyy hh:mm:ss")
            property string currentDate: Qt.formatDateTime(new Date(), "yyyyMMdd")
        Timer {
            interval: 1000
            running: true
            repeat: true
            onTriggered: {
                currentTime = Qt.formatDateTime(new Date(), "dd-MM-yyyy hh:mm:ss")
            }
        }
    //------------------------------------Time and Date end------------------------
    Rectangle{
        anchors.fill: parent
        color: "transparent"
        Rectangle
        {
            id: firstrect
            width: parent.width*0.99
            height: parent.height*0.07
            anchors.top: parent.top
            anchors.topMargin: parent.height*0.005
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#1e2a38"
            Rectangle
            {
               id: firstinfirstrect
               width: parent.width*0.20
               height: parent.height*0.85
               anchors.verticalCenter: parent.verticalCenter
               anchors.left: parent.left
               anchors.leftMargin: parent.width*0.005
               color: "transparent"
               border.color: "#f5f5f5"
               border.width: 1
               radius: 10
               Rectangle{
                   width: parent.width*0.25
                   height: parent.height*0.5
                   color: "transparent"
                   anchors.left: parent.left
                   anchors.verticalCenter: parent.verticalCenter
                   Image {
                       id: logoContainer
                       anchors.left: parent.left
                       anchors.leftMargin: parent.width * 0.04
                       anchors.top: parent.top
                       anchors.topMargin: parent.height * 0.025
                       width: parent.width
                       height: parent.height
                       fillMode: Image.PreserveAspectFit
                       source: "qrc:/icon/13'/Storage.png"
                   }
                   Text {
                       text: "dummy/dummy"
                       color: "white"
                       anchors.left: logoContainer.right
                       anchors.leftMargin: parent.width * 0.2
                       anchors.verticalCenter: parent.verticalCenter
                       font.pixelSize: Math.max(14, parent.width * 0.08)
                       font.weight: Font.Normal

                   }
               }

            }
            Rectangle
            {
                id: secondinfirstrect
                width: parent.width*0.43
                height: parent.height*0.85
                anchors.left: firstinfirstrect.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: parent.width*0.005
                color: "#5a6e80"
                radius: 10
                Rectangle{
                    id: livedatadisplay1
                    width: parent.width*0.185
                    height: parent.height*0.9
                    color: "transparent"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        id: logoContainerlive1
                        anchors.left: parent.left
                        width: parent.width*0.7
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectfit
                        source: "qrc:/icon/13'/C-arm Motions/Orbital.png"
                    }
                    Text {
                        text: "90"
                        color: "white"
                        anchors.left: logoContainerlive1.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: Math.max(14, parent.width * 0.08)
                        font.weight: Font.Normal

                    }
                }
                Rectangle{
                    id: livedatadisplay2
                    width: parent.width*0.185
                    height: parent.height*0.8
                    color: "transparent"
                    anchors.left: livedatadisplay1.right
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        id: logoContainerlive2
                        anchors.left: parent.left
                        width: parent.width*0.7
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/C-arm Motions/Angular.png"
                    }
                    Text {
                        text: "0"
                        color: "white"
                        anchors.left: logoContainerlive2.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: Math.max(14, parent.width * 0.08)
                        font.weight: Font.Normal

                    }
                }
                Rectangle{
                    id: livedatadisplay3
                    width: parent.width*0.185
                    height: parent.height*0.9
                    color: "transparent"
                    anchors.left: livedatadisplay2.right
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        id: logoContainerlive3
                        anchors.left: parent.left
                        width: parent.width*0.7
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/C-arm Motions/Vertical.png"
                    }
                    Text {
                        text: "20"+"cm"
                        color: "white"
                        anchors.left: logoContainerlive3.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: Math.max(14, parent.width * 0.08)
                        font.weight: Font.Normal

                    }
                }
                Rectangle{
                    id: livedatadisplay4
                    width: parent.width*0.185
                    height: parent.height*0.9
                    color: "transparent"
                    anchors.left: livedatadisplay3.right
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        id: logoContainerlive4
                        anchors.left: parent.left
                        width: parent.width*0.7
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/C-arm Motions/Horizontal.png"
                    }
                    Text {
                        text: "20"+"cm"
                        color: "white"
                        anchors.left: logoContainerlive4.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: Math.max(14, parent.width * 0.08)
                        font.weight: Font.Normal

                    }
                }
                Rectangle{
                    id: livedatadisplay5
                    width: parent.width*0.185
                    height: parent.height*0.9
                    color: "transparent"
                    anchors.left: livedatadisplay4.right
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        id: logoContainerlive5
                        anchors.left: parent.left
                        width: parent.width*0.7
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/C-arm Motions/Detector Vertical.png"
                    }
                    Text {
                        text: "25"+"cm"
                        color: "white"
                        anchors.left: logoContainerlive5.right
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: Math.max(14, parent.width * 0.08)
                        font.weight: Font.Normal

                    }
                }
            }
            Rectangle
            {
                id: thirdinfirstrect
                width: parent.width*0.35
                height: parent.height*0.85
                anchors.left: secondinfirstrect.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: parent.width*0.005
                color: "transparent"
                    Image {
                        id: logoContainer1
                        anchors.left: parent.left
//                        anchors.leftMargin: parent.width * 0.04
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width*0.125
                        height: parent.height*0.7
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/Home Disabled.png"
                    }
                    Image {
                        id: logoContainer2
                        anchors.left: logoContainer1.right
                        anchors.leftMargin: parent.width * 0.05
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width*0.125
                        height: parent.height*0.7
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/Modes Menu.png"
                    }
                    Image {
                        id: logoContainer3
                        anchors.left: logoContainer2.right
                        anchors.leftMargin: parent.width * 0.05
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width*0.125
                        height: parent.height*0.7
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/Images Disabled.png"
                    }
                    Image {
                        id: logoContainer4
                        anchors.left: logoContainer3.right
                        anchors.leftMargin: parent.width * 0.05
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width*0.125
                        height: parent.height*0.7
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/Processing.png"
                    }
                    Image {
                        id: logoContainer5
                        anchors.left: logoContainer4.right
                        anchors.leftMargin: parent.width * 0.05
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width*0.125
                        height: parent.height*0.7
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/CBCT.png"
                    }
                    Image {
                        id: logoContainer6
                        anchors.left: logoContainer5.right
                        anchors.leftMargin: parent.width * 0.05
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width*0.125
                        height: parent.height*0.7
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icon/13'/Settings.png"
                    }
            }
        }
        Rectangle
        {
            id: secondrect
            width: parent.width*0.99
            height: parent.height*0.88
            anchors.top: firstrect.bottom
            anchors.topMargin: parent.height*0.005
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#1e2a38"
            radius: 10
        }
        Rectangle
        {
            width: parent.width*0.99
            height: parent.height*0.025
            anchors.top: secondrect.bottom
            anchors.topMargin: parent.height*0.005
            anchors.horizontalCenter: parent.horizontalCenter
            color: "transparent"
            Text {
                text: "Software Version: "+appVersion
                font.pixelSize: 14
                color: "white"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: "Licensed To: "+hospitalname
                font.pixelSize: 14
                color: "white"
                anchors.centerIn:  parent
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: currenttime
                text: currentTime
                font.pixelSize: 14
                color: "white"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

    }
}
