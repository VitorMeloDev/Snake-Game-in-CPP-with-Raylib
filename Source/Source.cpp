#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>

#define WINDOW_TITLE "Snake Game - VM Games"

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool ElementInDeque(std::deque<Vector2> deque, Vector2 element)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if(Vector2Equals(deque[i], element))
            return true;
    }
    return false;
}

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake 
{
public:
    std::deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
	Vector2 direction = { 1, 0 };

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
			float x = offset + body[i].x * cellSize;
            float y = offset + body[i].y * cellSize;
            Rectangle segment = Rectangle{ x, y, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5f, 6, darkGreen);
        }
    }

    void Update()
    {
		body.pop_back();
        body.push_front(Vector2Add(body[0], direction));
    }

    void Reset()
    {
        body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
        direction = { 1, 0 };
	}
};

class Food
{
public:
    Vector2 position;
	Texture2D texture;

    Food(std::deque<Vector2> snakeBody)
    {
        Image image = LoadImage("Graphics/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

    ~Food()
    {
		UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
	}

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return { x, y };
    }

    Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody)
    {
        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(snakeBody, position))
        {
            position = GenerateRandomCell();
        }
		return position;
    }
};

class Game
{
public:
	Snake snake;
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;
	Sound eatSound;
    Sound wallSound;

    Game()
    {
		InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
		wallSound = LoadSound("Sounds/wall.mp3");
    }
    
    ~Game()
    {
		UnloadSound(eatSound);
		UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Update()
    {
        if (!running)
            return;
        if (eventTriggered(0.2))
            snake.Update();
		CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionWithSelf();
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            snake.body.push_back(snake.body[snake.body.size() - 1]);
            food.position = food.GenerateRandomPos(snake.body);
            PlaySound(eatSound);
            score++;
        }
	}

    void CheckCollisionWithEdges()
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1 || snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
        }
    }

    void CheckCollisionWithSelf()
    {
		std::deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(headlessBody, snake.body[0]))
        {
            GameOver();
		}
    }

    void GameOver()
    {
		PlaySound(wallSound);
        snake.Reset();
		score = 0;
		food.position = food.GenerateRandomPos(snake.body);
		running = false;
    }
};

int main()
{
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, WINDOW_TITLE);
	SetTargetFPS(60);

	Game game;

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();

		game.Update();

		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
            game.snake.direction = { 0, -1 };
        else if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
            game.snake.direction = { 0, 1 };
        else if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
            game.snake.direction = { -1, 0 };
        else if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
            game.snake.direction = { 1, 0 };

        if (GetKeyPressed() != 0)
            game.running = true;

		game.Draw();
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10 }, 5, darkGreen);
        
        DrawText("Snake Game - VM Games", offset - 5, 20, 40, darkGreen);
        DrawText(TextFormat("Score: %i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
        EndDrawing();    
    }
    CloseWindow();  

    return 0;
}