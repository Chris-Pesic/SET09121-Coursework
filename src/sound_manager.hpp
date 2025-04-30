#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <iostream>

class SoundManager {
public:
    static SoundManager& getInstance();

    void playJumpSound();

    void setVolume(float volume);
private:
    SoundManager();
    float currentPitch = 0.8f;

    float volume = 50.f;

    sf::SoundBuffer buffer;
    sf::Sound jumpSound;


};

#endif // SOUND_MANAGER_HPP
