#pragma once
#include <cstdint>
// プレイヤー陣営
const uint32_t kCollitionAttributePlayer = 0b1;
const uint32_t kCollitionAttributePlayerBullet = 0b1 << 2;
    // 敵陣営
const uint32_t kCollitionAttributeEnemy = 0b1 << 1;