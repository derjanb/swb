#pragma once
#include <webkit2/webkit2.h>
#include "swb.h"

void spawn_cmd(Browser *, char *);
void load_uri(WebKitWebView *, char *);
void exec_js_file(Browser *, char *);
void exec_js(Browser *, char *);
void exec_sh(Browser *, char *);
void set_command_mode(Browser *, char *);
void set_input_mode(Browser *, char *);
char *read_user_input(char *);
void set_tab_title(Browser *, WebKitWebView *, const gchar *);
bool save_history(Browser *, const gchar *);
WebKitWebView *new_web_view(Browser *);

#define GET_CURRENT_WEB_VIEW(browser) \
	WEBKIT_WEB_VIEW(\
			gtk_notebook_get_nth_page(GTK_NOTEBOOK(browser->notebook),\
			gtk_notebook_get_current_page(GTK_NOTEBOOK(browser->notebook))))

#define DEBUGARG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)
#define DEBUG(format) fprintf(stderr, format "\n")
