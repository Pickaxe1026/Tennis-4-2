#include <raylib.h>
#include <cmath>

class Ball{
public:
    float x, y;
    float speedX, speedY;
    int radius;
    float gravity = 600.0f;
    bool isActive;

    static const int TRAIL_LENGTH = 15;
    float trailX[TRAIL_LENGTH];
    float trailY[TRAIL_LENGTH];
    int trailIndex = 0;
    bool trailFilled = false;

    bool canBeHitByLeft = true;
    bool canBeHitByRight = false;
    int hitCountThisRally = 0; 

    Ball(int startX, int startY){
        x = startX;
        y = startY;
        speedX = 0;
        speedY = 0;
        radius = 8;
        isActive = false;

        canBeHitByLeft = true;
        canBeHitByRight = false;
        hitCountThisRally = 0;

        for(int i = 0; i < TRAIL_LENGTH; i++){
            trailX[i] = startX;
            trailY[i] = startY;
        }
    }

    void Hit(float angle, float power, bool hitByLeft){
        if (hitByLeft && !canBeHitByLeft){
            return;
        }
        if (!hitByLeft && !canBeHitByRight){
            return;
        }
        
        float direction = hitByLeft ? 1.0f : -1.0f;
        float horizontalPower = power * 0.8f;
        float verticalFactor = 1.5f;
        
        speedX = direction * (horizontalPower + fabs(cos(angle)) * power * 0.5f);
        speedY = sin(angle) * power * verticalFactor;
        isActive = true;
        
        canBeHitByLeft = !hitByLeft;
        canBeHitByRight = hitByLeft;
        hitCountThisRally++;
    }

    int Update(float dt){
        float prevX = x;

        trailX[trailIndex] = x;
        trailY[trailIndex] = y;
        trailIndex ++;
        if(trailIndex >= TRAIL_LENGTH){
            trailIndex = 0;
            trailFilled = true;
        }

        if(!isActive){
            return 0;
        }

        speedX *= 0.995f;
        speedY += gravity * dt;

        x += speedX * dt;
        y += speedY * dt;

        if(y + radius >= 800){
            y = 800 - radius;
            speedY = -speedY * 0.65f;
        }

        if(fabs(x - 500) <= radius){
            if(y >= 700 && y <= 800){
                if(x < 500){
                    x = 500 -radius;
                }
                else{
                    x = 500 + radius;
                }
                speedX = -speedX * 0.8f;
            }
        }

        if ((prevX < 500 && x >= 500) || (prevX >= 500 && x < 500)){
            if(x >= 500){
                canBeHitByLeft = false;
                canBeHitByRight = true;
            }
            else{
                canBeHitByLeft = true;
                canBeHitByRight = false;
            }
        }

        if(x - radius >= 1000){
            return 1;
        }
        if(x + radius <= 0){
            return 2;
        }

        return 0;
    }

    void DrawTrial(){
        int count = trailFilled ? TRAIL_LENGTH : trailIndex;

        for(int i = 0; i < count; ++ i){
            int idx = (trailIndex - 1 - i + TRAIL_LENGTH) % TRAIL_LENGTH;

            float ratio = 1.0f - (float)i / count;
            float size = radius * (0.3f + 0.7f * ratio);

            Color c = { 0, 0, (unsigned char)(50 + 205 * ratio), 255 };

            DrawCircle((int)trailX[idx], (int)trailY[idx], size, c);
        }
    }

    void Draw(){
        DrawTrial();
        DrawCircle((int)x, (int)y, radius, BLUE);
    }

    void Reset(bool leftServe) {
        x = leftServe ? 100.0f : 900.0f;
        y = 700.0f;
        speedX = 0;
        speedY = 0;
        isActive = false;
        
        canBeHitByLeft = leftServe;
        canBeHitByRight = !leftServe;
        hitCountThisRally = 0;
        
        for (int i = 0; i < TRAIL_LENGTH; i++) {
            trailX[i] = x;
            trailY[i] = y;
        }
        trailIndex = 0;
        trailFilled = false;
    }
};

