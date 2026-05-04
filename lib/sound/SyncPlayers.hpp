#pragma once


#include <PlayCursor.hpp>

#include <containers/SlotPool.hpp>
#include <concurrency/SPSCQueue.hpp>
#include <concurrency/TripleBuffer.hpp>

#include <variant>
#include <vector>


class SyncStaticPlayCursors {

public:

    using Handle = fc::SlotPool<PlayCursor>::Handle;

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
        void   removePlayCursor (Handle handle);

    private:

        SyncStaticPlayCursors &playCursors_;
    };

    class RenderView {

    public:

        explicit RenderView (SyncStaticPlayCursors &playCursors);

        PlayCursor::CreateInfo getPlayCursorInfo (Handle handle) const;

        fc::SlotPool<PlayCursor>& getPlayCursors () const;

    private:

        template<class... Ts>
        struct overloads : Ts... { using Ts::operator()...; };

        void parseCommandAdd    (Command::Add &cmd);
        void parseCommandRemove (Command::Remove &cmd);

    private:

        SyncStaticPlayCursors &playCursors_;
    };

public:

    SyncStaticPlayCursors () = default;

    MainView   getMainView ();
    RenderView getRenderView ();

private:

    // Main thread allocates handles via mainHandles_; the audio thread mirrors
    // the same insert/erase sequence on playCursors_, so handles stay consistent.
    // Both pools share Tag = PlayCursor so their Handle types are identical.
    fc::SlotPool<std::monostate, PlayCursor> mainHandles_;
    fc::SlotPool<PlayCursor>                 playCursors_;

    SPSCQueue<Command, 1024> cmdQueue_;
};



class SyncDynamicPlayCursors {

public:

    struct DynamicPlayerCreateInfo {

        SyncStaticPlayCursors::Handle playerHandle = SyncStaticPlayCursors::Handle::Invalid;
        double posOffset = 0.;
        float  volume    = 0.f;
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

    FrameWriter   getFrameWriter ();
    FrameRenderer getFrameRenderer ();

private:

    std::vector<PlayCursor> playCursors_;

    TripleBuffer<std::vector<DynamicPlayerCreateInfo>> buf_;
};
