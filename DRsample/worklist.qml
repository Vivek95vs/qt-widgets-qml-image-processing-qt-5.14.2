import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Item {
    id: name
    visible: true
//---------------------------------------------------------
    Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: parent.height*0.965
            color: "#b8e6e6"
            Row {
                anchors.fill: parent

                // Left section - Menu and User
                Row {
                    width: parent.width*0.2
                    height: parent.height
//                    spacing: parent.width*0.1

                    // Hamburger menu button
                    Rectangle {
                        width: parent.width*0.15
                        height: parent.height*0.4
                        color: hamburgerMouseArea.containsMouse ? "#80CBC4" : "transparent"
                        radius: 4
                        anchors.verticalCenter: parent.verticalCenter



                        MouseArea {
                            id: hamburgerMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked:
                            {
                                console.log("Menu clicked")
                                mainLoader.source = "leftleg.qml"
                            }
                        }

                        Column {
//                            anchors.centerIn: parent
                            width: parent.width*0.8
                            height: parent.height*0.8
                            spacing: 2
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            Rectangle {
                                width: parent.width*0.6
                                height: parent.height*0.2
                                anchors.horizontalCenter: parent.horizontalCenter
                                color: "#424242"
                            }
                            Rectangle {
                                width: parent.width*0.6
                                height: parent.height*0.2
                                anchors.horizontalCenter: parent.horizontalCenter
                                color: "#424242"
                            }
                            Rectangle {
                                width: parent.width*0.6
                                height: parent.height*0.2
                                anchors.horizontalCenter: parent.horizontalCenter
                                color: "#424242"
                            }
                        }
                    }

                    // User profile section
                    Row {
                        spacing: 8
                        anchors.verticalCenter: parent.verticalCenter

                        // User avatar
                        Rectangle {
                            width: 32
                            height: 32
                            color: "#616161"
                            radius: 16
                            anchors.verticalCenter: parent.verticalCenter

                            // User icon (simplified person icon)
                            Rectangle {
                                width: 10
                                height: 10
                                color: "white"
                                radius: 5
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 6
                            }

                            Rectangle {
                                width: 16
                                height: 10
                                color: "white"
                                radius: 8
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 4
                            }
                        }

                        // Username
                        Text {
                            text: "xxxxx"
                            width: parent.width*0.77
                            height: parent.height*0.5
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            color: "#424242"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                // Center section - Patient Information
                Rectangle {
                    width: parent.width*0.58 // Remaining space between left and right sections
                    height: parent.height
                    color: "transparent"

                    Row {
                        anchors.centerIn: parent
                        spacing: 24

                        // Patient Name
                        Row {
                            spacing: 4
                            Text {
                                text: "Worklist"
                                font.pixelSize: 20
                                font.weight: Font.Medium
                                color: "#424242"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                        }

                    }
                }

                // Right section - Logo
                Rectangle {
                    width: parent.width*0.2
                    height: parent.height
                    color: "transparent"

                    Text {
                        text: "LOGO"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        color: "#212121"
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }

//---------------------------------------------------------

    Rectangle {
        id: rectangle

        width: parent.width*0.995
        height: parent.height*0.955
        color: "#668080"
        border.color: "white"
        anchors.left: parent.left
        anchors.top: parent.top
//        anchors.leftMargin: parent.width*0.005
        anchors.topMargin: parent.height*0.04
        border.width: 2
        radius: 10


            // Sample data model
            ListModel {
                id: tableModel
                ListElement { name: "Alice"; itemId: "A001"; gender: "Female"; dateOfBirth: "15-03-1990" }
                ListElement { name: "Bob"; itemId: "B002"; gender: "Male"; dateOfBirth: "22-07-1985" }
                ListElement { name: "Alice1"; itemId: "A002"; gender: "Female"; dateOfBirth: "16-03-1990" }
                ListElement { name: "Bob1"; itemId: "B003"; gender: "Male"; dateOfBirth: "23-07-1985" }
                ListElement { name: "Alice2"; itemId: "A003"; gender: "Female"; dateOfBirth: "17-03-1990" }
                ListElement { name: "Bob2"; itemId: "B004"; gender: "Male"; dateOfBirth: "24-07-1985" }
                ListElement { name: "Alice3"; itemId: "A004"; gender: "Female"; dateOfBirth: "18-03-1990" }
                ListElement { name: "Bob3"; itemId: "B005"; gender: "Male"; dateOfBirth: "25-07-1985" }
                ListElement { name: "Alice4"; itemId: "A005"; gender: "Female"; dateOfBirth: "19-03-1990" }
                ListElement { name: "Bob4"; itemId: "B006"; gender: "Male"; dateOfBirth: "26-07-1985" }
                ListElement { name: "Alice5"; itemId: "A006"; gender: "Female"; dateOfBirth: "20-03-1990" }
                ListElement { name: "Bob5"; itemId: "B007"; gender: "Male"; dateOfBirth: "27-07-1985" }
                ListElement { name: "Alice6"; itemId: "A007"; gender: "Female"; dateOfBirth: "21-03-1990" }
                ListElement { name: "Bob6"; itemId: "B008"; gender: "Male"; dateOfBirth: "28-07-1985" }
                ListElement { name: "Alice7"; itemId: "A008"; gender: "Female"; dateOfBirth: "22-03-1990" }
                ListElement { name: "Bob7"; itemId: "B009"; gender: "Male"; dateOfBirth: "29-07-1985" }
                ListElement { name: "Alice8"; itemId: "A009"; gender: "Female"; dateOfBirth: "23-03-1990" }
                ListElement { name: "Bob8"; itemId: "B010"; gender: "Male"; dateOfBirth: "01-08-1985" }
                ListElement { name: "Alice9"; itemId: "A010"; gender: "Female"; dateOfBirth: "24-03-1990" }
                ListElement { name: "Bob9"; itemId: "B011"; gender: "Male"; dateOfBirth: "02-08-1985" }
                ListElement { name: "Alice10"; itemId: "A011"; gender: "Female"; dateOfBirth: "25-03-1990" }
                ListElement { name: "Bob10"; itemId: "B012"; gender: "Male"; dateOfBirth: "03-08-1985" }
                ListElement { name: "Alice11"; itemId: "A012"; gender: "Female"; dateOfBirth: "26-03-1990" }
                ListElement { name: "Bob11"; itemId: "B013"; gender: "Male"; dateOfBirth: "04-08-1985" }
                ListElement { name: "Alice12"; itemId: "A013"; gender: "Female"; dateOfBirth: "27-03-1990" }
                ListElement { name: "Bob12"; itemId: "B014"; gender: "Male"; dateOfBirth: "05-08-1985" }

            }

            // Sorting properties
            property string currentSortColumn: ""
            property bool sortAscending: true
            property string currentDate: "19/05/2025"

            // Sorting function
            function sortTable(column) {
                if (currentSortColumn === column) {
                    sortAscending = !sortAscending
                } else {
                    currentSortColumn = column
                    sortAscending = true
                }

                var items = []
                for (var i = 0; i < tableModel.count; i++) {
                    items.push(tableModel.get(i))
                }

                items.sort(function(a, b) {
                    var valueA = a[column].toString().toLowerCase()
                    var valueB = b[column].toString().toLowerCase()

                    if (column === "dateOfBirth") {
                        // Convert date format for proper sorting
                        var dateA = new Date(valueA.split('-').reverse().join('-'))
                        var dateB = new Date(valueB.split('-').reverse().join('-'))
                        return sortAscending ? dateA - dateB : dateB - dateA
                    }

                    if (valueA < valueB) return sortAscending ? -1 : 1
                    if (valueA > valueB) return sortAscending ? 1 : -1
                    return 0
                })

                tableModel.clear()
                for (var j = 0; j < items.length; j++) {
                    tableModel.append(items[j])
                }
            }

            // Date picker popup
            Popup {
                id: datePickerPopup
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                width: 300
                height: 200
                modal: true
                focus: true

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                    radius: 8
                    border.color: "#E0E0E0"
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 15

                        Text {
                            text: "Select Date"
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            color: "#333"
                        }

                        Row {
                            spacing: 10

                            SpinBox {
                                id: daySpinBox
                                from: 1
                                to: 31
                                value: 19
                            }

                            SpinBox {
                                id: monthSpinBox
                                from: 1
                                to: 12
                                value: 5
                            }

                            SpinBox {
                                id: yearSpinBox
                                from: 2020
                                to: 2030
                                value: 2025
                            }
                        }

                        Row {
                            spacing: 10
                            anchors.horizontalCenter: parent.horizontalCenter

                            Button {
                                text: "Cancel"
                                onClicked: datePickerPopup.close()
                            }

                            Button {
                                text: "OK"
                                highlighted: true
                                onClicked: {
                                    var day = daySpinBox.value.toString().padStart(2, '0')
                                    var month = monthSpinBox.value.toString().padStart(2, '0')
                                    var year = yearSpinBox.value.toString()
                                    currentDate = day + "/" + month + "/" + year
                                    datePickerPopup.close()
                                }
                            }
                        }
                    }
                }
            }

            // Main table container
            Rectangle {
                anchors.centerIn: parent
                width: parent.width*0.99
                height: parent.height*0.99
                color: "#668080"
                radius: 10
//                border.color: "#E0E0E0"
//                border.width: 1


                Column {
                    anchors.fill: parent

                    // Date header
                    Rectangle {
                        width: parent.width
                        height: parent.height*0.05
                        color: "#668080"
                        radius: 10

                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            height: parent.height / 2
                            color: "#668080"
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: datePickerPopup.open()
                            cursorShape: Qt.PointingHandCursor
                        }

                        Row {
                            anchors.fill: parent
                            anchors.margins: 16

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: "19/05/2025"
                                font.pixelSize: 14
                                font.weight: Font.Medium
                                color: "white"
                            }



                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: "▼"
                                font.pixelSize: 12
                                color: "white"
                            }
                        }
                    }

                    // Table header
                    Rectangle {
                        width: parent.width
                        height: parent.height*0.05
                        color: "#99c0c0"
                        border.color: "#c2f3f3"
                        border.width: 2
                        Row {
                            anchors.fill: parent

                            // Name header
                            Rectangle {
                                width: parent.width*0.25
                                height: parent.height
                                color: "#99c0c0"

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: sortTable("name")
                                    cursorShape: Qt.PointingHandCursor
                                }

                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 16
                                    spacing: 8

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: "Name"
                                        font.pixelSize: 14
                                        font.weight: Font.Medium
                                        color: "white"
                                    }

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: "▼"
                                        font.pixelSize: 10
                                        color: "white"
                                    }
                                }
                            }

                            // ID header
                            Rectangle {
                                width: parent.width*0.1875
                                height: parent.height
                                color:"#99c0c0"

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: sortTable("itemId")
                                    cursorShape: Qt.PointingHandCursor
                                }

                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 16
                                    spacing: 8

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: "ID"
                                        font.pixelSize: 14
                                        font.weight: Font.Medium
                                        color: "white"
                                    }

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: "▼"
                                        font.pixelSize: 10
                                        color: "white"
                                    }
                                }
                            }

                            // Gender header
                            Rectangle {
                                width: parent.width*0.25
                                height: parent.height
                                color: "#99c0c0"

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: sortTable("gender")
                                    cursorShape: Qt.PointingHandCursor
                                }

                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 16
                                    spacing: 8

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: "Gender"
                                        font.pixelSize: 14
                                        font.weight: Font.Medium
                                        color: "white"
                                    }

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: "▼"
                                        font.pixelSize: 10
                                        color: "white"
                                    }
                                }
                            }

                            // Date of Birth header
                            Rectangle {
                                width: parent.width*0.3125
                                height: parent.height
                                color: "#99c0c0"

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: sortTable("dateOfBirth")
                                    cursorShape: Qt.PointingHandCursor
                                }

                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 16
                                    spacing: 8

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: "Date of Birth"
                                        font.pixelSize: 14
                                        font.weight: Font.Medium
                                        color: "white"
                                    }

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: "▼"
                                        font.pixelSize: 10
                                        color: "white"
                                    }
                                }
                            }
                        }
                    }

                    // Table data rows
                    ListView {
                        width: parent.width
                        height: parent.height*0.9
                        model: tableModel
                        clip: true

                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 40
                            color: "#668080"

                            property bool hovered: false

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: parent.hovered = true
                                onExited: parent.hovered = false
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: parent.hovered ? "#4A5A56" : "transparent"
                                opacity: parent.hovered ? 0.5 : 0

                                Behavior on opacity {
                                    NumberAnimation { duration: 150 }
                                }
                            }

                            Row {
                                anchors.fill: parent

                                // Name cell
                                Rectangle {
                                    width: parent.width*0.25
                                    height: parent.height
                                    color: "transparent"

                                    Text {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 16
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: model.name
                                        font.pixelSize: 13
                                        color: "white"
                                    }
                                }

                                // ID cell
                                Rectangle {
                                    width: parent.width*0.1875
                                    height: parent.height
                                    color: "transparent"

                                    Text {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 16
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: model.itemId
                                        font.pixelSize: 13
                                        color: "white"
                                    }
                                }

                                // Gender cell
                                Rectangle {
                                    width: parent.width*0.25
                                    height: parent.height
                                    color: "transparent"

                                    Text {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 16
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: model.gender
                                        font.pixelSize: 13
                                        color: "white"
                                    }
                                }

                                // Date of Birth cell
                                Rectangle {
                                    width: parent.width*0.3125
                                    height: parent.height
                                    color: "transparent"

                                    Text {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 16
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: model.dateOfBirth
                                        font.pixelSize: 13
                                        color: "white"
                                    }
                                }
                            }
                        }
                    }
                }
            }


    }









}
