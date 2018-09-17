import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import org.kde.kirigami 2.2 as Kirigami
import org.kde.mauikit 1.0 as Maui
import "../../utils"
import "../../widgets/PlaylistsView"
import "../../view_models"
import "../../db/Queries.js" as Q
import "../../utils/Help.js" as H
import Link.Codes 1.0

BabeList
{
    id: linkingListRoot

    headBarExit: false
    headBarTitle: isLinked ?link.getIp() : qsTr("Disconnected")

    headBar.leftContent: Maui.ToolButton
    {
        anim : true
        iconName : "view-refresh"
        onClicked : refreshPlaylists()
    }

    headBar.rightContent: Maui.ToolButton
    {
        id: menuBtn
        iconName: "application-menu"
        onClicked: linkingConf.open()
    }

    ListModel
    {
        id: linkingListModel

        ListElement { playlist: qsTr("Albums"); icon: "view-media-album-cover"}
        ListElement { playlist: qsTr("Artists"); icon: "view-media-artist"}
        ListElement { playlist: qsTr("Most Played"); icon: "view-media-playcount" /*query: Q.Query.mostPlayedTracks*/ }
        ListElement { playlist: qsTr("Favorites"); icon: "view-media-favorite"}
        ListElement { playlist: qsTr("Recent"); icon: "view-media-recent"}
        ListElement { playlist: qsTr("Babes"); icon: "love"}
        ListElement { playlist: qsTr("Online"); icon: "internet-services"}
        ListElement { playlist: qsTr("Tags"); icon: "tag"}
        ListElement { playlist: qsTr("Relationships"); icon: "view-media-similarartists"}
        ListElement { playlist: qsTr("Popular"); icon: "view-media-chart"}
        ListElement { playlist: qsTr("Genres"); icon: "view-media-genre"}
    }

    model: linkingListModel

    delegate : Maui.ListDelegate
    {
        id: delegate
        width: linkingListRoot.width
        label: model.playlist

        Connections
        {
            target : delegate

            onClicked :
            {
                currentIndex = index
                var playlist = linkingListModel.get(index).playlist

                switch(playlist)
                {
                case qsTr("Artists"):
                    populateExtra(LINK.FILTER, "select artist as tag from artists", playlist)
                    break

                case qsTr("Albums"):
                    populateExtra(LINK.FILTER, "select album as tag, artist from albums", playlist)
                    break

                case qsTr("Most Played"):

                    playlistViewRoot.populate(Q.GET.mostPlayedTracks);
                    break;

                case qsTr("Favorites"):

                    filterList.section.property = "stars"
                    playlistViewRoot.populate(Q.GET.favoriteTracks);
                    break;

                case qsTr("Recent"):

                    playlistViewRoot.populate(Q.GET.recentTracks);
                    break;

                case qsTr("Babes"):

                    playlistViewRoot.populate(Q.GET.babedTracks);
                    break;

                case qsTr("Online"):

                    playlistViewRoot.populate(Q.GET.favoriteTracks);
                    break;

                case qsTr("Tags"):
                    populateExtra(LINK.FILTER, Q.GET.tags, playlist)
                    break;

                case qsTr("Relationships"):

                    playlistViewRoot.populate(Q.GET.favoriteTracks);
                    break;

                case qsTr("Popular"):

                    playlistViewRoot.populate(Q.GET.favoriteTracks);
                    break;

                case qsTr("Genres"):

                    populateExtra(LINK.FILTER, Q.GET.genres, playlist)
                    break;

                default:

                    playlistViewRoot.populate(Q.GET.playlistTracks_.arg(playlist));
                    break;

                }
            }
        }
    }
}
