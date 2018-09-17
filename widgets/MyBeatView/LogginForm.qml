import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.1

import "../../view_models"

BabePopup
{
    id: loginPopup
    padding: contentMargins*3
    maxHeight: loginLayout.implicitHeight+64
    maxWidth: loginLayout.implicitWidth+64
    modal: false
//    closePolicy: Popup.NoAutoClose

    ScrollView
    {
        anchors.fill: parent
        anchors.centerIn: parent
        clip: true

        contentWidth: parent.width
        contentHeight: loginLayout.implicitHeight+contentMargins

        ColumnLayout
        {
            id: loginLayout
            anchors.fill: parent
            anchors.centerIn: parent


            Item
            {
                Layout.fillHeight: true
            }

//            Item
//            {
//                Layout.fillWidth: true
//                Layout.fillHeight: true

//                Layout.alignment: Qt.AlignCenter
//                Layout.margins: contentMargins*2
//                width: parent.width
//                Image
//                {

//                    anchors.centerIn: parent
//                    id: beatsImg
//                    fillMode: Image.PreserveAspectFit
//                    mipmap: true
//                    source: "qrc:/assets/beat.svg"
//                    horizontalAlignment: Qt.AlignHCenter
//                }

//                ColorOverlay
//                {
//                    anchors.fill: beatsImg
//                    source: beatsImg
//                    color: textColor
//                }
//            }

            TextField
            {
                id: email
                visible : false
                width: parent.width

                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.alignment: Qt.AlignCenter

                placeholderText: qsTr("email *")
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                Material.accent: babeColor
                Material.background: backgroundColor
                Material.primary: backgroundColor
                Material.foreground: textColor
            }

            TextField
            {
                id: nickId
                width: parent.width
                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.alignment: Qt.AlignCenter
                placeholderText: email.visible ? qsTr("nick *") : qsTr("nick or email")
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                Material.accent: babeColor
                Material.background: backgroundColor
                Material.primary: backgroundColor
                Material.foreground: textColor
            }

            TextField
            {
                id: password

                width: parent.width

                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.alignment: Qt.AlignCenter

                placeholderText: qsTr("password *")
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                Material.accent: babeColor
                Material.background: backgroundColor
                Material.primary: backgroundColor
                Material.foreground: textColor
            }

            RowLayout
            {
                id: nameInfo
                visible: false
                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.alignment: Qt.AlignCenter
                TextField
                {
                    id: name
                    Layout.fillWidth: true
                    Layout.rightMargin: contentMargins/2
                    placeholderText: qsTr("name")
                    color: textColor
                    horizontalAlignment: Text.AlignHCenter
                    Material.accent: babeColor
                    Material.background: backgroundColor
                    Material.primary: backgroundColor
                    Material.foreground: textColor
                }

                TextField
                {
                    id: lastname
                    Layout.fillWidth: true
                    Layout.leftMargin: contentMargins/2

                    placeholderText: qsTr("lastname")
                    color: textColor
                    horizontalAlignment: Text.AlignHCenter
                    Material.accent: babeColor
                    Material.background: backgroundColor
                    Material.primary: backgroundColor
                    Material.foreground: textColor
                }

            }


            Button
            {
                width: parent.width
                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.alignment: Qt.AlignCenter
                Layout.topMargin:  contentMargins

                id: loginBtn

                text: qsTr("Login")

                Material.accent: babeColor
                Material.background: backgroundColor
                Material.primary: backgroundColor
                Material.foreground: textColor
            }

            Button
            {
                width: parent.width
                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.alignment: Qt.AlignCenter
                Layout.topMargin:  contentMargins

                id: signUp

                text: qsTr("Singup")

                Material.accent: babeColor
                Material.background: babeColor
                Material.primary: babeColor
                Material.foreground: darkTextColor

                onClicked: fullForm(true)
            }

            Item
            {
                Layout.fillHeight: true

            }

        }
    }

    function fullForm(state)
    {
        if(state)
        {
            email.visible = true
            nameInfo.visible = true
        }else
        {
            email.visible = false
            nameInfo.visible = false
        }
    }

}

