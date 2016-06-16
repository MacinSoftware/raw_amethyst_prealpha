#include "C:/Dev-Cpp_Lib/SDL/include/SDL.h"
#include "C:/Dev-Cpp_Lib/SDL/include/SDL_opengl.h"

bool key[321];
bool events()
{
SDL_Event event;
if( SDL_PollEvent(&event) )
{
switch( event.type )
{
   case SDL_KEYDOWN : key[ event.key.keysym.sym ]=true ;   break;
   case SDL_KEYUP   : key[ event.key.keysym.sym ]=false;   break;
   case SDL_QUIT    : return false; break;
}
}
return true;
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define pi 3.14159265

GLUtesselator *tobj = gluNewTess();
int keyPressed[257],keyRelease[257];
int xscr=1440,yscr=900;
GLuint pismo,grafika,texty;
#include "texture.h"

unsigned int pocet_levelu=8,xlevelu=30,ylevelu=20;
unsigned int current_level=0,score=0,time_left=90,diamonds_left;
int release_time=10;
int casovac_prodleva_preset=60,casovac_prodleva_accum=casovac_prodleva_preset;
int casovac_padani_preset=5,casovac_padani_accum=casovac_padani_preset;
int newlevel_timer=60;
int active_menu=0;
unsigned int pohyb=0;
unsigned int player_position[2];

short unsigned int time_limit[8]={30,20,30,80,70,40,120,100};
short unsigned int levely[8][20][30]=
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,6,1,
	1,5,2,2,2,2,2,5,2,2,2,2,2,0,0,0,0,0,2,4,2,2,2,2,2,5,2,2,2,1,
	1,0,2,2,2,2,2,2,2,2,2,0,2,1,2,2,2,1,2,0,2,2,2,2,2,2,2,2,5,1,
	1,0,2,4,4,4,2,2,2,2,2,0,2,1,4,4,4,1,2,0,2,2,2,0,0,0,0,2,5,1,
	2,0,2,5,4,4,2,2,2,2,2,0,2,1,4,4,4,1,2,0,2,4,2,0,0,0,0,2,2,1,
	2,0,2,2,2,2,2,4,2,2,2,0,2,1,4,4,4,1,2,0,4,2,2,0,0,0,0,2,2,1,
	2,0,2,2,2,2,2,2,2,2,2,0,2,1,5,5,5,1,2,0,2,2,2,2,2,2,2,2,2,1,
	1,4,4,2,2,4,2,2,2,4,2,2,5,1,2,2,2,1,2,0,2,2,2,2,2,2,2,2,2,1,
	1,4,2,2,2,2,2,2,2,2,2,4,2,1,2,2,2,0,2,0,2,2,2,2,2,2,2,2,2,1,
	1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
	1,2,2,2,2,2,2,2,8,2,2,2,2,1,5,2,2,1,2,0,2,2,2,2,2,2,2,2,2,1,
	1,2,2,0,0,2,2,2,2,2,2,2,2,1,2,2,2,1,4,0,2,2,2,5,5,5,2,4,2,1,
	2,2,2,2,0,0,2,2,2,2,2,2,2,1,2,2,2,0,4,0,2,2,4,5,5,5,2,4,2,1,
	2,2,2,2,2,0,0,2,2,4,4,4,2,1,2,4,4,1,2,0,2,2,2,2,4,2,2,2,2,1,
	2,2,2,2,2,2,0,0,2,2,2,2,2,1,2,2,2,1,2,0,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,4,2,2,0,0,2,2,2,2,1,4,4,2,1,2,0,2,2,2,2,2,2,2,4,2,1,
	1,2,2,2,2,2,2,2,0,0,2,2,2,1,2,2,2,1,2,0,2,4,2,2,5,4,2,2,4,1,
	1,4,4,2,2,2,2,2,2,0,0,2,2,1,2,2,2,1,2,0,2,2,2,2,2,2,2,2,2,1,
	1,5,2,2,4,2,2,2,2,2,2,2,2,0,2,2,2,1,2,0,2,2,2,2,2,2,5,2,3,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,

	1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,
	1,5,2,0,1,1,1,1,1,0,0,4,4,4,0,0,0,2,2,2,8,2,2,5,0,0,0,0,4,1,
	1,2,2,0,0,0,0,0,0,0,0,2,5,2,0,0,0,2,2,2,2,2,2,5,0,0,0,0,4,1,
	1,0,0,0,0,1,1,1,1,1,0,1,1,1,0,1,1,2,1,1,1,1,2,5,0,0,0,0,4,1,
	1,1,1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,6,6,6,1,2,5,0,0,0,0,4,1,
	1,2,0,2,1,2,4,2,0,0,0,0,2,0,0,0,0,2,2,2,6,1,2,5,0,0,0,0,4,1,
	1,8,2,0,1,0,2,0,0,0,0,2,2,2,0,0,0,2,2,2,6,1,2,5,0,0,0,0,4,1,
	1,2,0,2,0,2,0,2,0,0,0,0,2,0,0,0,0,2,6,6,6,1,2,5,0,0,0,0,4,1,
	1,0,2,0,1,0,2,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,5,0,0,0,0,4,1,
	1,2,0,2,1,2,4,2,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,5,0,0,0,0,4,1,
	1,1,1,1,1,0,2,0,0,2,0,0,0,0,0,1,1,2,2,2,2,2,2,5,0,0,0,0,4,1,
	1,5,2,0,0,0,0,0,2,5,2,0,0,0,0,1,0,2,2,2,2,2,2,5,0,0,0,0,4,1,
	1,2,0,0,0,0,0,0,0,2,0,0,0,1,1,1,0,2,2,2,2,2,2,5,0,0,0,0,4,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,2,2,2,3,2,2,5,0,0,0,0,4,1,
	1,4,2,4,0,0,2,2,0,0,0,0,0,1,0,0,0,2,2,2,2,2,2,5,0,0,0,0,4,1,
	1,2,4,2,4,0,0,2,0,0,1,1,1,1,0,0,0,2,2,2,2,2,2,5,0,0,0,0,4,1,
	1,4,2,4,2,4,0,2,0,0,1,0,0,0,0,0,0,2,2,2,2,2,2,5,0,0,0,0,4,1,
	1,2,4,2,4,2,0,0,0,0,1,0,0,0,0,0,0,2,2,1,1,1,2,5,0,0,0,0,4,1,
	1,4,8,4,2,4,0,0,5,0,0,0,0,0,0,0,0,2,2,1,0,1,2,5,0,0,0,0,4,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,

	0,0,0,0,0,0,0,0,0,2,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,0,0,0,1,0,0,0,0,4,4,0,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,1,8,1,6,1,0,0,0,2,0,0,0,1,3,1,1,0,0,0,0,0,0,0,1,6,1,8,2,0,
	0,1,2,1,0,1,0,0,0,0,0,0,1,2,2,2,2,1,0,0,0,0,0,0,1,0,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,1,2,2,4,4,2,2,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,2,2,4,5,4,2,2,4,1,2,2,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,4,2,2,4,2,4,4,4,4,5,1,5,2,0,0,0,2,1,2,0,
	0,4,2,4,0,0,0,0,1,4,4,2,2,2,2,4,4,4,4,4,4,1,2,0,0,0,0,0,0,0,
	0,2,5,2,0,0,0,1,4,4,4,2,4,4,2,4,4,4,5,4,4,4,1,0,0,0,0,0,0,0,
	0,2,2,2,0,0,1,4,4,5,4,2,4,5,2,2,2,2,2,2,2,2,2,1,0,0,0,0,4,4,
	0,0,0,0,0,1,2,2,2,4,4,2,2,2,2,2,2,0,0,0,0,0,0,0,1,0,0,0,2,5,
	0,0,0,0,0,0,1,2,2,2,2,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,2,2,
	0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,2,1,0,0,1,0,0,0,0,0,1,1,0,0,1,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,2,2,2,1,0,0,0,1,0,1,0,0,0,0,1,2,2,2,0,0,0,0,0,0,
	0,0,0,0,0,2,2,2,0,0,1,0,0,0,0,0,1,0,0,1,0,0,2,2,2,0,0,0,0,0,
	0,1,1,1,0,1,2,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,2,1,0,1,2,1,0,
	0,2,8,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,1,8,1,0,
	0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

	1,1,1,1,1,1,1,1,1,1,1,4,4,4,4,4,4,4,4,1,1,1,1,1,1,1,1,1,1,1,
	1,3,1,0,1,0,1,0,1,0,1,4,4,4,4,4,4,4,4,1,5,1,2,2,2,2,2,4,5,1,
	1,2,1,2,1,2,1,2,1,2,1,4,4,4,4,4,4,4,4,1,4,1,2,2,2,2,2,4,4,1,
	1,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,4,4,4,2,2,2,1,
	1,2,2,2,2,5,2,2,2,2,1,0,0,0,0,0,0,0,0,1,2,1,2,4,5,4,2,2,5,1,
	1,2,2,4,2,2,2,2,4,2,1,0,0,0,0,0,0,0,0,1,2,1,2,4,4,4,2,5,2,1,
	1,2,2,5,4,2,2,2,4,5,1,0,0,0,0,0,0,0,0,1,2,1,2,2,2,2,2,2,2,1,
	1,2,2,2,2,4,2,2,2,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,
	1,2,2,2,2,2,4,2,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,
	1,4,2,2,2,2,5,1,0,0,0,1,1,1,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,
	1,2,2,2,5,2,1,0,0,0,0,1,6,1,2,2,0,0,0,1,0,1,0,1,0,1,0,1,0,1,
	1,0,1,1,1,1,0,0,0,0,0,1,2,1,2,1,0,0,0,1,2,2,2,2,2,2,2,2,2,1,
	1,2,2,4,4,2,1,0,0,0,0,1,2,2,2,1,0,0,0,1,2,1,2,1,4,1,2,1,2,1,
	1,2,2,4,4,2,2,1,0,0,0,0,1,1,1,1,0,0,0,1,2,2,2,2,4,2,2,2,4,1,
	1,2,2,2,4,2,2,2,1,0,0,0,0,0,0,0,0,0,0,1,2,1,2,1,4,1,2,1,5,1,
	1,2,2,2,2,2,2,4,4,1,0,0,0,0,0,0,0,0,0,1,2,2,2,2,4,2,2,2,5,1,
	1,0,1,5,2,2,2,4,4,4,1,0,0,0,0,0,0,0,0,1,2,1,4,1,4,1,4,1,5,1,
	1,0,1,1,2,2,2,2,4,4,1,0,0,0,0,0,0,0,0,1,2,1,5,1,5,1,4,1,5,1,
	1,0,0,0,2,4,2,2,2,5,1,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,2,2,1,
	1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,5,1,1,1,1,1,1,1,1,1,1,1,		

	0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,0,0,0,0,0,1,4,0,1,1,4,1,4,4,4,1,1,0,0,0,0,0,
	0,0,0,0,1,1,4,1,0,0,0,0,0,1,1,0,1,1,0,1,4,5,4,1,1,1,0,0,0,0,
	0,0,0,1,1,4,4,1,0,0,1,0,0,1,1,5,1,1,0,1,4,4,4,4,4,4,1,0,0,0,
	0,0,1,1,5,4,4,1,0,0,0,0,0,1,1,2,1,1,0,1,4,4,4,5,4,4,1,1,0,0,
	0,1,1,4,4,4,4,1,0,0,0,0,0,1,1,2,1,1,0,1,1,4,4,4,4,4,1,1,1,0,
	1,1,4,4,4,4,4,1,1,1,1,1,1,1,1,2,1,1,0,1,1,4,4,4,4,4,1,1,1,1,
	1,5,2,2,2,2,2,2,4,1,6,6,6,1,2,2,2,2,0,2,2,1,2,2,2,2,4,4,5,1,
	1,2,2,2,2,2,2,2,4,4,4,2,2,1,2,2,2,2,0,5,2,1,2,2,2,2,4,4,4,1,
	1,2,2,2,2,2,2,2,4,4,4,2,2,1,2,2,2,1,0,2,3,1,2,2,2,2,4,4,4,1,
	1,2,4,4,4,4,4,4,4,4,4,2,4,1,1,1,1,1,0,4,1,1,2,2,2,2,2,2,2,1,
	1,2,4,4,4,4,4,4,4,4,4,2,2,5,4,2,2,2,0,2,2,2,2,2,2,2,2,2,2,1,
	1,2,1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,1,
	1,1,5,5,1,5,1,5,1,1,1,1,1,1,1,1,1,1,0,1,1,1,2,1,2,1,1,1,1,1,
	0,1,1,5,4,5,1,5,1,1,4,4,4,4,2,2,1,1,0,1,1,1,0,1,0,1,1,1,1,0,
	0,0,1,1,5,5,5,5,1,1,2,2,5,4,2,2,5,1,0,1,1,1,0,1,0,1,1,1,0,0,
	0,0,0,1,1,2,2,2,1,1,2,2,1,1,2,2,1,1,0,1,2,2,2,2,2,1,1,0,0,0,
	0,0,0,0,1,1,4,4,0,4,2,2,2,2,2,2,2,2,0,2,2,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,4,1,1,2,1,1,1,1,1,1,1,5,1,2,2,2,8,1,0,0,0,0,0,
	4,0,0,0,0,0,1,1,1,1,2,2,2,5,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,4,

 	1,0,1,5,1,0,1,0,1,4,1,0,1,0,1,0,0,1,0,1,4,1,0,1,0,1,0,1,6,1,   
	1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,1,
	1,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,8,8,4,1,
	1,2,1,4,1,2,1,2,1,2,1,0,1,2,1,4,4,1,2,1,2,1,2,1,2,1,2,1,5,1,
	1,2,2,4,2,2,2,2,2,2,2,0,2,2,2,4,4,2,2,2,2,2,2,2,2,2,2,2,4,1,
	1,2,1,4,1,2,1,2,1,2,1,0,1,2,1,2,2,1,0,1,2,1,0,1,2,1,2,1,4,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,5,2,0,2,2,2,0,2,0,2,2,2,2,1,
	1,2,1,0,1,0,1,0,1,0,1,2,1,2,1,5,2,1,0,1,2,1,0,1,0,1,0,1,0,1,
	1,2,2,0,2,2,2,2,2,0,2,2,2,2,2,2,5,2,2,2,2,2,0,2,4,2,2,2,2,1,
	1,2,1,0,1,0,1,0,1,0,1,0,1,2,1,2,2,1,2,1,2,1,0,1,4,1,0,1,4,1,
	1,2,2,2,2,2,2,5,2,0,2,0,2,2,2,2,2,2,2,2,2,2,0,2,4,2,4,2,4,1,
	1,2,1,2,1,2,1,4,1,0,1,1,1,5,1,2,2,1,5,1,2,1,2,1,2,1,5,1,5,1,
	1,2,2,2,2,2,2,4,2,2,1,5,2,4,2,2,5,5,5,5,5,2,2,2,2,2,4,2,4,1,
	1,2,1,2,1,2,1,4,1,2,1,1,1,4,1,2,2,1,5,1,2,1,2,1,2,1,0,1,4,1,
	1,2,2,2,2,2,2,4,2,2,2,0,1,4,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,1,2,1,2,1,4,1,2,1,2,1,4,1,2,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
	1,2,2,2,2,2,2,4,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,4,4,4,2,2,2,1,
	1,2,1,2,1,2,1,5,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,4,1,2,1,2,1,
	1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,3,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,  		
	
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,4,0,0,0,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,0,0,0,5,1,
	1,0,4,0,0,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,0,0,4,0,1,
	1,0,0,4,0,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,0,0,0,0,1,
	1,0,0,0,4,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,0,0,0,4,1,
	1,0,0,0,0,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,0,0,0,0,1,
	1,0,0,0,0,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,0,0,0,0,1,
	1,4,0,0,0,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,5,0,0,0,1,
	1,0,4,0,0,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,1,1,0,0,1,
	1,0,0,4,0,1,4,4,4,4,5,5,4,4,4,5,5,4,4,4,5,5,4,4,1,3,0,0,0,1,
	1,0,0,0,4,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,0,0,1,
	1,0,0,0,0,4,0,1,1,2,2,0,0,0,0,0,0,0,0,0,0,0,1,6,1,6,1,0,0,1,
	1,0,0,0,0,0,4,0,1,2,2,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,1,
	1,0,0,0,0,0,0,4,1,1,1,0,0,0,2,0,2,0,0,0,0,0,2,2,2,2,2,0,0,1,
	1,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,1,
	1,0,0,2,2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,2,2,2,2,2,0,0,1,
	1,0,0,2,2,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,2,5,2,5,2,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,2,2,2,2,2,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,
	0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,0,
	0,1,6,2,2,2,2,8,8,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6,2,6,1,0,
	0,1,1,2,2,2,2,8,8,1,0,0,0,0,0,2,2,2,0,0,0,0,0,0,1,1,2,1,1,0,
	0,0,1,2,2,2,2,2,2,1,0,0,0,0,0,2,4,2,0,0,0,0,0,0,0,0,2,0,0,0,
	0,0,1,2,2,2,2,2,2,1,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,8,8,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,8,8,2,2,2,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,2,2,2,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,4,2,2,0,0,0,0,0,2,2,4,2,2,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,2,2,2,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	4,4,4,5,4,4,4,4,5,4,5,4,4,5,4,4,4,4,5,4,4,4,4,4,4,4,4,4,5,4,
	4,4,4,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,4,4,4,4,4,4,4,5,5,4,
	4,4,4,4,5,4,4,4,4,4,5,4,5,5,4,4,4,4,5,5,4,4,4,4,4,4,4,5,5,4,
	4,4,4,4,5,5,4,4,4,5,5,4,5,5,4,4,5,4,4,4,4,4,5,4,4,4,4,4,4,4,
	4,5,4,4,4,5,4,4,4,4,5,4,4,4,4,4,5,4,4,4,4,4,5,4,4,4,4,4,4,4,
	1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,
	1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,1,
	1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1
};
short unsigned int level_play[20][30];

