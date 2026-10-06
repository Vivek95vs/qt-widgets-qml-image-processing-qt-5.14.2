import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Rectangle {
    id: root

    color: "#111A26"
    border.color: "#29384B"
    border.width: 1

    property int minimumValue: 0
    property int maximumValue: 65535

    property real brightnessValue: 0
    property real contrastValue: 1.0
    property string selectedToolTab: "B&C"
    property var dicomLoader

    signal minimumChangedByUser(int value)
    signal maximumChangedByUser(int value)

    signal brightnessChangedByUser(real value)
    signal contrastChangedByUser(real value)


    signal resetRequested()
    signal autoRequested()
    signal applyRequested()
    // =========================================================
    // MAIN CONTENT
    // =========================================================

    // =============================================================
    // B&C FLOATING PANEL
    // =============================================================

    Item {
            id: bcFloatingPanel

            // Put the floating panel on the application window content layer.
            // This allows it to float over the viewer instead of becoming
            // part of the Enhancement Tools layout.
           parent: Window.window ? Window.window.contentItem : root
            width: 265
            height: 500
            x: parent ? parent.width - width - 8 : 0
            y: 8
            z: 99999
           visible: false

          // =========================================================
          // PANEL BACKGROUND
          // =========================================================
            Rectangle {
               anchors.fill: parent
               color: "#111A26"
               border.color: "#29384B"
                border.width: 1
                radius: 2
            }
            // =========================================================
            // CONTENT
           // =========================================================
           ColumnLayout {
                anchors.fill: parent
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                anchors.topMargin: 4
                anchors.bottomMargin: 4
                spacing: 4

               // =====================================================
                // HEADER
               // =====================================================

                Item {
                    id: bcHeader
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: "B&C"
                        color: "#FFFFFF"
                        font.pixelSize: 13
                       font.bold: true
                   }

                   Button {
                        id: bcCloseButton
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        width: 22

                        height: 22
                        background: Rectangle {
                            color: "transparent"

                        }
                        contentItem: Text {

                            text: "×"
                            color: "#9CAFC2"
                            font.pixelSize: 18
                            horizontalAlignment:
                                    Text.AlignHCenter
                            verticalAlignment:
                                    Text.AlignVCenter

                        }

                        onClicked: {

                            bcFloatingPanel.visible = false

                        }
                    }
                   // =================================================
                   // DRAG HEADER
                   // =================================================

                    MouseArea {
                        id: bcDragArea

                        anchors.left: parent.left
                        anchors.right: bcCloseButton.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        property real startX: 0
                        property real startY: 0

                        onPressed: {
                            startX = mouse.x
                            startY = mouse.y
                        }

                        onPositionChanged: {
                            if (pressed) {
                                bcFloatingPanel.x += mouse.x - startX
                                bcFloatingPanel.y += mouse.y - startY
                            }
                        }
                    }
                }

                Rectangle {
                   Layout.fillWidth: true
                    height: 1
                    color: "#29384B"
                }
                // =====================================================
                // MINIMUM
                // =====================================================

                Text {
                    text: "Minimum"
                    color: "#FFFFFF"
                    font.pixelSize: 11
                    Layout.fillWidth: true

                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    Slider {
                        id: bcMinimumSlider
                        Layout.fillWidth: true
                        from: 0
                        to: 65535
                        value: root.minimumValue
                        onMoved: {
                            var newValue =
                                    Math.round(value)
                            if (newValue >
                                root.maximumValue)
                                newValue =
                                        root.maximumValue
                            root.minimumValue =
                                    newValue
                            root.minimumChangedByUser(
                                        newValue)


                        }
                        background: Rectangle {
                            x: bcMinimumSlider.leftPadding
                            y: bcMinimumSlider.topPadding
                               + bcMinimumSlider.availableHeight / 2
                               - height / 2
                            width:
                                    bcMinimumSlider.availableWidth

                            height: 4

                            radius: 2

                            color: "#475569"
                            Rectangle {
                               width:
                                    bcMinimumSlider.visualPosition
                                    * parent.width

                                height:
                                    parent.height
                                radius: 2
                                color: "#3B82F6"

                            }

                        }
                        handle: Rectangle {
                           x:bcMinimumSlider.leftPadding
                                + bcMinimumSlider.visualPosition
                                * (bcMinimumSlider.availableWidth
                                  - width)

                           y:bcMinimumSlider.topPadding
                                + bcMinimumSlider.availableHeight / 2
                                - height / 2

                            width: 12
                            height: 12
                            radius: width / 2
                            color:
                                bcMinimumSlider.pressed
                                ? "#60A5FA"
                                : "#3B82F6"

                            border.color: "#93C5FD"
                            border.width: 1

                        }

                    }
                    TextField {
                        Layout.preferredWidth: 56
                        Layout.preferredHeight: 30
                        text: root.minimumValue
                        horizontalAlignment:
                                Text.AlignHCenter

                        color: "#DCE6F0"

                        font.pixelSize: 11
                        background: Rectangle {
                            color: "#162333"

                            border.color: "#34475B"

                            border.width: 1

                            radius: 3

                        }

                    }

                }
                // =====================================================
                // MAXIMUM
                // =====================================================
                Text {
                    text: "Maximum"
                   color: "#FFFFFF"
                    font.pixelSize: 11
                    Layout.fillWidth: true

                }

                RowLayout {

                    Layout.fillWidth: true
                    spacing: 8

                    Slider {

                        id: bcMaximumSlider

                        Layout.fillWidth: true

                        from: 0
                        to: 65535
                       value: root.maximumValue
                        onMoved: {
                            var newValue =
                                    Math.round(value)

                           if (newValue <
                                root.minimumValue)
                                newValue =
                                        root.minimumValue

                            root.maximumValue =
                                    newValue
                            root.maximumChangedByUser(

                                        newValue)

                        }


                        background: Rectangle {

                            x: bcMaximumSlider.leftPadding

                            y: bcMaximumSlider.topPadding

                               + bcMaximumSlider.availableHeight / 2

                               - height / 2

                            width:
                                    bcMaximumSlider.availableWidth
                            height: 4

                            radius: 2

                            color: "#475569"

                            Rectangle {

                                width:

                                    bcMaximumSlider.visualPosition

                                    * parent.width

                                height:

                                    parent.height

                                radius: 2

                               color: "#3B82F6"

                            }

                        }


                        handle: Rectangle {
                            x:  bcMaximumSlider.leftPadding

                                + bcMaximumSlider.visualPosition

                                * (bcMaximumSlider.availableWidth

                                   - width)
                           y:

                                bcMaximumSlider.topPadding
                                + bcMaximumSlider.availableHeight / 2
                                - height / 2

                            width: 12
                            height: 12
                            radius: width / 2

                            color:
                              bcMaximumSlider.pressed
                                ? "#60A5FA"
                                : "#3B82F6"
                            border.color: "#93C5FD"
                            border.width: 1

                        }

                    }

                    TextField {

                        Layout.preferredWidth: 56
                        Layout.preferredHeight: 30
                        text: root.maximumValue
                        horizontalAlignment:

                                Text.AlignHCenter

                        color: "#DCE6F0"
                        font.pixelSize: 11
                        background: Rectangle {

                            color: "#162333"

                            border.color: "#34475B"

                            border.width: 1

                            radius: 3

                        }

                    }

                }
                // =====================================================
                // BRIGHTNESS
                // =====================================================

                Text {
                    text: "Brightness"
                    color: "#FFFFFF"
                    font.pixelSize: 11
                    Layout.fillWidth: true
                }
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    Slider {
                        id: bcBrightnessSlider

                        Layout.fillWidth: true

                        from: -100
                        to: 100
                        value: root.brightnessValue

                        onMoved: {

                            var newValue =
                                    Math.round(value)


                            root.brightnessValue =
                                    newValue

                            root.brightnessChangedByUser(
                                        newValue)
                        }

                        background: Rectangle {
                            x:
                                bcBrightnessSlider.leftPadding

                            y:
                                bcBrightnessSlider.topPadding
                                + bcBrightnessSlider.availableHeight / 2
                                - height / 2
                            width:
                                bcBrightnessSlider.availableWidth
                            height: 4
                            radius: 2
                            color: "#475569"
                            Rectangle {
                                width:
                                    bcBrightnessSlider.visualPosition
                                    * parent.width
                                height:
                                    parent.height
                                radius: 2
                                color: "#3B82F6"

                            }

                        }
                        handle: Rectangle {
                            x:
                                bcBrightnessSlider.leftPadding
                                + bcBrightnessSlider.visualPosition
                                * (bcBrightnessSlider.availableWidth
                                   - width)
                            y:

                                bcBrightnessSlider.topPadding
                                + bcBrightnessSlider.availableHeight / 2
                                - height / 2

                            width: 12

                            height: 12
                            radius: width / 2
                            color:
                                bcBrightnessSlider.pressed
                                ? "#60A5FA"
                                : "#3B82F6"
                            border.color: "#93C5FD"
                            border.width: 1

                        }

                    }
                   TextField {
                        Layout.preferredWidth: 56
                        Layout.preferredHeight: 30

                        text: root.brightnessValue

                        horizontalAlignment:
                               Text.AlignHCenter
                       color: "#DCE6F0"
                        font.pixelSize: 11

                        background: Rectangle {
                            color: "#162333"
                            border.color: "#34475B"
                            border.width: 1
                            radius: 3

                        }

                    }

                }

                // =====================================================
                // CONTRAST
                // =====================================================

                Text {
                   text: "Contrast"
                    color: "#FFFFFF"
                   font.pixelSize: 11
                  Layout.fillWidth: true

                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    Slider {
                        id: bcContrastSlider
                        Layout.fillWidth: true
                        from: 0.0
                        to: 2.0

                        stepSize: 0.1
                       value: root.contrastValue
                        onMoved: {
                           var newValue =
                                  Math.round(value * 10) / 10
                            root.contrastValue =
                                    newValue
                            root.contrastChangedByUser(
                                        newValue)

                        }
                        background: Rectangle {
                            x:
                               bcContrastSlider.leftPadding
                            y:
                                bcContrastSlider.topPadding
                                + bcContrastSlider.availableHeight / 2
                                - height / 2
                            width:
                                bcContrastSlider.availableWidth
                            height: 4
                            radius: 2
                            color: "#475569"

                            Rectangle {
                                width:
                                    bcContrastSlider.visualPosition
                                    * parent.width
                                height:
                                    parent.height
                                radius: 2
                                color: "#3B82F6"

                            }

                        }

                        handle: Rectangle {
                            x:
                                bcContrastSlider.leftPadding
                                + bcContrastSlider.visualPosition
                                * (bcContrastSlider.availableWidth
                                   - width)
                            y:
                                bcContrastSlider.topPadding
                                + bcContrastSlider.availableHeight / 2
                                - height / 2
                            width: 12
                            height: 12
                            radius: width / 2
                            color:
                                bcContrastSlider.pressed
                                ? "#60A5FA"
                                : "#3B82F6"
                            border.color: "#93C5FD"
                            border.width: 1

                        }

                    }

                    TextField {
                        Layout.preferredWidth: 56
                        Layout.preferredHeight: 30
                        text:
                            root.contrastValue.toFixed(1)
                        horizontalAlignment:
                                Text.AlignHCenter
                        color: "#DCE6F0"
                        font.pixelSize: 11
                        background: Rectangle {
                            color: "#162333"
                            border.color: "#34475B"
                            border.width: 1
                            radius: 3

                        }

                    }
                }
                // =====================================================
                // AUTO / RESET
                // =====================================================
                RowLayout {
                    Layout.fillWidth: true

                    spacing: 6
                    Button {
                        id: bcAutoButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        text: "Auto"
                        background: Rectangle {
                            radius: 3
                            color:
                                bcAutoButton.pressed
                                ? "#356896"
                                : bcAutoButton.hovered
                                  ? "#263B52"
                                  : "#1A2838"
                            border.color:
                                bcAutoButton.pressed
                                ? "#60A5FA"
                                : "#34475B"
                            border.width: 1

                        }
                        contentItem: Text {
                            text:
                                bcAutoButton.text
                            color:
                                bcAutoButton.pressed
                                ? "#FFFFFF"
                                : "#C9D4DF"
                            font.pixelSize: 11

                            horizontalAlignment:
                                    Text.AlignHCenter
                            verticalAlignment:
                                    Text.AlignVCenter

                        }
                        onClicked: {
                            root.autoRequested()
                        }

                    }

                    Button {
                        id: bcResetButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        text: "Reset"
                        background: Rectangle {
                            radius: 3
                            color:
                                bcResetButton.pressed
                                ? "#356896"
                                : bcResetButton.hovered
                                  ? "#263B52"
                                  : "#1A2838"

                            border.color:
                                bcResetButton.pressed
                                ? "#60A5FA"
                                : "#34475B"

                            border.width: 1
                        }
                        contentItem: Text {
                            text:
                                bcResetButton.text

                            color:
                                bcResetButton.pressed
                                ? "#FFFFFF"
                                : "#C9D4DF"

                            font.pixelSize: 11
                            horizontalAlignment:
                                    Text.AlignHCenter
                            verticalAlignment:
                                    Text.AlignVCenter

                        }

                        onClicked: {
                            root.resetRequested()
                        }
                    }
                }

                // =====================================================
                // SET / APPLY
                // =====================================================
                RowLayout {

                    Layout.fillWidth: true
                    spacing: 6

                    Button {
                        id: bcSetButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        text: "Set"

                        background: Rectangle {
                            radius: 3
                            color:
                                bcSetButton.pressed
                                ? "#356896"
                                : bcSetButton.hovered
                                  ? "#263B52"
                                  : "#1A2838"
                            border.color:
                                bcSetButton.pressed
                                ? "#60A5FA"
                                : "#34475B"
                            border.width: 1
                        }
                        contentItem: Text {
                            text:
                                bcSetButton.text
                            color:
                                bcSetButton.pressed
                                ? "#FFFFFF"
                                : "#C9D4DF"

                            font.pixelSize: 11
                            horizontalAlignment:

                                    Text.AlignHCenter

                            verticalAlignment:
                                    Text.AlignVCenter

                        }

                        onClicked: {
                            root.applyRequested()
                        }
                    }
                    Button {
                        id: bcApplyButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        text: "Apply"
                        background: Rectangle {
                            radius: 3
                            color:
                                bcApplyButton.pressed
                                ? "#356896"
                                : bcApplyButton.hovered
                                  ? "#263B52"
                                  : "#1A2838"
                            border.color:
                                bcApplyButton.pressed
                                ? "#60A5FA"
                                : "#34475B"
                            border.width: 1

                        }
                        contentItem: Text {
                            text:
                                bcApplyButton.text
                            color:
                                bcApplyButton.pressed

                                ? "#FFFFFF"
                                : "#C9D4DF"

                            font.pixelSize: 11
                            horizontalAlignment:
                                    Text.AlignHCenter

                            verticalAlignment:
                                    Text.AlignVCenter
                        }
                        onClicked: {
                            root.applyRequested()
                        }
                    }
                }

                // =====================================================
                // HISTOGRAM TITLE
                // =====================================================
                Text {
                    text: "HISTOGRAM"
                    color: "#FFFFFF"
                    font.pixelSize: 11
                    font.bold: true
                    Layout.fillWidth: true

                }
                // =====================================================
                // HISTOGRAM
                // =====================================================


                // =====================================================
                // HISTOGRAM
                // =====================================================

                Rectangle {
                    id: bcHistogramContainer

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 130

                    color: "#0B111A"
                    border.color: "#34475B"
                    border.width: 1
                    radius: 2

                    // =================================================
                    // HISTOGRAM CANVAS
                    // =================================================

                    Canvas {
                        id: bcHistogramCanvas

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        anchors.leftMargin: 4
                        anchors.rightMargin: 4
                        anchors.topMargin: 4
                        anchors.bottomMargin: 16

                        onPaint: {

                            var ctx = getContext("2d")

                            // =========================================
                            // CLEAR
                            // =========================================

                            ctx.clearRect(
                                0,
                                0,
                                width,
                                height
                            )

                            // =========================================
                            // CHECK LOADER
                            // =========================================

                            if (!root.dicomLoader)
                                return

                            // =========================================
                            // HISTOGRAM DATA
                            // =========================================

                            var data =
                                    root.dicomLoader.histogramData

                            if (!data ||
                                data.length === 0)
                                return

                            // =========================================
                            // GRAPH AREA
                            // =========================================

                            var graphHeight =
                                    height

                            // =========================================
                            // FIND MAX HISTOGRAM VALUE
                            // =========================================

                            var maxCount = 0

                            for (var i = 0;
                                 i < data.length;
                                 ++i)
                            {
                                var count =
                                        Number(data[i])

                                if (count > maxCount)
                                    maxCount = count
                            }

                            if (maxCount <= 0)
                                return

                            // =========================================
                            // BIN WIDTH
                            // =========================================

                            var binWidth =
                                    width / data.length

                            // =========================================
                            // DRAW HISTOGRAM
                            // =========================================

                            ctx.beginPath()

                            ctx.moveTo(
                                0,
                                graphHeight
                            )

                            for (var j = 0;
                                 j < data.length;
                                 ++j)
                            {
                                var value =
                                        Number(data[j])

                                var barHeight =
                                        (value / maxCount)
                                        * graphHeight

                                var posX =
                                        j * binWidth

                                var posY =
                                        graphHeight
                                        - barHeight

                                ctx.lineTo(
                                    posX,
                                    posY
                                )
                            }

                            ctx.lineTo(
                                width,
                                graphHeight
                            )

                            ctx.closePath()

                            ctx.fillStyle =
                                    "#718096"

                            ctx.fill()


                            // =================================================
                            // IMAGE RANGE
                            // =================================================

                            var dataMin =
                                    root.dicomLoader.histogramMin

                            var dataMax =
                                    root.dicomLoader.histogramMax

                            var dataRange =
                                    dataMax - dataMin

                            if (dataRange <= 0)
                                dataRange = 1


                            // =================================================
                            // PROCESSING VALUES
                            // =================================================

                            var minimum =
                                    root.dicomLoader.minimum

                            var maximum =
                                    root.dicomLoader.maximum

                            var brightness =
                                    root.dicomLoader.brightness

                            var contrast =
                                    root.dicomLoader.contrast


                            // =================================================
                            // MINIMUM / MAXIMUM → X POSITION
                            // =================================================

                            var startX =
                                    ((minimum - dataMin)
                                     / dataRange)
                                    * width

                            var endX =
                                    ((maximum - dataMin)
                                     / dataRange)
                                    * width


                            // =================================================
                            // CLAMP X
                            // =================================================

                            startX =
                                    Math.max(
                                        0,
                                        Math.min(
                                            width,
                                            startX
                                        )
                                    )

                            endX =
                                    Math.max(
                                        0,
                                        Math.min(
                                            width,
                                            endX
                                        )
                                    )

                            if (endX <= startX)
                                endX = startX + 1


                            // =================================================
                            // BRIGHTNESS
                            // -100 → +100
                            // =================================================

                            var brightnessNormalized =
                                    brightness / 100.0


                            // =================================================
                            // CONTRAST
                            // 0 → 2
                            // =================================================

                            contrast =
                                    Math.max(
                                        0.0,
                                        Math.min(
                                            2.0,
                                            contrast
                                        )
                                    )


                            // =================================================
                            // CLIP ONLY TRANSFER LINE
                            // =================================================

                            ctx.save()

                            ctx.beginPath()

                            ctx.rect(
                                0,
                                0,
                                width,
                                graphHeight
                            )

                            ctx.clip()


                            // =================================================
                            // DRAW TRANSFER FUNCTION
                            // =================================================

                            ctx.beginPath()

                            var contrastValue =
                                    contrast


                            // =================================================
                            // CONTRAST >= 2
                            // VERTICAL LINE
                            // =================================================

                            if (contrastValue >= 1.99)
                            {
                                var centerX =
                                        (startX + endX) / 2

                                var brightnessOffset =
                                        brightnessNormalized
                                        * graphHeight

                                var centerY =
                                        (graphHeight / 2)
                                        - brightnessOffset

                                ctx.moveTo(
                                    centerX,
                                    graphHeight
                                )

                                ctx.lineTo(
                                    centerX,
                                    0
                                )
                            }

                            // =================================================
                            // NORMAL CONTRAST
                            // =================================================

                            else
                            {
                                var firstPoint =
                                        true

                                for (var x = startX;
                                     x <= endX;
                                     x += 1)
                                {
                                    // =====================================
                                    // NORMALIZED INPUT
                                    // =====================================

                                    var input =
                                            (x - startX)
                                            / (endX - startX)


                                    // =====================================
                                    // CONTRAST
                                    // =====================================

                                    var effectiveContrast =
                                            contrastValue

                                    var output =
                                            ((input - 0.5)
                                             * effectiveContrast)
                                            + 0.5


                                    // =====================================
                                    // BRIGHTNESS
                                    // =====================================

                                    output =
                                            output
                                            + brightnessNormalized


                                    // =====================================
                                    // DO NOT CLAMP
                                    // =====================================

                                    var y =
                                            graphHeight
                                            - (output * graphHeight)


                                    // =====================================
                                    // DRAW POINT
                                    // =====================================

                                    if (firstPoint)
                                    {
                                        ctx.moveTo(
                                            x,
                                            y
                                        )

                                        firstPoint =
                                                false
                                    }
                                    else
                                    {
                                        ctx.lineTo(
                                            x,
                                            y
                                        )
                                    }
                                }
                            }


                            // =================================================
                            // DRAW WHITE TRANSFER LINE
                            // =================================================

                            ctx.strokeStyle =
                                    "#FFFFFF"

                            ctx.lineWidth =
                                    1

                            ctx.stroke()


                            // =================================================
                            // REMOVE CLIPPING
                            // =================================================

                            ctx.restore()


                            // =================================================
                            // BOTTOM GRAPH LINE
                            // =================================================

                            ctx.beginPath()

                            ctx.moveTo(
                                0,
                                graphHeight
                            )

                            ctx.lineTo(
                                width,
                                graphHeight
                            )

                            ctx.strokeStyle =
                                    "#34475B"

                            ctx.lineWidth =
                                    1

                            ctx.stroke()
                        }

                        // =================================================
                        // INITIAL PAINT
                        // =================================================

                        Component.onCompleted: {
                            requestPaint()
                        }
                    }


                    // =====================================================
                    // MIN VALUE LABEL
                    // =====================================================

                    Text {
                        id: bcHistogramMinLabel

                        anchors.left:
                                parent.left

                        anchors.bottom:
                                parent.bottom

                        anchors.leftMargin:
                                5

                        anchors.bottomMargin:
                                2

                        text: root.dicomLoader
                              ? root.dicomLoader.histogramMin
                              : "0"

                        color:
                                "#9CA3AF"

                        font.pixelSize:
                                9
                    }


                    // =====================================================
                    // MAX VALUE LABEL
                    // =====================================================

                    Text {
                        id: bcHistogramMaxLabel

                        anchors.right:
                                parent.right

                        anchors.bottom:
                                parent.bottom

                        anchors.rightMargin:
                                5

                        anchors.bottomMargin:
                                2

                        text: root.dicomLoader
                              ? root.dicomLoader.histogramMax
                              : "65535"

                        color:
                                "#9CA3AF"

                        font.pixelSize:
                                9
                    }
                }


                // =====================================================
                // HISTOGRAM LIVE UPDATE
                // =====================================================

                Connections {
                    target: root.dicomLoader

                    function onHistogramChanged()
                    {
                        bcHistogramCanvas.requestPaint()
                    }

                    function onImageProcessingChanged()
                    {
                        bcHistogramCanvas.requestPaint()
                    }
                }


                // =====================================================
                // B&C VALUES LIVE UPDATE
                // =====================================================

                Connections {
                    target: root

                    function onMinimumValueChanged()
                    {
                        bcHistogramCanvas.requestPaint()
                    }

                    function onMaximumValueChanged()
                    {
                        bcHistogramCanvas.requestPaint()
                    }

                    function onBrightnessValueChanged()
                    {
                        bcHistogramCanvas.requestPaint()
                    }

                    function onContrastValueChanged()
                    {
                        bcHistogramCanvas.requestPaint()
                    }
                }

           }
            // =========================================================
            // HISTOGRAM REFRESH
            // =========================================================
            Connections {
                target: root.dicomLoader

                function onHistogramDataChanged() {
                    bcHistogramCanvas.requestPaint()
                }
                function onImageProcessingChanged() {
                    bcHistogramCanvas.requestPaint()
                }
            }

            // =========================================================
            // INITIAL HISTOGRAM
            // =========================================================
            onVisibleChanged: {

                if (visible) {
                    bcHistogramCanvas.requestPaint()

                }
            }
        }

    ColumnLayout {
        anchors.fill: parent

        anchors.leftMargin: 4
        anchors.rightMargin: 4
        anchors.topMargin: 6
        anchors.bottomMargin: 6

        spacing: 6

        // =====================================================
        // ENHANCEMENT TOOLS TITLE
        // =====================================================

        Text {
            text: "ENHANCEMENT TOOLS"

            color: "#D6DEE8"

            font.pixelSize: 12
            font.bold: true

            Layout.fillWidth: true
        }

        // =====================================================
        // TOOL TABS
        // =====================================================



        RowLayout {

            Layout.fillWidth: true

            spacing: 6


            // =================================================
            // B&C TAB
            // =================================================

            Button {

                id: bncButton

                text: "B&C"

                Layout.fillWidth: true
                Layout.preferredHeight: 34

                hoverEnabled: true

                property bool selected:
                    root.selectedToolTab === "B&C"

                background: Rectangle {

                    radius: 3

                    color: bncButton.selected
                           ? "#3B82F6"
                           : bncButton.hovered
                             ? "#263B52"
                             : "#1A2838"

                    border.color: bncButton.selected
                                  ? "#60A5FA"
                                  : "#34475B"

                    border.width: 1
                }

                contentItem: Text {

                    text: bncButton.text

                    color: bncButton.selected
                           ? "#FFFFFF"
                           : "#C9D4DF"

                    font.pixelSize: 11

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {                    
                    root.selectedToolTab = "B&C"
                    bcFloatingPanel.visible = true
                    // Always calculate fresh histogram
                    dicomLoader.calculateHistogram()
                    bcHistogramCanvas.requestPaint()
                    console.log("Selected Tab: B&C")
                }
            }


            // =================================================
            // ADVANCED TAB
            // =================================================

            Button {

                id: advancedButton

                text: "Advanced"

                Layout.fillWidth: true
                Layout.preferredHeight: 34

                hoverEnabled: true

                property bool selected:
                    root.selectedToolTab === "Advanced"

                background: Rectangle {

                    radius: 3

                    color: advancedButton.selected
                           ? "#3B82F6"
                           : advancedButton.hovered
                             ? "#263B52"
                             : "#172434"

                    border.color: advancedButton.selected
                                  ? "#60A5FA"
                                  : "#34465A"

                    border.width: 1
                }

                contentItem: Text {

                    text: advancedButton.text

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    color: advancedButton.selected
                           ? "#FFFFFF"
                           : "#C9D4DF"

                    font.pixelSize: 11
                }

                onClicked: {

                    root.selectedToolTab = "Advanced"

                    console.log("Selected Tab: Advanced")
                }
            }


            // =================================================
            // FILTERS TAB
            // =================================================

            Button {

                id: filtersButton

                text: "Filters"

                Layout.fillWidth: true
                Layout.preferredHeight: 34

                hoverEnabled: true

                property bool selected:
                    root.selectedToolTab === "Filters"

                background: Rectangle {

                    radius: 3

                    color: filtersButton.selected
                           ? "#3B82F6"
                           : filtersButton.hovered
                             ? "#263B52"
                             : "#172434"

                    border.color: filtersButton.selected
                                  ? "#60A5FA"
                                  : "#34465A"

                    border.width: 1
                }

                contentItem: Text {

                    text: filtersButton.text

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    color: filtersButton.selected
                           ? "#FFFFFF"
                           : "#C9D4DF"

                    font.pixelSize: 11
                }

                onClicked: {

                    root.selectedToolTab = "Filters"

                    console.log("Selected Tab: Filters")
                }
            }


            // =================================================
            // AI TOOLS TAB
            // =================================================

            Button {

                id: aiToolsButton

                text: "AI Tools"

                Layout.fillWidth: true
                Layout.preferredHeight: 34

                hoverEnabled: true

                property bool selected:
                    root.selectedToolTab === "AI Tools"

                background: Rectangle {

                    radius: 3

                    color: aiToolsButton.selected
                           ? "#3B82F6"
                           : aiToolsButton.hovered
                             ? "#263B52"
                             : "#172434"

                    border.color: aiToolsButton.selected
                                  ? "#60A5FA"
                                  : "#34465A"

                    border.width: 1
                }

                contentItem: Text {

                    text: aiToolsButton.text

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    color: aiToolsButton.selected
                           ? "#FFFFFF"
                           : "#C9D4DF"

                    font.pixelSize: 11
                }

                onClicked: {

                    root.selectedToolTab = "AI Tools"

                    console.log("Selected Tab: AI Tools")
                }
            }
        }
     /*   // =====================================================
        // MINIMUM
        // =====================================================

        Text {
            text: "Minimum"

            color: "#B8C5D3"

            font.pixelSize: 11
        }

        RowLayout {

            Layout.fillWidth: true

            spacing: 10

            Slider {
                id: minimumSlider

                Layout.fillWidth: true
                from: 0
                to: 65535
                value: root.minimumValue

//                onValueChanged: {
//                    root.minimumValue = Math.round(value)
//                }
                onMoved: {
                    var newValue = Math.round(value)

                    // Minimum should not exceed Maximum
                    if (newValue > root.maximumValue)
                        newValue = root.maximumValue

                    root.minimumValue = newValue
                    root.minimumChangedByUser(newValue)
                }


                background: Rectangle {
                    x: minimumSlider.leftPadding
                    y: minimumSlider.topPadding
                       + minimumSlider.availableHeight / 2
                       - height / 2

                    width: minimumSlider.availableWidth
                    height: 4

                    radius: 2
                    color: "#475569"

                    Rectangle {
                        width: minimumSlider.visualPosition
                               * parent.width

                        height: parent.height
                        radius: 2

                        color: "#3B82F6"
                    }
                }

                handle: Rectangle {
                    x: minimumSlider.leftPadding
                       + minimumSlider.visualPosition
                         * (minimumSlider.availableWidth - width)

                    y: minimumSlider.topPadding
                       + minimumSlider.availableHeight / 2
                       - height / 2

                    width: 12
                    height: 12

                    radius: width / 2

                    color: minimumSlider.pressed
                           ? "#60A5FA"
                           : "#3B82F6"

                    border.color: "#93C5FD"
                    border.width: 1
                }
            }
            TextField {

                Layout.preferredWidth: 56
                Layout.preferredHeight: 30

                text: root.minimumValue

                horizontalAlignment: Text.AlignHCenter

                color: "#DCE6F0"

                font.pixelSize: 11

                background: Rectangle {

                    radius: 3

                    color: "#162333"

                    border.color: "#34475B"
                    border.width: 1
                }

                onEditingFinished: {
                    var newValue = Math.round(Number(text))

                    if (isNaN(newValue))
                        newValue = 0

                    if (newValue < 0)
                        newValue = 0

                    if (newValue > root.maximumValue)
                        newValue = root.maximumValue

                    root.minimumValue = newValue
                    root.minimumChangedByUser(newValue)
                }
            }
        }

        // =====================================================
        // MAXIMUM
        // =====================================================

        Text {
            text: "Maximum"

            color: "#B8C5D3"

            font.pixelSize: 11
        }

        RowLayout {

            Layout.fillWidth: true

            spacing: 10



            Slider {
                id: maximumSlider

                Layout.fillWidth: true
                from: 0
                to: 65535
                value: root.maximumValue

                onMoved: {
                    var newValue = Math.round(value)

                    if (newValue < root.minimumValue)
                        newValue = root.minimumValue

                    root.maximumValue = newValue
                    root.maximumChangedByUser(newValue)
                }

                background: Rectangle {
                    x: maximumSlider.leftPadding
                    y: maximumSlider.topPadding
                       + maximumSlider.availableHeight / 2
                       - height / 2

                    width: maximumSlider.availableWidth
                    height: 4

                    radius: 2
                    color: "#475569"

                    Rectangle {
                        width: maximumSlider.visualPosition
                               * parent.width

                        height: parent.height
                        radius: 2

                        color: "#3B82F6"
                    }
                }

                handle: Rectangle {
                    x: maximumSlider.leftPadding
                       + maximumSlider.visualPosition
                         * (maximumSlider.availableWidth - width)

                    y: maximumSlider.topPadding
                       + maximumSlider.availableHeight / 2
                       - height / 2

                    width: 12
                    height: 12

                    radius: width / 2

                    color: maximumSlider.pressed
                           ? "#60A5FA"
                           : "#3B82F6"

                    border.color: "#93C5FD"
                    border.width: 1
                }
            }
            TextField {

                Layout.preferredWidth: 56
                Layout.preferredHeight: 30

                text: root.maximumValue

                horizontalAlignment: Text.AlignHCenter

                color: "#DCE6F0"

                font.pixelSize: 11

                background: Rectangle {

                    radius: 3

                    color: "#162333"

                    border.color: "#34475B"
                    border.width: 1
                }

                onEditingFinished: {
                    var newValue = Math.round(Number(text))

                    if (isNaN(newValue))
                        newValue = 65535

                    if (newValue > 65535)
                        newValue = 65535

                    if (newValue < root.minimumValue)
                        newValue = root.minimumValue

                    root.maximumValue = newValue
                    root.maximumChangedByUser(newValue)
                }
            }
        }

        // =====================================================
        // BRIGHTNESS
        // =====================================================

        Text {
            text: "Brightness"

            color: "#B8C5D3"

            font.pixelSize: 11
        }

        RowLayout {

            Layout.fillWidth: true

            spacing: 10



            Slider {
                id: brightnessSlider

                Layout.fillWidth: true
                from: -100
                to: 100
                value: root.brightnessValue

                onMoved: {
                    var newValue = Math.round(value)

                    root.brightnessValue = newValue
                    root.brightnessChangedByUser(newValue)
                }

                background: Rectangle {
                    x: brightnessSlider.leftPadding
                    y: brightnessSlider.topPadding
                       + brightnessSlider.availableHeight / 2
                       - height / 2

                    width: brightnessSlider.availableWidth
                    height: 4

                    radius: 2
                    color: "#475569"

                    Rectangle {
                        width: brightnessSlider.visualPosition
                               * parent.width

                        height: parent.height
                        radius: 2

                        color: "#3B82F6"
                    }
                }

                handle: Rectangle {
                    x: brightnessSlider.leftPadding
                       + brightnessSlider.visualPosition
                         * (brightnessSlider.availableWidth - width)

                    y: brightnessSlider.topPadding
                       + brightnessSlider.availableHeight / 2
                       - height / 2

                    width: 12
                    height: 12

                    radius: width / 2

                    color: brightnessSlider.pressed
                           ? "#60A5FA"
                           : "#3B82F6"

                    border.color: "#93C5FD"
                    border.width: 1
                }
            }

            TextField {

                Layout.preferredWidth: 56
                Layout.preferredHeight: 30

                text: Math.round(root.brightnessValue)

                horizontalAlignment: Text.AlignHCenter

                color: "#DCE6F0"

                font.pixelSize: 11

                background: Rectangle {

                    radius: 3

                    color: "#162333"

                    border.color: "#34475B"
                    border.width: 1
                }
                onEditingFinished: {
                    var newValue = Math.round(Number(text))

                    if (isNaN(newValue))
                        newValue = 0

                    if (newValue < -100)
                        newValue = -100

                    if (newValue > 100)
                        newValue = 100

                    root.brightnessValue = newValue
                    root.brightnessChangedByUser(newValue)
                }
            }
        }

        // =====================================================
        // CONTRAST
        // =====================================================

        Text {
            text: "Contrast"

            color: "#B8C5D3"

            font.pixelSize: 11
        }

        RowLayout {

            Layout.fillWidth: true

            spacing: 10


            Slider {
                id: contrastSlider

                Layout.fillWidth: true

                from: 0.0
                to: 2.0
                stepSize: 0.1

                value: root.contrastValue

                onMoved: {
                    var newValue = Math.round(value * 10) / 10

                    root.contrastValue = newValue
                    root.contrastChangedByUser(newValue)
                }

                background: Rectangle {
                    x: contrastSlider.leftPadding
                    y: contrastSlider.topPadding
                       + contrastSlider.availableHeight / 2
                       - height / 2

                    width: contrastSlider.availableWidth
                    height: 4

                    radius: 2
                    color: "#475569"

                    Rectangle {
                        width: contrastSlider.visualPosition * parent.width
                        height: parent.height
                        radius: 2
                        color: "#3B82F6"
                    }
                }

                handle: Rectangle {
                    x: contrastSlider.leftPadding
                       + contrastSlider.visualPosition
                         * (contrastSlider.availableWidth - width)

                    y: contrastSlider.topPadding
                       + contrastSlider.availableHeight / 2
                       - height / 2

                    width: 12
                    height: 12

                    radius: width / 2

                    color: contrastSlider.pressed
                           ? "#60A5FA"
                           : "#3B82F6"

                    border.color: "#93C5FD"
                    border.width: 1
                }
            }

            TextField {

                Layout.preferredWidth: 56
                Layout.preferredHeight: 30

                text: root.contrastValue.toFixed(1.0)

                horizontalAlignment: Text.AlignHCenter

                color: "#DCE6F0"

                font.pixelSize: 11

                background: Rectangle {

                    radius: 3

                    color: "#162333"

                    border.color: "#34475B"
                    border.width: 1
                }
                onEditingFinished: {
                    var newValue = Number(text)

                    if (isNaN(newValue))
                        newValue = 1.0

                    if (newValue < 0.0)
                        newValue = 0.1

                    if (newValue > 2.0)
                        newValue = 2.0

                    root.contrastValue = newValue
                    root.contrastChangedByUser(newValue)
                }
            }
        }

        // =====================================================
        // SEPARATOR
        // =====================================================

        Rectangle {

            Layout.fillWidth: true
            Layout.preferredHeight: 1

            color: "#344050"
        }

        // =====================================================
        // AUTO / RESET
        // =====================================================

        RowLayout {

            Layout.fillWidth: true

            spacing: 6

            Button {
                id: autoButton

                text: "Auto"

                Layout.fillWidth: true
                Layout.preferredHeight: 30

                background: Rectangle {
                    radius: 3

                    color: autoButton.pressed
                           ? "#356896"
                           : autoButton.hovered
                             ? "#263B52"
                             : "#1A2838"

                    border.color: autoButton.pressed
                                  ? "#60A5FA"
                                  : "#34475B"

                    border.width: 1
                }

                contentItem: Text {
                    text: autoButton.text

                    color: autoButton.pressed
                           ? "#FFFFFF"
                           : "#C9D4DF"

                    font.pixelSize: 11

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    console.log("AUTO BUTTON CLICKED")
                    root.autoRequested()
                }
            }


            Button {
                id: resetButton

                text: "Reset"

                Layout.fillWidth: true
                Layout.preferredHeight: 30

                background: Rectangle {
                    radius: 3

                    color: resetButton.pressed
                           ? "#356896"
                           : resetButton.hovered
                             ? "#263B52"
                             : "#1A2838"

                    border.color: resetButton.pressed
                                  ? "#60A5FA"
                                  : "#34475B"

                    border.width: 1
                }

                contentItem: Text {
                    text: resetButton.text

                    color: resetButton.pressed
                           ? "#FFFFFF"
                           : "#C9D4DF"

                    font.pixelSize: 11

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    console.log("RESET BUTTON CLICKED")
                    root.resetRequested()
                }
            }

        }

        // =====================================================
        // SET / APPLY
        // =====================================================

        RowLayout {

            Layout.fillWidth: true

            spacing: 6


            Button {
                id: setoButton

                text: "Set"

                Layout.fillWidth: true
                Layout.preferredHeight: 30

                background: Rectangle {
                    radius: 3

                    color: setoButton.pressed
                           ? "#3B82F6"
                           : setoButton.hovered
                             ? "#263B52"
                             : "#1A2838"

                    border.color: setoButton.pressed
                                  ? "#60A5FA"
                                  : "#34475B"

                    border.width: 1
                }

                contentItem: Text {
                    text: setoButton.text

                    color: setoButton.pressed
                           ? "#FFFFFF"
                           : "#C9D4DF"

                    font.pixelSize: 11

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {

                    root.setRequested()
                }
            }

            Button {
                id: applyButton

                text: "Apply"

                Layout.fillWidth: true
                Layout.preferredHeight: 30

                background: Rectangle {
                    radius: 3

                    color: applyButton.pressed
                           ? "#3B82F6"
                           : applyButton.hovered
                             ? "#263B52"
                             : "#1A2838"

                    border.color: applyButton.pressed
                                  ? "#60A5FA"
                                  : "#34475B"

                    border.width: 1
                }

                contentItem: Text {
                    text: applyButton.text

                    color: applyButton.pressed
                           ? "#FFFFFF"
                           : "#C9D4DF"

                    font.pixelSize: 11

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    console.log("APPLY BUTTON CLICKED")
                    root.applyRequested()
                }
            }

        }

        // =====================================================
        // HISTOGRAM
        // =====================================================

        Text {

            text: "HISTOGRAM"

            color: "#D6DEE8"

            font.pixelSize: 11
            font.bold: true
        }

        Rectangle {
            id: histogramContainer

            Layout.fillWidth: true
            Layout.preferredHeight: 105

            radius: 4

            color: "#0F172A"

            border.color: "#34475B"
            border.width: 1


            Canvas {
                id: histogramCanvas

                anchors.fill: parent
                anchors.margins: 4

                onPaint: {

                    var ctx = getContext("2d")

                    ctx.clearRect(
                                0,
                                0,
                                width,
                                height)


                    if (!root.dicomLoader)
                        return


                    var data =
                            root.dicomLoader.histogramData


                    if (!data ||
                        data.length === 0)
                    {
                        return
                    }


                    // =========================================
                    // GRAPH AREA
                    // =========================================

                    var graphHeight =
                            height - 12


                    // =========================================
                    // FIND MAX HISTOGRAM VALUE
                    // =========================================

                    var maxCount = 0

                    for (var i = 0;
                         i < data.length;
                         ++i)
                    {
                        var count =
                                Number(data[i])

                        if (count > maxCount)
                            maxCount = count
                    }


                    if (maxCount <= 0)
                        return


                    // =========================================
                    // DRAW HISTOGRAM
                    // =========================================

                    var binWidth =
                            width / data.length


                    ctx.beginPath()

                    ctx.moveTo(
                                0,
                                graphHeight)


                    for (var i = 0;
                         i < data.length;
                         ++i)
                    {
                        var value =
                                Number(data[i])


                        var barHeight =
                                (value / maxCount)
                                * graphHeight


                        var posX =
                                i * binWidth


                        var posY =
                                graphHeight
                                - barHeight


                        ctx.lineTo(
                                    posX,
                                    posY)
                    }


                    ctx.lineTo(
                                width,
                                graphHeight)

                    ctx.closePath()


                    ctx.fillStyle = "#718096"

                    ctx.fill()


                    // =========================================
                    // IMAGE RANGE
                    // =========================================

                    var dataMin =
                            root.dicomLoader.histogramMin

                    var dataMax =
                            root.dicomLoader.histogramMax


                    var dataRange =
                            dataMax - dataMin


                    if (dataRange <= 0)
                        dataRange = 1


                    // =========================================
                    // PROCESSING VALUES
                    // =========================================

                    var minimum =
                            root.dicomLoader.minimum

                    var maximum =
                            root.dicomLoader.maximum

                    var brightness =
                            root.dicomLoader.brightness

                    var contrast =
                            root.dicomLoader.contrast


                    // =========================================
                    // MINIMUM / MAXIMUM → X POSITION
                    // =========================================

                    var startX =
                            ((minimum - dataMin)
                             / dataRange)
                            * width


                    var endX =
                            ((maximum - dataMin)
                             / dataRange)
                            * width


                    // Clamp X only

                    startX =
                            Math.max(
                                0,
                                Math.min(
                                    width,
                                    startX))


                    endX =
                            Math.max(
                                0,
                                Math.min(
                                    width,
                                    endX))


                    if (endX <= startX)
                        endX = startX + 1


                    // =========================================
                    // BRIGHTNESS
                    //
                    // Assumed slider range:
                    // -100 → +100
                    // =========================================

                    var brightnessNormalized =
                            brightness / 100.0


                    // =========================================
                    // CONTRAST
                    //
                    // Range:
                    // 0 → 2
                    //
                    // 0 = flat
                    // 1 = normal
                    // 2 = very steep
                    // =========================================

                    contrast =
                            Math.max(
                                0.0,
                                Math.min(
                                    2.0,
                                    contrast))


                    // =========================================
                    // CLIP ONLY TRANSFER LINE
                    // =========================================

                    ctx.save()

                    ctx.beginPath()

                    ctx.rect(
                                0,
                                0,
                                width,
                                graphHeight)

                    ctx.clip()


                    // =========================================
                    // DRAW TRANSFER FUNCTION
                    // =========================================

                    // =========================================
                    // DRAW TRANSFER FUNCTION
                    // =========================================

                    ctx.beginPath()

                    var contrastValue = contrast

                    // -----------------------------------------
                    // CONTRAST = 3
                    // Draw 90 degree vertical line
                    // -----------------------------------------

                    if (contrastValue >= 1.99)
                    {
                        var centerX =
                                (startX + endX) / 2


                        var brightnessOffset =
                                brightnessNormalized
                                * graphHeight


                        var centerY =
                                (graphHeight / 2)
                                - brightnessOffset


                        ctx.moveTo(
                                    centerX,
                                    graphHeight)

                        ctx.lineTo(
                                    centerX,
                                    0)
                    }
                    else
                    {
                        // -----------------------------------------
                        // NORMAL CONTRAST
                        // -----------------------------------------

                        var firstPoint = true


                        for (var x = startX;
                             x <= endX;
                             x += 1)
                        {
                            var input =
                                    (x - startX)
                                    / (endX - startX)


                            // Map contrast 0 → 3
                            //
                            // 0 = flat
                            // 1.0 = normal
                            // 2 = almost vertical

                            var effectiveContrast =
                                    contrastValue


                            var output =
                                    ((input - 0.5)
                                     * effectiveContrast)
                                    + 0.5


                            // Brightness

                            output =
                                    output
                                    + brightnessNormalized


                            // DO NOT CLAMP

                            var y =
                                    graphHeight
                                    - (output * graphHeight)


                            if (firstPoint)
                            {
                                ctx.moveTo(
                                            x,
                                            y)

                                firstPoint = false
                            }
                            else
                            {
                                ctx.lineTo(
                                            x,
                                            y)
                            }
                        }
                    }


                    // =========================================
                    // DRAW LINE
                    // =========================================

                    ctx.strokeStyle = "#FFFFFF"

                    ctx.lineWidth = 1

                    ctx.stroke()

                    // =========================================
                    // REMOVE CLIPPING
                    // =========================================

                    ctx.restore()


                    // =========================================
                    // BOTTOM GRAPH LINE
                    // =========================================

                    ctx.beginPath()

                    ctx.moveTo(
                                0,
                                graphHeight)

                    ctx.lineTo(
                                width,
                                graphHeight)

                    ctx.strokeStyle = "#34475B"

                    ctx.lineWidth = 1

                    ctx.stroke()
                }


                // =========================================
                // UPDATE HISTOGRAM
                // =========================================

                Connections {

                    target: root.dicomLoader


                    function onHistogramChanged()
                    {
                        histogramCanvas.requestPaint()
                    }


                    function onImageProcessingChanged()
                    {
                        histogramCanvas.requestPaint()
                    }
                }


                Component.onCompleted:
                {
                    requestPaint()
                }
            }
            // =============================================
            // MIN VALUE LABEL
            // =============================================

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 5

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2

                text: root.dicomLoader
                      ? root.dicomLoader.histogramMin
                      : "0"

                color: "#9CA3AF"

                font.pixelSize: 9
            }


            // =============================================
            // MAX VALUE LABEL
            // =============================================

            Text {
                anchors.right: parent.right
                anchors.rightMargin: 5

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2

                text: root.dicomLoader
                      ? root.dicomLoader.histogramMax
                      : "65535"

                color: "#9CA3AF"

                font.pixelSize: 9
            }
        }
        // =====================================================
        // PRESETS
        // =====================================================

        Text {

            text: "PRESETS"

            color: "#D6DEE8"

            font.pixelSize: 11
            font.bold: true
        }

        GridLayout {

            Layout.fillWidth: true

            columns: 4

            columnSpacing: 6
            rowSpacing: 6

            Repeater {

                model: [
                    "Brain",
                    "Lung",
                    "Bone",
                    "Abdomen",
                    "Soft Tissue",
                    "Spine",
                    "Default"
                ]

                delegate: Button {

                    text: modelData

                    Layout.fillWidth: true
                    Layout.preferredHeight: 28

                    background: Rectangle {

                        radius: 2

                        color: "#1A2838"

                        border.color: "#2E4054"
                        border.width: 1
                    }

                    contentItem: Text {

                        text: parent.text

                        color: "#C9D4DF"

                        font.pixelSize: 10

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        elide: Text.ElideRight
                    }
                }
            }
        }

        // =====================================================
        // MEASUREMENTS
        // =====================================================

        Rectangle {

            Layout.fillWidth: true
            Layout.preferredHeight: 1

            color: "#344050"
        }

        Text {

            text: "MEASUREMENTS"

            color: "#D6DEE8"

            font.pixelSize: 11
            font.bold: true
        }

        // =====================================================
        // MEASUREMENT TOOLS
        // =====================================================

        RowLayout {

            id: measurementRow

            Layout.fillWidth: true
            Layout.preferredHeight: 55

            spacing: 2

            Repeater {

                model: [
                    {
                        symbol: "/",
                        title: "Length"
                    },
                    {
                        symbol: "∠",
                        title: "Angle"
                    },
                    {
                        symbol: "○",
                        title: "Area"
                    },
                    {
                        symbol: "⊙",
                        title: "Ellipse"
                    },
                    {
                        symbol: "↘",
                        title: "Arrow"
                    }
                ]

                delegate: Rectangle {

                    Layout.fillWidth: true
                    Layout.preferredHeight: 55

                    color: measureMouse.containsMouse
                           ? "#1D2E40"
                           : "transparent"

                    radius: 3

                    Column {

                        anchors.centerIn: parent

                        spacing: 4

                        Text {

                            width: parent.parent.width

                            text: modelData.symbol

                            horizontalAlignment: Text.AlignHCenter

                            color: "#AFC1D3"

                            font.pixelSize: 15
                        }

                        Text {

                            width: parent.parent.width

                            text: modelData.title

                            horizontalAlignment: Text.AlignHCenter

                            color: "#C8D3DE"

                            font.pixelSize: 9

                            elide: Text.ElideRight
                        }
                    }

                    MouseArea {

                        id: measureMouse

                        anchors.fill: parent

                        hoverEnabled: true

                        cursorShape: Qt.PointingHandCursor

                        onClicked: {

                            console.log(
                                "Measurement selected:",
                                modelData.title
                            )
                        }
                    }
                }
            }
        }

        // =====================================================
        // MEASUREMENT TABLE HEADER
        // =====================================================

        Rectangle {

            Layout.fillWidth: true
            Layout.preferredHeight: 28

            color: "#1D2B3A"

            border.color: "#304256"
            border.width: 1

            RowLayout {

                anchors.fill: parent

                anchors.leftMargin: 12
                anchors.rightMargin: 12

                Text {

                    text: "#"

                    Layout.preferredWidth: 35

                    color: "#AEBECE"

                    font.pixelSize: 10

                    horizontalAlignment: Text.AlignHCenter
                }

                Text {

                    text: "Type"

                    Layout.fillWidth: true

                    color: "#AEBECE"

                    font.pixelSize: 10

                    horizontalAlignment: Text.AlignHCenter
                }

                Text {

                    text: "Value"

                    Layout.preferredWidth: 70

                    color: "#AEBECE"

                    font.pixelSize: 10

                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }*/

        // =====================================================
        // SPACER
        // =====================================================

        Item {
            Layout.fillHeight: true
        }
    }
}
