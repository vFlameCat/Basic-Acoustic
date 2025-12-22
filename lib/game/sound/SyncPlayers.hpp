#pragma once


#include <PlayCursor.hpp>

#include <cstdint>
#include <mutex>
#include <vector>


class AudioRenderer;


class SyncStaticPlayCursors {

public:

    friend class AudioRenderer;


    enum class Handle: uint32_t {

        Invalid = uint32_t(-1)
    };


    SyncStaticPlayCursors () = default;

    Handle addPlayCursor (PlayCursor playCursor);
    PlayCursor& getPlayCursor (Handle handle) &;
    void removePlayCursor (Handle handle);

private:

    std::vector<PlayCursor> playCursors_{};

    std::mutex playCursorsSync_{};
};



class SyncDynamicPlayCursors {

public:

    struct DynamicPlayerCreateInfo {

        SyncStaticPlayCursors::Handle playerHandle = SyncStaticPlayCursors::Handle::Invalid;
        double posOffset = 0.;
        float volume = 0.f;
    };

public:

    friend class AudioRenderer;


    SyncDynamicPlayCursors () = default;

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