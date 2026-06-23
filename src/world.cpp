#include <world.h>
#include <raylib.h>
#include <common.h>
#include <string>

struct BlockData {
    std::string name;
    Texture2D tex;
};

BlockData blockRegister[static_cast<u64>(BlockType::BLOCKCOUNT)];

void registerBlock(BlockType t, std::string name) {
    u64 typeIndex = static_cast<u64>(t);
    blockRegister[typeIndex].name = name;

    Texture2D tex = LoadTextureSafe(TextFormat("assets/textures/%s.png", name));
    blockRegister[typeIndex].tex = tex;
}

void InitBlockRegister() {
    registerBlock(BlockType::DIRT, "dirt");
}

Texture2D GetBlockTexture(BlockType t) {
    u64 typeIndex = static_cast<u64>(t);
    return blockRegister[typeIndex].tex;
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
			if (world.data[y][x].type == BlockType::AIR) continue;

			Rectangle blockRect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}; 
            DrawTextureEx(GetBlockTexture(world.data[y][x].type), {x * BLOCK_SIZE, y * BLOCK_SIZE}, 0.0f, 1.0f, WHITE);
		}
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