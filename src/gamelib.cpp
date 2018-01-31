#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "netlib.hpp"
#include "guilib.hpp"
#include "gamelib.hpp"

#include "vector"
#include <time.h> 


#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/wait.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>




using namespace sf;
using namespace std;

int WATCHPORT;


Game::Game()
{
       time = clock();
       
       m_state=0;
       
       m_turn=1;
       
       window.create(VideoMode(WIN_W,WIN_H), GAME_NAME);   
}


void Game::init()
{   
       
       m_buffer.R_port = WATCHPORT;
       
       pthread_create(&m_thread_server, NULL,tcpWatchdog,(void *) &m_buffer);       
       
       m_font.loadFromFile(GAME_FONT_TITLE);
       m_title.setFont(m_font);
       m_title.setString(GAME_NAME);
       m_title.setCharacterSize(40);
       m_title.setStyle(sf::Text::Bold);
       m_title.setPosition(Vector2f(50, 65));
       m_title.setColor(Color(255,232,30));
       
       button.setFont(GAME_FONT_BUTTON);
       button.setTexture(GAME_THEME_BUTTON);
       
       m_arrayConnectionTexture.loadFromFile(GAME_THEME_ARRAY);
       
       int array[13][3]={   {4,7,-1},
                            {4,1,0},
                            {3,5,2},
                            {3,7,2},
                            {3,1,-1},
                            {3,7,-1},
                            {3,6,5},
                            {6,1,7},
                            {6,5,7},
                            {6,1,2},
                            {6,0,-1},
                            {2,0,-1},
                            {4,1,-1}      };
       for(int i=0;i<13;i++)
              for(int j=0;j<3;j++)
                     m_charactersArray[i][j]=array[i][j];
                     
       char imgName[]="media/SH13_00.png";
       for(int i=0;i<13;i++)
       {
              sprintf(imgName,"media/SH13_%d.png",i);
              printf("%s\n",imgName);
              m_persoTextures.push_back(new Texture);
              if (!m_persoTextures[i]->loadFromFile(imgName, sf::IntRect(0, 0, 300, 198)))
              {
                  // erreur...
              }
              m_persoSprites.push_back(new Sprite);
              m_persoSprites[i]->setTexture(*m_persoTextures[i]);
              m_persoSprites[i]->setScale(0.5,0.5);
              m_persoSprites[i]->setPosition(Vector2f(550+160*(i%4),10+110*(int)(i/4)));
       }
       
       char iconName[]="media/icons/SH13_icon00_120x120.png";
       for(int i=0;i<8;i++)
       {
              sprintf(iconName,"media/icons/SH13_icon%d_120x120.png",i);
              m_iconsTextures.push_back(new Texture);
              if (!m_iconsTextures[i]->loadFromFile(iconName, sf::IntRect(0, 0, 300, 198)))
              {
                  // erreur...
              }
              m_iconsSprites.push_back(new Sprite);
              m_iconsSprites[i]->setTexture(*m_iconsTextures[i]);
              m_iconsSprites[i]->setScale(0.3,0.3);
              m_iconsSprites[i]->setPosition(Vector2f(250+39*i,140));
              
       }
       
       if (!m_woodTexture.loadFromFile("media/theme/wood.png", sf::IntRect(0, 0, 230, 100)))
       {
           // erreur...
       }
       for(int i=0;i<4;i++)
       {
              m_playerPlateSprites.push_back(new Sprite);
              m_playerPlateSprites[i]->setTexture(m_woodTexture);
              m_playerPlateSprites[i]->setPosition(Vector2f(10,120+70+110*i));
              
              m_labelPlayerName.push_back(new Text);
              m_labelPlayerName[i]->setFont(m_font);
              m_labelPlayerName[i]->setString("NAME:");
              m_labelPlayerName[i]->setCharacterSize(20);
              m_labelPlayerName[i]->setPosition(Vector2f(20,120+80+110*i));
              m_labelPlayerName[i]->setColor(Color(255,255,255));
              
              m_labelPlayerIP.push_back(new Text);
              m_labelPlayerIP[i]->setFont(m_font);
              m_labelPlayerIP[i]->setString("IP ADDR.:");
              m_labelPlayerIP[i]->setCharacterSize(10);
              m_labelPlayerIP[i]->setPosition(Vector2f(20,120+110+110*i));
              m_labelPlayerIP[i]->setColor(Color(255,255,255));
              
              m_playersObjects.push_back(new Text);
              m_playersObjects[i]->setFont(m_font);
              m_playersObjects[i]->setString(". . . . . . . .");
              m_playersObjects[i]->setCharacterSize(24);
              m_playersObjects[i]->setPosition(Vector2f(257,120+110+110*i));
              m_playersObjects[i]->setColor(Color(255,255,255));
       }
       
       
       m_state=0;
}

