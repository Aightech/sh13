#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "network.hpp"
#include "guilib.hpp"
#include "gamelib.hpp"

#include "vector"

#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/wait.h>

#include <netdb.h>
#include <arpa/inet.h>


#include <fcntl.h>


using namespace sf;
using namespace std;


Game::Game()
{
       time = clock();
       m_state=0;
       m_turn=1;
       LANport=LANPORT;
       
       window.create(VideoMode(WIN_W,WIN_H), GAME_NAME);   
}

void Game::init()
{   
       m_font.loadFromFile(GAME_FONT_TITLE);
       m_title.setFont(m_font);
       m_title.setString(GAME_NAME);
       m_title.setCharacterSize(40);
       m_title.setStyle(sf::Text::Bold);
       m_title.setPosition(Vector2f(50, 65));
       m_title.setColor(Color(255,232,30));
       
       button.setFont(GAME_FONT_BUTTON);
       button.setTexture(GAME_THEME_BUTTON);
       
       m_state=0;
}

void Game::menu()
{
       createMenu();
       
       string text="hey";
       //Textbox bar(&text,)
       
       while(window.isOpen())
       {
              Event event;
              while(window.pollEvent(event))
              {
                     if(event.type==Event::Closed)
                            window.close();
                     if (event.type == sf::Event::Resized)
                     {
                            window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                            createMenu();
                     }
                     if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                     {
                         // le bouton gauche est enfoncé : on tire
                         //warehouse.setPosition(msPos.x/10*10,msPos.y/10*10);
                         
                     }
                     if (event.type == sf::Event::TextEntered)
                     {
                         //if (event.text.unicode < 128)
                         char c=event.text.unicode;
                             std::cout << "ASCII character typed: " << (int)c << std::endl;
                     }
                     
              }
              
              
              msPos=Mouse::getPosition(window);
              
              window.clear(Color(48,48,48));
              //window.draw(button);
              
             int nbButt=button.getNbOfBt();
             int select=0;
             
              for(int i=0;i<nbButt;i++)
                     select+=buttons[i]->update(window);
              if(select)
              {
                     switch(abs(select))
                     {
                            case PLAY_BUTT:
                            
                            break;
                            case LAN_BUTT:
                                   m_state=LAN_MENU;
                                   createMenu();
                            break;
                            case OPTION_BUTT:
                                   m_state=OPTION_MENU;
                                   createMenu();
                            break;
                            case QUIT_BUTT:
                                   window.close();
                            break;
                            
                            case BACK_BUTT:
                                   m_state=MAIN_MENU;
                                   createMenu();
                            break;
                            
                            case JOIN_BUTT:
                                   m_state=GAME_ONLINE;
                                   return ;
                            break;
                            
                            case HOST_BUTT:
                            break;
                            
                            case SAVE_BUTT:
                            break;
                            
                            case _BUTT:
                            break;
                     }
              }
              
              window.draw(m_title);
              window.display();
       }
       
}

void Game::createMenu()
{
       cout<<buttons.size()<<endl;
       m_title.setPosition(Vector2f(window.getSize().x/2-170, window.getSize().y/7));
       for(int i=buttons.size()-1;i>-1;i--)
       {
              delete buttons[i];
              buttons.pop_back();
       }
       
       switch(m_state)
       {
              case MAIN_MENU:
              {
                     string buttonsLabel[]={"play","lan","options","quit"};
                     int buttonIndex[]={PLAY_BUTT,LAN_BUTT,OPTION_BUTT,QUIT_BUTT};
                     for(int i=0;i<4;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(window.getSize().x/2-60, window.getSize().y*2/7+80*i),buttonIndex[i]));
                     }
              }
              break;
              case OPTION_MENU:
              {
                     string buttonsLabel[]={"save","back","quit"};
                     int buttonIndex[]={SAVE_BUTT,BACK_BUTT,QUIT_BUTT};
                     for(int i=0;i<3;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(20+window.getSize().x*i/3,window.getSize().y*7/8),buttonIndex[i]));
                     }
              }
              break;
              case LAN_MENU:
              {
                     string buttonsLabel[]={"host game","join game","back"};
                     int buttonIndex[]={HOST_BUTT,JOIN_BUTT,BACK_BUTT};
                     for(int i=0;i<3;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(20+window.getSize().x*i/3,window.getSize().y*7/8),buttonIndex[i]));
                     }
              }
              break;
       }
}

