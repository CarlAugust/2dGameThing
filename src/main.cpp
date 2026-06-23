// 2dGameThing.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <raylib.h>
#include <vector>
#include <cmath>
#include <algorithm>

#include <common.h>
#include <world.h>

void HandleInput(Body& playerBody, double dt) {
	if (IsKeyDown(KEY_W)) {
		playerBody.velocity.y += -BASE_SPEED * dt;
	}
	if (IsKeyDown(KEY_S)) {
		playerBody.velocity.y += BASE_SPEED * dt;
	}
	if (IsKeyDown(KEY_A)) {
		playerBody.velocity.x += -BASE_SPEED * dt;
	}
	if (IsKeyDown(KEY_D)) {
		playerBody.velocity.x += BASE_SPEED * dt;
	}
}

void DrawWorldInCamera(World &world, Camera2D& camera) {	
	// Safety checks
	i64 xToIndex = camera.target.x / BLOCK_SIZE;
	i64 yToIndex = camera.target.y / BLOCK_SIZE;
	i64 screen_height = GetScreenHeight();
	i64 screen_width = GetScreenWidth();
	screen_height /= i64(BLOCK_SIZE);
	screen_width /= i64(BLOCK_SIZE);
	
	i64 blocksToLeft = (screen_width / 2);
	i64 blocksToRight = (screen_width / 2);
	i64 blocksToUp = (screen_height / 2);
	i64 blocksToDown = (screen_height / 2);

	i64 x0 = std::max(xToIndex - blocksToLeft, i64(0));
	i64 x1 = std::min(xToIndex + blocksToRight, i64(world.width));
	i64 y0 = std::max(yToIndex - blocksToUp, i64(0));
	i64 y1 = std::min(yToIndex + blocksToDown, i64(world.height));

	for (i64 y = y0; y < y1; y++) {
		for (i64 x = x0; x < x1; x++) {
			if (world.data[y][x].type != BlockType::AIR) {
				Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
				DrawRectanglePro(blockRect, { 0, 0 }, 0, PURPLE);
				DrawRectangleLinesEx(blockRect, 1, BLACK);
			}
		}
	}
}

void UpdatePlayerCameraWithBorder(Camera2D& camera, World& world, Body& player) {
	const float worldWidthFloat = static_cast<float>(world.realWidth);
	const float worldHeightFloat = static_cast<float>(world.realHeight);
	
	if (player.position.x - camera.offset.x < 0) {
		camera.target.x = camera.offset.x;
	}
	else if (player.position.x + camera.offset.x > worldWidthFloat) {
		camera.target.x = worldWidthFloat - camera.offset.x;
	}
	else {
		camera.target.x = player.position.x;
	}

	if (player.position.y - camera.offset.y < 0) {
		camera.target.y = camera.offset.y;
	}
	else if (player.position.y + camera.offset.y > worldHeightFloat) {
		camera.target.y = worldHeightFloat - camera.offset.y;
	}
	else {
		camera.target.y = player.position.y;
	}
}

int main()
{
	// INITILIZATION ----------------------------------------------------

	const i64 screenWidth = 1600;
	const i64 screenHeight = 900;
	InitWindow(screenWidth, screenHeight, "2dGameThing");
	SetTargetFPS(144);

	Body player;
	player.position = { screenWidth / 2.0f, screenHeight / 2.0f };
	player.prevPosition = player.position;
	player.size = { BLOCK_SIZE, BLOCK_SIZE };


	World world;
	world.realWidth = (screenWidth * 4);
	world.realHeight = (screenHeight * 2);
	world.width = world.realWidth / BLOCK_SIZE;
	world.height = world.realHeight / BLOCK_SIZE;

	world.data.assign(world.height, std::vector(world.height, Block{BlockType::AIR}));

	for (int i = 10; i < 20; i++) {
		for (int j = 10; j < 20; j++) {
			world.data[i][j] = Block{BlockType::DIRT};
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
		WorldBodyCollide(world, player, dt);
		WorldBorderBodyCollide(world, player, dt);
		player.UpdatePosition(dt);		

		UpdatePlayerCameraWithBorder(camera, world, player);
		
		// DRAWING ---

		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
		{
			DrawRectangleV(player.position, player.size, RED);			
			DrawWorldInCamera(world, camera);
		}
		EndMode2D();

		DrawFPS(screenWidth - 100, 10);
		EndDrawing();
	}

	return 0;
}