#ifndef CALLBACK
#define CALLBACK
#endif

void CALLBACK beginCallback(GLenum which)
{
   glBegin(which);
}

void CALLBACK endCallback()
{
   glEnd();
}

void CALLBACK vertexCallback(GLdouble *vertex)
{
   const GLdouble *pointer;
   pointer = (GLdouble *) vertex;
   glVertex3dv(vertex);
}

void LoadLevel()
{
   time_left=time_limit[current_level];
   diamonds_left=0;
   for(int poradi2000=0;poradi2000<xlevelu;poradi2000++)
   {
      for(int poradi2001=0;poradi2001<ylevelu;poradi2001++)
      {
   	     if(levely[current_level][poradi2001][poradi2000]==5){diamonds_left++;}else
   	     if(levely[current_level][poradi2001][poradi2000]==3){player_position[0]=poradi2000;player_position[1]=poradi2001;}
   	     level_play[poradi2001][poradi2000]=levely[current_level][poradi2001][poradi2000];
      }
   }
}

void init()
{
   for(int poradi702=0;poradi702<257;poradi702++){keyRelease[poradi702]=0;}
   tobj = gluNewTess();
   gluTessCallback(tobj, GLU_TESS_VERTEX,(void (CALLBACK *)())vertexCallback);
   gluTessCallback(tobj, GLU_TESS_BEGIN,(void (CALLBACK *)())beginCallback); 
   gluTessCallback(tobj, GLU_TESS_END, endCallback);
   glClearColor(0.0f ,0.0f ,0.0f ,0.0f);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60,(GLfloat)xscr/(GLfloat)yscr,0.01,100.0f);
   glMatrixMode(GL_MODELVIEW);
   LoadLevel();
}

