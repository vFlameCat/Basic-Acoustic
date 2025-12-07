#include "SyncPlayers.hpp"
#include <iostream>
#include <SoundPlayer.hpp>


SyncStaticPlayCursors::PlayCursorHandle SyncStaticPlayCursors::addPlayCursor (PlayCursor playCursor) {

    std::lock_guard<std::mutex> sync(playCursorsSync_);

    playCursors_.emplace(nextHandle_, playCursor);

    return nextHandle_++;
}

PlayCursor& SyncStaticPlayCursors::getPlayCursor (PlayCursorHandle handle) & {

    std::lock_guard<std::mutex> sync(playCursorsSync_);

    return playCursors_.at(handle);
}

void SyncStaticPlayCursors::removePlayCursor (PlayCursorHandle handle) {

    std::lock_guard<std::mutex> sync(playCursorsSync_);

    playCursors_.erase(handle);
}




void SyncDynamicPlayCursors::addPlayCursor (PlayCursor playCursor) {

    writeBuf_.push_back(playCursor);
}

void SyncDynamicPlayCursors::addPlayCursor (DynamicPlayerCreateInfo info) {

    PlayCursor::CreateInfo playerInfo = SoundPlayer::getInstance().getStaticPlayCursor(info.playerHandle).getInfo();
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