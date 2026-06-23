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

struct Body {
	Vector2 position = { 0,0 };
	Vector2 prevPosition = { 0,0 };
	Vector2 velocity = { 0, 0 };
	Vector2 size = { 0,0 };

	void UpdatePosition(double dt) {
		prevPosition = position;
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;

		const float drag = BASE_SPEED * 0.5f;		

		velocity.x -= drag * floatSign(velocity.x) * dt;
		velocity.y -= drag * floatSign(velocity.y) * dt;
			
	}
};