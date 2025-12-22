#include "PlayCursor.hpp"
#include <SyncPlayers.hpp>
#include <AudioPlayer.hpp>
#include <vector>


SyncStaticPlayCursors::Handle SyncStaticPlayCursors::addPlayCursor (PlayCursor playCursor) {

    std::lock_guard<std::mutex> sync(playCursorsSync_);

    Handle handle = static_cast<Handle>(playCursors_.size());
    playCursors_.emplace_back(playCursor);

    return handle;
}

PlayCursor& SyncStaticPlayCursors::getPlayCursor (Handle handle) & {

    std::lock_guard<std::mutex> sync(playCursorsSync_);

    return playCursors_[static_cast<std::vector<PlayCursor>::size_type>(handle)];
}

void SyncStaticPlayCursors::removePlayCursor (Handle handle) {

    std::lock_guard<std::mutex> sync(playCursorsSync_);

    std::vector<PlayCursor>::size_type id = static_cast<std::vector<PlayCursor>::size_type>(handle);
    if (id + 1 != playCursors_.size()) {

        std::swap(playCursors_[id], playCursors_.back());
    }
    playCursors_.pop_back();
}




void SyncDynamicPlayCursors::addPlayCursor (PlayCursor playCursor) {

    writeBuf_.push_back(playCursor);
}

void SyncDynamicPlayCursors::addPlayCursor (DynamicPlayerCreateInfo info) {

    PlayCursor::CreateInfo playerInfo = AudioPlayer::getInstance().getStaticPlayCursors().getPlayCursor(info.playerHandle).getInfo();
    playerInfo.posOffset += info.posOffset;
    playerInfo.volume = info.volume;

    writeBuf_.push_back(playerInfo);
}

void SyncDynamicPlayCursors::dispatch () {

    std::lock_guard<std::mutex> sync(playCursorsSync_);

    writeBuf_.swap(swapBuf_);
    isSwapReady = true;
    writeBuf_.clear();
}

void SyncDynamicPlayCursors::recieve () {

    std::lock_guard<std::mutex> sync(playCursorsSync_);

    if (isSwapReady) {

        playCursors_.swap(swapBuf_);
        isSwapReady = false;
    }
}