void Game::onlineGame()
{
       m_state=MAIN_GAME;
       createGameContext();
       
       while(window.isOpen())
       {
              Event event;
              while(window.pollEvent(event))
              {
                     if(event.type==Event::Closed)
                            window.close();
                     if (event.type == sf::Event::Resized)
                     {
                            window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                            createGameContext();
                     }
                     
              }
              
              
              msPos=Mouse::getPosition(window);
              
              window.clear(Color(48,48,48));
              
             int nbButt=button.getNbOfBt();
             int select=0;
              for(int i=0;i<nbButt;i++)
                     select+=buttons[i]->update(window);
              if(select)
              {
                     printf("%d\n",select);
                     switch(abs(select))
                     {
                            case QUIT_BUTT:
                                   window.close();
                            break;
                            
                            case GENPOLL_BUTT:
                                   m_state=GENPOLL_GAME;
                                   createGameContext();
                            break;
                            
                            
                            case INQUIRY_BUTT:
                                   m_state=INQUIRY_GAME;
                                   createGameContext();
                            break;
                            
                            case DENOUNCE_BUTT:
                                   m_state=DENOUNCE_GAME;
                                   createGameContext();
                            break;
                            
                            case BACK_BUTT:
                                   m_state=MAIN_GAME;
                                   createGameContext();
                            break;
                            
                     }
              }
              window.draw(m_title);
              window.display();
       }
       
}

void Game::createGameContext()
{
       
       m_title.setPosition(Vector2f(10, 10));
       for(int i=buttons.size()-1;i>-1;i--)
       {
              delete buttons[i];
              buttons.pop_back();
       }
       
       switch(m_state)
       {
              case MAIN_GAME:
              {
                     
                     string buttonsLabel[]={"gen. poll","inquiry","denounce","quit"};
                     int buttonIndex[]={GENPOLL_BUTT,INQUIRY_BUTT,DENOUNCE_BUTT,QUIT_BUTT};
                     int buttonPos[][2]={{window.getSize().x*4/8,window.getSize().y*7/9},{window.getSize().x*5/8,window.getSize().y*7/9},{window.getSize().x*6/8,window.getSize().y*7/9},{window.getSize().x*7/8,window.getSize().y*7/8}};
                     for(int i=0;i<4;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(buttonPos[i][0],buttonPos[i][1]),buttonIndex[i]));
                     }
                     if(!isMyTurn())
                            for(int i=0;i<3;i++)
                                   buttons[i]->disable();
                     
              }
              break;
              case GENPOLL_GAME:
              {
                     string buttonsLabel[]={"gen. poll","inquiry","denounce","quit","back"};
                     int buttonIndex[]={GENPOLL_BUTT,INQUIRY_BUTT,DENOUNCE_BUTT,QUIT_BUTT,BACK_BUTT};
                     int buttonPos[][2]={{window.getSize().x*4/8,window.getSize().y*7/9},{window.getSize().x*5/8,window.getSize().y*7/9},{window.getSize().x*6/8,window.getSize().y*7/9},{window.getSize().x*7/8,window.getSize().y*7/8},{window.getSize().x*6/8,window.getSize().y*7/8}};
                     for(int i=0;i<5;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(buttonPos[i][0],buttonPos[i][1]),buttonIndex[i]));
                     }
                     for(int i=0;i<3;i++)
                            buttons[i]->disable();
                            
                     string buttonsIconLabel[]={"jewelry","bulb","book","medal","skull","eye","pipe","fist"};
                     int buttonIconIndex[]={JEWELRY_BUTT,BULB_BUTT,BOOK_BUTT,MEDAL_BUTT,SKULL_BUTT,EYE_BUTT,PIPE_BUTT,FIST_BUTT};
                     for(int i=0;i<8;i++)
                     {
                            buttons.push_back(new Button(buttonsIconLabel[i],Vector2f(window.getSize().x/2-90, window.getSize().y*1/7+50*i),buttonIconIndex[i]));
                     }
              }
              break;
              case INQUIRY_GAME:
              {
                     string buttonsLabel[]={"gen. poll","inquiry","denounce","quit","back"};
                     int buttonIndex[]={GENPOLL_BUTT,INQUIRY_BUTT,DENOUNCE_BUTT,QUIT_BUTT,BACK_BUTT};
                     int buttonPos[][2]={{window.getSize().x*4/8,window.getSize().y*7/9},{window.getSize().x*5/8,window.getSize().y*7/9},{window.getSize().x*6/8,window.getSize().y*7/9},{window.getSize().x*7/8,window.getSize().y*7/8},{window.getSize().x*6/8,window.getSize().y*7/8}};
                     for(int i=0;i<5;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(buttonPos[i][0],buttonPos[i][1]),buttonIndex[i]));
                     }
                     for(int i=0;i<3;i++)
                            buttons[i]->disable();
                            
                     string buttonsIconLabel[]={"jewelry","bulb","book","medal","skull","eye","pipe","fist"};
                     int buttonIconIndex[]={JEWELRY_BUTT,BULB_BUTT,BOOK_BUTT,MEDAL_BUTT,SKULL_BUTT,EYE_BUTT,PIPE_BUTT,FIST_BUTT};
                     for(int i=0;i<8;i++)
                     {
                            buttons.push_back(new Button(buttonsIconLabel[i],Vector2f(window.getSize().x/2-90, window.getSize().y*1/7+50*i),buttonIconIndex[i]));
                     }
                     
              }
              break;
       }
              
}

