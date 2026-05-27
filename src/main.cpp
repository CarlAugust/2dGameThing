// 2dGameThing.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <raylib.h>
#include <vector>
#include <cmath>
#include <algorithm>

#define BLOCK_SIZE 32.0f
#define SPEED BLOCK_SIZE * 2

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

	void UpdatePosition(double dt) {
		prevPosition = position;
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;

		const float drag = SPEED * 0.5f;
		
		/*
		TODO: I have to uh... deal with friction and directions and stuff i hate this man lowkey...

		But the gist of it. drag currently happens if you dont move no matter the direction
		Thus Drag has to happen if its the same direction as the player is moving or something...
		maybe enum

		Time to sleep!
		*/

		velocity.x -= drag * floatSign(velocity.x) * dt;
		velocity.y -= drag * floatSign(velocity.y) * dt;
			
	}
};

void HandleInput(Body& playerBody, double dt) {
	if (IsKeyDown(KEY_W)) {
		playerBody.velocity.y += -SPEED * dt;
	}
	if (IsKeyDown(KEY_S)) {
		playerBody.velocity.y += SPEED * dt;
	}
	if (IsKeyDown(KEY_A)) {
		playerBody.velocity.x += -SPEED * dt;
	}
	if (IsKeyDown(KEY_D)) {
		playerBody.velocity.x += SPEED * dt;
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

void MapCollide(Map &map, Body &player, double dt) {
	/*
		TODO: When map size increases i should make the loop only work for elements around the player
		Especially if i want to store the entire map in one 2d array
	*/

	Rectangle playerRect = {player.position.x, player.position.y, player.size.x, player.size.y};
	for (u64 y = 0; y < map.size(); y++) {
		for (u64 x = 0; x < map[y].size(); x++) {
			if (map[y][x] == 0) continue;
			
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle playerRectTemp = playerRect;

			// Test x direction
			playerRectTemp.x += player.velocity.x * dt;
			bool collideX = CheckCollisionRecs(blockRect, playerRectTemp);
			if (collideX) {
				DrawRectanglePro(blockRect, { 0, 0 }, 0, PURPLE);
				playerRectTemp.x -= player.velocity.x * dt;
				if (player.velocity.x < 0.0f) {
					player.position.x = blockRect.x + blockRect.width;
				}
				else {
					player.position.x = blockRect.x - player.size.x;
				}
				player.velocity.x = 0.0f;
			}
		}
	}

	for (u64 y = 0; y < map.size(); y++) {
		for (u64 x = 0; x < map[y].size(); x++) {
			if (map[y][x] == 0) continue;
			
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle playerRectTemp = playerRect;

			// Test y direction
			playerRectTemp.y += player.velocity.y * dt;
			bool collideY = CheckCollisionRecs(blockRect, playerRectTemp);
			if (collideY) {
				DrawRectanglePro(blockRect, { 0, 0 }, 0, PURPLE);
				playerRectTemp.y -= player.velocity.y * dt;
				if (player.velocity.y < 0.0f) {
					player.position.y = blockRect.y + blockRect.height;
				}
				else {
					player.position.y = blockRect.y - player.size.y;
				}

				player.velocity.y = 0.0f;
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
		MapCollide(map, player, dt);
		player.UpdatePosition(dt);	
	
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(screenWidth - 100, 10);

		DrawRectangleV(player.position, player.size, RED);
		MapDraw(map);

		EndDrawing();
	}

	return 0;
}
