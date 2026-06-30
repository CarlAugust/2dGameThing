// 2dGameThing.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <raylib.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <filesystem>

#include <common.h>
#include <world.h>

void HandleInput(Body& playerBody, double dt) {
	float speed = 3200.0f;
	if (IsKeyDown(KEY_D)) {
		playerBody.UpdateVelocity(speed * dt, 0);
	}
	if (IsKeyDown(KEY_A)) {
		playerBody.UpdateVelocity(-speed * dt, 0);
	}
	if (IsKeyDown(KEY_W)) {
		// Flying
		playerBody.UpdateVelocity(0, -speed * 1.5 * dt);
	}
	if (IsKeyDown(KEY_S)) {
		// Fall faster when holding down added to gravity
		playerBody.UpdateVelocity(0, speed * 0.5 * dt);
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
	// GLOBAL INITILIZATION ----------------------------------------------------

	const i64 screenWidth = 1600;
	const i64 screenHeight = 900;
	InitWindow(screenWidth, screenHeight, "2dGameThing");
	SetTargetFPS(144);
	

	InitBlockRegister();


	// ------

	// Per world initilization

	Body player;
	player.position = { screenWidth / 2.0f, screenHeight / 2.0f };
	player.prevPosition = player.position;
	player.size = { BLOCK_SIZE, BLOCK_SIZE };
	player.maxVelocity = {2000.0f, 2000.0f};

	World world = GenerateWorld(100, 100, 1000);

	Camera2D camera;
	camera.target = { player.position.x + player.size.x / 2, player.position.y + player.size.y / 2 };
	camera.offset = { screenWidth / 2, screenHeight / 2 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	// GOD MODE
	bool godModeOn = false;

	// MAIN LOOP ----------------------------------------------------

	while (!WindowShouldClose())
	{
		double dt = GetFrameTime();

		// GAME EVENTS


		if (IsKeyDown(KEY_Q)) godModeOn = !godModeOn;		
		HandleInput(player, dt);

		if (!godModeOn) WorldBodyCollide(world, player, dt);
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
		DrawText(TextFormat("x: %f y: %f", player.position.x, player.position.y), 0, 0, 20, RED);
		DrawText(TextFormat("x: %f y: %f", player.velocity.x, player.velocity.y), 0, 30, 20, RED);

		EndDrawing();

	}

	return 0;
}