void startServer()
{


       socklen_t clilen;
       struct sockaddr_in serv_addr, cli_addr;
       sockfd = socket(AF_INET, SOCK_STREAM, 0);
       if (sockfd < 0) error("ERROR opening socket");
       bzero((char *) &serv_addr, sizeof(serv_addr));
       portno = atoi(argv[1]);
       serv_addr.sin_family = AF_INET;
       serv_addr.sin_addr.s_addr = INADDR_ANY;
       serv_addr.sin_port = htons(portno);

       if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              printf("error(ERROR on binding\n");

       listen(sockfd,5);//define la taille de la liste d'attente
       clilen = sizeof(cli_addr);
       //////
       
       
     
       while (1)
       {    
              newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);      
              if (newsockfd < 0) printf("error(ERROR on binding\n");

              bzero(buffer,256);
              n=read(newsockfd,buffer,255);
              if (n < 0) printf("error(ERROR on binding\n");
              printf("Address %s on port %d :\n",inet_ntoa(serv_addr.sin_addr), ntohs(cli_addr.sin_port));
              printf("request:%s\n",strchr(buffer,'G')+1);
              switch(atoi(strchr(buffer,'G')+1))
              {
                     case 0:
                            printf("server ?\n");
                            strcpy(buffer,"G0Q0P0O0");
                            n = write(newsockfd,buffer,strlen(buffer));
                            printf("reply: yes\n");
                     break;
                     case 1:
                            printf("join ?\n");
                            if(0)
                            {
                                   strcpy(buffer,"G1Q0P0O0");
                                   n = write(newsockfd,buffer,strlen(buffer));
                                   printf("reply: yes\n");
                            }
                            else
                            {
                                   strcpy(buffer,"G-1Q0P0O0");
                                   n = write(newsockfd,buffer,strlen(buffer));
                                   printf("reply: no\n");
                            }
                     break;
              }

              close(newsockfd);
     	}
     
     close(sockfd);
     return 0; 
}


/*

       
       char imgName[]="media/SH13_00.png";
       for(int i=0;i<13;i++)
       {
              sprintf(imgName,"media/SH13_%d.png",i);
              printf("%s\n",imgName);
              persoTextures.push_back(new Texture);
              if (!persoTextures[i]->loadFromFile(imgName, sf::IntRect(0, 0, 300, 198)))
              {
                  // erreur...
              }
              persoSprites.push_back(new Sprite);
              persoSprites[i]->setTexture(*persoTextures[i]);
              persoSprites[i]->setScale(0.5,0.5);
              persoSprites[i]->setPosition(Vector2f(550+160*(i%4),10+110*(int)(i/4)));
       }
       
       char iconName[]="media/icons/SH13_icon00_120x120.png";
       for(int i=0;i<8;i++)
       {
              sprintf(iconName,"media/icons/SH13_icon%d_120x120.png",i);
              iconsTextures.push_back(new Texture);
              if (!iconsTextures[i]->loadFromFile(iconName, sf::IntRect(0, 0, 300, 198)))
              {
                  // erreur...
              }
              iconsSprites.push_back(new Sprite);
              iconsSprites[i]->setTexture(*iconsTextures[i]);
              iconsSprites[i]->setScale(0.3,0.3);
              iconsSprites[i]->setPosition(Vector2f(250+80*(i%4),80+50*(int)(i/4)));
       }
       /*
       Texture textureIcon;
       Sprite icon;
       if (!textureIcon.loadFromFile("media/icons/SH13_icon0_120x120.png", sf::IntRect(0, 0, 300, 198)))
       {
           // erreur...
       }
       icon.setTexture(textureIcon);
       icon.setScale(0.3,0.3);
       icon.setPosition(Vector2f(250,70));
       Texture texture;
       if (!texture.loadFromFile("media/theme/wood.png", sf::IntRect(0, 0, 230, 100)))
       {
           // erreur...
       }
       if (!font.loadFromFile(GAME_FONT_BUTTON))
       {
           // erreur...
       }
       for(int i=0;i<4;i++)
       {
              playerPlateSprites.push_back(new Sprite);
              playerPlateSprites[i]->setTexture(texture);
              playerPlateSprites[i]->setPosition(Vector2f(10,70+110*i));
              
              labelPlayerName.push_back(new Text);
              labelPlayerName[i]->setFont(font);
              labelPlayerName[i]->setString("NAME:");
              labelPlayerName[i]->setCharacterSize(20);
              labelPlayerName[i]->setPosition(Vector2f(20,80+110*i));
              labelPlayerName[i]->setColor(Color(255,255,255));
              
              labelPlayerIP.push_back(new Text);
              labelPlayerIP[i]->setFont(font);
              labelPlayerIP[i]->setString("IP ADDR.:");
              labelPlayerIP[i]->setCharacterSize(10);
              labelPlayerIP[i]->setPosition(Vector2f(20,110+110*i));
              labelPlayerIP[i]->setColor(Color(255,255,255));
       }
       */
