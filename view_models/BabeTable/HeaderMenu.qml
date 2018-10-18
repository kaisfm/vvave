import QtQuick 2.0
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.2 as Kirigami
import org.kde.mauikit 1.0 as Maui

import "../../utils"
import ".."

Maui.Menu
{
    signal saveListClicked()
    signal queueListClicked()
    signal sortClicked()

    property alias menuItem : customItems.children

    Maui.MenuItem
    {
        text: qsTr("Queue list")
        onTriggered:
        {
            queueListClicked()
            close()
        }
    }

    Maui.MenuItem
    {
        text: qsTr("Save list to...")
        onTriggered:
        {
            saveListClicked()
            close()
        }
    }

    Maui.MenuItem
    {
        text: qsTr("Send list to...")
    }

    MenuSeparator {}

    Maui.MenuItem
    {
        text: qsTr("Visible info...")
        onTriggered: {close()}
    }

    MenuSeparator {}

    Maui.MenuItem
    {
        text: qsTr("Sort...")
        onTriggered:
        {
            sortClicked()
            close()
        }
    }

    Maui.MenuItem
    {
        text: qsTr("Selection ")+ (selectionMode ? qsTr("OFF") : qsTr("ON"))
        onTriggered: selectionMode = !selectionMode
    }

    Column
    {
        id: customItems
    }
}
