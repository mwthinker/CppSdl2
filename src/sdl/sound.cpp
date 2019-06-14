#include "sound.h"
#include "logger.h"

#include <algorithm>

namespace sdl {

	int Sound::lastId_ = 0;

	Sound::Sound() {
		channel_ = -1;
		id_ = ++lastId_;
		volume_ = 1;
	}

	Sound::~Sound() {
		stopPlaying();
	}

	Sound::Sound(const std::string& filename) {
		soundBuffer_ = std::make_shared<SoundBuffer>(filename);
		channel_ = -1;
		id_ = ++lastId_;
		volume_ = 1;
	}
	
	Sound::Sound(const Sound& sound) {
		soundBuffer_ = sound.soundBuffer_;
		channel_ = -1;
		id_ = ++lastId_;
		volume_ = sound.volume_;
	}

	Sound& Sound::operator=(const Sound& sound) {
		soundBuffer_ = sound.soundBuffer_;
		channel_ = -1;
		id_ = ++lastId_;
		volume_ = sound.volume_;
		return *this;
	}

	void Sound::play(int loops) {
		if (soundBuffer_ && soundBuffer_->valid_) {
			// Have no chanel? Or has stopped played the sound?
			if (!ownChannel() || !isPlaying()) {
				channel_ = Mix_PlayChannel(-1, soundBuffer_->mixChunk_, loops);
				if (channel_ != -1) {
					SoundBuffer::channelList[channel_] = id_;
					// Set the volume on the current channel.
					setVolume(volume_);
				} else {
					// All channels is being used.
					logger()->warn("[Sound] Failed to play sound, id {}, all channels is being used! {}", id_);
					logger()->warn("[Sound] Mix_GetError(): {}", Mix_GetError());
				}
			} else {
				logger()->warn("[Sound] Failed to play sound, id {}, already playing.", id_);
			}
		}
	}

	void Sound::pause() {
		if (soundBuffer_ && soundBuffer_->valid_) {
			// Owns the channel?
			if (SoundBuffer::channelList[channel_] == id_) {
				Mix_Pause(channel_);
			}
		}
	}

	void Sound::resume() {
		if (soundBuffer_ && soundBuffer_->valid_) {
			// Owns the channel?
			if (SoundBuffer::channelList[channel_] == id_) {
				Mix_Resume(channel_);
			}
		}
	}

	bool Sound::isPaused() const {
		if (soundBuffer_ && soundBuffer_->valid_) {
			// Owns the channel?
			if (SoundBuffer::channelList[channel_] == id_) {
				return Mix_Paused(id_) == 1;
			}
		}
		return false;
	}

	void Sound::stopPlaying() {
		if (ownChannel() && isPlaying()) {
			Mix_HaltChannel(channel_);
		}
	}

	void Sound::setVolume(float volume) {
		volume_ = volume;
		if (soundBuffer_ && soundBuffer_->valid_) {
			Mix_Volume(channel_, (int) (volume_ * MIX_MAX_VOLUME));
		}
	}

	float Sound::getVolume() const {
		return volume_;
	}

	bool Sound::isValid() const {
		return soundBuffer_ ? soundBuffer_->valid_ : false;
	}

	int Sound::getChannel() const {
		// Owns the channel?
		if (SoundBuffer::channelList[channel_] == id_) {
			return channel_;
		}
		return 0;
	}

	std::map<int, int> Sound::SoundBuffer::channelList;

	Sound::SoundBuffer::SoundBuffer(const std::string& filename) : valid_(true) {
		mixChunk_ = Mix_LoadWAV(filename.c_str());
		if (mixChunk_ == nullptr) {
			valid_ = false;
			logger()->warn("[Sound] Mix_LoadWAV {}: {} failed to load. ", filename, Mix_GetError());
		}
	}

	Sound::SoundBuffer::~SoundBuffer() {
		if (mixChunk_ != 0) {
			Mix_FreeChunk(mixChunk_);
		}
	}

} // Namespace sdl.
