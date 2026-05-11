#include "Player.hpp"
#include "SpatialFramePlayers.hpp"

#include <vector>


SpatialFramePlayers::Writer::Writer (SpatialFramePlayers &players):
  players_(players), writeBuf_(players.buf_.getWriteBuffer()) {

    writeBuf_.clear();
}

SpatialFramePlayers::Writer::~Writer () {

    players_.buf_.publish();
}

void SpatialFramePlayers::Writer::addPlayer (PlayerCreateInfo info) const {

    writeBuf_.push_back(info);
}



SpatialFramePlayers::Reader::Reader (SpatialFramePlayers &players):
  players_(players),
  isNewPlayers_(players.buf_.fetch()) {}

void SpatialFramePlayers::Reader::buildPlayers (const PlayersPool::Reader &poolReader) {

    if (!isNewPlayers_)
        return;

    fc::SlotPool<Player> &pool = poolReader.getPlayers();

    players_.players_.clear();
    std::vector<PlayerCreateInfo> &playersInfo = players_.buf_.getReadBuffer();
    for (const PlayerCreateInfo &info : playersInfo) {

        Player::CreateInfo createInfo = pool.get(info.playerHandle).getInfo();
        createInfo.posOffset = info.posOffset;      // may be need to adjust these parameters
        createInfo.volume    = info.volume;         // no to set them

        players_.players_.emplace_back(createInfo);
    }
}

std::vector<Player>& SpatialFramePlayers::Reader::getPlayers () const {

    return players_.players_;
}



SpatialFramePlayers::SpatialFramePlayers (std::size_t capacity) {

    reserve(capacity);
}

void SpatialFramePlayers::reserve (std::size_t capacity) {

    players_.reserve(capacity);
    buf_.apply([capacity](std::vector<PlayerCreateInfo> &v) { v.reserve(capacity); });
}


SpatialFramePlayers::Writer SpatialFramePlayers::getWriter () {

    return Writer(*this);
}

SpatialFramePlayers::Reader SpatialFramePlayers::getReader () {

    return Reader(*this);
}
