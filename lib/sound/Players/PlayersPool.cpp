#include "Player.hpp"
#include "PlayersPool.hpp"

#include <cassert>


PlayersPool::Writer::Writer (PlayersPool &pool):
  pool_(pool) {}

PlayersPool::Handle PlayersPool::Writer::addPlayer (Player player) {

    Handle handle = pool_.mainHandles_.insert(std::monostate{});

    bool pushed = pool_.cmdQueue_.push(Command{ Command::Add{ player }});
    assert(pushed && "PlayersPool command queue overflow on add");
    (void) pushed;

    return handle;
}

void PlayersPool::Writer::removePlayer (Handle handle) {

    if (!pool_.mainHandles_.erase(handle)) return;

    bool pushed = pool_.cmdQueue_.push(Command{ Command::Remove{ handle }});
    assert(pushed && "PlayersPool command queue overflow on remove");
    (void) pushed;
}



PlayersPool::Reader::Reader (PlayersPool &pool):
  pool_(pool) {

    Command cmd;
    while (pool_.cmdQueue_.pop(cmd)) {
        std::visit(overloads {

            [](std::monostate) {},
            [this](Command::Add    &cmd) { parseCommandAdd(cmd); },
            [this](Command::Remove &cmd) { parseCommandRemove(cmd); },
        }, cmd.data);
    }
}

Player::CreateInfo PlayersPool::Reader::getPlayerCreateInfo (Handle handle) const {

    return pool_.players_.get(handle).getInfo();
}

fc::SlotPool<Player>& PlayersPool::Reader::getPlayers () const {

    return pool_.players_;
}

void PlayersPool::Reader::parseCommandAdd (Command::Add &cmd) {

    pool_.players_.insert(cmd.player);
}

void PlayersPool::Reader::parseCommandRemove (Command::Remove &cmd) {

    pool_.players_.erase(cmd.handle);
}



PlayersPool::PlayersPool (std::size_t capacity) {

    reserve(capacity);
}

void PlayersPool::reserve (std::size_t capacity) {

    mainHandles_.reserve(capacity);
    players_.reserve(capacity);
}


PlayersPool::Writer PlayersPool::getWriter () {

    return Writer(*this);
}

PlayersPool::Reader PlayersPool::getReader () {

    return Reader(*this);
}
