import QtQuick 2.0
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.2 as Kirigami

import "../../view_models/BabeMenu"
import "../../utils"
import ".."

BabeMenu
{
    signal saveListClicked()
    signal queueListClicked()
    signal sortClicked()

    property alias menuItem : customItems.children

    MenuItem
    {
        text: qsTr("Queue list")
        onTriggered:
        {
            queueListClicked()
            close()
        }
    }

    MenuItem
    {
        text: qsTr("Save list to...")
        onTriggered:
        {
            saveListClicked()
            close()
        }
    }

    MenuItem
    {
        text: qsTr("Send list to...")
    }

    MenuSeparator {}

    MenuItem
    {
        text: qsTr("Visible info...")
        onTriggered: {close()}
    }

    MenuSeparator {}

    MenuItem
    {
        text: qsTr("Sort...")
        onTriggered:
        {
            sortClicked()
            close()
        }
    }

    MenuItem
    {
        text: qsTr("Selection ")+ (selectionMode ? qsTr("OFF") : qsTr("ON"))
        onTriggered: selectionMode = !selectionMode
    }

    Column
    {
        id: customItems
    }

}
