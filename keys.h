#pragma once
#include "handlers.h"
#include "config.h"
#include "navigation.h"
#include "helpers.h"

typedef struct {
	unsigned int keyval;
	void (*func)(Browser *, char *);
	char *arg;
} Key;

Key keys[] = {
	{GDK_KEY_o, spawn_cmd, (char *) OPEN},
	{GDK_KEY_t, spawn_cmd, (char *) TABOPEN},
	{GDK_KEY_q, close_tab, NULL},
	{GDK_KEY_f, go_forward, NULL},
	{GDK_KEY_d, go_back, NULL},
	{GDK_KEY_slash, spawn_cmd, (char *) FIND},
	{GDK_KEY_n, find_next, NULL},
	{GDK_KEY_Escape, set_command_mode, NULL},
	{GDK_KEY_i, set_input_mode, NULL},
	{GDK_KEY_e, enable_follow_hints, NULL},
	{GDK_KEY_k, exec_js, "window.scrollBy(0,-100);"},
	{GDK_KEY_j, exec_js, "window.scrollBy(0,100);"},
	{GDK_KEY_h, exec_js, "window.scrollBy(-100,0);"},
	{GDK_KEY_l, exec_js, "window.scrollBy(100,0);"}
};
