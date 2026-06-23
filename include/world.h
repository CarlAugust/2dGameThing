#pragma once

#include <vector>
#include <common.h>

enum class BlockType {
    AIR,
    DIRT,

    // ----------------------------------
    BLOCKCOUNT // IMPORTANT TO KEEP LAST
};

struct Block {
    BlockType type;
};

struct World {
    std::vector<std::vector<Block>> data;
    i64 width = 0;
    i64 height = 0;
    i64 realWidth = 0;
    i64 realHeight = 0;
};

void InitBlockRegister();

void WorldBorderBodyCollide(World &world, Body &body, double dt);
void WorldBodyCollide(World &world, Body &body, double dt);
void DrawWorldInCamera(World &world, Camera2D& camera);

