#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <iostream>

class SoundManager {
public:
    static SoundManager& getInstance();

    void playJumpSound();
    void playBounceSound();
    void playAlertSound();
    void playDeathSound();
    void playGoalSound();

    void playBGM1();
    void stopBGM1();

    void setVolume(float volume);
private:
    SoundManager();
    float currentPitch = 0.8f;

    float volume = 1.0f;

    //background music
    sf::Music bgm1;

    //jump sound
    sf::SoundBuffer jumpBuffer;

    sf::Sound jumpSound;

    //bounce pad sound
    sf::SoundBuffer bounceBuffer;
    sf::Sound bounceSound;

    //deviled egg alert sound
    sf::SoundBuffer alertBuffer;
    sf::Sound alertSound;

    //death sound
    sf::SoundBuffer deathBuffer;
    sf::Sound deathSound;

    //reached goal/completed level sound
    sf::SoundBuffer goalBuffer;
    sf::Sound goalSound;
};

#endif // SOUND_MANAGER_HPP
