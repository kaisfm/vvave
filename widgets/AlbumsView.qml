import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../view_models/BabeGrid"
import "../view_models/BabeTable"

import "../db/Queries.js" as Q
import "../utils/Help.js" as H
import org.kde.kirigami 2.2 as Kirigami
import org.kde.mauikit 1.0 as Maui


BabeGrid
{
    id: albumsViewGrid

    property string currentAlbum: ""
    property string currentArtist: ""

    property var tracks: []

    property alias table : albumsViewTable
    property alias tagBar : tagBar

    signal rowClicked(var track)
    signal playTrack(var track)
    signal queueTrack(var track)

    signal appendAll(string album, string artist)
    signal playAll(string album, string artist)
    //    signal albumCoverClicked(string album, string artist)
    signal albumCoverPressedAndHold(string album, string artist)

    visible: true
    //        topPadding: space.large
    onAlbumCoverPressed: albumCoverPressedAndHold(album, artist)
    headBarVisible: true
    headBarExit: false
    headBar.leftContent: Maui.ToolButton
    {
        id : playAllBtn
        visible : headBarVisible && albumsViewGrid.count > 0
        anim : true
        iconName : "media-playlist-play"
        onClicked : playAll()
    }

    headBar.rightContent: [

        Maui.ToolButton
        {
            id: appendBtn
            visible: headBarVisible && albumsViewGrid.count > 0
            anim : true
            iconName : "media-playlist-append"//"media-repeat-track-amarok"
            onClicked: appendAll()
        },

        Maui.ToolButton
        {
            id: menuBtn
            iconName: /*"application-menu"*/ "overflow-menu"
            //                onClicked: isMobile ? headerMenu.open() : headerMenu.popup()
        }
    ]

    Maui.Dialog
    {
        id: albumDialog
        parent: parent
        maxHeight: maxWidth
        maxWidth: unit * 600
        defaultButtons: false
        page.margins: 0

//        verticalAlignment: Qt.AlignBottom

        ColumnLayout
        {
            id: albumFilter
            anchors.fill: parent
            spacing: 0

            BabeTable
            {
                id: albumsViewTable
                Layout.fillHeight: true
                Layout.fillWidth: true
                trackNumberVisible: true
                headBarVisible: true
                headBarExit: false
                coverArtVisible: true
                quickPlayVisible: true
                focus: true

                holder.emoji: "qrc:/assets/ElectricPlug.png"
                holder.isMask: false
                holder.title : qsTr("Oops!")
                holder.body: qsTr("This list is empty")
                holder.emojiSize: iconSizes.huge

                onRowClicked:
                {
                    albumsViewGrid.rowClicked(model.get(index))
                }

                onQuickPlayTrack:
                {
                    albumsViewGrid.playTrack(model.get(index))
                }

                onQueueTrack:
                {
                    albumsViewGrid.queueTrack(model.get(index))
                }

                onPlayAll:
                {
                    albumDialog.close()
                    albumsViewGrid.playAll(currentAlbum, currentArtist)
                }

                onAppendAll:
                {
                    albumDialog.close()
                    albumsViewGrid.appendAll(currentAlbum, currentArtist)
                }


            }

            Maui.TagsBar
            {
                id: tagBar
                Layout.fillWidth: true
                allowEditMode: false
                onTagClicked: H.searchFor("tag:"+tag)
            }
        }


    }

    function populate(query)
    {
        var map = bae.get(query)

        if(map.length > 0)
            for(var i in map)
                gridModel.append(map[i])
    }

    function populateTable(album, artist)
    {
        console.log("PAPULATE ALBUMS VIEW")
        albumDialog.open()
        table.clearTable()

        var query = ""
        var tagq = ""

        currentAlbum = album === undefined ? "" : album
        currentArtist= artist

        if(album && artist)
        {
            query = Q.GET.albumTracks_.arg(album)
            query = query.arg(artist)
            albumsView.table.headBarTitle = album
            tagq = Q.GET.albumTags_.arg(album)

        }else if(artist && album === undefined)
        {
            query = Q.GET.artistTracks_.arg(artist)
            artistsView.table.headBarTitle = artist
            tagq = Q.GET.artistTags_.arg(artist)
        }

        tracks = bae.get(query)

        if(tracks.length > 0)
        {
            for(var i in tracks)
                albumsViewTable.model.append(tracks[i])

            tagq = tagq.arg(artist)
            var tags = bae.get(tagq)
            console.log(tagq, "TAGS", tags)
            tagBar.populate(tags)
        }
    }

    function filter(tracks)
    {
        var matches = []
        for(var i = 0; i<tracks.length; i++)
            matches.push(find(tracks[i].album))

        for(var j = 0 ; j < albumsViewGrid.gridModel.count; j++)
            albumsViewGrid.gridModel.remove(j,1)


        //        for(var match in matches)
        //        {
        //            albumsViewGrid.gridModel.get(match).hide = true
        //            console.log(match)
        //        }
    }

    function find(query)
    {
        var indexes = []
        for(var i = 0 ; i < albumsViewGrid.gridModel.count; i++)
            if(albumsViewGrid.gridModel.get(i).album.includes(query))
                indexes.push(i)

    }
}