void Game::menu()
{
       createMenu();
       
       
       while(window.isOpen() && m_state!=GAME_ONLINE)
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
              
              processBuffer();
              if(!(window.isOpen() && m_state!=GAME_ONLINE))
                     printf("01\n"); 
              
              window.clear(Color(48,48,48));
              
              
              for(int i=0;i<m_arraySprites.size();i++)
                     window.draw(*m_arraySprites[i]);
              for(int i=0;i<m_arrayText.size();i++)
                     window.draw(*m_arrayText[i]);
              
             int nbButt=button.getNbOfBt();
             int select=0;
             int s=0;
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
                                   
                                   m_nbOfServers=scanServers(m_servers);
                                   if(m_nbOfServers==0)
                                        printf("no server\n");  
                                   createMenu();
                            break;
                            
                            
                            case JOIN_S1_BUTT:
                            case JOIN_S2_BUTT:
                            case JOIN_S3_BUTT:
                            case JOIN_S4_BUTT:
                            
                                   m_server=abs(select)-JOIN_S1_BUTT;
                                   printf("G: server :%d\n",m_server);
                                   
                                   sprintf(m_buffer.Tx,"G%dP%dN%s",1,m_buffer.R_port,"aight;");
                                   m_buffer.T_flag=1;
                                   sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer); 
                                   m_host=0; 
                                     m_buffer.R_flag=0;
                                   //if(atoi(strchr(m_buffer.Rx,'G')+1)==1)
                                          m_state=CREATION_MENU;
                                          
                            break;
                            
                            case HOST_BUTT:
                                   m_state=CREATION_MENU;
                                   m_host=1;
                                   m_nbPlayer=1;
                                   strcpy(m_players[0].name,"mwa");
                                   strcpy(m_players[0].IPaddress,"127.0.0.1");
                                   m_players[0].portNo=m_buffer.R_port;
                                   m_players[0].no=0;
                                   
                                   if(!fork())
                                   {
                                          startServer();
                                          exit(0);
                                   }
                                   
                                   m_server=0;
                                   strcpy(m_servers[0].IPaddress,"127.0.0.1");
                                   m_servers[0].portNo=SERVERPORT;
                                   
                                   
                                   createMenu();
                            break;
                            
                            case KILL_P1_BUTT: 
                            case KILL_P2_BUTT: 
                            case KILL_P3_BUTT: 
                            case KILL_P4_BUTT:
                            { 
                                   int toKill = abs(select)-KILL_P1_BUTT;
                                   printf("G: s : %d\n",m_server);
                                   printf("G: Killing : %d , to [%s] on %d\n",toKill,m_servers[m_server].IPaddress,m_servers[m_server].portNo);
                                   sprintf(m_buffer.Tx,"G%dU%d",10,toKill);
                                   m_buffer.T_flag=1;
                                   sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer); 
                            }            
                            break;
                            
                            case START_BUTT:
                            { 
                                   if(m_nbPlayer>1)
                                   {
                                          sprintf(m_buffer.Tx,"G%d",20);
                                          m_buffer.T_flag=1;
                                          sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer);
                                          
                                          
                                   }
                                   
                                   
                                   
                            }            
                            break;
                            
                            
                            case OPTION_BUTT:
                                   m_state=OPTION_MENU;
                                   createMenu();
                            break;
                            case QUIT_BUTT:
                                   window.close();
                            break;
                            
                            case BACK_BUTT:
                                   
                                   if(m_state=CREATION_MENU)
                                   {
                                          sprintf(m_buffer.Tx,"G%dU%d",10,m_myNo);
                                          m_buffer.T_flag=1;
                                          sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer); 
                                   }
                                   m_state=MAIN_MENU;
                                   
                                   createMenu();
                            break;
                            
                            case SAVE_BUTT:
                            break;
                     }
              }
              
              window.draw(m_title);
              window.display();
       }
       if(!(window.isOpen()))
              wait(NULL);
       
}

