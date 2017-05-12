#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780

#define rayon 20
#define SPEED 2

typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;


}game;

typedef struct gameState{

    int g_bRunning;


}gameState;

typedef struct coordonnees{

    double x;
    double y;

}coordonnees;


//prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);
void destroy(game *myGame);;
void delay(unsigned int frameLimit);

void Terrain(game *myGame);
void raquette(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *z );
void DessineBalle (game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *balle);
void MoveRaquette(int raquette1,int raquette2,coordonnees *dep,coordonnees *dep2,coordonnees *z,coordonnees *s);
void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *z,coordonnees *s, int *raquette1, int *raquette2, coordonnees *balle, coordonnees *MouvementBallon);




int main(int argc, char *argv[])
{


     game myGame;
     gameState state;

     unsigned int frameLimit = SDL_GetTicks() + 16;

     coordonnees dep;
     coordonnees dep2;
     coordonnees balle;
     coordonnees s;
     coordonnees z;
     coordonnees MouvementBallon;
     MouvementBallon.x=1;
     MouvementBallon.y=1;

     int raquette1,raquette2;

     int choix=0;

     balle.x=SCREEN_WIDTH/2;
    balle.y=SCREEN_HEIGHT/2;




     state.g_bRunning=1;

        myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);




        while(state.g_bRunning){



                if(choix==0){
                    printf("1: jouer au pong\n");

                    scanf("%i",&choix);


                }
                switch (choix){


                        case 1:init("pong",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame);
                                while(state.g_bRunning){
                                //on affiche le terrain, la raquette et le mouvement de la balle
                                    handleEvents(&state,&dep,&dep2,&s,&z,&raquette1,&raquette2,&balle,&MouvementBallon);
                                    MoveRaquette(raquette1,raquette2,&dep,&dep2,&z,&s);
                                    raquette(&myGame,&dep,&dep2,&z);
                                    Terrain(&myGame);
                                    DessineBalle(&myGame,&dep,&dep2,&balle);
                                }

                                 break;
                        default:break;


                }

                 system ("cls");


            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;



        }

        destroy(&myGame);

        SDL_Quit();


    return 0;
}


int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame){


    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;


    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {

            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);

            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);


            }


    }else{


        return 0;
    }



    return 1;



}

//fonction qui dessinne et affiche le terrain
void Terrain(game *myGame){

        SDL_SetRenderDrawColor(myGame->g_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(myGame->g_pRenderer);

        SDL_SetRenderDrawColor(myGame->g_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_Rect ligne;

        ligne.x=SCREEN_WIDTH/2;
        ligne.y=0;
        ligne.w=10;
        ligne.h=780;



        SDL_RenderFillRect(myGame->g_pRenderer,&ligne);


}

// fonction qui dessine et affiche les raquettes
void raquette(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *z ) {


        SDL_Rect rectangle;
        rectangle.x=0;
        rectangle.y=dep->y;
        rectangle.w=10;
        rectangle.h=100;



        SDL_Rect rectangle2;
        rectangle2.x=SCREEN_WIDTH-10;
        rectangle2.y=dep2->y;
        rectangle2.w=10;
        rectangle2.h=100;



        SDL_SetRenderDrawColor(myGame->g_pRenderer,0,84,114,174);
        SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture);

        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle);
        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle2);

        SDL_SetRenderTarget(myGame->g_pRenderer, NULL);

        SDL_Rect position;
        position.x = 0;
        position.y = 0;
        SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);


         SDL_RenderPresent(myGame->g_pRenderer);

         SDL_SetRenderDrawColor(myGame->g_pRenderer,0,0,0,255);
         SDL_DestroyTexture(myGame->g_ptexture);
         SDL_RenderClear(myGame->g_pRenderer);




}

// fonction qui dessine la balle
void DessineBalle(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *balle){

    int i;
    int j;

    SDL_Point boule;

        for(j=rayon;j!=0;j--){

            for(i=1;i<=360;i++){

                    boule.x=j*cos(i);
                    boule.y=j*sin(i);
                    SDL_SetRenderDrawColor(myGame->g_pRenderer,0,84,114,174);
                    SDL_RenderDrawPoint(myGame->g_pRenderer,boule.x+balle->x,boule.y+balle->y);
                                }
                            }
}

//on détruit notre jeu
void destroy(game *myGame){


     if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);


     if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);


     if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);


    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}

// gestion des évènements de mouvements des raquettes et du mouvements de la balle
void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *z,coordonnees *s, int *raquette1, int *raquette2,coordonnees *balle, coordonnees *MouvementBallon){

    balle->x-= MouvementBallon->x;
    balle->y-=MouvementBallon->y;

    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_UP){
                                *raquette1 = 1;
                            }

                            if (event.key.keysym.sym == SDLK_DOWN){
                                *raquette1 = 2;
                            }
                            if (event.key.keysym.sym == SDLK_z){
                                *raquette2 = 1;
                            }

                            if (event.key.keysym.sym == SDLK_s){
                                *raquette2 = 2;
                            };break;

        case SDL_KEYUP:

                            if (event.key.keysym.sym == SDLK_UP){
                                *raquette1 = 0;
                            }

                            if (event.key.keysym.sym == SDLK_DOWN){
                                *raquette1 = 0;
                            }
                            if (event.key.keysym.sym == SDLK_z){
                                *raquette2 = 0;
                            }

                            if (event.key.keysym.sym == SDLK_s){
                                *raquette2 = 0;
                            };break;

        default:break;

        }
    }

}


void MoveRaquette(int raquette1, int raquette2, coordonnees *dep, coordonnees *dep2, coordonnees *z, coordonnees *s){

    if (raquette1==1 && dep->y>0){
        dep->y-=SPEED;
    }else if (raquette1==2 && dep->y<SCREEN_HEIGHT-100){
        dep->y+=SPEED;
    }

    if (raquette2==1 && dep2->y>0){
        dep2->y-=SPEED;
    }else if (raquette2==2 && dep2->y<SCREEN_HEIGHT-100){
        dep2->y+=SPEED;
    }

}




void delay(unsigned int frameLimit)
{

    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}
