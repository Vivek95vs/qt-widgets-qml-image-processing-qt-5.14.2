import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: window
    visible: true
    visibility: "FullScreen"
    width: 1200
    height: 700
    title: "C-ARM"
    color: "#dfe6e9"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 2
        spacing: 2

        // Right Rectangle (new parallel section)
        Rectangle {
            id: rightRect
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "white"
            radius: 5
            border.color: "#bdc3c7"
            border.width: 1

            Image {
                id: editstudyimage
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icon/logo_pmt.png"
                smooth: true
            }
        }
        // Left Rectangle (contains your existing UI)
        Rectangle {
            id: leftRect
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "white"
            radius: 10
            border.color: "#bdc3c7"
            border.width: 1
            anchors.margins: 2

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 2
                spacing: 10

                // Header
                ColumnLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "C-ARM"
                        font.pixelSize: 32
                        font.bold: true
                        color: "#2c3e50"
                        Layout.alignment: Qt.AlignCenter
                    }

                }

                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 2; color: "#bdc3c7" }

                // Sign In section
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 15

                    Text {
                        text: "SIGN IN"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#2c3e50"
                        Layout.alignment: Qt.AlignCenter
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 5
                        Text { text: "User ID:"; font.pixelSize: 14; color: "#2c3e50" }
                        TextField {
                            id: userIdField
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            placeholderText: "______"
                            font.pixelSize: 14
                            background: Rectangle {
                                color: "white"
                                border.color: "#bdc3c7"
                                border.width: 1
                                radius: 3
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 5
                        Text { text: "Password:"; font.pixelSize: 14; color: "#2c3e50" }
                        TextField {
                            id: passwordField
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            placeholderText: "______"
                            echoMode: TextInput.Password
                            font.pixelSize: 14
                            background: Rectangle {
                                color: "white"
                                border.color: "#bdc3c7"
                                border.width: 1
                                radius: 3
                            }
                        }
                    }
                }

                Button {
                    id: signBButton
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 40
                    text: "SIGN B:"
                    font.pixelSize: 14
                    contentItem: Text {
                        text: signBButton.text
                        font: signBButton.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle { color: signBButton.down ? "#2980b9" : "#3498db"; radius: 5 }
                    onClicked: console.log("Sign B clicked")
                }

                Text { text: "Click"; font.pixelSize: 12; color: "#7f8c8d"; Layout.alignment: Qt.AlignCenter }

                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: "#bdc3c7" }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Text { text: "Disclaimer Notice:"; font.pixelSize: 14; font.bold: true; color: "#2c3e50" }
                    Text {
                        text: "This sentence is protected by copyright laws and international treaties. Unauthorized duplication or distribution of this software or any portion of it may result in severe civil and criminal penalties and will be prosecuted to the maximum extent possible under the law"
                        font.pixelSize: 12
                        color: "#7f8c8d"
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }

                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: "#bdc3c7" }

                Text {
                    text: "Licensed To:"
                    font.pixelSize: 14
                    font.bold: true
                    color: "#2c3e50"
                    Layout.alignment: Qt.AlignLeft
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 20

                    Button {
                        id: helpButton
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 35
                        text: "HELP"
                        font.pixelSize: 14
                        contentItem: Text {
                            text: helpButton.text
                            font: helpButton.font
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle { color: helpButton.down ? "#27ae60" : "#2ecc71"; radius: 5 }
                        onClicked: console.log("Help clicked")
                    }

                    Button {
                        id: exitButton
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 35
                        text: "EXIT"
                        font.pixelSize: 14
                        contentItem: Text {
                            text: exitButton.text
                            font: exitButton.font
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle { color: exitButton.down ? "#c0392b" : "#e74c3c"; radius: 5 }
                        onClicked: Qt.quit()
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }


    }
}