void Game::createMenu()
{
       m_title.setPosition(Vector2f(window.getSize().x/2-170, window.getSize().y/7));
       for(int i=buttons.size()-1;i>-1;i--)
       {
              delete buttons[i];
              buttons.pop_back();
       }
       for(int i=m_arraySprites.size()-1;i>-1;i--)
       {
              delete m_arraySprites[i];
              m_arraySprites.pop_back();
       }
        for(int i=m_arrayText.size()-1;i>-1;i--)
       {
              delete m_arrayText[i];
              m_arrayText.pop_back();
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
                     string buttonsLabel[]={"host game","refresh","back"};
                     int buttonIndex[]={HOST_BUTT,REFRESH_BUTT,BACK_BUTT};
                     for(int i=0;i<3;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(20+window.getSize().x*i/3,window.getSize().y*7/8),buttonIndex[i]));
                     }
                     
                     m_arraySprites.push_back(new Sprite);
                     m_arraySprites[0]->setTexture(m_arrayConnectionTexture);
                     m_arraySprites[0]->setTextureRect(sf::IntRect(0, 0, 651,37 ));
                     m_arraySprites[0]->setPosition(Vector2f(window.getSize().x/2-350,window.getSize().y*3/10));
                     
                     for(int i=0;i<m_nbOfServers;i++)
                     {      
                            int n=5;
                            
                            m_arraySprites.push_back(new Sprite);
                            m_arraySprites[i+1]->setTexture(m_arrayConnectionTexture);
                            m_arraySprites[i+1]->setTextureRect(sf::IntRect(1, 38, 649,50 ));
                            m_arraySprites[i+1]->setPosition(Vector2f(window.getSize().x/2-350+1,window.getSize().y*3/10+37+50*i));
                            
                            //Server number
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i]->setFont(m_font);
                            char no[]="1";
                            no[0]='1'+i;
                            m_arrayText[n*i]->setString(no);
                            m_arrayText[n*i]->setCharacterSize(15);
                            m_arrayText[n*i]->setPosition(Vector2f(window.getSize().x/2-330,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i]->setColor(Color(0,0,0));
                            
                            //Server name
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+1]->setFont(m_font);
                            m_arrayText[n*i+1]->setString(m_servers[i].name);
                            m_arrayText[n*i+1]->setCharacterSize(15);
                            m_arrayText[n*i+1]->setPosition(Vector2f(window.getSize().x/2-300,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+1]->setColor(Color(0,0,0));
                            
                            //Server IPaddress
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+2]->setFont(m_font);
                            m_arrayText[n*i+2]->setString(m_servers[i].IPaddress);
                            m_arrayText[n*i+2]->setCharacterSize(15);
                            m_arrayText[n*i+2]->setPosition(Vector2f(window.getSize().x/2-100,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+2]->setColor(Color(0,0,0));
                            
                            //Server Port no
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+3]->setFont(m_font);
                            char port[]="20270000";
                            sprintf(port,"%d",m_servers[i].portNo);
                            m_arrayText[n*i+3]->setString(port);
                            m_arrayText[n*i+3]->setCharacterSize(15);
                            m_arrayText[n*i+3]->setPosition(Vector2f(window.getSize().x/2+70,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+3]->setColor(Color(0,0,0));
                            
                            //Server date
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+4]->setFont(m_font);
                            char date[]="17h25m23";
                            m_arrayText[n*i+4]->setString(date);
                            m_arrayText[n*i+4]->setCharacterSize(15);
                            m_arrayText[n*i+4]->setPosition(Vector2f(window.getSize().x/2+170,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+4]->setColor(Color(0,0,0));
                            
                            buttons.push_back(new Button("join",Vector2f(window.getSize().x/2+305,window.getSize().y*3/10+35+50*i),JOIN_S1_BUTT+i));
                     }
                     
              }
              break;
              case CREATION_MENU:
              {
                     string buttonsLabel[]={"start","back"};
                     int buttonIndex[]={START_BUTT,BACK_BUTT};
                     for(int i=0;i<2;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(20+window.getSize().x*7/9,window.getSize().y*(i+8)/10),buttonIndex[i]));
                     }
                     if(!m_host)
                            buttons[0]->disable();
                     m_arraySprites.push_back(new Sprite);
                     m_arraySprites[0]->setTexture(m_arrayConnectionTexture);
                     m_arraySprites[0]->setTextureRect(sf::IntRect(0, 0, 651,37 ));
                     m_arraySprites[0]->setPosition(Vector2f(window.getSize().x/2-350,window.getSize().y*3/10));
                     
                     for(int i=0;i<m_nbPlayer;i++)
                     {      
                            int n=5;
                            
                            m_arraySprites.push_back(new Sprite);
                            m_arraySprites[i+1]->setTexture(m_arrayConnectionTexture);
                            m_arraySprites[i+1]->setTextureRect(sf::IntRect(1, 38, 649,50 ));
                            m_arraySprites[i+1]->setPosition(Vector2f(window.getSize().x/2-350+1,window.getSize().y*3/10+37+50*i));
                            
                            //Player number
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i]->setFont(m_font);
                            char no[]="1";
                            no[0]='1'+i;
                            m_arrayText[n*i]->setString(no);
                            m_arrayText[n*i]->setString(no);
                            m_arrayText[n*i]->setCharacterSize(15);
                            m_arrayText[n*i]->setPosition(Vector2f(window.getSize().x/2-330,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i]->setColor(Color(0,0,0));
                            
                            //Player name
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+1]->setFont(m_font);
                            m_arrayText[n*i+1]->setString(m_players[i].name);
                            m_arrayText[n*i+1]->setCharacterSize(15);
                            m_arrayText[n*i+1]->setPosition(Vector2f(window.getSize().x/2-300,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+1]->setColor(Color(0,0,0));
                            
                            //Player IPaddress
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+2]->setFont(m_font);
                            char IP[]="192.168.0.1";
                            m_arrayText[n*i+2]->setString(m_players[i].IPaddress);
                            m_arrayText[n*i+2]->setCharacterSize(15);
                            m_arrayText[n*i+2]->setPosition(Vector2f(window.getSize().x/2-100,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+2]->setColor(Color(0,0,0));
                            
                            //Player Port no
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+3]->setFont(m_font);
                            char port[]="20270000";
                            sprintf(port,"%d",m_players[i].portNo);
                            m_arrayText[n*i+3]->setString(port);
                            m_arrayText[n*i+3]->setCharacterSize(15);
                            m_arrayText[n*i+3]->setPosition(Vector2f(window.getSize().x/2+70,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+3]->setColor(Color(0,0,0));
                            
                            //Player Port no
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+4]->setFont(m_font);
                            char date[]="17h25m23";
                            m_arrayText[n*i+4]->setString(date);
                            m_arrayText[n*i+4]->setCharacterSize(15);
                            m_arrayText[n*i+4]->setPosition(Vector2f(window.getSize().x/2+170,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+4]->setColor(Color(0,0,0));
                            
                            buttons.push_back(new Button("X",Vector2f(window.getSize().x/2+305,window.getSize().y*3/10+35+50*i),KILL_P1_BUTT+i));
                            if(!m_host)
                                   buttons[buttons.size()-1]->disable();
                     }
                     
              }
              break;
       }
}

