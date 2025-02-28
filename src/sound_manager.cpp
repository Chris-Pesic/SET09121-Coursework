#include "sound_manager.hpp"

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager() {
    if (!buffer.loadFromFile("C:/Users/angus/SET09121-Coursework/res/sound/jump.wav")) {
        std::cerr << "Error: Could not load jump.wav" << std::endl;
    }
    jumpSound.setBuffer(buffer);
}

void SoundManager::playJumpSound() {
    jumpSound.setVolume(50.f);

    currentPitch += 0.2f;
    if (currentPitch > 1.2f) {
        currentPitch = 0.8f; 
    }

    jumpSound.setPitch(currentPitch);
    jumpSound.play();
}
