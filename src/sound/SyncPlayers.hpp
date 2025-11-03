#pragma once


#include <cstdint>
#include <mutex>
#include <PlayCursor.hpp>


class AudioRenderer;


class SyncStaticPlayCursors {

public:

    friend class AudioRenderer;


    using PlayCursorHandle = std::uint64_t;
    static constexpr PlayCursorHandle INVALID_PLAYER_HANDLE = 0;


    SyncStaticPlayCursors () {};

    PlayCursorHandle addPlayCursor (PlayCursor playCursor);
    PlayCursor& getPlayCursor (PlayCursorHandle handle) &;
    void removePlayCursor (PlayCursorHandle handle);

private:

    PlayCursorHandle nextHandle_ = 1;

    std::unordered_map <PlayCursorHandle, PlayCursor> playCursors_{};

    std::mutex playCursorsSync_{};
};



class SyncDynamicPlayCursors {

public:

    struct DynamicPlayerCreateInfo {

        SyncStaticPlayCursors::PlayCursorHandle playerHandle = SyncStaticPlayCursors::INVALID_PLAYER_HANDLE;
        double posOffset = 0.;
        float volume = 0.f;
    };

public:

    friend class AudioRenderer;


    SyncDynamicPlayCursors () {};

    void addPlayCursor (PlayCursor playCursor);
    void addPlayCursor (DynamicPlayerCreateInfo info);

    void dispatch ();
    void recieve ();

private:

    // buffer for sound thread to read from
    std::vector <PlayCursor> playCursors_{};

    std::vector <PlayCursor> swapBuf_{};

    // buffer for main thread to fill
    std::vector <PlayCursor> writeBuf_{};

    bool isSwapReady = false;
    std::mutex playCursorsSync_{};
};