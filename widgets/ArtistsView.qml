import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../view_models"
import "../utils/Icons.js" as MdiFont
import "../utils"

BabeGrid
{
    id:artistsViewGrid
    visible: true
    albumSize: 150
    borderRadius: 20

    signal rowClicked(var track)
    signal playAlbum(var tracks)
    signal playTrack(var track)
    signal queueTrack(var track)
    signal appendAlbum(var tracks)

    Drawer
    {
        id: drawer
        height: parent.height * 0.4
        width: parent.width
        edge: Qt.BottomEdge
        interactive: false

        Column
        {
            anchors.fill: parent

            Rectangle
            {
                id: titleBar
                width: parent.width
                height: 48
                z: 1
                color: bae.midColor()

                Row
                {
                    anchors.fill: parent

                    ToolButton
                    {
                        width: parent.height
                        height: parent.height

                        id: playAllBtn
                        Icon {text: MdiFont.Icon.playBoxOutline}

                        onClicked:
                        {
                            drawer.close()
                            var data = artistsViewGrid.gridModel.get(artistsViewGrid.grid.currentIndex)

                            var query = "select * from tracks where artist = \""+data.artist+"\""
                            var tracks = bae.get(query)
                            playAlbum(tracks)

                        }
                    }
                    ToolButton
                    {
                        id: appendBtn

                        width: parent.height
                        height: parent.height

                        Icon {text: MdiFont.Icon.playlistPlus}

                        onClicked:
                        {
                            var data = artistsViewGrid.gridModel.get(artistsViewGrid.grid.currentIndex)
                            var query = "select * from tracks where artist = \""+data.artist+"\""
                            var tracks = bae.get(query)
                            appendAlbum(tracks)
                            drawer.close()

                        }
                    }
                    Label
                    {
                        id: artistTitle
                        width: parent.width - closeBtn.width - playAllBtn.width - appendBtn.width
                        height: parent.height
                        elide: Text.ElideRight
                        font.pointSize: 12
                        font.bold: true
                        lineHeight: 0.7
                        color: bae.foregroundColor()
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                    }


                    ToolButton
                    {
                        id: closeBtn
                        width: parent.height
                        height: parent.height

                        Icon
                        {
                            text: MdiFont.Icon.close
                        }

                        onClicked:
                        {
                            drawer.close()
                            console.log("close drawer")
                        }
                    }
                }
            }

            BabeTable
            {
                id: drawerList
                width: parent.width
                height: parent.height - titleBar.height
                trackNumberVisible: true
                quickBtnsVisible: true

                onRowClicked:
                {
                    drawer.close()
                    artistsViewGrid.rowClicked(model.get(index))
                }

                onQuickPlayTrack:
                {
                    drawer.close()
                    artistsViewGrid.playTrack(model.get(index))
                }

                onQueueTrack:
                {
                    drawer.close()
                    artistsViewGrid.queueTrack(model.get(index))
                }
            }
        }
    }

    onAlbumCoverClicked:
    {
        artistTitle.text = artist
        drawer.open()
        drawerList.clearTable()

        var query = "select * from tracks where artist = \""+artist+"\" order by album, track"
        var map = bae.get(query)

        for(var i in map)
            drawerList.model.append(map[i])

    }

    function populate()
    {
        var map = bae.get("select * from artists order by artist asc")
        for(var i in map)
            gridModel.append(map[i])
    }

    Component.onCompleted: populate()

}
