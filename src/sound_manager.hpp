#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <iostream>

class SoundManager {
public:
    static SoundManager& getInstance(); 

    void playJumpSound(); 

private:
    SoundManager();
    float currentPitch = 0.8f;

    sf::SoundBuffer buffer;
    sf::Sound jumpSound;


};

#endif // SOUND_MANAGER_HPP