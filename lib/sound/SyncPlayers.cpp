#include "PlayCursor.hpp"
#include <SyncPlayers.hpp>
#include <AudioPlayer.hpp>

#include <cassert>
#include <vector>


SyncStaticPlayCursors::MainView::MainView (SyncStaticPlayCursors &playCursors):
  playCursors_(playCursors) {}

SyncStaticPlayCursors::Handle SyncStaticPlayCursors::MainView::addPlayCursor (PlayCursor playCursor) {

    Handle handle = playCursors_.mainHandles_.insert(std::monostate{});

    bool pushed = playCursors_.cmdQueue_.push(Command{ Command::Add{ playCursor }});
    assert(pushed && "SyncStaticPlayCursors command queue overflow on add");
    (void) pushed;

    return handle;
}

void SyncStaticPlayCursors::MainView::removePlayCursor (Handle handle) {

    if (!playCursors_.mainHandles_.erase(handle)) return;

    bool pushed = playCursors_.cmdQueue_.push(Command{ Command::Remove{ handle }});
    assert(pushed && "SyncStaticPlayCursors command queue overflow on remove");
    (void) pushed;
}



SyncStaticPlayCursors::RenderView::RenderView (SyncStaticPlayCursors &playCursors):
  playCursors_(playCursors) {

    Command cmd;
    while (playCursors_.cmdQueue_.pop(cmd)) {
        std::visit(overloads {

            [](std::monostate) {},
            [this](Command::Add    &cmd) { parseCommandAdd(cmd); },
            [this](Command::Remove &cmd) { parseCommandRemove(cmd); },
        }, cmd.data);
    }
}

PlayCursor::CreateInfo SyncStaticPlayCursors::RenderView::getPlayCursorInfo (Handle handle) const {

    return playCursors_.playCursors_.get(handle).getInfo();
}

fc::SlotPool<PlayCursor>& SyncStaticPlayCursors::RenderView::getPlayCursors () const {

    return playCursors_.playCursors_;
}

void SyncStaticPlayCursors::RenderView::parseCommandAdd (Command::Add &cmd) {

    playCursors_.playCursors_.insert(cmd.playCursor);
}

void SyncStaticPlayCursors::RenderView::parseCommandRemove (Command::Remove &cmd) {

    playCursors_.playCursors_.erase(cmd.handle);
}



SyncStaticPlayCursors::MainView SyncStaticPlayCursors::getMainView () {

    return MainView(*this);
}

SyncStaticPlayCursors::RenderView SyncStaticPlayCursors::getRenderView () {

    return RenderView(*this);
}




SyncDynamicPlayCursors::FrameWriter::FrameWriter (SyncDynamicPlayCursors &playCursors):
  playCursors_(playCursors), writeBuf_(playCursors.buf_.getWriteBuffer()) {

    writeBuf_.clear();
}

SyncDynamicPlayCursors::FrameWriter::~FrameWriter () {

    playCursors_.buf_.publish();
}

void SyncDynamicPlayCursors::FrameWriter::addPlayCursor (DynamicPlayerCreateInfo info) const {

    writeBuf_.push_back(info);
}



SyncDynamicPlayCursors::FrameRenderer::FrameRenderer (SyncDynamicPlayCursors &playCursors):
  playCursors_(playCursors),
  isNewCursors_(playCursors.buf_.fetch()) {}

void SyncDynamicPlayCursors::FrameRenderer::buildPlayCursors (const SyncStaticPlayCursors::RenderView &renderView) {

    if (!isNewCursors_)
        return;

    fc::SlotPool<PlayCursor> &staticPool = renderView.getPlayCursors();

    playCursors_.playCursors_.clear();
    std::vector<DynamicPlayerCreateInfo> &playersInfo = playCursors_.buf_.getReadBuffer();
    for (const DynamicPlayerCreateInfo &dynamicInfo : playersInfo) {

        PlayCursor::CreateInfo info = staticPool.get(dynamicInfo.playerHandle).getInfo();
        info.posOffset = dynamicInfo.posOffset;
        info.volume    = dynamicInfo.volume;

        playCursors_.playCursors_.emplace_back(info);
    }
}

std::vector<PlayCursor>& SyncDynamicPlayCursors::FrameRenderer::getPlayCursors () const {

    return playCursors_.playCursors_;
}



SyncDynamicPlayCursors::FrameWriter SyncDynamicPlayCursors::getFrameWriter () {

    return FrameWriter(*this);
}

SyncDynamicPlayCursors::FrameRenderer SyncDynamicPlayCursors::getFrameRenderer () {

    return FrameRenderer(*this);
}
