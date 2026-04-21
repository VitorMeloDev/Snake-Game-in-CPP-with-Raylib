#include <raylib.h>
#include <iostream>
#include <deque>

#define WINDOW_TITLE "Snake Game - VM Games"

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;

class Snake 
{
public:
    std::deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
			float x = body[i].x * cellSize;
            float y = body[i].y * cellSize;
            Rectangle segment = Rectangle{ x, y, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5f, 6, darkGreen);
        }
    }
};

class Food
{
public:
    Vector2 position;
	Texture2D texture;
    Food()
    {
        Image image = LoadImage("Graphics/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();
	}

    ~Food()
    {
		UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

    Vector2 GenerateRandomPos()
    {
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);

		return { x, y };
    }
};

int main()
{
    InitWindow(cellSize*cellCount, cellSize * cellCount, WINDOW_TITLE);
	SetTargetFPS(60);

	Food food = Food();
	Snake snake = Snake();

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
		food.Draw();
		snake.Draw();
        ClearBackground(green);
        EndDrawing();    
    }
    CloseWindow();  

    return 0;
}