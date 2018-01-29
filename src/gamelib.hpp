#ifndef GAME_LIB_H
#define GAME_LIB_H

#define MAIN_MENU 0
#define OPTION_MENU 1
#define LAN_MENU 2
#define CREATION_MENU 3
#define GAME_ONLINE 4
#define MAIN_GAME 5

#define INQUIRY_GAME 6
#define DENOUNCE_GAME 7
#define GENPOLL_GAME 8

#define GAME_NAME "SHERLOCK 13"
#define WIN_W 1200
#define WIN_H 700

#define GAME_FONT_TITLE "media/fonts/SHERLOCKED.ttf"


#define GAME_FONT_BUTTON "media/fonts/SHERLOCKED.ttf"
#define GAME_THEME_BUTTON "media/theme/button.png"
#define GAME_THEME_ARRAY "media/theme/arrayConnection.png"

#define LANPORT 2027

#define PLAY_BUTT 1
#define LAN_BUTT 2
#define OPTION_BUTT 3
#define QUIT_BUTT 4
#define SAVE_BUTT 5
#define BACK_BUTT 6
#define HOST_BUTT 7
#define JOIN_BUTT 8
#define GENPOLL_BUTT 9
#define INQUIRY_BUTT 10
#define DENOUNCE_BUTT 11
#define JEWELRY_BUTT 12
#define BULB_BUTT 13
#define BOOK_BUTT 14
#define MEDAL_BUTT 15
#define SKULL_BUTT 16
#define EYE_BUTT 17
#define PIPE_BUTT 18
#define FIST_BUTT 19
#define START_BUTT 20

#define KILL_P1_BUTT 21
#define KILL_P2_BUTT 22
#define KILL_P3_BUTT 23
#define KILL_P4_BUTT 24

#define JOIN_S1_BUTT 31
#define JOIN_S2_BUTT 32
#define JOIN_S3_BUTT 33
#define JOIN_S4_BUTT 34

#define _BUTT 20

#define NB_OBJ 8




#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/wait.h>

#include <netdb.h>
#include <arpa/inet.h>

using namespace sf;
using namespace std;

typedef struct _Player {

	int no;
	char name[16];
	char IPaddress[16];
	int  portNo;
	
	int obj[NB_OBJ];
	int card[3];
	
}Player;



class Game
{
       public:
       Game();
       ~Game(){};
       void init();
       void createMenu();
       void menu();
       int getState(){return m_state;};
       void onlineGame();
       void createGameContext();
       int isMyTurn(){return m_turn;};
       int isHost(){return m_host;};
       
       void startServer();
       
       private:
       float time;
       int m_state;
       int LANport;
       int m_turn;
       
       int m_host;
       
       int serv_portNo;
       int serv_clientPortNo[4];
       char serv_clientIPAddr[4][16];
       int serv_sfd;
       int serv_cpsfd;
       struct sockaddr_in serv_addr;
       struct sockaddr_in serv_clientAddr[4];
       socklen_t serv_clilen;
       
       
       Buffer serv_buff;
       int portNo;
       
       Buffer m_buffer;
       int processBuffer();
       static void * tcpWatchdog(void * p_data);
       pthread_t m_thread_server;
       
       Server m_servers[5];
       int m_nbOfServers;
       
       
       
       int m_nbPlayer;
       Texture m_arrayConnectionTexture;
       vector<Sprite*> m_arraySprites;
       vector<Text*> m_arrayText;
       
       Player m_players;
       
       char m_arrayPlayersNames[4][20];
       char m_arrayPlayersIPaddr[4][20];
       int m_arrayPlayersPort[4];
       
       
       RenderWindow window;
       Text m_title;
       Font m_font;
       Texture m_buttonTexture;
       Button button;
       
       
       vector<Button *> buttons;
       
       Vector2i msPos; 
       
       vector<Texture*> persoTextures;
       vector<Sprite*> persoSprites; 
       vector<Sprite*> playerPlateSprites; 
       vector<Text*> labelPlayerName;
       vector<Text*> labelPlayerIP;
       
       vector<Texture*> iconsTextures;
       vector<Sprite*> iconsSprites; 
       
       Font font;
        
       
};




#endif