void sleep( clock_t wait )
{
   clock_t goal;
   goal = wait + clock();
   while( goal > clock() )
   {
   }
}

void Win()
{
	current_level=0;
	LoadLevel();
}

void Lose()
{
	LoadLevel();
	score-=10;
	if(score<0){score=0;}
	active_menu=3;
}

void Tic()
{
   //PADANI OBJEKTU
   casovac_padani_accum--;
   if(casovac_padani_accum==0)
   {
   	  casovac_padani_accum=casovac_padani_preset;
   	  for(int poradi2000=0;poradi2000<xlevelu;poradi2000++)
      {
         for(int poradi2001=ylevelu-2;poradi2001>=0;poradi2001--)
         {
   	        if((level_play[poradi2001][poradi2000]==4)|(level_play[poradi2001][poradi2000]==5)|(level_play[poradi2001][poradi2000]==8))
   	        {
   	        	int typ_objektu2000=level_play[poradi2001][poradi2000];
   	        	bool priznak_padu=0;
   	        	if(level_play[poradi2001+1][poradi2000]==0)
				{
					level_play[poradi2001+1][poradi2000]=typ_objektu2000;
					level_play[poradi2001][poradi2000]=0;
					priznak_padu=1;
				}else
				if(   (level_play[poradi2001+1][poradi2000]==1)   |   (level_play[poradi2001+1][poradi2000]==2)   |   (level_play[poradi2001+1][poradi2000]==3)   |   (level_play[poradi2001+1][poradi2000]==6)   )
				{
					priznak_padu=1;
				}
				
				if(priznak_padu==0)
				{
					if(poradi2000!=0)
					{	
					   if(   (level_play[poradi2001+1][poradi2000-1]==0)   &   (level_play[poradi2001][poradi2000-1]!=1)   &   (level_play[poradi2001][poradi2000-1]!=2)   &   (level_play[poradi2001][poradi2000-1]!=3)   &   (level_play[poradi2001][poradi2000-1]!=6)   )
					   {
					   	  level_play[poradi2001+1][poradi2000-1]=typ_objektu2000;
						  level_play[poradi2001][poradi2000]=0;
						  priznak_padu=1;
				       }
					}
				}
				if(priznak_padu==0)
				{
					if(poradi2000!=xlevelu-1)
					{	
					   if(   (level_play[poradi2001+1][poradi2000+1]==0)   &   (level_play[poradi2001][poradi2000+1]!=1)   &   (level_play[poradi2001][poradi2000+1]!=2)   &   (level_play[poradi2001][poradi2000+1]!=3)   &   (level_play[poradi2001][poradi2000+1]!=6)   )
					   {
					   	  level_play[poradi2001+1][poradi2000+1]=typ_objektu2000;
						  level_play[poradi2001][poradi2000]=0;
						  priznak_padu=1;
				       }
					}
				}
			}
         }
      }
   }


	
   //POHYB HRACE
   bool priznak_pohybu=0;
   unsigned int target[2];
   if((pohyb==1)&(player_position[1]!=0))
   {
   	  target[0]=player_position[0];target[1]=player_position[1]-1;
   	  priznak_pohybu=1;
   }else
   if((pohyb==2)&(player_position[0]!=xlevelu-1))
   {
   	  target[0]=player_position[0]+1;target[1]=player_position[1];
   	  priznak_pohybu=1;
   }else
   if((pohyb==3)&(player_position[1]!=ylevelu-1))
   {
   	  target[0]=player_position[0];target[1]=player_position[1]+1;
   	  priznak_pohybu=1;
   }else
   if((pohyb==4)&(player_position[0]!=0))
   {
   	  target[0]=player_position[0]-1;target[1]=player_position[1];
   	  priznak_pohybu=1;
   }
 if(priznak_pohybu==1)
 {
   
   
   
   if(   ((level_play[target[1]][target[0]])==0)   |   ((level_play[target[1]][target[0]])==2)   )
   {
   	  level_play[player_position[1]][player_position[0]]=0;
   	  level_play[target[1]][target[0]]=3;
   	  player_position[1]=target[1];player_position[0]=target[0];
   }else
   if(   ((level_play[target[1]][target[0]])==5)   )
   {
   	  level_play[player_position[1]][player_position[0]]=0;
   	  level_play[target[1]][target[0]]=3;   
	  player_position[1]=target[1];player_position[0]=target[0];	 
	  diamonds_left--;
	  if(diamonds_left==0)
	  {
	     for(int poradi2000=0;poradi2000<xlevelu;poradi2000++)
         {
            for(int poradi2001=0;poradi2001<ylevelu;poradi2001++)
            {
            	if(level_play[poradi2001][poradi2000]==6){level_play[poradi2001][poradi2000]=7;}
            }
		 }
	  }
	  
   }else
   if(   ((level_play[target[1]][target[0]])==4)   )
   {
   	  if(pohyb==2)
	  {
	  	 if(player_position[0]<xlevelu-2)
		 {
		 	if(level_play[player_position[1]][player_position[0]+2]==0)
			{
				level_play[player_position[1]][player_position[0]+2]=4;
				level_play[player_position[1]][player_position[0]]=0;
   	  			level_play[target[1]][target[0]]=3;   
	  			player_position[1]=target[1];player_position[0]=target[0];
			}
		 }
	  }else
	  if(pohyb==4)
	  {
	  	 if(player_position[0]>=2)
		 {
		 	if(level_play[player_position[1]][player_position[0]-2]==0)
			{
				level_play[player_position[1]][player_position[0]-2]=4;
				level_play[player_position[1]][player_position[0]]=0;
   	  			level_play[target[1]][target[0]]=3;   
	  			player_position[1]=target[1];player_position[0]=target[0];
			}
		 }
	  }
   }else
   if(   ((level_play[target[1]][target[0]])==8)   )
   {
   	  level_play[player_position[1]][player_position[0]]=0;
   	  level_play[target[1]][target[0]]=3; 
	  player_position[1]=target[1];player_position[0]=target[0];  	 
	  time_left+=15; 
   }else
   if(   ((level_play[target[1]][target[0]])==7)   &   (diamonds_left==0)   )
   {
   	 if(current_level<pocet_levelu-1)
   	 {
   	  score+=time_left;
   	  current_level++;
   	  LoadLevel();
   	  active_menu=2;
   	  for(int poradi2002=0;poradi2002<257;poradi2002++){keyRelease[poradi2002]=newlevel_timer;}
     }else
     {
      Win();
	 }
   }
   
   
   
   pohyb=0;
   
   
   
 }
   
   
   //CASOVY LIMIT
   casovac_prodleva_accum--;
   if(casovac_prodleva_accum==0)
   {
   	  casovac_prodleva_accum=casovac_prodleva_preset;
   	  time_left--;
   	  if(time_left==0){Lose();}
   }
}

