#include "PlayCursor.hpp"
#include <SyncPlayers.hpp>
#include <AudioPlayer.hpp>
#include <cstdint>
#include <vector>


SyncStaticPlayCursors::MainView::MainView (SyncStaticPlayCursors &playCursors): 
  playCursors_(playCursors) {}

SyncStaticPlayCursors::Handle SyncStaticPlayCursors::MainView::addPlayCursor (PlayCursor playCursor) {

    playCursors_.cmdQueue_.push(Command{ Command::Add { playCursor }});

    Handle handle = playCursors_.nextFreeHandle_;
    playCursors_.nextFreeHandle_ = static_cast<Handle>(static_cast<uint32_t>(playCursors_.nextFreeHandle_) + 1);

    return handle;
}

void SyncStaticPlayCursors::MainView::removePlayCursor (Handle handle) {

    playCursors_.cmdQueue_.push(Command{ Command::Remove { handle }});
}



SyncStaticPlayCursors::RenderView::RenderView (SyncStaticPlayCursors &playCursors): 
  playCursors_(playCursors) {

    Command cmd;
    while (playCursors_.cmdQueue_.pop(cmd)) {
        std::visit(overloads {
            
            [](std::monostate) {},
            [this](Command::Add &cmd)    { parseCommandAdd(cmd); },
            [this](Command::Remove &cmd) { parseCommandRemove(cmd); },
        }, cmd.data);
    }
}

PlayCursor::CreateInfo SyncStaticPlayCursors::RenderView::getPlayCursorInfo (Handle handle) const {

    return playCursors_.playCursors_[static_cast<uint32_t>(handle)].getInfo();
}

std::vector<PlayCursor>& SyncStaticPlayCursors::RenderView::getPlayCursors () const {

    return playCursors_.playCursors_;
}

void SyncStaticPlayCursors::RenderView::parseCommandAdd (Command::Add &cmd) {

    playCursors_.playCursors_.emplace_back(cmd.playCursor);
}

void SyncStaticPlayCursors::RenderView::parseCommandRemove (Command::Remove &cmd) {

    // Not implemented
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



SyncDynamicPlayCursors::FrameRenderer::FrameRenderer (SyncDynamicPlayCursors &playCursors, const SyncStaticPlayCursors::RenderView &renderView):
  playCursors_(playCursors) {

    if(!playCursors_.buf_.fetch()) 
        return;

    playCursors_.playCursors_.clear();
    std::vector<DynamicPlayerCreateInfo> &playersInfo = playCursors_.buf_.getReadBuffer();
    for (DynamicPlayerCreateInfo dynamicInfo : playersInfo) {

        PlayCursor::CreateInfo info = renderView.getPlayCursorInfo(dynamicInfo.playerHandle);
        info.posOffset = dynamicInfo.posOffset;
        info.volume = dynamicInfo.volume;

        playCursors_.playCursors_.emplace_back(PlayCursor(info));
    }
}

std::vector<PlayCursor>& SyncDynamicPlayCursors::FrameRenderer::getPlayCursors () const {

    return playCursors_.playCursors_;
}



SyncDynamicPlayCursors::FrameWriter SyncDynamicPlayCursors::getFrameWriter () {

    return FrameWriter(*this);
}

SyncDynamicPlayCursors::FrameRenderer SyncDynamicPlayCursors::getFrameRenderer (const SyncStaticPlayCursors::RenderView &renderView) {

    return FrameRenderer(*this, renderView);
}