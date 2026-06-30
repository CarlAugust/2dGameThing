#pragma once

#include <stdint.h>
#include <raylib.h>

typedef int64_t i64;
typedef uint64_t u64;

#define BLOCK_SIZE 32.0f
#define BASE_SPEED BLOCK_SIZE * 8
#define COLLISION_RANGE 4


static float floatSign(float x) {
	if (x > 0) return 1.0f;
	if (x < 0) return -1.0f;
	return 0.0f;
}

static float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

static Texture2D LoadTextureSafe(const char* fileName) {
    Image img = LoadImage(fileName);
    if (img.width == 0 && img.height == 0) {
        img = GenImageChecked(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE / 2, BLOCK_SIZE / 2, RED, BLACK);
    }
    
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    return tex;
}

struct Body {
	Vector2 position = { 0,0 };
	Vector2 prevPosition = { 0,0 };
	Vector2 velocity = { 0, 0 };
	Vector2 size = { 0, 0 };
	Vector2 speedMultiplier = { 1.0f, 1.0f };
	Vector2 maxVelocity = { 0, 0 };

	void UpdatePosition(double dt) {
		prevPosition = position;
		// position.x += velocity.x * dt;
		// position.y += velocity.y * dt;	

		position.x += velocity.x * dt;
		position.y += velocity.y * dt;

		float damping = 0.95f;
		velocity.x = lerp(velocity.x, 0, damping * dt);
		velocity.y = lerp(velocity.y, 0, damping * dt);

		velocity.x *= 1.0f - 3.0f * dt;
		velocity.y *= 1.0f - 0.5f * dt;

		float gravity = 2000.0f;
		velocity.y += gravity * dt;
	}

	void UpdateVelocity(double vx, double vy) {
		velocity.x += vx;
		velocity.y += vy;

		if (velocity.x > maxVelocity.x) {
			velocity.x = maxVelocity.x;
		} else if (velocity.x < -maxVelocity.x) {
			velocity.x = -maxVelocity.x;
		}

		if (velocity.y > maxVelocity.y) {
			velocity.y = maxVelocity.y;
		} else if (velocity.y < -maxVelocity.y) {
			velocity.y = -maxVelocity.y;
		}
	}
};