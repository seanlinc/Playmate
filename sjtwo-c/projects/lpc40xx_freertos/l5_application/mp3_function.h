#pragma once

#include <stdbool.h>
#include <stdio.h>

void mp3__play();
void mp3__pause();
void mp3__next();
void mp3__previous();

void mp3__increase_volume();
void mp3__decrease_volume();

void mp3__increase_bass();
void mp3__decrease_bass();

void mp3__increase_trele();
void mp3__decrease_treble();

bool mp3__is_mp3_file(const char *file_name);