//NEFUNGUJE PRO ZAPORNE !!!
void text_write(int posx711,int posy711,int velikost711,int cislo_k_zapisu)
{
   glBindTexture( GL_TEXTURE_2D, pismo );
   glBegin(GL_QUADS);  
   int poradi713=4;
   int cislo_mezikrok711=cislo_k_zapisu;
   for(;;)
   {
   	  int kod_znaku=cislo_mezikrok711%10;
   	  glTexCoord2d(((kod_znaku) +1)/10.,1); glVertex3f(posx711+velikost711*xscr/1000.+poradi713*velikost711*xscr/1000.,posy711+velikost711*yscr/1000.,0);  
      glTexCoord2d(((kod_znaku))/10.,1);    glVertex3f(posx711+poradi713*velikost711*xscr/1000.,posy711+velikost711*yscr/1000.,0);   
      glTexCoord2d(((kod_znaku))/10.,0);       glVertex3f(posx711+poradi713*velikost711*xscr/1000.,posy711,0);    
      glTexCoord2d(((kod_znaku) +1)/10.,0);    glVertex3f(posx711+velikost711*xscr/1000.+poradi713*velikost711*xscr/1000.,posy711,0);
      cislo_mezikrok711=cislo_mezikrok711/10;
      if(cislo_mezikrok711==0){break;}else{poradi713--;}
   }
   glEnd();
}

