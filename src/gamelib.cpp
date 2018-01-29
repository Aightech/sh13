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

#define SERVERPORT 4000
#define WATCHPORT 4001


using namespace sf;
using namespace std;


Game::Game()
{
       time = clock();
       
       m_state=0;
       
       m_turn=1;
       
       window.create(VideoMode(WIN_W,WIN_H), GAME_NAME);   
}

void Game::init()
{   
       m_buffer.R_port=WATCHPORT;
       
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
       
       
       m_state=0;
}

void Game::menu()
{
       createMenu();
       
       
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
              
              processBuffer();
              
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
                                   sprintf(m_buffer.Tx,"G%dP%dN%s",1,m_buffer.R_port,"aight;");
                                   m_buffer.T_flag=1;
                                   
                                   sendTCP(m_servers[0].IPaddress,m_servers[0].portNo,&m_buffer);       
                                   
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
                                   m_state=CREATION_MENU;
                                   m_host=0;
                                   createMenu();
                            break;
                            
                            case HOST_BUTT:
                                   m_state=CREATION_MENU;
                                   m_host=1;
                                   m_nbPlayer=1;
                                   strcpy(m_players[0].name,"mwa");
                                   strcpy(m_players[0].IPaddress,"127.0.0.1");
                                   m_players[0].portNo=m_buffer.R_port;
                                   
                                   if(!fork())
                                   {
                                          startServer();
                                          exit(0);
                                   }
                                   createMenu();
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
                     string buttonsLabel[]={"host game","back"};
                     int buttonIndex[]={HOST_BUTT,JOIN_BUTT,BACK_BUTT};
                     for(int i=0;i<2;i++)
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
                            sprintf(port,"%d",m_players[i].portNo);
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
                            case 1:
                                   
                                   char *name=strtok((strchr(serv_buff.Rx,'N')+1),";");    //get the name of the player
                                   int port = atoi(strchr(serv_buff.Rx,'P')+1);            //get the port of the player
                                   
                                   printf("S: %s want to join the game.\n",name);
                                   
                                   if(m_nbPlayer<NB_MAX_PLAYERS)
                                   {
                                          setPlayer(m_nbPlayer,name,inet_ntoa(serv_clientAddr[0].sin_addr),port);
                                          
                                          sprintf(serv_buff.Tx,"G%d",1);
                                          write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                          printf("S: replied : %s \n",serv_buff.Tx);
                                          
                                          //strcpy(serv_clientIPAddr[m_nbPlayer],inet_ntoa(serv_clientAddr[0].sin_addr));
                                          //serv_clientPortNo[m_nbPlayer]=ntohs(serv_clientAddr[0].sin_port);
                                          m_nbPlayer++;
                                          
                                          for(int i=0;i<m_nbPlayer;i++)
                                          {
                                                 for(int j=0;j<m_nbPlayer;j++)
                                                 {
                                                        printf("S: send to player %d: info player %d \n",i,j);
                                                        sprintf(serv_buff.Tx,"G%dP%dU%dN%s;I%s;",11,m_players[j].portNo,j,m_players[j].name,m_players[j].IPaddress);
                                                        serv_buff.T_flag=1;
                                                        sendTCP(m_players[j].IPaddress, m_players[i].portNo, &serv_buff);
                                                 }
                                          }
                                          
                                   }
                                   else
                                   {
                                          strcpy(serv_buff.Tx,"G-1");
                                          write(serv_cpsfd,serv_buff.Tx,strlen(serv_buff.Tx));
                                          printf("S: replied : %s \n",serv_buff.Tx);
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
              switch(atoi(strchr(m_buffer.Rx,'G')+1))
              {
                     case 0:
                            printf("G: Nothing to do \n");
                     break;
                     case 11:
                     
                            printf("G: buff: %s \n",m_buffer.Rx);
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
                                
                     break;
              }
              m_buffer.R_flag=0;
       }
}
