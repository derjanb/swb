#pragma once
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

WebKitWebView *new_tab(Browser *);
void find(Browser *b);
void find_next(Browser *b);
void open_page(Browser *);
void tabopen_page(Browser *);
void close_tab(Browser *);
void go_forward(Browser *);
void go_back(Browser *);
