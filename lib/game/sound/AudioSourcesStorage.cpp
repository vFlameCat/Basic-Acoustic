#include <AudioSourcesStorage.hpp>


AudioSourcesStorage::Handle AudioSourcesStorage::addAudioSource (const AudioSource &source) {

    Handle handle = static_cast<Handle>(storage_.size());
    storage_.emplace_back(source);

    return handle;
}

const AudioSource& AudioSourcesStorage::getAudioSource (Handle handle) const {

    return storage_[static_cast<std::vector<AudioSource>::size_type>(handle)];
}

AudioSource& AudioSourcesStorage::getAudioSource (Handle handle) {

    return storage_[static_cast<std::vector<AudioSource>::size_type>(handle)];
}

void AudioSourcesStorage::removeAudioSource (Handle handle) {

    storage_[static_cast<std::vector<AudioSource>::size_type>(handle)] = AudioSource();
}