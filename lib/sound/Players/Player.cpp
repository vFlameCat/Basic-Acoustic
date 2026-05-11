#include "Player.hpp"


Player::Player (CreateInfo info):
  pitch    (info.pitch),
  volume   (info.volume),
  pos_     (info.pos),
  posOffset_(info.posOffset),
  audio_   (&info.audio),
  isLooped_(info.isLooped) {}


Player::CreateInfo Player::getInfo () const {

    return CreateInfo {
        .pos       = pos_,
        .posOffset = posOffset_,
        .pitch     = pitch,
        .volume    = volume,
        .audio     = *audio_,
        .isLooped  = isLooped_,
    };
}