void display3D()
{
   glLoadIdentity();
   glViewport(0,0,xscr,yscr);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0,xscr,0,yscr);
   
   if(current_level==0){glColor3f(1,1,1);}else
   if(current_level==1){glColor3f(0.6,0.6,0);}else
   if(current_level==2){glColor3f(0.6,0.3,0);}else
   if(current_level==3){glColor3f(0.6,0,0);}else
   if(current_level==4){glColor3f(0.3,0.6,0);}else
   if(current_level==5){glColor3f(0,0.6,0);}else
   if(current_level==6){glColor3f(0,0.6,0.6);}else
   if(current_level==7){glColor3f(0,0.3,0.6);}else
   if(current_level==8){glColor3f(0,0,0.6);}else
   if(current_level==9){glColor3f(0.6,0,0.6);}else
   {glColor3f(1,1,1);}
   glEnable( GL_TEXTURE_2D );
   
   glBindTexture( GL_TEXTURE_2D, grafika );
   glBegin(GL_QUADS);
   for(int poradi2000=0;poradi2000<xlevelu;poradi2000++)
   {
      for(int poradi2001=0;poradi2001<ylevelu;poradi2001++)
      {
   	     if(level_play[poradi2001][poradi2000]!=0)
   	     {
   			glTexCoord2d((level_play[poradi2001][poradi2000])/8.,1);	glVertex3f(xscr/10+(poradi2000+1)*(xscr-xscr/5)/xlevelu,yscr/10+(ylevelu-poradi2001+1)*(yscr-yscr/5)/ylevelu,0);  
   			glTexCoord2d((level_play[poradi2001][poradi2000]-1)/8.,1);		glVertex3f(xscr/10+poradi2000*(xscr-xscr/5)/xlevelu,yscr/10+(ylevelu-poradi2001+1)*(yscr-yscr/5)/ylevelu,0);   
   			glTexCoord2d((level_play[poradi2001][poradi2000]-1)/8.,0);		glVertex3f(xscr/10+poradi2000*(xscr-xscr/5)/xlevelu,yscr/10+(ylevelu-poradi2001)*(yscr-yscr/5)/ylevelu,0);    
   			glTexCoord2d((level_play[poradi2001][poradi2000])/8.,0);	glVertex3f(xscr/10+(poradi2000+1)*(xscr-xscr/5)/xlevelu,yscr/10+(ylevelu-poradi2001)*(yscr-yscr/5)/ylevelu,0);   
		 }
      }
   }
   glEnd();
   
   glBindTexture( GL_TEXTURE_2D, texty );
   glBegin(GL_QUADS);   
   glTexCoord2d(1,2./8.);	glVertex3f(xscr/8,2*yscr/20,0);  
   glTexCoord2d(0,2./8.);	glVertex3f(0,2*yscr/20,0); 
   glTexCoord2d(0,1./8.);	glVertex3f(0,yscr/20,0);   
   glTexCoord2d(1,1./8.);	glVertex3f(xscr/8,yscr/20,0);
   
   glTexCoord2d(1,3./8.);	glVertex3f(3*xscr/8,2*yscr/20,0);  
   glTexCoord2d(0,3./8.);	glVertex3f(2*xscr/8,2*yscr/20,0); 
   glTexCoord2d(0,2./8.);	glVertex3f(2*xscr/8,yscr/20,0);   
   glTexCoord2d(1,2./8.);	glVertex3f(3*xscr/8,yscr/20,0);
   
   glTexCoord2d(1,4./8.);	glVertex3f(5*xscr/8,2*yscr/20,0);  
   glTexCoord2d(0,4./8.);	glVertex3f(4*xscr/8,2*yscr/20,0); 
   glTexCoord2d(0,3./8.);	glVertex3f(4*xscr/8,yscr/20,0);   
   glTexCoord2d(1,3./8.);	glVertex3f(5*xscr/8,yscr/20,0);
   
   glTexCoord2d(1,5./8.);	glVertex3f(7*xscr/8,2*yscr/20,0);  
   glTexCoord2d(0,5./8.);	glVertex3f(6*xscr/8,2*yscr/20,0); 
   glTexCoord2d(0,4./8.);	glVertex3f(6*xscr/8,yscr/20,0);   
   glTexCoord2d(1,4./8.);	glVertex3f(7*xscr/8,yscr/20,0);
   glEnd();
   
   text_write(xscr*0.75,yscr/18,32,current_level+1);
   text_write(xscr*0.57,yscr/18,32,time_left);
   text_write(xscr*0.30,yscr/18,32,diamonds_left);
   text_write(xscr*0.07,yscr/18,32,score);
   
   if(active_menu==1)
   {
   	  glColor3f(0.5,0.5,0.5);
      glBegin(GL_QUADS);  
      glVertex3f(xscr/4,yscr/4,0);
      glVertex3f(3*xscr/4,yscr/4,0);
      glVertex3f(3*xscr/4,3*yscr/4,0);
      glVertex3f(xscr/4,3*yscr/4,0);
      glEnd();   	  
   	  
      glBindTexture( GL_TEXTURE_2D, texty );
      glBegin(GL_QUADS);  
      glTexCoord2d(1,8./8.);	glVertex3f(6*xscr/8,6*yscr/8,0);  
      glTexCoord2d(0,8./8.);	glVertex3f(3*xscr/8,6*yscr/8,0); 
      glTexCoord2d(0,5./8.);	glVertex3f(3*xscr/8,3*yscr/8,0);   
      glTexCoord2d(1,5./8.);	glVertex3f(6*xscr/8,3*yscr/8,0);
      glEnd();
   }else
   if(active_menu==3)
   {
   	  glColor3f(0.5,0.5,0.5);
      glBegin(GL_QUADS);  
      glVertex3f(xscr/4,yscr/4,0);
      glVertex3f(3*xscr/4,yscr/4,0);
      glVertex3f(3*xscr/4,3*yscr/4,0);
      glVertex3f(xscr/4,3*yscr/4,0);
      glEnd();   	  
   	  
      glBindTexture( GL_TEXTURE_2D, texty );
      glBegin(GL_QUADS);  
      glTexCoord2d(1,7./8.);	glVertex3f(6*xscr/8,4*yscr/8,0);  
      glTexCoord2d(0,7./8.);	glVertex3f(3*xscr/8,4*yscr/8,0); 
      glTexCoord2d(0,6./8.);	glVertex3f(3*xscr/8,3*yscr/8,0);   
      glTexCoord2d(1,6./8.);	glVertex3f(6*xscr/8,3*yscr/8,0);
      glEnd();
   }else
   if(active_menu==2)
   {
   	  glColor3f(0.5,0.5,0.5);
      glBegin(GL_QUADS);  
      glVertex3f(xscr/4,yscr/4,0);
      glVertex3f(2*xscr/4,yscr/4,0);
      glVertex3f(2*xscr/4,3*yscr/4,0);
      glVertex3f(xscr/4,3*yscr/4,0);
      glEnd();   	  
   	  
      glBindTexture( GL_TEXTURE_2D, texty );
      glBegin(GL_QUADS);  
      glTexCoord2d(1,5./8.);	glVertex3f(6*xscr/8,7*yscr/8,0);  
      glTexCoord2d(0,5./8.);	glVertex3f(2*xscr/8,7*yscr/8,0); 
      glTexCoord2d(0,4./8.);	glVertex3f(2*xscr/8,5*yscr/8,0);   
      glTexCoord2d(1,4./8.);	glVertex3f(6*xscr/8,5*yscr/8,0);
      glEnd(); 
	  
	  text_write(xscr*0.2,4*yscr/8,100,current_level+1);  	
   }
}

