// 2dGameThing.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <raylib.h>
#include <vector>

#define BLOCK_SIZE 32.0f
#define SPEED 100.0f
float floatSign(float x) {
	if (x > 0) return 1.0f;
	if (x < 0) return -1.0f;
	return 0.0f;
}
typedef size_t u64;
typedef std::vector<std::vector<int>> Map;

struct Body {
	Vector2 position = { 0,0 };
	Vector2 prevPosition = { 0,0 };
	Vector2 velocity = { 0, 0 };
	Vector2 size = { 0,0 };
	bool FlagMovedY = false;
	bool FlagMovedX = false;

	void UpdatePosition(double dt) {
		prevPosition = position;
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;

		const float drag = SPEED * 2.0f;
		
		/*
		TODO: I have to uh... deal with friction and directions and stuff i hate this man lowkey...

		But the gist of it. drag currently happens if you dont move no matter the direction
		Thus Drag has to happen if its the same direction as the player is moving or something...
		maybe enum

		Time to sleep!
		*/
		if (FlagMovedX);
		else if (abs(velocity.x) - drag * dt < 0) velocity.x = 0;
		else velocity.x -= drag * floatSign(velocity.x) * dt;

		if (FlagMovedY);
		else if (abs(velocity.y) - drag * dt < 0) velocity.y = 0;
		else velocity.y -= drag * floatSign(velocity.y) * dt;
			
	}
};

void HandleInput(Body& playerBody, double dt) {
	if (IsKeyDown(KEY_W)) {
		playerBody.velocity.y += -SPEED * dt;
		playerBody.FlagMovedY = !playerBody.FlagMovedY;
	}
	if (IsKeyDown(KEY_S)) {
		playerBody.velocity.y += SPEED * dt;
		playerBody.FlagMovedY = !playerBody.FlagMovedY;
	}
	if (IsKeyDown(KEY_A)) {
		playerBody.velocity.x += -SPEED * dt;
		playerBody.FlagMovedX = !playerBody.FlagMovedX;
	}
	if (IsKeyDown(KEY_D)) {
		playerBody.velocity.x += SPEED * dt;
		playerBody.FlagMovedX = !playerBody.FlagMovedX;
	}
}

void MapDraw(Map &map) {	
	for (u64 y = 0; y < map.size(); y++) {
		for (u64 x = 0; x < map[y].size(); x++) {
			if (map[y][x] != 0) {
				Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
				DrawRectanglePro(blockRect, { 0, 0 }, 0, PURPLE);
			}
		}
	}
}

void MapCollide(Map &map, Body &player) {
	for (u64 y = 0; y < map.size(); y++) {
		for (u64 x = 0; x < map[y].size(); x++) {
			if (map[y][x] == 0) continue;
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle playerRect = {player.position.x, player.position.y, player.size.x, player.size.y};

			bool collide = CheckCollisionRecs(blockRect, playerRect);
			if (collide) {
				DrawRectanglePro(blockRect, { 0, 0 }, 0, PURPLE);
				player.velocity.x = 0;
				player.velocity.y = 0;

				player.position = player.prevPosition;
			}
		}
	}
}

int main()
{
	const int screenWidth = 1600;
	const int screenHeight = 900;
	InitWindow(screenWidth, screenHeight, "2dGameThing");
	SetTargetFPS(60);
	Body player;
	player.position = { screenWidth / 2.0f, screenHeight / 2.0f };
	player.prevPosition = player.position;
	player.size = { BLOCK_SIZE, BLOCK_SIZE };

	std::vector<std::vector<int>> map = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	while (!WindowShouldClose())
	{
		double dt = GetFrameTime();

		HandleInput(player, dt);
		player.UpdatePosition(dt);	
		MapCollide(map, player);


		// Reset body flags!
		player.FlagMovedX = false;
		player.FlagMovedY = false;
	
		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawRectangleV(player.position, player.size, RED);
		MapDraw(map);

		EndDrawing();
	}

	return 0;
}
