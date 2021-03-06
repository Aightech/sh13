#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "src/netlib.hpp"
#include "src/guilib.hpp"
#include "src/gamelib.hpp"

#include "vector"

#include <time.h> 



int Button::s_nb=0;
Texture Button::s_texture;
Font Button::s_font;

int Textbox::s_nb=0;
Font Textbox::s_font;

extern int WATCHPORT;


int main(int argc,char ** argv)
{
       srand(time(NULL));
       if(argc>1)
       {
              WATCHPORT=atoi(argv[1]);
              //strcpy(m_myname,argv[1]);
       }
       else
              WATCHPORT=4001;
              
       Game game;
       game.init();
       
       game.menu();
       
       switch(game.getState())
       {
              case GAME_ONLINE:
                     game.onlineGame();
              break;
       }
             
       return 0;
}