void DrawKnob(int centerX, int centerY, float angle, bool facingRight, float radius = 40.0f){
    float dirX = facingRight ? 1.0f : -1.0f;
    float dirY = 1.0f;

    for(float a = -1.0f; a <= 1.0f; a += 0.1f){
        float displayX = cos(a) * dirX;
        float displayY = sin(a) * dirY;
        float x1 = centerX + displayX * radius;
        float y1 = centerY + displayY * radius;
        float x2 = centerX + displayX * (radius - 5);
        float y2 = centerY + displayY * (radius - 5);
        DrawLine((int)x1, (int)y1, (int)x2, (int)y2, BLUE);
    }

    DrawCircle(centerX, centerY, 5, BLUE);

    float pointerX = cos(angle) * dirX;
    float pointerY = sin(angle) * dirY;
    float arrowEndX = centerX + pointerX * radius;
    float arrowEndY = centerY + pointerY * radius;
    DrawLine(centerX, centerY, (int)arrowEndX, (int)arrowEndY, BLUE);

    float headSize = 8.0f;
    float pointerAngle = atan2(pointerY, pointerX);
    float leftAngle = pointerAngle + 2.6f;
    float rightAngle = pointerAngle - 2.6f;
    DrawLine((int)arrowEndX, (int)arrowEndY, (int)(arrowEndX + cos(leftAngle) * headSize), (int)(arrowEndY + sin(leftAngle) * headSize), RED);
    DrawLine((int)arrowEndX, (int)arrowEndY, (int)(arrowEndX + cos(rightAngle) * headSize), (int)(arrowEndY + sin(rightAngle) * headSize), RED);
}

int main(){
    int leftScore = 0;
    int rightScore = 0;

    float leftAimAngle = 0.0f;
    float rightAimAngle = 0.0f;

    float angleSpeed = 2.0f;
    float hitPower = 500.0f;

    InitWindow(1000, 1000, "Tennis 4 2");
    SetTargetFPS(60);

    Ball ball(100, 700);

    while(!WindowShouldClose()){
        float dt = GetFrameTime();

        if(IsKeyDown(KEY_A)){
            leftAimAngle -= angleSpeed * dt;
        }
        if(IsKeyDown(KEY_D)){
            leftAimAngle += angleSpeed * dt;
        }
        if(leftAimAngle > 1.0f){
            leftAimAngle = 1.0f;
        }
        if(leftAimAngle < -1.0f){
            leftAimAngle = -1.0f;
        }

        if(IsKeyDown(KEY_LEFT)){
            rightAimAngle -= angleSpeed * dt;
        }
        if(IsKeyDown(KEY_RIGHT)){
            rightAimAngle += angleSpeed * dt;
        }
        if(rightAimAngle > 1.0f){
            rightAimAngle = 1.0f;
        }
        if(rightAimAngle < -1.0f){
            rightAimAngle = -1.0f;
        }

        if(IsKeyPressed(KEY_SPACE) && ball.x < 500){
            ball.Hit(leftAimAngle, hitPower, true);
        }
        if(IsKeyPressed(KEY_ENTER) && ball.x > 500){
            ball.Hit(rightAimAngle, hitPower, false);
            ball.speedX = -fabs(ball.speedX);
        }


        int score = ball.Update(dt);
        if(score == 1){
            leftScore ++;
            ball.Reset(false);
            ball.x = 900;
            ball.y = 700;
        }
        else if(score == 2){
            rightScore ++;
            ball.Reset(true);
            ball.x = 100;
            ball.y = 700;
        }

        BeginDrawing();
            ClearBackground(BLACK);

            for(int i = 0; i <= 1000; i += 100){
                DrawLine(0, i, 1000, i, DARKGRAY);
                DrawLine(i, 0, i, 1000, DARKGRAY);
            }

            DrawLine(0, 800, 1000, 800, BLUE);
            DrawLine(500, 800, 500, 700, BLUE);

            ball.Draw();

            DrawText(TextFormat("%d", leftScore), 400, 50, 60, BLUE);
            DrawText(TextFormat("%d", rightScore), 550, 50, 60, BLUE);

            DrawKnob(150, 850, leftAimAngle, true);
            DrawKnob(850, 850, rightAimAngle, false);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}