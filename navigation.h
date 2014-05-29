#pragma once
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

WebKitWebView *new_tab(Browser *);
void find(Browser *b, char *NOT_USED);
void find_next(Browser *b, char *NOT_USED);
void open_page(Browser *, char *NOT_USED);
void tabopen_page(Browser *, char *NOT_USED);
void close_tab(Browser *, char *NOT_USED);
void go_forward(Browser *, char *NOT_USED);
void go_back(Browser *, char *NOT_USED);
