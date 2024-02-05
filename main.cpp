#include <iostream>
#include <raylib.h>
using namespace std;

int player_score=0;
int cpu_score=0;

class Ball{
    public:
        float x,y;
        int speed_x,speed_y;
        int radius;

        void Draw(){
            DrawCircle(x, y, radius, WHITE);
        }

        void Update(){
            x += speed_x;
            y += speed_y;
            if(y + radius >= GetScreenHeight() || y-radius <= 0){
                speed_y *= -1;
            }
            if(x + radius >= GetScreenWidth()){
                cpu_score++;
                ResetBall();
            }
            
            if(x-radius <= 0){
                player_score++;
                ResetBall();
            } 
        }

        void ResetBall(){
            x = GetScreenWidth()/2;
            y = GetScreenHeight()/2;

            int speed_change[2] = {-1,1};
            speed_x *= speed_change[GetRandomValue(0,1)];
            speed_y *= speed_change[GetRandomValue(0,1)];
        }
};

class Paddle{
    protected:
        void LimitMovement(){
            if(y<=0){
                y=0;
            }
            if(y+height >= GetScreenHeight()){
                y = GetScreenHeight() - height;
            }
        }

    public:
        float x,y;
        int speed_y;
        float width,height;

        void Draw(){
            DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
        }

        void Update(){
            if(IsKeyDown(KEY_UP)){
                y = y-speed_y;
            }
            if(IsKeyDown(KEY_DOWN)){
                y = y+speed_y;
            }

            LimitMovement();
            
        }
};

class Cpu_Paddle: public Paddle{
    public:
        void Update(int ball_y){
            if(y + height/2 > ball_y){
                y = y - speed_y;
            }

            if(y+ height/2 <= ball_y){
                y = y + speed_y;
            }

            LimitMovement();
        }
};

int main () {
    const int screen_width = 1280;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "PONG!!");
    SetTargetFPS(60);
    //Objects

    Ball ball;
    ball.x=screen_width/2;
    ball.y=screen_height/2;
    ball.radius=20;
    ball.speed_x=7;
    ball.speed_y=7;

    Paddle player;
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width -10;
    player.y = screen_height/2 - player.height/2;
    player.speed_y = 6; 

    Cpu_Paddle cpu;
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed_y = 6;

    while(WindowShouldClose()==false){
        BeginDrawing();


        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *= -1;
        }

        ClearBackground(BLACK);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        DrawCircleLines(screen_width/2,screen_height/2, 150, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i",cpu_score), screen_width/4 -20, 20, 80, WHITE);
        DrawText(TextFormat("%i",player_score), 3*screen_width/4 -20, 20, 80, WHITE);


        EndDrawing();
    }

    CloseWindow();

    return 0;
}