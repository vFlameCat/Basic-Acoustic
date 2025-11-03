#include "SyncPlayers.hpp"


#include <iostream>

SyncStaticPlayers::SyncStaticPlayers () {};


SyncStaticPlayers::PlayerHandle SyncStaticPlayers::addPlayer (PlayCursor player) {

    modifyBuf_.emplace(nextHandle_, player);

    return nextHandle_++;
}

PlayCursor& SyncStaticPlayers::getPlayer (PlayerHandle handle) & {

    return readBuf_.at(handle);
}

PlayCursor SyncStaticPlayers::getPlayer (PlayerHandle handle) const & {

    return readBuf_.at(handle);
}

void SyncStaticPlayers::removePlayer (PlayerHandle handle) {

    modifyBuf_.erase(handle);
}


void SyncStaticPlayers::syncModifyBuf () {

    std::lock_guard<std::mutex> sync(playersSync_);

    swapBuf_ = modifyBuf_;
    isSwapReady = true;
}

void SyncStaticPlayers::syncReadBuf () {

    std::lock_guard<std::mutex> sync(playersSync_);

    if (isSwapReady) {

        readBuf_.swap(swapBuf_);
        isSwapReady = false;
    }
}



SyncOneShotPlayers::SyncOneShotPlayers () {}

void SyncOneShotPlayers::addPlayer (PlayCursor player) {

    writeBuf_.push_back(player);
}

void SyncOneShotPlayers::syncWriteBuf () {

    std::lock_guard<std::mutex> sync(playersSync_);

    writeBuf_.swap(swapBuf_);
    isSwapReady = true;
    writeBuf_.clear();
}

void SyncOneShotPlayers::syncReadBuf () {

    std::lock_guard<std::mutex> sync(playersSync_);

    if (isSwapReady) {

        readBuf_.swap(swapBuf_);
        isSwapReady = false;
    }
}