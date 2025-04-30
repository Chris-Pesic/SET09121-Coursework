#include "sound_manager.hpp"

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager() {
    //load jump sound
    if (!jumpBuffer.loadFromFile("./res/sound/jump.wav")) {

        std::cerr << "Error: Could not load jump.wav" << std::endl;
    }
    jumpSound.setBuffer(jumpBuffer);

    //load bounce sound
    if (!bounceBuffer.loadFromFile("./res/sound/bouncePad.ogg")) {
        std::cerr << "Error: Could not load bouncePad.ogg" << std::endl;
    }
    bounceSound.setBuffer(bounceBuffer);

    //load alert sound
    if (!alertBuffer.loadFromFile("./res/sound/deggAlert.ogg")) {
        std::cerr << "Error: Could not load deggAlert.ogg" << std::endl;
    }
    alertSound.setBuffer(alertBuffer);

    //load death sound (egg cracking)
    if (!deathBuffer.loadFromFile("./res/sound/eggyCrack.ogg")) {
        std::cerr << "Error: Could not load eggyCrack.ogg" << std::endl;
    }
    deathSound.setBuffer(deathBuffer);

    //load level clear sound
    if (!goalBuffer.loadFromFile("./res/sound/levelComplete.ogg")) {
        std::cerr << "Error: Could not load levelComplete.ogg" << std::endl;
    }
    goalSound.setBuffer(goalBuffer);

    //load background music
    if (!bgm1.openFromFile("./res/sound/kevinMacleod.ogg")) {
        std::cerr << "Error: Could not load background music." << std::endl;
    }
}

void SoundManager::setVolume(float volume) {
    this->volume = volume;
    bgm1.setVolume(volume);
}

void SoundManager::playJumpSound() {
    jumpSound.setVolume(volume);

    currentPitch += 0.2f;
    if (currentPitch > 1.2f) {
        currentPitch = 0.8f;
    }

    jumpSound.setPitch(currentPitch);
    jumpSound.play();
}

void SoundManager::playBounceSound() {
    bounceSound.setVolume(volume);
    bounceSound.play();
}

void SoundManager::playAlertSound() {
    alertSound.setVolume(volume);
    alertSound.play();
}

void SoundManager::playDeathSound() {
    deathSound.setVolume(volume);
    deathSound.play();
}

void SoundManager::playGoalSound() {
    goalSound.setVolume(volume);
    goalSound.play();
}

void SoundManager::playBGM1() {
    bgm1.setLoop(true);
    bgm1.setVolume(volume);
    bgm1.play();
}

void SoundManager::stopBGM1() {
    bgm1.stop();
}
