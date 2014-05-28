#pragma once
typedef struct 
{
	GtkWidget *window;
	GtkWidget *notebook;
	WebKitWebContext *web_context;
	enum {INPUT, COMMAND} mode;
} Browser;

void setup_browser(Browser *b);
