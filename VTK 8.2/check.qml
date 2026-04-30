import QtQuick 2.14
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 1200
    height: 800
    title: "Medical Imaging Interface"

    // Main background with medical teal color
    Rectangle {
        anchors.fill: parent
        color: "#5A7A7A"  // Medical teal background

        // Main content container
        Column {
            anchors.fill: parent
            anchors.margins: parent.width * 0.02  // 2% margins
            spacing: parent.height * 0.03  // 3% spacing

            // Top Carousel Section
            Rectangle {
                id: carouselSection
                width: parent.width
                height: parent.height * 0.35  // 35% of window height
                color: "transparent"

                // Carousel container with rounded corners
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width * 0.9  // 90% of section width
                    height: parent.height * 0.85  // 85% of section height
                    color: "#4A6A6A"  // Darker teal
                    radius: height * 0.05  // 5% of container height

                    // Drop shadow effect
                    layer.enabled: true
                    layer.effect: DropShadow {
                        horizontalOffset: 0
                        verticalOffset: parent.height * 0.02  // 2% of container height
                        radius: parent.width * 0.01  // 1% of container width
                        samples: 17
                        color: "#40000000"  // 25% black
                    }

                    // Carousel content
                    Item {
                        anchors.fill: parent
                        anchors.margins: parent.height * 0.08  // 8% margins

                        // Left navigation arrow
                        Rectangle {
                            id: leftArrow
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.height * 0.15  // 15% of content height
                            height: width
                            radius: width / 2
                            color: leftArrowMouseArea.pressed ? "#3A5A5A" :
                                   (leftArrowMouseArea.containsMouse ? "#3F5F5F" : "#5A7A7A")

                            // Smooth hover animation
                            Behavior on color {
                                ColorAnimation {
                                    duration: 200
                                    easing.type: Easing.InOutQuad
                                }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "◀"
                                color: "white"
                                font.pixelSize: parent.width * 0.4  // 40% of arrow width
                                font.weight: Font.Bold
                            }

                            MouseArea {
                                id: leftArrowMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    if (carouselModel.currentPage > 0) {
                                        carouselModel.currentPage--
                                    }
                                }
                            }
                        }

                        // Right navigation arrow
                        Rectangle {
                            id: rightArrow
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.height * 0.15  // 15% of content height
                            height: width
                            radius: width / 2
                            color: rightArrowMouseArea.pressed ? "#3A5A5A" :
                                   (rightArrowMouseArea.containsMouse ? "#3F5F5F" : "#5A7A7A")

                            // Smooth hover animation
                            Behavior on color {
                                ColorAnimation {
                                    duration: 200
                                    easing.type: Easing.InOutQuad
                                }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "▶"
                                color: "white"
                                font.pixelSize: parent.width * 0.4  // 40% of arrow width
                                font.weight: Font.Bold
                            }

                            MouseArea {
                                id: rightArrowMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    if (carouselModel.currentPage < carouselModel.totalPages - 1) {
                                        carouselModel.currentPage++
                                    }
                                }
                            }
                        }

                        // Image cards container
                        Row {
                            anchors.centerIn: parent
                            anchors.leftMargin: leftArrow.width + (parent.width * 0.05)  // Arrow width + 5% spacing
                            anchors.rightMargin: rightArrow.width + (parent.width * 0.05)  // Arrow width + 5% spacing
                            spacing: parent.width * 0.03  // 3% spacing between cards

                            Repeater {
                                id: imageRepeater
                                model: 3  // Show 3 images per page

                                Rectangle {
                                    id: imageCard
                                    width: (parent.parent.width - leftArrow.width - rightArrow.width - (parent.parent.width * 0.1) - (parent.spacing * 2)) / 3  // Calculate width for 3 cards
                                    height: parent.parent.height * 0.7  // 70% of content height
                                    radius: height * 0.05  // 5% of card height
                                    color: cardMouseArea.containsMouse ? "#6A8A8A" : "#5A7A7A"
                                    border.width: carouselModel.selectedIndex === (carouselModel.currentPage * 3 + index) ? 3 : 1
                                    border.color: carouselModel.selectedIndex === (carouselModel.currentPage * 3 + index) ? "#7DD3FC" : "#7A9A9A"

                                    property var imageData: carouselModel.getImageData(carouselModel.currentPage * 3 + index)

                                    // Smooth hover and selection animations
                                    Behavior on color {
                                        ColorAnimation {
                                            duration: 200
                                            easing.type: Easing.InOutQuad
                                        }
                                    }

                                    Behavior on border.color {
                                        ColorAnimation {
                                            duration: 200
                                            easing.type: Easing.InOutQuad
                                        }
                                    }

                                    Column {
                                        anchors.fill: parent
                                        anchors.margins: parent.height * 0.08  // 8% margins
                                        spacing: parent.height * 0.05  // 5% spacing

                                        // Image placeholder
                                        Rectangle {
                                            width: parent.width
                                            height: parent.height * 0.7  // 70% of card height
                                            radius: height * 0.05  // 5% of image height
                                            color: "#3A5A5A"  // Darker background for image
                                            border.width: 1
                                            border.color: "#7A9A9A"

                                            // Medical imaging visualization
                                            Rectangle {
                                                anchors.centerIn: parent
                                                width: parent.width * 0.6  // 60% of image area
                                                height: parent.height * 0.6  // 60% of image area
                                                radius: width * 0.1  // 10% of visualization width
                                                color: "#6A8A8A"

                                                // Anatomical representation
                                                Rectangle {
                                                    anchors.centerIn: parent
                                                    width: parent.width * 0.7  // 70% of container
                                                    height: parent.height * 0.7  // 70% of container
                                                    radius: imageCard.imageData ? (imageCard.imageData.name.includes("Skull") ? width/2 : width*0.1) : width*0.1
                                                    color: "#8AAABA"

                                                    // Crosshair overlay for targeting
                                                    Rectangle {
                                                        anchors.centerIn: parent
                                                        width: parent.width * 0.8  // 80% of anatomy
                                                        height: 2
                                                        color: "#FFD700"  // Gold crosshair
                                                    }

                                                    Rectangle {
                                                        anchors.centerIn: parent
                                                        width: 2
                                                        height: parent.height * 0.8  // 80% of anatomy
                                                        color: "#FFD700"  // Gold crosshair
                                                    }
                                                }
                                            }
                                        }

                                        // Image label
                                        Text {
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            text: imageCard.imageData ? imageCard.imageData.name : ""
                                            color: "white"
                                            font.pixelSize: parent.height * 0.12  // 12% of card height
                                            font.weight: Font.Medium
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }

                                    MouseArea {
                                        id: cardMouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            carouselModel.selectedIndex = carouselModel.currentPage * 3 + index
                                            bodyInterface.currentImagingType = imageCard.imageData ? imageCard.imageData.name : ""
                                            console.log("Selected:", bodyInterface.currentImagingType)
                                        }
                                    }
                                }
                            }
                        }

                        // Page indicators
                        Row {
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: parent.width * 0.02  // 2% spacing

                            Repeater {
                                model: carouselModel.totalPages

                                Rectangle {
                                    width: carouselSection.height * 0.03  // 3% of section height
                                    height: width
                                    radius: width / 2
                                    color: index === carouselModel.currentPage ? "#7DD3FC" : "#7A9A9A"

                                    // Smooth page indicator animation
                                    Behavior on color {
                                        ColorAnimation {
                                            duration: 200
                                            easing.type: Easing.InOutQuad
                                        }
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            carouselModel.currentPage = index
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Central Body Interface Section
            Rectangle {
                id: bodySection
                width: parent.width
                height: parent.height * 0.6  // 60% of window height
                color: "transparent"

                // Body interface container
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width * 0.9  // 90% of section width
                    height: parent.height * 0.9  // 90% of section height
                    color: "#4A6A6A"  // Darker teal
                    radius: height * 0.02  // 2% of container height

                    // Drop shadow effect
                    layer.enabled: true
                    layer.effect: DropShadow {
                        horizontalOffset: 0
                        verticalOffset: parent.height * 0.01  // 1% of container height
                        radius: parent.width * 0.005  // 0.5% of container width
                        samples: 17
                        color: "#40000000"  // 25% black
                    }

                    // Body interface content
                    Item {
                        id: bodyInterface
                        anchors.fill: parent
                        anchors.margins: parent.height * 0.05  // 5% margins

                        property real crosshairX: width * 0.5  // Center X position
                        property real crosshairY: height * 0.4  // Center Y position (chest area)
                        property string currentImagingType: "Chest-PA"

                        // Left imaging plate
                        Rectangle {
                            id: leftPlate
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width * 0.15  // 15% of interface width
                            height: parent.height * 0.6  // 60% of interface height
                            radius: width * 0.1  // 10% of plate width
                            color: "#6A8A8A"
                            border.width: 2
                            border.color: "#8AAABA"

                            // Plate details
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width * 0.8  // 80% of plate width
                                height: parent.height * 0.9  // 90% of plate height
                                radius: width * 0.05  // 5% of detail width
                                color: "#5A7A7A"
                                border.width: 1
                                border.color: "#7A9A9A"
                            }
                        }

                        // Right imaging plate
                        Rectangle {
                            id: rightPlate
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width * 0.15  // 15% of interface width
                            height: parent.height * 0.6  // 60% of interface height
                            radius: width * 0.1  // 10% of plate width
                            color: "#6A8A8A"
                            border.width: 2
                            border.color: "#8AAABA"

                            // Plate details
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width * 0.8  // 80% of plate width
                                height: parent.height * 0.9  // 90% of plate height
                                radius: width * 0.05  // 5% of detail width
                                color: "#5A7A7A"
                                border.width: 1
                                border.color: "#7A9A9A"
                            }
                        }

                        // Central body area
                        Rectangle {
                            id: bodyArea
                            anchors.centerIn: parent
                            width: parent.width * 0.6  // 60% of interface width
                            height: parent.height * 0.8  // 80% of interface height
                            color: "#5A7A7A"
                            radius: height * 0.03  // 3% of body area height
                            border.width: 2
                            border.color: "#7A9A9A"

                            // Human body silhouette
                            Item {
                                id: humanBody
                                anchors.centerIn: parent
                                width: parent.width * 0.6  // 60% of body area
                                height: parent.height * 0.85  // 85% of body area

                                // Head
                                Rectangle {
                                    id: head
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.top: parent.top
                                    width: parent.width * 0.25  // 25% of body width
                                    height: width
                                    radius: width / 2
                                    color: "#6A8A8A"
                                    border.width: 1
                                    border.color: "#8AAABA"
                                }

                                // Torso
                                Rectangle {
                                    id: torso
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.top: head.bottom
                                    anchors.topMargin: parent.height * 0.02  // 2% spacing
                                    width: parent.width * 0.5  // 50% of body width
                                    height: parent.height * 0.45  // 45% of body height
                                    radius: width * 0.1  // 10% of torso width
                                    color: "#6A8A8A"
                                    border.width: 1
                                    border.color: "#8AAABA"
                                }

                                // Left arm
                                Rectangle {
                                    id: leftArm
                                    anchors.right: torso.left
                                    anchors.top: torso.top
                                    anchors.topMargin: torso.height * 0.1  // 10% from torso top
                                    width: parent.width * 0.15  // 15% of body width
                                    height: parent.height * 0.4  // 40% of body height
                                    radius: width * 0.3  // 30% of arm width
                                    color: "#6A8A8A"
                                    border.width: 1
                                    border.color: "#8AAABA"
                                }

                                // Right arm
                                Rectangle {
                                    id: rightArm
                                    anchors.left: torso.right
                                    anchors.top: torso.top
                                    anchors.topMargin: torso.height * 0.1  // 10% from torso top
                                    width: parent.width * 0.15  // 15% of body width
                                    height: parent.height * 0.4  // 40% of body height
                                    radius: width * 0.3  // 30% of arm width
                                    color: "#6A8A8A"
                                    border.width: 1
                                    border.color: "#8AAABA"
                                }

                                // Left leg
                                Rectangle {
                                    id: leftLeg
                                    anchors.right: parent.horizontalCenter
                                    anchors.rightMargin: parent.width * 0.05  // 5% from center
                                    anchors.top: torso.bottom
                                    anchors.topMargin: parent.height * 0.02  // 2% spacing
                                    width: parent.width * 0.18  // 18% of body width
                                    height: parent.height * 0.45  // 45% of body height
                                    radius: width * 0.25  // 25% of leg width
                                    color: "#6A8A8A"
                                    border.width: 1
                                    border.color: "#8AAABA"
                                }

                                // Right leg
                                Rectangle {
                                    id: rightLeg
                                    anchors.left: parent.horizontalCenter
                                    anchors.leftMargin: parent.width * 0.05  // 5% from center
                                    anchors.top: torso.bottom
                                    anchors.topMargin: parent.height * 0.02  // 2% spacing
                                    width: parent.width * 0.18  // 18% of body width
                                    height: parent.height * 0.45  // 45% of body height
                                    radius: width * 0.25  // 25% of leg width
                                    color: "#6A8A8A"
                                    border.width: 1
                                    border.color: "#8AAABA"
                                }
                            }

                            // Crosshair targeting overlay
                            Item {
                                id: crosshair
                                anchors.fill: parent

                                // Horizontal crosshair line
                                Rectangle {
                                    x: bodyInterface.crosshairX - width/2
                                    y: bodyInterface.crosshairY - height/2
                                    width: bodyArea.width * 0.3  // 30% of body area width
                                    height: 3
                                    color: "#FFD700"  // Gold color
                                    radius: height / 2

                                    // Smooth position animation
                                    Behavior on x {
                                        NumberAnimation {
                                            duration: 300
                                            easing.type: Easing.OutCubic
                                        }
                                    }

                                    Behavior on y {
                                        NumberAnimation {
                                            duration: 300
                                            easing.type: Easing.OutCubic
                                        }
                                    }
                                }

                                // Vertical crosshair line
                                Rectangle {
                                    x: bodyInterface.crosshairX - width/2
                                    y: bodyInterface.crosshairY - height/2
                                    width: 3
                                    height: bodyArea.height * 0.3  // 30% of body area height
                                    color: "#FFD700"  // Gold color
                                    radius: width / 2

                                    // Smooth position animation
                                    Behavior on x {
                                        NumberAnimation {
                                            duration: 300
                                            easing.type: Easing.OutCubic
                                        }
                                    }

                                    Behavior on y {
                                        NumberAnimation {
                                            duration: 300
                                            easing.type: Easing.OutCubic
                                        }
                                    }
                                }

                                // Targeting area highlight
                                Rectangle {
                                    x: bodyInterface.crosshairX - width/2
                                    y: bodyInterface.crosshairY - height/2
                                    width: bodyArea.width * 0.25  // 25% of body area width
                                    height: bodyArea.height * 0.2  // 20% of body area height
                                    color: "#FFD70040"  // Semi-transparent gold
                                    radius: width * 0.1  // 10% of highlight width
                                    border.width: 2
                                    border.color: "#FFD700"

                                    // Smooth position animation
                                    Behavior on x {
                                        NumberAnimation {
                                            duration: 300
                                            easing.type: Easing.OutCubic
                                        }
                                    }

                                    Behavior on y {
                                        NumberAnimation {
                                            duration: 300
                                            easing.type: Easing.OutCubic
                                        }
                                    }
                                }
                            }

                            // Click area for crosshair positioning
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.CrossCursor
                                onClicked: {
                                    bodyInterface.crosshairX = mouse.x
                                    bodyInterface.crosshairY = mouse.y
                                    console.log("Crosshair positioned at:", mouse.x, mouse.y)
                                }
                            }
                        }
                    }
                }
            }

            // Control Panel Section
            Rectangle {
                id: controlPanel
                width: parent.width
                height: parent.height * 0.08  // 8% of window height
                color: "transparent"

                Row {
                    anchors.centerIn: parent
                    spacing: parent.width * 0.05  // 5% spacing

                    // Current position display
                    Rectangle {
                        width: controlPanel.width * 0.3  // 30% of panel width
                        height: controlPanel.height * 0.8  // 80% of panel height
                        radius: height * 0.2  // 20% of display height
                        color: "#4A6A6A"
                        border.width: 1
                        border.color: "#7A9A9A"

                        Text {
                            anchors.centerIn: parent
                            text: "Current: " + bodyInterface.currentImagingType
                            color: "white"
                            font.pixelSize: parent.height * 0.4  // 40% of display height
                            font.weight: Font.Medium
                        }
                    }

                    // Capture button
                    Rectangle {
                        id: captureButton
                        width: controlPanel.width * 0.15  // 15% of panel width
                        height: controlPanel.height * 0.8  // 80% of panel height
                        radius: height * 0.2  // 20% of button height
                        color: captureMouseArea.pressed ? "#0F766E" :
                               (captureMouseArea.containsMouse ? "#14B8A6" : "#5EEAD4")

                        // Smooth hover animation
                        Behavior on color {
                            ColorAnimation {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "CAPTURE"
                            color: "white"
                            font.pixelSize: parent.height * 0.35  // 35% of button height
                            font.weight: Font.Bold
                        }

                        MouseArea {
                            id: captureMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                console.log("Capture image at position:", bodyInterface.crosshairX, bodyInterface.crosshairY)
                                console.log("Imaging type:", bodyInterface.currentImagingType)
                            }
                        }
                    }

                    // Reset button
                    Rectangle {
                        id: resetButton
                        width: controlPanel.width * 0.15  // 15% of panel width
                        height: controlPanel.height * 0.8  // 80% of panel height
                        radius: height * 0.2  // 20% of button height
                        color: resetMouseArea.pressed ? "#DC2626" :
                               (resetMouseArea.containsMouse ? "#EF4444" : "#F87171")

                        // Smooth hover animation
                        Behavior on color {
                            ColorAnimation {
                                duration: 150
                                easing.type: Easing.InOutQuad
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "RESET"
                            color: "white"
                            font.pixelSize: parent.height * 0.35  // 35% of button height
                            font.weight: Font.Bold
                        }

                        MouseArea {
                            id: resetMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                bodyInterface.crosshairX = bodyArea.width * 0.5
                                bodyInterface.crosshairY = bodyArea.height * 0.4
                                carouselModel.selectedIndex = 2  // Reset to Chest-PA
                                bodyInterface.currentImagingType = "Chest-PA"
                                console.log("Interface reset")
                            }
                        }
                    }
                }
            }
        }
    }

    // Carousel data model
    QtObject {
        id: carouselModel

        property int currentPage: 0
        property int selectedIndex: 2  // Default to Chest-PA
        property int totalPages: 2  // 4 images, 3 per page = 2 pages

        property var images: [
            { name: "Skull-AP", type: "skull" },
            { name: "Leg Foot-LAT", type: "leg" },
            { name: "Chest-PA", type: "chest" },
            { name: "Spine-LAT", type: "spine" }
        ]

        function getImageData(index) {
            if (index >= 0 && index < images.length) {
                return images[index]
            }
            return null
        }
    }
}
