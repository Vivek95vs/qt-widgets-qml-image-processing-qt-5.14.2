import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Item {
    id: screen2
    visible: true

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

    // Right Rectangle
    Rectangle {
        id: rightRect
        width: parent.width
        height: parent.height
        color: "#1e2a38"
        radius: 10
        border.width: 1

        Rectangle {
            width: parent.width*0.5
            height: parent.height
            color: "transparent"
            anchors.left: parent.left

            Image {
                id: logoContainer
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.04
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.025
                width: parent.width * 0.09
                height: parent.height * 0.06
                source: "qrc:/icon/logo_pmt.png"
            }

            Text {
                text: "C-ARM"
                color: "white"
                anchors.centerIn: parent
                font.pixelSize: Math.max(14, parent.width * 0.08)
                font.weight: Font.Normal
            }
        }

        Rectangle {
            width: parent.width*0.5
            height: parent.height
            color: "transparent"
            anchors.right: parent.right

            // Login form (similar to PrimaryLoginScreen but simplified)
            Rectangle {
                id: loginrect
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: parent.height*0.18
                width: parent.width * 0.5
                height: parent.height * 0.3
                radius: width * 0.025
                color: "transparent"

                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width * 0.9
                    height: parent.height * 0.8
                    color: "transparent"

                    Column {
                        anchors.fill: parent
                        spacing: 20

                        Text {
                            text: "SIGN IN"
                            color: "white"
                            font.pixelSize: 24
                            font.bold: true
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Column {
                            width: parent.width
                            spacing: 10

                            TextField {
                                id: usertextInput
                                width: parent.width
                                placeholderText: "User ID"
                                color: "white"
                                background: Rectangle {
                                    color: "transparent"
                                    Rectangle {
                                        anchors.bottom: parent.bottom
                                        width: parent.width
                                        height: 2
                                        color: usertextInput.activeFocus ? "#3498DB" : "white"
                                    }
                                }

                                Keys.onTabPressed: {
                                    handleTab()
                                    event.accepted = true
                                }
                            }

                            TextField {
                                id: passwordtextInput
                                width: parent.width
                                placeholderText: "Password"
                                color: "white"
                                echoMode: TextInput.Password
                                background: Rectangle {
                                    color: "transparent"
                                    Rectangle {
                                        anchors.bottom: parent.bottom
                                        width: parent.width
                                        height: 2
                                        color: passwordtextInput.activeFocus ? "#3498DB" : "white"
                                    }
                                }

                                Keys.onTabPressed: {
                                    handleTab()
                                    event.accepted = true
                                }
                            }
                        }

                        Row {
                            width: parent.width
                            spacing: 10

                            Button {
                                text: "SIGN IN"
                                width: parent.width * 0.48
                                onClicked: {
                                    if(usertextInput.text === "s" && passwordtextInput.text === "s") {
                                        mainitem.primaryWindowInstance.primaryLoader.source = "PrimaryWorklist.qml"
                                        if (mainitem.secondaryWindowInstance) {
                                            mainitem.secondaryWindowInstance.secondaryLoader.source = "SecondaryWorklist.qml"
                                        }
                                    }
                                }
                            }

                            Button {
                                text: "CLEAR"
                                width: parent.width * 0.48
                                onClicked: {
                                    usertextInput.text = ""
                                    passwordtextInput.text = ""
                                }
                            }
                        }
                    }
                }
            }

            Text {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Licensed To: " + hospitalname
                color: "white"
                font.pixelSize: 12
            }
        }
    }
}
