import QtQuick 2.9
import QtQuick.Controls 2.2
import org.kde.kirigami 2.2 as Kirigami
import org.kde.mauikit 1.0 as Maui
import QtQuick.Layouts 1.3

import "../../view_models/BabeMenu"
import "../../view_models"
import "../../utils/Player.js" as Player
import "../../db/Queries.js" as Q

BabeMenu
{
    signal sortBy(string text)
    MenuItem
    {
        text: qsTr("Artist")
        onTriggered: sortBy("artist")
    }
    MenuItem
    {
        text: qsTr("Album")
        onTriggered: sortBy("album")
    }
    MenuItem
    {
        text: qsTr("Genre")
        onTriggered: sortBy("genre")
    }
    MenuItem
    {
        text: qsTr("Stars")
        onTriggered: sortBy("stars")
    }
}
