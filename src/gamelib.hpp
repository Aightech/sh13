#ifndef GAME_LIB_H
#define GAME_LIB_H

#define MAIN_MENU 0
#define OPTION_MENU 1
#define LAN_MENU 2
#define GAME_ONLINE 3
#define MAIN_GAME 4
#define GENPOLL_GAME 5
#define INQUIRY_GAME 6
#define DENOUNCE_GAME 7

#define GAME_NAME "SHERLOCK 13"
#define WIN_W 1200
#define WIN_H 700

#define GAME_FONT_TITLE "media/fonts/SHERLOCKED.ttf"


#define GAME_FONT_BUTTON "media/fonts/SHERLOCKED.ttf"
#define GAME_THEME_BUTTON "media/theme/button.png"

#define LANPORT 1027

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
#define _BUTT 20

#define NB_OBJ 8

#define SIZE_BUFF 256

using namespace sf;
using namespace std;

typedef struct _Player {

	int no;
	char name[16];
	char IPaddress[16];
	int  portNo;
	
	int obj[NB_OBJ];
	int card[3];
	
}PLAYER;

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
       
       char serv_RxBuff[SIZE_BUFF];
       char serv_TxBuff[SIZE_BUFF];
       int serv_portNo;
       int serv_clientPortNo[4];
       char serv_clientIPAddr[4][16];
       int serv_sfd;
       int serv_cpsfd;
       struct sockaddr_in serv_addr;
       struct sockaddr_in serv_clientAddr;
       
       char RxBuff[SIZE_BUFF];
       char TxBuff[SIZE_BUFF];
       int portNo;
       
       
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