void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glDisable(GL_DEPTH_TEST);
   glLoadIdentity();
   display3D();
   SDL_GL_SwapBuffers();
}

void controlsSDL()
{
   //if (key[27]) {exit(0);} //Esc
   
   if ((key[27])&(keyRelease[27]==0)){keyRelease[27]=release_time;if(active_menu==0){active_menu=1;}else if((active_menu==1)|(active_menu==3)){active_menu=0;}}
   
   if((active_menu==1))
   {
   	  if ((key['r'])&(keyRelease['r']==0)){keyRelease['r']=release_time;Lose();active_menu=0;}
   	  if ((key['q'])){exit(0);}
   	  if ((key['n'])&(keyRelease['n']==0)){keyRelease['n']=release_time;current_level=0;LoadLevel();score=0;active_menu=0;}
   }else
   if((active_menu==2))
   {
   	  for(int poradi710=0;poradi710<257;poradi710++)
	  {
	  	if ((key[poradi710])&(keyRelease[poradi710]==0)){active_menu=0;}
	  }
   }
   
   if ((key['w'])&(keyRelease['w']==0)){keyRelease['w']=release_time;pohyb=1;}
   if ((key['s'])&(keyRelease['s']==0)){keyRelease['s']=release_time;pohyb=3;}
   if ((key['a'])&(keyRelease['a']==0)){keyRelease['a']=release_time;pohyb=4;}
   if ((key['d'])&(keyRelease['d']==0)){keyRelease['d']=release_time;pohyb=2;}
}

