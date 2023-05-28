#pragma once

#define DGFX_VERSION 1.0

#include <graphx.h>
#include <tice.h>
#include <stdlib.h>
#include <string.h>

void dgfx_Sprite_FlipX(gfx_sprite_t* sprite, int x, int y);
void dgfx_TransparentSprite_FlipX(gfx_sprite_t* sprite, int x, int y);
void dgfx_Sprite_FlipY(gfx_sprite_t* sprite, int x, int y);
void dgfx_TransparentSprite_FlipY(gfx_sprite_t* sprite, int x, int y);

void dgfx_Sprite_FlipX_NoClip(gfx_sprite_t* sprite, int x, int y);
void dgfx_TransparentSprite_FlipX_NoClip(gfx_sprite_t* sprite, int x, int y);
void dgfx_Sprite_FlipY_NoClip(gfx_sprite_t* sprite, int x, int y);
void dgfx_TransparentSprite_FlipY_NoClip(gfx_sprite_t* sprite, int x, int y);

void dgfx_Sprite_Section(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_TransparentSprite_Section(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_Sprite_Section_FlipX(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_TransparentSprite_Section_FlipX(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_Sprite_Section_FlipY(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_TransparentSprite_Section_FlipY(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);

void dgfx_Sprite_Section_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_TransparentSprite_Section_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_Sprite_Section_FlipX_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_TransparentSprite_Section_FlipX_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_Sprite_Section_FlipY_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);
void dgfx_TransparentSprite_Section_FlipY_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y);