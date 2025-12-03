#pragma once  
#include <SFML/Audio.hpp>  
#include <iostream>  
class AudioManager  
{  
public:  
   AudioManager() : shootSound(shootBuffer), explosionSound(explosionBuffer)
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

       if (!ingameMusic.openFromFile("ingamemusic.wav"))
       {
           std::cerr << "Error loading menumusic.wav\n";
       }
       ingameMusic.setLoopPoints({ sf::milliseconds(0) });
   }  

   void playShoot() { shootSound.play(); };  
   void playExplosion() { explosionSound.play(); };  
   void playMenuMusic() { menuMusic.play(); };  
   void stopMenuMusic() { menuMusic.stop(); }; 
   void playIngameMusic() { ingameMusic.play(); };
   void stopIngameMusic() { ingameMusic.stop(); };

private:
   
    sf::SoundBuffer shootBuffer;
    sf::Sound shootSound;

    sf::SoundBuffer explosionBuffer;
    sf::Sound explosionSound;

    sf::Music menuMusic;
    sf::Music ingameMusic;
};