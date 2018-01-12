#ifndef GUI8LIB_H
#define GUI8LIB_H


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace sf;


class Button
{
       public:
       Button(){s_nb++;};
       Button(std::string s, sf::Vector2f position,int nb = 0);
       void create(std::string s, sf::Vector2f position,int nb = 0);
       ~Button(){s_nb--;};//std::cout<<"deleted"<<std::endl;};
       
       void setTexture(const std::string &filename)
       {s_texture.loadFromFile(filename);};
       void setFont(const std::string &filename)
       {s_font.loadFromFile(filename);};
       
       int update(RenderWindow &w);
       int clicked(){return m_state;};
       
       int getNbOfBt(){return s_nb-1;};
       
       void disable(){m_state=-1;label.setColor(Color(155,155,155));};
       void enable(){m_state=0;label.setColor(Color(255,255,255));};
      
       
       private:
       static int s_nb;
       static Texture s_texture;
       static Font s_font;
       Vector2i msPos;
       Vector2f btPos;
       FloatRect btSize;
       
       int m_nb;
       
       Text label;
       Sprite button;
       
       int m_state;
       int msInButton;
       int click;
       
};

class Textbox
{
       public:
       Textbox(){s_nb++;};
       Textbox(std::string *s, sf::Vector2f position,int nb = 0);
       void create(std::string *s, sf::Vector2f position,int nb = 0);
       ~Textbox(){s_nb--;};//std::cout<<"deleted"<<std::endl;};
       
       void setFont(const std::string &filename)
       {s_font.loadFromFile(filename);};
       
       int update(RenderWindow &w);
       int clicked(){return m_state;};
       
       int getNbOfBx(){return s_nb-1;};
      
       
       private:
       static int s_nb;
       //static Texture s_texture;
       static Font s_font;
       Vector2i mxPos;
       Vector2f bxPos;
       FloatRect bxSize;
       
       int m_nb;
       
       std::string *text;
       Text label;
       RectangleShape box;
       
       int m_state;
       int msInBox;
       int click;
       
};


#endif