void Game::onlineGame()
{
       m_state=INIT_GAME;
       printf("G: game started\n");
       processBuffer();
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
             
             for(int i=0;i<m_arraySprites.size();i++)
                     window.draw(*m_arraySprites[i]);
              for(int i=0;i<m_arrayText.size();i++)
                     window.draw(*m_arrayText[i]);
             
              for(int i=0;i<nbButt;i++)
                     select+=buttons[i]->update(window);
              if(select)
              {
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
                            
                            
                            case JEWELRY_BUTT:
                            case BULB_BUTT :
                            case BOOK_BUTT :
                            case MEDAL_BUTT :
                            case SKULL_BUTT :
                            case EYE_BUTT :
                            case PIPE_BUTT :
                            case FIST_BUTT :
                            {
                                   m_obj=abs(select)-JEWELRY_BUTT;
                                   switch(m_state)
                                   {
                                          case GENPOLL_GAME:
                                                 
                                                 printf("01\n");
                                                 sprintf(m_buffer.Tx,"G%dQ%d",21,m_obj);
                                                 m_buffer.T_flag=1;
                                                 printf("G: ask server : %s\n",m_buffer.Tx);
                                                 sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer); 
                                                 m_state=MAIN_GAME;
                                                 m_turn=-1;
                                          break;
                                          case INQUIRY_GAME:
                                                 m_state=INQUIRY_GAME2;
                                                 createGameContext();
                                          break;
                                   }
                            }
                            break;
                            
                            case ASK1_BUTT:
                            case ASK2_BUTT:
                            case ASK3_BUTT:
                            case ASK4_BUTT:
                                   sprintf(m_buffer.Tx,"G%dP%dQ%d",22,abs(select)-ASK1_BUTT,m_obj);
                                   m_buffer.T_flag=1;
                                   printf("G: ask server : %s\n",m_buffer.Tx);
                                   sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer);
                                   m_state=MAIN_GAME;
                                   m_turn=-1;
                            
                            break;
                            
                            case DENOUNCE1_BUTT:
                            case DENOUNCE2_BUTT:
                            case DENOUNCE3_BUTT:
                            case DENOUNCE4_BUTT:
                            case DENOUNCE5_BUTT:
                            case DENOUNCE6_BUTT:
                            case DENOUNCE7_BUTT:
                            case DENOUNCE8_BUTT:
                            case DENOUNCE9_BUTT:
                            case DENOUNCE10_BUTT:
                            
                                   sprintf(m_buffer.Tx,"G%dC%d",23,m_otherCharacters[abs(select)-DENOUNCE1_BUTT]);
                                   m_buffer.T_flag=1;
                                   printf("G: ask server : %s\n",m_buffer.Tx);
                                   sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer);
                                   m_state=MAIN_GAME;
                                   m_turn=-1;
                            
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
       m_title.setCharacterSize(30);
       for(int i=buttons.size()-1;i>-1;i--)
       {
              delete buttons[i];
              buttons.pop_back();
       }
       
       switch(m_state)
       {
              case INIT_GAME:
              {
                     for(int i=m_arraySprites.size()-1;i>-1;i--)
                     {
                            delete m_arraySprites[i];
                            m_arraySprites.pop_back();
                     }
                      for(int i=m_arrayText.size()-1;i>-1;i--)
                     {
                            delete m_arrayText[i];
                            m_arrayText.pop_back();
                     }
                     
                     string buttonsLabel[]={"gen. poll","inquiry","denounce","quit"};
                     int buttonIndex[]={GENPOLL_BUTT,INQUIRY_BUTT,DENOUNCE_BUTT,QUIT_BUTT};
                     int buttonPos[][2]={{window.getSize().x*4/8,window.getSize().y*7/9},{window.getSize().x*5/8,window.getSize().y*7/9},{window.getSize().x*6/8,window.getSize().y*7/9},{window.getSize().x*7/8,window.getSize().y*7/8}};
                     for(int i=0;i<4;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(buttonPos[i][0],buttonPos[i][1]),buttonIndex[i]));
                     }
                     if(m_turn!=m_myNo)
                            for(int i=0;i<3;i++)
                                   buttons[i]->disable();
                                   
                     
                     
                     int cards[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};
                     
                     for(int i=0;i<12/m_nbPlayer;i++)
                     {
                            m_persoSprites[m_players[m_myNo].card[i]]->setPosition(Vector2f(290+906/(12/m_nbPlayer)*i,10));
                            m_arraySprites.push_back(m_persoSprites[m_players[m_myNo].card[i]]);
                            cards[m_players[m_myNo].card[i]]=-1;
                     }
                     int c=0;
                     
                     for(int i=0;i<13-12/m_nbPlayer;i++)
                     {
                            while(cards[c]==-1) c++;
                            m_persoSprites[c]->setPosition(Vector2f(1020-152*((int)i/4),130+100*(i%4)));
                            m_arraySprites.push_back(m_persoSprites[c]);
                            m_otherCharacters[i]=c;
                            printf("C:%d\n",c);
                            c++;
                     }
                     for(int i=0;i<10;i++)
                            printf("[%d]\n",m_otherCharacters[i]);
                     
                     for(int i=0; i<m_nbPlayer;i++)
                     {
                            m_arrayText.push_back(m_labelPlayerIP[i]);
                            m_arrayText.push_back(m_labelPlayerName[i]);
                            m_arrayText.push_back(m_playersObjects[i]);
                            m_arraySprites.push_back(m_playerPlateSprites[i]);
                     }
                     
                     for(int i=0; i<8;i++)
                     {
                            m_arraySprites.push_back(m_iconsSprites[i]);
                     }
                     
              
                     
                     
                   m_state=INIT_GAME;  
              }
              break;
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
                            buttons.push_back(new Button(buttonsIconLabel[i],Vector2f(window.getSize().x/2-30, window.getSize().y*2/10+50*i),buttonIconIndex[i]));
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
                            buttons.push_back(new Button(buttonsIconLabel[i],Vector2f(window.getSize().x/2-30, window.getSize().y*2/10+50*i),buttonIconIndex[i]));
                     }
                     
              }
              break;
              case INQUIRY_GAME2:
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
                            
                     string buttonsIconLabel[]={"ask","ask","ask","ask"};
                     int buttonIconIndex[]={ASK1_BUTT,ASK2_BUTT,ASK3_BUTT,ASK4_BUTT};
                     for(int i=0;i<m_nbPlayer;i++)
                     {
                            buttons.push_back(new Button(buttonsIconLabel[i],Vector2f(110,240+110*i),buttonIconIndex[i]));
                     }
                     
              }
              break;
              case DENOUNCE_GAME:
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
                            
                     string buttonsIconLabel[]={"denounce"};
                     int buttonIconIndex[]={DENOUNCE1_BUTT};
                     for(int i=0;i<13-12/m_nbPlayer;i++)
                     {
                            buttons.push_back(new Button(buttonsIconLabel[0],Vector2f(Vector2f(1025-152*((int)i/4),135+100*(i%4))),DENOUNCE1_BUTT+i));
                     }
                     
              }
              break;
       }
              
}

