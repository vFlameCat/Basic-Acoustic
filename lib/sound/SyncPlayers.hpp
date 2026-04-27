#pragma once


#include <PlayCursor.hpp>

#include <cstdint>
#include <vector>

#include <variant>
#include <concurrency/SPSCQueue.hpp>

#include <concurrency/TripleBuffer.hpp>


class SyncStaticPlayCursors {

public:

    enum class Handle: uint32_t {

        Invalid = uint32_t(-1)
    };

private:

    struct Command {

        struct Add {

            PlayCursor playCursor;
        };

        struct Remove {

            Handle handle;
        };

        std::variant<std::monostate, Add, Remove> data;
    };

public:

    class MainView {

    public:

        explicit MainView (SyncStaticPlayCursors &playCursors);

        Handle addPlayCursor (PlayCursor playCursor);
        void removePlayCursor (Handle handle);

    private:

        SyncStaticPlayCursors &playCursors_;
    };

    class RenderView {

    public:

        explicit RenderView (SyncStaticPlayCursors &playCursors);

        PlayCursor::CreateInfo getPlayCursorInfo (Handle handle) const;

        std::vector<PlayCursor>& getPlayCursors () const;

    private:

        template<class... Ts>
        struct overloads : Ts... { using Ts::operator()...; };

        void parseCommandAdd (Command::Add &cmd);
        void parseCommandRemove (Command::Remove &cmd);

    private:

        SyncStaticPlayCursors &playCursors_;
    };

public:

    SyncStaticPlayCursors () = default;

    MainView getMainView ();
    RenderView getRenderView ();

private:

    Handle nextFreeHandle_ = static_cast<Handle>(0);

    std::vector<PlayCursor> playCursors_;

    SPSCQueue<Command, 1024> cmdQueue_;
};



class SyncDynamicPlayCursors {

public:

    struct DynamicPlayerCreateInfo {

        SyncStaticPlayCursors::Handle playerHandle = SyncStaticPlayCursors::Handle::Invalid;
        double posOffset = 0.;
        float volume = 0.f;
    };

    class FrameWriter {
    
    public:

        explicit FrameWriter (SyncDynamicPlayCursors &playCursors);

        ~FrameWriter ();

        void addPlayCursor (DynamicPlayerCreateInfo info) const;
    
    private:

        SyncDynamicPlayCursors &playCursors_;
        std::vector<DynamicPlayerCreateInfo> &writeBuf_;
    };

    class FrameRenderer {

    public:

        explicit FrameRenderer (SyncDynamicPlayCursors &playCursors);

        void buildPlayCursors (const SyncStaticPlayCursors::RenderView &renderView);

        std::vector<PlayCursor>& getPlayCursors () const;

    private:

        SyncDynamicPlayCursors &playCursors_;
        bool isNewCursors_;
    };

public:

    SyncDynamicPlayCursors () = default;

    FrameWriter getFrameWriter ();
    FrameRenderer getFrameRenderer ();

private:

    std::vector<PlayCursor> playCursors_;

    TripleBuffer<std::vector<DynamicPlayerCreateInfo>> buf_;
};