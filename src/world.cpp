#include <world.h>
#include <raylib.h>
#include <common.h>
#include <string>

std::string BlockNameFromType(BlockType t) {
    switch (t) {
        case BlockType::AIR:
            return "air";
        case BlockType::DIRT:
            return "dirt";
    }
}

void WorldBodyCollide(World &world, Body &body, double dt) {
	// Safety checks
	i64 xToIndex = body.position.x / BLOCK_SIZE;
	i64 yToIndex = body.position.y / BLOCK_SIZE;
	i64 x0 = std::max(xToIndex - COLLISION_RANGE, 0i64);
	i64 x1 = std::min(xToIndex + COLLISION_RANGE, world.width);
	i64 y0 = std::max(yToIndex - COLLISION_RANGE, 0i64);
	i64 y1 = std::min(yToIndex + COLLISION_RANGE, world.height);

	Rectangle bodyRect = {body.position.x, body.position.y, body.size.x, body.size.y};
	
	for (i64 y = y0; y < y1; y++) {
		for (i64 x = x0; x < x1; x++) {
			if (world.data[y][x].type == BlockType::AIR) continue;
			
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle bodyRectTemp = bodyRect;

			// Test x direction
			bodyRectTemp.x += body.velocity.x * dt;
			bool collideX = CheckCollisionRecs(blockRect, bodyRectTemp);
			if (collideX) {
				bodyRectTemp.x -= body.velocity.x * dt;
				if (body.velocity.x < 0.0f) {
					body.position.x = blockRect.x + blockRect.width;
				}
				else {
					body.position.x = blockRect.x - body.size.x;
				}
				body.velocity.x = 0.0f;
			}
		}
	}

	for (i64 y = y0; y < y1; y++) {
		for (i64 x = x0; x < x1; x++) {
			if (world.data[y][x].type == BlockType::AIR) continue;
			
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle bodyRectTemp = bodyRect;

			// Test y direction
			bodyRectTemp.y += body.velocity.y * dt;
			bool collideY = CheckCollisionRecs(blockRect, bodyRectTemp);
			if (collideY) {
				bodyRectTemp.y -= body.velocity.y * dt;
				if (body.velocity.y < 0.0f) {
					body.position.y = blockRect.y + blockRect.height;
				}
				else {
					body.position.y = blockRect.y - body.size.y;
				}

				body.velocity.y = 0.0f;
			}
		}
	}
}

void WorldBorderBodyCollide(World &world, Body &body, double dt) {
	// Safety checks
	i64 xToIndex = body.position.x / BLOCK_SIZE;
	i64 yToIndex = body.position.y / BLOCK_SIZE;
	i64 x0 = std::max(xToIndex - COLLISION_RANGE, 0i64);
	i64 x1 = std::min(xToIndex + COLLISION_RANGE, world.width);
	i64 y0 = std::max(yToIndex - COLLISION_RANGE, 0i64);
	i64 y1 = std::min(yToIndex + COLLISION_RANGE, world.height);

	Rectangle bodyRect = {body.position.x, body.position.y, body.size.x, body.size.y};
	
	for (i64 y = y0; y < y1; y++) {
		for (i64 x = x0; x < x1; x++) {
			if (world.data[y][x].type == BlockType::AIR) continue;
			
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle bodyRectTemp = bodyRect;

			// Test x direction
			bodyRectTemp.x += body.velocity.x * dt;
			bool collideX = CheckCollisionRecs(blockRect, bodyRectTemp);
			if (collideX) {
				bodyRectTemp.x -= body.velocity.x * dt;
				if (body.velocity.x < 0.0f) {
					body.position.x = blockRect.x + blockRect.width;
				}
				else {
					body.position.x = blockRect.x - body.size.x;
				}
				body.velocity.x = 0.0f;
			}
		}
	}

	for (i64 y = y0; y < y1; y++) {
		for (i64 x = x0; x < x1; x++) {
			if (world.data[y][x].type == BlockType::AIR) continue;
			
			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
			Rectangle bodyRectTemp = bodyRect;

			// Test y direction
			bodyRectTemp.y += body.velocity.y * dt;
			bool collideY = CheckCollisionRecs(blockRect, bodyRectTemp);
			if (collideY) {
				bodyRectTemp.y -= body.velocity.y * dt;
				if (body.velocity.y < 0.0f) {
					body.position.y = blockRect.y + blockRect.height;
				}
				else {
					body.position.y = blockRect.y - body.size.y;
				}

				body.velocity.y = 0.0f;
			}
		}
	}
}