#include <raylib.h>


const int window_height{800};
const int window_width{1200};

Color Purple = Color{128,0,128,255};
Color DarkPurple = Color{48, 25, 52, 255};
Color LightPurple = Color{216, 191, 216, 255};

int player_score{};
int cpu_score{};

class Ball{
    public:
        int xpos{};
        int ypos{};
        int x_spd{};
        int y_spd{};
        double radius{};

        Ball(int x, int y, int x_sped, int y_sped, double rad): xpos{x}, ypos{y}, x_spd{x_sped}, y_spd{y_sped}, radius{rad}{}
        

        void Draw(){DrawCircle(xpos, ypos, radius, WHITE);}
        void Reset(){xpos = window_width/2; ypos = window_height/2;}

        void Update(){
            if(xpos + radius >= window_width){player_score++; Reset();}
            if(xpos - radius <= 0){cpu_score++; Reset();}

            if(ypos + radius >= window_height){y_spd *= -1;}
            if(ypos - radius <= 0){y_spd *= -1;}

            xpos += x_spd;
            ypos += y_spd;
        }
};


class Paddle{
    public:
        int xpos;
        int ypos;
        int spd;
        int width;
        int height;

        explicit Paddle(int x, int y, int s, int w, int h): xpos{x}, ypos{y}, spd{s}, width{w}, height{h}{}

        void Draw(){DrawRectangleRounded(Rectangle{static_cast<float>(xpos), static_cast<float>(ypos), static_cast<float>(width), static_cast<float>(height)}, 0.8f, 0, WHITE);}
        void Update(){
            if(IsKeyDown(KEY_DOWN)){ypos += spd;}
            if(IsKeyDown(KEY_UP)){ypos -= spd;}
            LimitMovement();
        }
    protected:
        void LimitMovement(){
            if(ypos < 0){ypos = 0;}
            if(ypos > window_height - height){ypos = window_height - height;}
        }
};


class CpuPaddle: public Paddle{
    public:
        using Paddle::Paddle;
        void Draw(){DrawRectangleRounded(Rectangle{static_cast<float>(xpos),static_cast<float>(ypos),static_cast<float>(width),static_cast<float>(height)}, 0.8, 0, WHITE);}

        void Update(const Ball& ball){
            if(ypos < ball.ypos){ypos+= spd;}
            else if(ypos > ball.ypos){ypos-= spd;}
            LimitMovement();
        }

};




int main(){
    InitWindow(window_width, window_height, "pong game");
    SetTargetFPS(60);

    Ball ball{window_width/2, window_height/2, 12, 12, 25};
    Paddle player{0,window_height/2-70,16,35,140};
    CpuPaddle cpu{window_width-35,window_height/2-70,16,35,140};

    while(!WindowShouldClose()){
        BeginDrawing();
        ball.Update();
        player.Update();
        cpu.Update(ball);

        Rectangle recplayer{static_cast<float>(player.xpos),static_cast<float>(player.ypos),static_cast<float>(player.width),static_cast<float>(player.height)};
        Rectangle reccpu{static_cast<float>(cpu.xpos), static_cast<float>(cpu.ypos), static_cast<float>(cpu.width), static_cast<float>(cpu.height)};
        if(CheckCollisionCircleRec(Vector2{static_cast<float>(ball.xpos), static_cast<float>(ball.ypos)} , ball.radius, recplayer)){
            ball.x_spd *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{static_cast<float>(ball.xpos), static_cast<float>(ball.ypos)} , ball.radius, reccpu)){
            ball.x_spd *= -1;
        }
        
        
        ClearBackground(DarkPurple);
        DrawCircle(window_width/2, window_height/2, 200, LightPurple);
        DrawLine(window_width/2, 0, window_width/2, window_height, DarkPurple);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        
        DrawText(TextFormat("%i",player_score),window_width/4,30,70,WHITE);
        DrawText(TextFormat("%i",cpu_score),window_width/4 * 3,30,70,WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;

}
