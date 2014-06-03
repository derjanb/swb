#pragma once
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

WebKitWebView *new_tab(Browser *);
void find(Browser *, char *);
void find_next(Browser *, char *NOT_USED);
void open_page(Browser *, char *);
void tabopen_page(Browser *, char *);
void close_tab(Browser *, char *NOT_USED);
void go_forward(Browser *, char *NOT_USED);
void go_back(Browser *, char *NOT_USED);
void follow_hint(Browser *, char *);
void enable_follow_hints(Browser *, char *NOT_USED);
