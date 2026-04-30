import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Item {
    id: screen1
    visible: true
    property var drawingManager: null

    property int currentFocus: 0
    function handleTab() {
            if (currentFocus === 0) {
                passwordtextInput.forceActiveFocus()
                currentFocus = 1
            } else {
                usertextInput.forceActiveFocus()
                currentFocus = 0
            }
        }

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
            Rectangle{
                width: parent.width*0.5
                height: parent.height
                color: "transparent"
                anchors.left: parent.left
                Text {
                    text: "C-ARM"
                    color: "white"
                    anchors.centerIn: parent
                    font.pixelSize: Math.max(14, parent.width * 0.1)
                    font.weight: Font.Normal

                }
            }
            Rectangle{
                width: parent.width*0.5
                height: parent.height
                color: "transparent"
                anchors.right: parent.right
                Rectangle {
                    id: loginrect
                    anchors.centerIn: parent
                    width: parent.width * 0.7
                    height: parent.height * 0.25
                    radius: width * 0.025
                    color: "transparent"
//                    border.color: "white"
//                    border.width: 1

                    // Prevent clicks from propagating to background
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {} // Consume the click
                    }



                    Rectangle {
                        id: content
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: parent.height * 0.7 // 50% of dialog height
                        color: "transparent"
                        Text {
                            id: headtext
                            text: "SIGN IN"
                            color: "white"
                            anchors.top: parent.top
                            anchors.left: parent.left
                            width: parent.width*0.3
                            height:parent.height*0.2
                            font.pixelSize: Math.max(12, parent.width * 0.065)
                            font.weight: Font.Normal

                        }
                        Rectangle {
                            id: patientnamerect
                            width: parent.width
                            height: parent.height *0.8
                            anchors.top: headtext.bottom
                            anchors.left: parent.left
                            //                    anchors.leftMargin: parent.width*0.15
                            color: "transparent"
                            Item {
                                id: usernameitem
                                anchors.top: parent.top
                                anchors.left: parent.left
                                width: parent.width
                                height:parent.height*0.45
                                anchors.topMargin: parent.height*0.1


                                Text {
                                    id: usernametxt
                                    text: "User ID"
                                    color: "white"
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    width: parent.width*0.4
                                    height:parent.height*0.3
                                    font.pixelSize: Math.max(12, parent.width * 0.055)
                                    font.weight: Font.Normal

                                }

                                // Text input field
                                TextInput {
                                    id: usertextInput
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.top: usernametxt.bottom
                                    anchors.topMargin: parent.height*0.15

                                    width: parent.width
                                    height:parent.height*0.3

                                    color: "white"
                                    font.pixelSize: Math.max(10, parent.width * 0.045)
                                    selectByMouse: true
                                    clip: true

                                    Keys.onTabPressed: {
                                        handleTab()
                                        event.accepted = true
                                    }

                                    onActiveFocusChanged: {
                                        if (activeFocus) currentFocus = 0
                                    }

                                    // Placeholder text
                                    property string placeholderTextuser: ""

                                    Text {
                                        anchors.top: parent.top
                                        text: usertextInput.placeholderTextuser
                                        color: "#B0BEC5" // Light grey for placeholder
                                        font.pixelSize: usertextInput.font.pixelSize
                                        visible: !usertextInput.text && !usertextInput.activeFocus
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    // Cursor
                                    cursorVisible: false  // handled by custom delegate

                                    cursorDelegate: Rectangle {
                                        width: 1
                                        height: parent.height/2
                                        anchors.verticalCenter: parent.verticalCenter
                                        color: "white"
                                        visible: parent.activeFocus  // ⬅️ Hides cursor completely when not focused

                                        SequentialAnimation on opacity {
                                            loops: Animation.Infinite
                                            running: parent.activeFocus  // only when focused
                                            PropertyAnimation { to: 0; duration: 500 }
                                            PropertyAnimation { to: 1; duration: 500 }
                                        }
                                    }
                                }

                                // Underline
                                Rectangle {
                                    id: userunderline
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.top: usertextInput.bottom
                                    width: parent.width
                                    height: 2
                                    color: usertextInput.activeFocus ? "#3498DB" : "white" // Blue when focused, white otherwise

                                    Behavior on color {
                                        ColorAnimation {
                                            duration: 200
                                            easing.type: Easing.InOutQuad
                                        }
                                    }
                                }
                            }
                            Item {
                                id: passworditem
                                anchors.top: usernameitem.bottom
                                anchors.left: parent.left
                                width: parent.width
                                height:parent.height*0.45
                              Text {
                                    id: passwordtxt
                                    text: "Password"
                                    color: "white"
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    width: parent.width*0.4
                                    height:parent.height*0.3
                                    font.pixelSize: Math.max(12, parent.width * 0.055)
                                    font.weight: Font.Normal

                                }
                                Image {
                                    id: paswordeye
                                    width: parent.width*0.15
                                    height:parent.height*0.3
                                    anchors.top: parent.top
                                    anchors.right: passworditem.right
                                    source: "qrc:/icon/Login - Eye icon.png"
                                    fillMode: Image.PreserveAspectFit
                                    CustomToolTip {
                                        id: eyetooltip
                                        tooltipText:
                                        {
                                            if(passwordtextInput.echoMode === TextInput.Password)
                                            {
                                                return "show"
                                            }
                                            else if(passwordtextInput.echoMode === TextInput.Normal)
                                            {
                                                return "hide"
                                            }
                                        }
                                        anchors.bottom: parent.top
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.bottomMargin: 5
                                    }

                                    MouseArea
                                    {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onEntered: eyetooltip.show = true
                                        onExited: eyetooltip.show = false
                                        onClicked:
                                        {
                                            if(passwordtextInput.echoMode === TextInput.Password)
                                            {
                                               passwordtextInput.echoMode= TextInput.Normal
                                               paswordeye.source= "qrc:/icon/Login - Eye Off icon.png"
                                            }
                                            else
                                            {
                                                passwordtextInput.echoMode = TextInput.Password
                                                paswordeye.source= "qrc:/icon/Login - Eye icon.png"
                                            }

                                        }

                                    }
                                }

                                // Text input field
                                TextInput {
                                    id: passwordtextInput
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.top: passwordtxt.bottom
                                    anchors.topMargin: parent.height*0.15
                                    width: parent.width
                                    height:parent.height*0.3

                                    color: "white"
                                    font.pixelSize: Math.max(10, parent.width * 0.045)
                                    selectByMouse: true
                                    clip: true
                                    echoMode: TextInput.Password

                                    Keys.onTabPressed: {
                                        handleTab()
                                        event.accepted = true
                                    }

                                    onActiveFocusChanged: {
                                        if (activeFocus) currentFocus = 1
                                    }

                                    // Placeholder text
                                    property string placeholderTextpassword: ""

                                    Text {
                                        anchors.fill: parent
                                        text: passwordtextInput.placeholderTextpassword
                                        color: "#B0BEC5" // Light grey for placeholder
                                        font.pixelSize: passwordtextInput.font.pixelSize
                                        visible: !passwordtextInput.text && !passwordtextInput.activeFocus
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    // Cursor
                                    cursorVisible: false  // handled by custom delegate

                                    cursorDelegate: Rectangle {
                                        width: 1
                                        height: parent.height/2
                                        anchors.verticalCenter: parent.verticalCenter
                                        color: "white"
                                        visible: parent.activeFocus  // ⬅️ Hides cursor completely when not focused

                                        SequentialAnimation on opacity {
                                            loops: Animation.Infinite
                                            running: parent.activeFocus  // only when focused
                                            PropertyAnimation { to: 0; duration: 500 }
                                            PropertyAnimation { to: 1; duration: 500 }
                                        }
                                    }
                                }

                                // Underline
                                Rectangle {
                                    id: passwordunderline
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.top: passwordtextInput.bottom
                                    width: parent.width
                                    height: 2
                                    color: passwordtextInput.activeFocus ? "#3498DB" : "white" // Blue when focused, white otherwise

                                    Behavior on color {
                                        ColorAnimation {
                                            duration: 200
                                            easing.type: Easing.InOutQuad
                                        }
                                    }
                                }
                            }

                            Text {
                                id: invalidcredentials
            //                    visible: passwordField.text.length === 0
                                visible: false
                                text: "Invalid login credentials"
                                color: "red"
                                anchors.top: passworditem.bottom
            //                    anchors.topMargin: 10
                                font.pixelSize: Math.max(10, parent.width * 0.04)
                                anchors.left: parent.left
                            }


                        }

                    }

                    Rectangle {
                        id: buttonSection
                        anchors.top: content.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: parent.height * 0.2
                        color: "transparent"
                        anchors.topMargin: parent.height*0.08

                        Row {
                            id: buttonRow
                            anchors.fill: parent
                            spacing: parent.width * 0.05

                            Rectangle {
                                id: signinButton
                                width: buttonSection.width * 0.475
                                height: buttonSection.height * 0.5
                                anchors.right: clearButton.left
                                anchors.rightMargin: parent.width*0.05
                                radius: height * 0.12
                                color: signinButtonMouseArea.containsMouse ? "#33ccff" : "#33ddff"
                                border.color: "#668080"
                                border.width: Math.max(1, width * 0.006)

                                // Smooth hover animation
                                Behavior on color {
                                    ColorAnimation {
                                        duration: 150
                                        easing.type: Easing.InOutQuad
                                    }
                                }

                                Text {
                                    anchors.centerIn: parent
                                    text: "SIGN IN"
                                    color: "black"
                                    font.pixelSize: parent.height * 0.5
                                    font.weight: Font.Medium
                                }


                                MouseArea {
                                    id: signinButtonMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    property int validateuser
                                    onClicked: {
                                        if(usertextInput.text==="s" & passwordtextInput.text==="s")
                                        {
                                            mainitem.primaryWindowInstance.primaryLoader.source = "PrimaryWorklist.qml"
                                            mainitem.secondaryWindowInstance.secondaryLoader.source = "SecondaryWorklist.qml"

                                            // Pass drawing manager to worklist
                                            if (drawingManager) {
                                                mainitem.primaryWindowInstance.primaryLoader.item.drawingManager = drawingManager;
                                                mainitem.secondaryWindowInstance.secondaryLoader.item.drawingManager = drawingManager;
                                            }

                                            console.log("Authorization Confirm clicked")
                                        }
                                        else
                                        {
                                            userunderline.color = "#ba4136"
                                            passwordunderline.color = "#ba4136"
                                            invalidcredentials.text = "Invalid login credentials"
                                            invalidcredentials.visible = true
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                id: clearButton
                                width: buttonSection.width * 0.477
                                height: buttonSection.height * 0.5
                                anchors.right: parent.right
//                                anchors.rightMargin: parent.width*0.045
                                radius: height * 0.12
                                color: clearButtonMouseArea.containsMouse ? "#33ccff" : "#33ddff"
                                border.color: "#668080"
                                border.width: Math.max(1, width * 0.006)

                                // Smooth hover animation
                                Behavior on color {
                                    ColorAnimation {
                                        duration: 150
                                        easing.type: Easing.InOutQuad
                                    }
                                }

                                Text {
                                    anchors.centerIn: parent
                                    text: "CLEAR"
                                    color: "black"
                                    font.pixelSize: parent.height * 0.5
                                    font.weight: Font.Medium
                                }

                                MouseArea {
                                    id: clearButtonMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        usertextInput.text = ""
                                        passwordtextInput.text = ""

                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: disclaimerrect
                    anchors.top: loginrect.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width * 0.7
                    height: parent.height * 0.15
                    radius: width * 0.025
                    anchors.horizontalCenterOffset: 0
                    anchors.topMargin: 0
                    color: "transparent"
                    Text {
                        id: disclaimerheadtext
                        text: "Disclaimer Notice:"
                        color: "white"
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width*0.4
                        height:parent.height*0.15
                        font.pixelSize: Math.max(12, parent.width * 0.05)
                        font.weight: Font.Normal

                    }
                    Text {
                        id: disclaimertext
                        text: "This software is protected by copyright law and international treaties. Unauthorized duplication or distribution of this software or any portion of it may result in severe civil and criminal penalties and will be prosecuted to the maximum extent possible under the law"
                        wrapMode: Text.WordWrap
                        color: "white"
                        anchors.left: parent.left
                        anchors.top: disclaimerheadtext.bottom
                        width: parent.width
                        height:parent.height*0.8
                        font.pixelSize: Math.max(14, parent.width * 0.04)
                        font.weight: Font.Normal
                        font.wordSpacing: parent.width*0.015

                        lineHeight: font.pixelSize * 1.5
                        lineHeightMode: Text.FixedHeight  // or Text.ProportionalHeight

                        horizontalAlignment: Text.AlignJustify

                    }
                }
                Rectangle
                {
                    id: licenserect
                    width: parent.width * 0.7
                    height: parent.height * 0.06
                    anchors.top: disclaimerrect.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    Text {
                        id: licensetext
                        text: "Licensed To: "+hospitalname
                        color: "white"
                        anchors.left: parent.left
                        font.pixelSize: Math.max(12, parent.width * 0.045)
                        font.weight: Font.Normal

                    }
                }
                Rectangle
                {

                    id: exitrect
                    width: parent.width * 0.7
                    height: parent.height * 0.06
                    anchors.top: licenserect.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    Rectangle {
                        id: helpButton
                        width: parent.width * 0.475
                        height: parent.height * 0.5
                        anchors.right: exitButton.left
                        anchors.rightMargin: parent.width*0.05
                        radius: height * 0.12
                        color: helpButtonMouseArea.containsMouse ? "#546E7A" : "transparent"
    //                    border.color: "#668080"
    //                    border.width: Math.max(1, width * 0.006)

                        // Smooth hover animation
                        Behavior on color {
                            ColorAnimation {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "HELP"
                            color: "white"
                            font.pixelSize: parent.height * 0.45
                            font.weight: Font.Medium
                        }

                        MouseArea {
                            id: helpButtonMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {

                            }
                        }
                    }
                    Rectangle {
                        id: exitButton
                        width: parent.width * 0.475
                        height: parent.height * 0.5
                        anchors.right: parent.right
                        radius: height * 0.12
                        color: exitButtonMouseArea.containsMouse ? "#aa3333" : "#964c4c"
    //                    border.color: "#668080"
    //                    border.width: Math.max(1, width * 0.006)

                        // Smooth hover animation
                        Behavior on color {
                            ColorAnimation {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "EXIT"
                            color: "white"
                            font.pixelSize: parent.height * 0.45
                            font.weight: Font.Medium
                        }
                        MouseArea {
                            id: exitButtonMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                Qt.quit()
            //                    folderstructure.killbackend()
                            }
                        }
                    }

                }

            }



        }

    }
}
