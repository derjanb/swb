#pragma once
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

WebKitWebView *new_tab(Browser *);
bool open_page(Browser *);
bool tabopen_page(Browser *);
bool close_tab(Browser *);
bool go_forward(Browser *);
bool go_back(Browser *);