void main_loop_function()
{
   clock_t t,t1,t2;
   while( events() )
   {
   	  t1=clock();
      display();
      controlsSDL();
      
      //PRODLEVY PO KLAVESACH
      for(int poradi710=0;poradi710<257;poradi710++)
      {
     	  if(keyRelease[poradi710]!=0){keyRelease[poradi710]--;}
      }
      
      if(active_menu==0){Tic();}
      
      t = clock() - t1;
      sleep(10-t);
      t2 = clock() - t1;
   }
}

#undef main
int main(int argc, char **argv)
{
   SDL_Init(SDL_INIT_VIDEO);
   const SDL_VideoInfo* info = SDL_GetVideoInfo();	
   int vidFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;;
   if (info->hw_available) {vidFlags |= SDL_HWSURFACE;}
   else {vidFlags |= SDL_SWSURFACE;}
   int bpp = info->vfmt->BitsPerPixel;
   SDL_SetVideoMode(xscr,yscr, bpp, vidFlags);
   init();
   
   pismo = LoadTexture( 1, "font.bmp", 320, 32,255 );
   grafika = LoadTexture( 1, "graphics.bmp", 256, 32,255 );
   texty = LoadTexture( 1, "texts.bmp", 256, 256,255 );
   
   main_loop_function();
   
   return 0;
}
