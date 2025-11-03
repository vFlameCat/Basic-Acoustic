#pragma once


#include <mutex>

#include "PlayCursor.hpp"


class SoundPlayer;


class SyncStaticPlayers {

public:

    friend class SoundPlayer;

    using PlayerHandle = int;

    SyncStaticPlayers ();

    // currently not thread safe at runtime
    PlayerHandle addPlayer (PlayCursor player);

    // in each frame it is wrong to call getPlayer before calling syncReadBuf
    PlayCursor& getPlayer (PlayerHandle handle) &;
    PlayCursor getPlayer (PlayerHandle handle) const &;

    // currently not thread safe at runtime
    void removePlayer (PlayerHandle handle);

    void syncModifyBuf ();
    void syncReadBuf ();

    std::unordered_map <PlayerHandle, PlayCursor> modifyBuf_{};

private:

    PlayerHandle nextHandle_ = 1;

    // contains PlayCursors that changes in sound thread

    std::unordered_map <PlayerHandle, PlayCursor> swapBuf_{};

    // dump storage for main thread to get info from
    std::unordered_map <PlayerHandle, PlayCursor> readBuf_{};

    bool isSwapReady = false;

    std::mutex playersSync_{};
};



class SyncOneShotPlayers {

public:

    friend class SoundPlayer;

    SyncOneShotPlayers ();

    void addPlayer (PlayCursor player);

    void syncWriteBuf ();
    void syncReadBuf ();

private:

    // buffer for main thread to fill
    std::vector <PlayCursor> writeBuf_{};

    std::vector <PlayCursor> swapBuf_{};

    // buffer for sound thread to read from
    std::vector <PlayCursor> readBuf_{};

    bool isSwapReady = false;

    std::mutex playersSync_{};
};