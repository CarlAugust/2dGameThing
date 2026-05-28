// 2dGameThing.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <raylib.h>
#include <vector>
#include <cmath>
#include <algorithm>

#define BLOCK_SIZE 32.0f
#define SPEED BLOCK_SIZE * 8
#define COLLISION_RANGE 4

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
				DrawRectangleLinesEx(blockRect, 1, BLACK);
			}
		}
	}
}

void MapBlockCollide(Map &map, Body &player, double dt) {
	/*
		TODO: When map size increases i should make the loop only work for elements around the player
		Especially if i want to store the entire map in one 2d array
	*/

	// Realistically we probobly only need to check like a 5x5 around teh player


	// Safety checks
	u64 xToIndex = player.position.x / BLOCK_SIZE;
	u64 yToIndex = player.position.y / BLOCK_SIZE;
	u64 x0 = std::max(xToIndex - COLLISION_RANGE, u64(0));
	u64 x1 = std::min(xToIndex + COLLISION_RANGE, map[0].size());
	u64 y0 = std::max(yToIndex - COLLISION_RANGE, u64(0));
	u64 y1 = std::min(yToIndex + COLLISION_RANGE, map.size());

	Rectangle playerRect = {player.position.x, player.position.y, player.size.x, player.size.y};
	
	for (u64 y = y0; y < y1; y++) {
		for (u64 x = x0; x < x1; x++) {
			if (map[y][x] == 0) continue;
			
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle playerRectTemp = playerRect;

			// Test x direction
			playerRectTemp.x += player.velocity.x * dt;
			bool collideX = CheckCollisionRecs(blockRect, playerRectTemp);
			if (collideX) {
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

	for (u64 y = y0; y < y1; y++) {
		for (u64 x = x0; x < x1; x++) {
			if (map[y][x] == 0) continue;
			
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle playerRectTemp = playerRect;

			// Test y direction
			playerRectTemp.y += player.velocity.y * dt;
			bool collideY = CheckCollisionRecs(blockRect, playerRectTemp);
			if (collideY) {
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

void MapBorderCollide(Body& player, const u64 mapWidthReal, const u64 mapHeightReal, double dt) {
	const float mapWidthFloat = float(mapWidthReal);
	const float mapHeightFloat = float(mapHeightReal);

	if (player.position.x + player.velocity.x * dt < 0.0f) {
		player.position.x = 0.0f;
		player.velocity.x = 0.0f;
	}
	else if (player.position.x + player.size.x + player.velocity.x * dt > mapWidthFloat) {
		player.position.x = mapWidthFloat - player.size.x;
		player.velocity.x = 0.0f;
	}

	if (player.position.y + player.velocity.y * dt < 0.0f) {
		player.position.y = 0.0f;
		player.velocity.y = 0.0f;
	}
	else if (player.position.y + player.size.y + player.velocity.y * dt > mapHeightFloat) {
		player.position.y = mapHeightFloat - player.size.y;
		player.velocity.y = 0.0f;
	}
}

void UpdatePlayerCamera(Camera2D& camera, Body& player, const u64 mapWidthReal, const u64 mapHeightReal) {
	const float mapWidthFloat = float(mapWidthReal);
	const float mapHeightFloat = float(mapHeightReal);
	
	if (player.position.x - camera.offset.x < 0) {
		camera.target.x = camera.offset.x;
	}
	else if (player.position.x + camera.offset.x > mapWidthFloat) {
		camera.target.x = mapWidthFloat - camera.offset.x;
	}
	else {
		camera.target.x = player.position.x;
	}

	if (player.position.y - camera.offset.y < 0) {
		camera.target.y = camera.offset.y;
	}
	else if (player.position.y + camera.offset.y > mapHeightFloat) {
		camera.target.y = mapHeightFloat - camera.offset.y;
	}
	else {
		camera.target.y = player.position.y;
	}
}

int main()
{
	// INITILIZATION ----------------------------------------------------

	const u64 screenWidth = 1600;
	const u64 screenHeight = 900;
	InitWindow(screenWidth, screenHeight, "2dGameThing");
	SetTargetFPS(144);

	Body player;
	player.position = { screenWidth / 2.0f, screenHeight / 2.0f };
	player.prevPosition = player.position;
	player.size = { BLOCK_SIZE, BLOCK_SIZE };

	const u64 mapWidthReal = (screenWidth * 4);
	const u64 mapHeightReal = (screenHeight * 2);

	std::vector<std::vector<int>> map(mapHeightReal / BLOCK_SIZE, std::vector<int>(mapWidthReal / BLOCK_SIZE, 0));
	for (int i = 10; i < 20; i++) {
		for (int j = 10; j < 20; j++) {
			map[i][j] = 1;
		}
	}

	Camera2D camera;
	camera.target = { player.position.x + player.size.x / 2, player.position.y + player.size.y / 2 };
	camera.offset = { screenWidth / 2, screenHeight / 2 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	// MAIN LOOP ----------------------------------------------------

	while (!WindowShouldClose())
	{
		double dt = GetFrameTime();

		// GAME EVENTS

		HandleInput(player, dt);
		MapBlockCollide(map, player, dt);
		MapBorderCollide(player, mapWidthReal, mapHeightReal, dt);
		player.UpdatePosition(dt);		

		UpdatePlayerCamera(camera, player, mapWidthReal, mapHeightReal);
		
		// DRAWING ---

		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
		{
			DrawRectangleV(player.position, player.size, RED);			
			MapDraw(map);
		}
		EndMode2D();

		DrawFPS(screenWidth - 100, 10);
		EndDrawing();
	}

	return 0;
}
