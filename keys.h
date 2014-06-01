#pragma once
#include "handlers.h"
#include "navigation.h"
#include "helpers.h"

typedef struct {
	unsigned int keyval;
	void (*func)(Browser *, char *);
	char *arg;
} Key;

Key keys[] = {
	{GDK_KEY_o, open_page, NULL},
	{GDK_KEY_t, tabopen_page, NULL},
	{GDK_KEY_q, close_tab, NULL},
	{GDK_KEY_f, go_forward, NULL},
	{GDK_KEY_d, go_back, NULL},
	{GDK_KEY_slash, find, NULL},
	{GDK_KEY_n, find_next, NULL},
	{GDK_KEY_Escape, set_command_mode, NULL},
	{GDK_KEY_i, set_input_mode, NULL},
	{GDK_KEY_g, follow_hints, NULL},
};