void Game::startServer()
{
       // Receiving server variables - Initialization//
       serv_sfd = socket(AF_INET, SOCK_STREAM, 0);
       if (serv_sfd < 0) printf("ERROR opening socket\n");
       bzero((char *) &serv_addr, sizeof(serv_addr));
       serv_portNo = SERVERPORT;
       serv_addr.sin_family = AF_INET;
       serv_addr.sin_addr.s_addr = INADDR_ANY;
       serv_addr.sin_port = htons(serv_portNo);

       if (bind(serv_sfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              printf("error(ERROR on binding\n");

       listen(serv_sfd,5);
       serv_clilen = sizeof(serv_clientAddr[0]);
       
       serv_clientPortNo[0]=m_buffer.R_port;
       sprintf(serv_clientIPAddr[0],"%s","127.0.0.1");
       
       printf("TCP IP Host-server sh13 started on Port: %d\n", serv_portNo);
       // ------------------------------------------ //
       
       while (1)
       {    
              serv_cpsfd = accept(serv_sfd, (struct sockaddr *) &serv_clientAddr[0], &serv_clilen);      
              if (serv_cpsfd < 0) 
              {
                     printf("S:error(ERROR on accepting)\n");
                     exit(0);
              }

              bzero(serv_buff.Rx,SIZE_BUFF);
              if(read(serv_cpsfd,serv_buff.Rx,SIZE_BUFF)>0)
              {
                     printf("\nS: ## -Start of communication- ##\n");
                     printf("S: Address [%s] on port %d :\n",inet_ntoa(serv_clientAddr[0].sin_addr), ntohs(serv_clientAddr[0].sin_port));
                     printf("S: Request:%s\n",serv_buff.Rx);//strchr(serv_RxBuff,'G')+1);
                     switch(atoi(strchr(serv_buff.Rx,'G')+1))
                     {
                            case 0://if a client ask if we are a sh13 server
                                   printf("S: Want to know if we were a sh13 server. \n");
                                   sprintf(serv_buff.Tx,"G%dP%dU%dN%s;",0,serv_portNo,m_nbPlayer,"XxpartyxX");
                                   write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                   printf("S: replied : %s \n",serv_buff.Tx);
                            break;
                            
                            
                            case 1://Joining
                            {  
                                   char *name=strtok((strchr(serv_buff.Rx,'N')+1),";");    //get the name of the player
                                   int port = atoi(strchr(serv_buff.Rx,'P')+1);            //get the port of the player
                                   
                                   printf("S: %s want to join the game.\n",name);
                                   
                                   if(m_nbPlayer<NB_MAX_PLAYERS)
                                   {
                                          setPlayer(m_nbPlayer,name,inet_ntoa(serv_clientAddr[0].sin_addr),port);
                                          
                                          sprintf(serv_buff.Tx,"G%d",1);
                                          write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                          printf("S: replied : %s \n",serv_buff.Tx);
                                          
                                          m_nbPlayer++;
                                          
                                          for(int i=0;i<m_nbPlayer;i++)
                                          {
                                                 for(int j=0;j<m_nbPlayer;j++)
                                                 {
                                                        sprintf(serv_buff.Tx,"G%dH%dP%dU%dN%s;I%s;",11,m_players[i].no,m_players[j].portNo,j,m_players[j].name,m_players[j].IPaddress);
                                                        printf("S: send to player %d: info player %d : %s\n",i,j,serv_buff.Tx);
                                                        serv_buff.T_flag=1;
                                                        sendTCP(m_players[i].IPaddress, m_players[i].portNo, &serv_buff);
                                                 }
                                          }
                                          
                                          
                                   }
                                   else
                                   {
                                          strcpy(serv_buff.Tx,"G-1");
                                          write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                          printf("S: replied : %s \n",serv_buff.Tx);
                                   }
                            }
                            break;
                            
                            case 99://Killing
                            {      
                                   
                                   int U = atoi(strchr(serv_buff.Rx,'U')+1);            //get the player to kill
                                   printf("S: Killing : %d \n",U);
                                   if(U==0)
                                   {
                                          for(int i=1;i<m_nbPlayer;i++)
                                          { 
                                                 //say to client he s been kicked out
                                                 sprintf(serv_buff.Tx,"G%d",99);
                                                 serv_buff.T_flag=1;
                                                 sendTCP(m_players[i].IPaddress, m_players[i].portNo, &serv_buff);
                                          }
                                          exit(0);
                                   }
                                          
                                   //say to client he s been kicked out
                                   sprintf(serv_buff.Tx,"G%d",99);
                                   serv_buff.T_flag=1;
                                   sendTCP(m_players[U].IPaddress, m_players[U].portNo, &serv_buff);
                                   
                                   
                                   
                                   for(int i=U;i<m_nbPlayer-1;i++)
                                          setPlayer(m_players[i+1].no,m_players[i+1].name,m_players[i+1].IPaddress,m_players[i+1].portNo);
                                    
                                   m_nbPlayer--;
                                         
                                          
                                   for(int i=0;i<m_nbPlayer;i++)
                                   {
                                          for(int j=0;j<m_nbPlayer;j++)
                                          {
                                                 sprintf(serv_buff.Tx,"G%dP%dU%dN%s;I%s;",11,m_players[j].portNo,j,m_players[j].name,m_players[j].IPaddress);
                                                 printf("S: send to player %d: info player %d : %s\n",i,j,serv_buff.Tx);
                                                 serv_buff.T_flag=1;
                                                 sendTCP(m_players[i].IPaddress, m_players[i].portNo, &serv_buff);
                                          }
                                   }
                                          
                            }
                            break;
                            case 20:
                                   m_turn=0;
                                   shareObj();
                                   sprintf(serv_buff.Tx,"G20");
                                   printf("S: send to player %d: info player : %s\n",0,serv_buff.Tx);
                                   write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                   
                                   for(int i=0;i<m_nbPlayer;i++)
                                   {
                                          sprintf(serv_buff.Tx,"G%dC",20);
                                          for(int j=0;j<12/m_nbPlayer;j++)
                                                sprintf(serv_buff.Tx,"%s%d;",serv_buff.Tx,m_players[i].card[j]);

                                          printf("S: send to player %d: info player : %s\n",i,serv_buff.Tx);
                                          serv_buff.T_flag=1;
                                          sendTCP(m_players[i].IPaddress, m_players[i].portNo, &serv_buff);
                                   }
                                   sprintf(serv_buff.Tx,"G%d",21);
                                   serv_buff.T_flag=1;
                                   sendTCP(m_players[0].IPaddress, m_players[0].portNo, &serv_buff);
                                   
                            break;
                            case 21://GENPOLL
                                   if(strcmp(m_players[m_turn].IPaddress,inet_ntoa(serv_clientAddr[0].sin_addr))==0)
                                   {
                                          int obj=atoi(strchr(serv_buff.Rx,'Q')+1);
                                          sprintf(serv_buff.Tx,"G%dQ%dO",21,obj);
                                          for(int i=0;i<m_nbPlayer;i++)
                                          {
                                                 int o=0;
                                                 for(int j=0;j<12/m_nbPlayer;j++)
                                                 {
                                                        for(int k=0;k<3;k++) 
                                                        {
                                                               if(m_charactersArray[m_players[i].card[j]][k]==obj)  
                                                                      o=1;  
                                                        }      
                                                 }
                                                 sprintf(serv_buff.Tx,"%s%d;",serv_buff.Tx,o);
                                          }

                                          printf("S: send to player %d: info player : %s\n",m_turn,serv_buff.Tx);
                                          serv_buff.T_flag=1;
                                          sendTCP(m_players[m_turn].IPaddress, m_players[m_turn].portNo, &serv_buff);
                                          m_turn=(m_turn+1)%m_nbPlayer;
                                   }
                                   
                            case 22://INQUIRY
                                   if(strcmp(m_players[m_turn].IPaddress,inet_ntoa(serv_clientAddr[0].sin_addr))==0)
                                   {
                                          int obj=atoi(strchr(serv_buff.Rx,'Q')+1);
                                          int p=atoi(strchr(serv_buff.Rx,'P')+1);
                                          
                                          int o=0;
                                          for(int j=0;j<12/m_nbPlayer;j++)
                                          {
                                                 for(int k=0;k<3;k++) 
                                                 {
                                                        if(m_charactersArray[m_players[p].card[j]][k]==obj)  
                                                               o++;  
                                                 }      
                                          }
                                          sprintf(serv_buff.Tx,"G%dP%dO%d",21,p,o);
                                          

                                          printf("S: send to player %d: info player : %s\n",m_turn,serv_buff.Tx);
                                          serv_buff.T_flag=1;
                                          sendTCP(m_players[m_turn].IPaddress, m_players[m_turn].portNo, &serv_buff);
                                          m_turn=(m_turn+1)%m_nbPlayer;
                                   }
                                   
                                   
                            break;
                            case 23://DENOUNCE
                                   if(strcmp(m_players[m_turn].IPaddress,inet_ntoa(serv_clientAddr[0].sin_addr))==0)
                                   {
                                          int c=atoi(strchr(serv_buff.Rx,'C')+1);
                                          if(c==m_culprit)
                                          {
                                                 //win
                                          }
                                          else
                                          {
                                                 //lose
                                          }
                                   }
                            break;
                     }
                     
              printf("S: ## - End of communication - ##\n\n");
              }
              close(serv_cpsfd);
              
                     
     	}
     
     close(serv_sfd);
     
}

void * Game::tcpWatchdog(void * p_data)
{
       // Receiving server variables - Declaration//
       Buffer * buff= (Buffer *) p_data;
       int _sfd;
       int _cpsfd;
       struct sockaddr_in _addr;
       struct sockaddr_in _clientAddr;
       socklen_t _clilen;
       // --------------------------- //
       
       
       // Receiving server variables - Initialization //
       _sfd = socket(AF_INET, SOCK_STREAM, 0);
       if (_sfd < 0) printf("ERROR opening socket\n");
       
       bzero((char *) &_addr, sizeof(_addr));
       _addr.sin_family = AF_INET;
       _addr.sin_addr.s_addr = INADDR_ANY;
       _addr.sin_port = htons(buff->R_port);

       if (bind(_sfd, (struct sockaddr *) &_addr,sizeof(_addr)) < 0) 
              printf("watch: error(ERROR on binding)\n");
              
       printf("TCP IP thread started on Port: %d\n", buff->R_port);

       listen(_sfd,5);
       _clilen = sizeof(_clientAddr);
       buff->R_flag=0;
       // --------------------------------------------- //
       
       
       while (1)
       {    
              if(!buff->R_flag)//if the last data received has been process
              { 
                     
                     _cpsfd = accept(_sfd, (struct sockaddr *) &_clientAddr, &_clilen); //wait for a connection
                     if (_cpsfd < 0) //if the connection went wrong.
                     {
                            printf("W: error(ERROR on binding)\n");
                            exit(0);
                     }
                     
                     bzero(buff->Rx,SIZE_BUFF);
                     if(read(_cpsfd,buff->Rx,SIZE_BUFF)>0)
                     {      
                            printf("\nW: ## -Start of communication- ##\n");
                            printf("W: Address [%s] on port %d :\n",inet_ntoa(_clientAddr.sin_addr), ntohs(_clientAddr.sin_port));
                            printf("W: Request:%s\n",buff->Rx);
                            buff->R_flag=1;
                            printf("W: ## - End of communication - ##\n\n");
                     }
                     else
                            printf("W: Nothing was received.\n\n");
                     
                     
                     
                     close(_cpsfd);
              }
       }
 
       return NULL;
}

int Game::setPlayer(int no, char * name, char * IPaddr, int port)
{
       if(no<NB_MAX_PLAYERS)
       {
              m_players[no].no=no;
              strcpy(m_players[no].name,name);
              strcpy(m_players[no].IPaddress,IPaddr);
              m_players[no].portNo=port;
              return 1;
       }
       else 
              return -1;
}

int Game::processBuffer()
{
       if(m_buffer.R_flag==1)//double check if new stuff is to read 
       {
              printf("G: Buffer: %s \n",m_buffer.Rx);
              switch(atoi(strchr(m_buffer.Rx,'G')+1))
              {
                     case 0:
                            printf("G: Nothing to do \n");
                     break;
                     case 99:
                     {
                            printf("G: You've been banned from the game'\n");
                            m_state=MAIN_MENU;
                            createMenu();
                                
                     break;
                     
                     case 11:
                            printf("G: buff: %s \n",m_buffer.Rx);
                            
                            m_myNo=atoi(strchr(m_buffer.Rx,'H')+1);
                            printf("G: my no:%d\n",m_myNo);
                            
                            int no        = atoi(strchr(m_buffer.Rx,'U')+1);
                            printf("G: no:%d\n",no);
                            char *IP      = strtok((strchr(m_buffer.Rx,'I')+1),";");
                            printf("G: IP:%s\n",IP);
                            char *name    = strtok((strchr(m_buffer.Rx,'N')+1),";");
                            printf("G: name:%s\n",name);
                            int port      = atoi(strchr(m_buffer.Rx,'P')+1);
                            printf("G: port:%d\n",port);
                            setPlayer(no, name, IP, port);
                            
                            
                            
                            m_nbPlayer= no+1 ;
                            
                            createMenu();
                     }
                     break;
                     
                     case 20:
                     {
                            printf("G: get my card\n");
                            m_state=GAME_ONLINE;
                            char *ptr=strchr(m_buffer.Rx,'C')+1;
                            for(int i=0;i<12/m_nbPlayer;i++)
                            {
                                   printf("G: %d / i=%d / %d\n",m_myNo,i,atoi(ptr));
                                   m_players[m_myNo].card[i]=atoi(ptr);
                                   printf("G: %d\n",m_players[m_myNo].card[i]);
                                   ptr=strchr(m_buffer.Rx,';')+1;
                            }
                            m_turn=-1;
                     
                     }  
                     break;
                     
                     case 21:
                            m_turn=m_myNo;
                            printf("G: My turn\n");//my turn
                            
                                   
                     break;
                     
                     case 22:
                            //my turn
                     break;
                     
                     case 23:
                            //my turn
                     break;
              }
              m_buffer.R_flag=0;
       }
}


void Game::shareObj()
{
       int obj,objArray[13],nbo=1;
       
       obj=rand()%13;
       objArray[0]=obj;
       
       while(nbo<13)
       {
              obj=rand()%13;
              int i=0;
              while(i<nbo)
              {
                     if(objArray[i]==obj)
                            break;
                     i++;
                     if(i==nbo)
                     {
                            objArray[nbo++]=obj;
                            break;
                     }
              }
       }
       
       m_culprit=objArray[12];
       
       for(int i=0;i<m_nbPlayer;i++)
              for(int j=0;j<12/m_nbPlayer;j++)
                     m_players[i].card[j]=objArray[i*12/m_nbPlayer+j];
                     
       for(int i=0;i<m_nbPlayer;i++)
              for(int j=0;j<12/m_nbPlayer;j++)
                     printf("P%d:%d\n",i,m_players[i].card[j]);
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
       }*/






