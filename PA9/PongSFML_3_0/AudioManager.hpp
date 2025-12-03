#pragma once  
#include <SFML/Audio.hpp>  
#include <iostream>  
class AudioManager  
{  
public:  
   AudioManager() // Explicitly initialize sf::Sound objects  
   {  
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

       if (!menuMusic.openFromFile("menumusic.wav"))  
       {  
           std::cerr << "Error loading menumusic.wav\n";  
       }  
       menuMusic.setLoopPoints({sf::milliseconds(0)});  
   }  

   void playShoot() { shootSound.play(); };  
   void playExplosion() { explosionSound.play(); };  
   void playMenuMusic() { menuMusic.play(); };  
   void stopMenuMusic() { menuMusic.stop(); };  

   /*AudioManager(const AudioManager&) = delete;  
   AudioManager& operator=(const AudioManager&) = delete;*/  

private:  
   sf::SoundBuffer shootBuffer;  
   sf::Sound shootSound;  

   sf::SoundBuffer explosionBuffer;  
   sf::Sound explosionSound;  

   sf::Music menuMusic;  
};