#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define offset 10

bool sdl_init();
void move_ball(ball_rect);
void check_ball_collision();
void ia();
//void player_move();

typedef struct {
    int height;
    int width;
    int x;
    int speed;
    int score;
}Player;

typedef struct{
    int x;       // posición horizontal
    int y;       // posición vertical
    int width;
    int height;
    int speed;
    int vel_x;   // velocidad horizontal
    int vel_y;   // velocidad vertical
}Ball;

int main(){
    printf("GUI v0.1\n");
    printf("window width: %d\n", WINDOW_WIDTH);
    printf("window height: %d\n", WINDOW_HEIGHT);

    bool running = sdl_init();

    //SDL
    SDL_Event event;
    SDL_Surface* screenSurface = NULL;

    //Player
    Player player;
    player.height = 100;
    player.width = 20;
    player.x = 10;
    player.speed = 12;
    player.score = 0;


    //CPU
    Player cpu;
    cpu.height = 100;
    cpu.width = 20;
    cpu.x = 200;
    cpu.speed = 4;
    cpu.score = 0;

    //Ball
    Ball ball;
    ball.x = 50;
    ball.y = 200;
    ball.width = 15;
    ball.height = 15;
    ball.speed = 3;
    ball.vel_x = ball.speed;  // velocidad en x
    ball.vel_y = ball.speed;  // velocidad en y

    SDL_Window* window = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(window == NULL){
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        return -1;
    }

    screenSurface = SDL_GetWindowSurface(window);

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE: running = false; break;
                    case SDLK_DOWN: player_move(&player, 1); break;
                    case SDLK_UP: player_move(&player, 0); break;
                }
            }
        }

        //refresh screen
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

        //draw player
        SDL_Rect player_rect = {offset, player.x, player.width, player.height};
        SDL_FillRect(screenSurface, &player_rect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

        //draw cpu
        SDL_Rect cpu_rect = {WINDOW_WIDTH - (offset + cpu.width), cpu.x, cpu.width, cpu.height};
        SDL_FillRect(screenSurface, &cpu_rect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

        //draw ball
        // Actualiza la posición de la pelota
        move_ball(&ball,&player,&cpu);
        SDL_Rect ball_rect = {ball.y, ball.x, ball.width, ball.height};
        SDL_FillRect(screenSurface, &ball_rect, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));

        //comprobar colision
        check_ball_collision(&ball, &player);

        ia(&cpu, &ball);

        //pintar el surface
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }
    printf("Player: %d\n CPU: %d", player.score, cpu.score);
    return 1;
}

bool sdl_init(){
    bool status = true;
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("SDL_Init error: %s\n", SDL_GetError());
        status = false;
    }
   return status;
}

void move_ball(Ball *ball, Player *player, Player *cpu) {
    // Actualizar posición
    ball->x += ball->vel_x;
    ball->y += ball->vel_y;

    // Rebote en los laterales
    if(ball->x <= 0 || ball->x + ball->width >= WINDOW_HEIGHT) {
        ball->vel_x = -ball->vel_x;
    }
    
    // Rebote en la parte superior e inferior
    if(ball->y <= 0 || ball->y + ball->height >= WINDOW_WIDTH) {
        ball->vel_y = -ball->vel_y;
        if(ball->y <= 0){
            cpu->score++;
        }else if(ball->y + ball->height >= WINDOW_WIDTH){
             player->score++;
        }
    }
}

void check_ball_collision(Ball *ball, Player *player, Player *cpu){
    if(ball->y <= player->width + offset){
        if(ball->x > player->x && ball->x < player->x + player->height){
            if(ball->vel_y <1){
                ball->vel_y = -ball->vel_y;
            }
        }
    } 
    if(ball->y+ball->height >= WINDOW_WIDTH - (player->width + offset)){
        if(ball->x > cpu->x && ball->x < cpu->x+cpu->height){
            if(ball->vel_y > 1){
                ball->vel_y = -ball->vel_y;
            }
        }
    }
}

void ia(Player *cpu, Ball *ball){
    if(ball->x < cpu->x){
        cpu->x -= cpu->speed;
    }else if(ball->x > cpu->x+cpu->height){
        cpu->x += cpu->speed;
    }
}

void player_move(Player *player, int direction){
    if(direction == 0){//up
        if(player->x > 0){
            player->x -= player->speed;
        }
    }else if(direction == 1){//down
        if(player->x + player->height < WINDOW_HEIGHT){
            player->x += player->speed;
        }            
    }

}
