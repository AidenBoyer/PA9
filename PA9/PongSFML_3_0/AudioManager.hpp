#pragma once  
#include <SFML/Audio.hpp>  
#include <iostream>

enum class Track
{
    None, 
    Menu,
    Ingame
};

class AudioManager  
{  
public:  
   AudioManager() : shootSound(shootBuffer), explosionSound(explosionBuffer), deathSound(deathBuffer)
   {
       //SFX
       if (!shootBuffer.loadFromFile("shoot.wav"))  
       {  
           std::cerr << "Error loading shoot.wav\n";  
       }
       shootSound.setBuffer(shootBuffer);

       if (!explosionBuffer.loadFromFile("explosion.wav"))  
       {  
           std::cerr << "Error loading explosion.wav\n";  
       }  
       explosionSound.setBuffer(explosionBuffer);  

       if (!deathBuffer.loadFromFile("Lego yoda death sound.mp3"))
       {
           std::cerr << "Error loading lego yoda death sound.mp3\n";
       }
       deathSound.setBuffer(deathBuffer);

       //MUSIC
       if (!menuMusic.openFromFile("alien shooter mainmenu theme.wav"))  
       {  
           std::cerr << "Error loading menumusic\n";  
       }  
       menuMusic.setLooping(true);

       if (!ingameMusic.openFromFile("alien shooter ingame.wav"))
       {
           std::cerr << "Error loading ingame music\n";
       }
       ingameMusic.setLooping(true);
   }  

   void playShoot() { shootSound.play(); };  
   void playExplosion() { explosionSound.play(); };  

   void playMenuMusic() {
       if (currentTrack != Track::Menu)
       {
           stopAllMusic();
           menuMusic.play();
           currentTrack = Track::Menu;
       }
   };
   void playIngameMusic() {
       if (currentTrack != Track::Ingame)
       {
           stopAllMusic();
           ingameMusic.play();
           currentTrack = Track::Ingame;
       }
   };
   void stopAllMusic() { ingameMusic.stop(); menuMusic.stop(); };

private:
   
    sf::SoundBuffer shootBuffer;
    sf::Sound shootSound;

    sf::SoundBuffer explosionBuffer;
    sf::Sound explosionSound;

    sf::SoundBuffer deathBuffer;
    sf::Sound deathSound;

    sf::Music menuMusic;
    sf::Music ingameMusic;

    Track currentTrack = Track::None;
};