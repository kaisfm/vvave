#include "player.h"
#include <QMediaMetaData>
#include "../utils/bae.h"

Player::Player(QObject *parent) : QObject(parent)
{
    this->player = new QMediaPlayer(this);
    connect(player, &QMediaPlayer::durationChanged, this, [&](qint64 dur) {
        emit this->durationChanged(BAE::transformTime(dur/1000));
    });


    this->player->setVolume(100);

    this->updater = new QTimer(this);
    connect(this->updater, &QTimer::timeout, this, &Player::update);
}

void Player::source(const QString &url)
{
    this->sourceurl = url;
    this->player->setMedia(QUrl::fromLocalFile(this->sourceurl));
}

void Player::play()
{
    if(sourceurl.isEmpty()) return;

    if(!updater->isActive())
        this->updater->start(1000);

    this->player->play();
}

void Player::pause()
{
    this->player->pause();
}

void Player::stop()
{
    this->player->stop();
    this->updater->stop();
}

void Player::seek(const int &pos)
{
    this->player->setPosition(pos);
}

int Player::duration()
{
    if(this->sourceurl.isEmpty()) return 0;

    return static_cast<int>(this->player->duration());
}

bool Player::isPaused()
{
    return !(this->player->state() == QMediaPlayer::PlayingState);
}

QString Player::transformTime(const int &pos)
{
    auto time =  BAE::transformTime(pos);
    return time;
}



void Player::update()
{
    emit this->pos(static_cast<int>(static_cast<double>(this->player->position())/this->player->duration()*1000));
    emit this->timing(BAE::transformTime(player->position()/1000));
    if(this->player->state() == QMediaPlayer::StoppedState)
        emit this->finished();
}
