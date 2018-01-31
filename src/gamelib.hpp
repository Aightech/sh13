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
#define INIT_GAME 9
#define INQUIRY_GAME2 10

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
#define REFRESH_BUTT 25

#define KILL_P1_BUTT 21
#define KILL_P2_BUTT 22
#define KILL_P3_BUTT 23
#define KILL_P4_BUTT 24

#define JOIN_S1_BUTT 31
#define JOIN_S2_BUTT 32
#define JOIN_S3_BUTT 33
#define JOIN_S4_BUTT 34

#define ASK1_BUTT 31
#define ASK2_BUTT 32
#define ASK3_BUTT 33
#define ASK4_BUTT 34

#define DENOUNCE1_BUTT 21
#define DENOUNCE2_BUTT 22
#define DENOUNCE3_BUTT 23
#define DENOUNCE4_BUTT 24
#define DENOUNCE5_BUTT 25
#define DENOUNCE6_BUTT 26
#define DENOUNCE7_BUTT 27
#define DENOUNCE8_BUTT 28
#define DENOUNCE9_BUTT 29
#define DENOUNCE10_BUTT 30



#define NB_OBJ 8

#define NB_MAX_PLAYERS 4

#define SERVERPORT 4000
//#define WATCHPORT 4001






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
	int card[6];
	
}Player;





class Game
{
       /*! \class Game
       * \brief This class represent the game application.
       *      It owns the different methods running the game.
       */
       public:
       
       /*! \brief Constructor */
       Game();
       
       /*! \brief Destructor */
       ~Game(){};
       
       /*! \brief Initialization methode. Some parametere are initialize inside */
       void init();
       
       /*! \brief Process the 'menu' state of the game. It's mainly a event loop */
       void menu();
       
       /*! \brief Accessor method to get the state of the game from the main loop.   */
       int getState(){return m_state;};
       
       /*! \brief launch an online game. The game have to be setted (through menu() )*/
       void onlineGame();
       
       /*! \brief Accessor method to get the turn of the game from the main loop.   */
       int isMyTurn(){return m_turn;};
       
       /*! \brief Accessor method to get the host of the game from the main loop.   */
       int isHost(){return m_host;};
       
       void shareObj();
       
       
       private:
       /*! \brief Generate the graphics of the different menu's states of the game. It's mainly SFML sprites/Text/Buttons creation   */
       void createMenu();
       
       /*! \brief Generate the graphics of the different game's states of the game. It's mainly SFML sprites/Text/Buttons creation   */
       void createGameContext();
       
       /*! \brief Launch a sh13 server.   */
       void startServer();
       
       //server related attributes
       int serv_portNo;            /*!< Liste des morceaux*/
       int serv_clientPortNo[4];
       char serv_clientIPAddr[4][16];
       int serv_sfd;
       int serv_cpsfd;
       struct sockaddr_in serv_addr;
       struct sockaddr_in serv_clientAddr[4];
       socklen_t serv_clilen;
       Buffer serv_buff;
       
       //general attributes
       float time;
       int m_state;
       int m_turn;
       int m_myNo;
       int m_host;
       int m_culprit;
       int m_charactersArray[13][3];
       int m_obj;
       int m_otherCharacters[10];
       
       //TCP related attributes
       Buffer m_buffer;
       
       /*! \brief Process a buffer filled by the tcp thread.   */
       int processBuffer();
       pthread_t m_thread_server;
       
       /*! \brief Process the different connection to the client and fill up a shared buffer with incomming data.   
        *  \param p_data : the shared buffer.*/
       static void * tcpWatchdog(void * p_data);
       
       //servers related attributes
       Server m_servers[5];
       int m_server;
       int m_nbOfServers;
       
       //Players related attributes
       int m_nbPlayer;
       Player m_players[4];
       
       
       
       /*! \brief Set new data in the players array.   
        *  \param no : the numero of the player.
        *  \param name : the name of the player.
        *  \param IPaddr : the IP address of the player.
        *  \param port : the port of the player.
        */
       int setPlayer(int no, char * name, char * IPaddr, int port);
       
       
       //Graphics related attributes
       Texture m_arrayConnectionTexture;
       vector<Sprite*> m_arraySprites;
       vector<Text*> m_arrayText;
       
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
       
       vector<Texture*> m_persoTextures;
       vector<Sprite*> m_persoSprites; 
       
       Texture m_woodTexture;
       vector<Sprite*> m_playerPlateSprites; 
       vector<Text*> m_labelPlayerName;
       vector<Text*> m_labelPlayerIP;
       vector<Text*> m_playersObjects;
       
       vector<Texture*> m_iconsTextures;
       vector<Sprite*> m_iconsSprites; 
       
       Font font;
        
       
};




#endif
