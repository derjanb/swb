#define _GNU_SOURCE
#include <webkit2/webkit2.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sqlite3.h>
#include "helpers.h"
#include "handlers.h"
#include "swb.h"
#include "config.h"

void spawn_cmd(Browser *b, char *type)
{
	fifo_funcs fifo_func = (fifo_funcs)type;
	char *type_str = g_strdup_printf("%d", fifo_func), *cmd = NULL;
	switch(fifo_func)
	{
		case OPEN:
			cmd = READ_URL_CMD("Open:", type_str);
			break;
		case TABOPEN:
			cmd = READ_URL_CMD("Tabopen:", type_str);
			break;
		case FIND:
			cmd = READ_ANY_CMD("Find:", type_str);
			break;
		default:
			break;
	}
	if(cmd != NULL)
	{
		exec_sh(b, cmd);
		g_free(cmd);
	}
}

void exec_js(Browser *b, char *js)
{
	webkit_web_view_run_javascript(GET_CURRENT_WEB_VIEW(b), js, NULL, NULL, NULL);
}


void exec_js_file(Browser *b, char *js)
{
	char *script;
	if(g_file_get_contents(js, &script, NULL, NULL))
	{
		webkit_web_view_run_javascript(GET_CURRENT_WEB_VIEW(b), script, NULL, NULL, NULL);
	}
	g_free(script);
}

void exec_sh(Browser *b, char *sh)
{
	g_spawn_command_line_async(sh, NULL);
}
	
void set_input_mode(Browser *b, char *NOT_USED)
{
	b->mode = INPUT;
}

void set_command_mode(Browser *b, char *NOT_USED)
{
	b->mode = COMMAND;
}

void load_uri(WebKitWebView *wv, char *uri)
{
	if(uri!=NULL && strlen(uri)>=2)
	{
		char *uri_tmp = strstr(uri, "://")!=NULL ? g_strdup(uri) : g_strdup_printf("http://%s", uri);
		webkit_web_view_load_uri(wv, uri_tmp);
		g_free(uri_tmp);
	}
}

void set_tab_title(Browser *b, WebKitWebView *wv, const gchar *title)
{
	gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(b->notebook),
			GTK_WIDGET(wv),
			title);
}

bool save_history(Browser *b, const gchar *uri)
{
	sqlite3 *db;
	bool sucess = sqlite3_open(HISTORY_FILE, &db) == 0;
if(sucess)
{
		char sql[BUF_LEN];
		sprintf(sql, "CREATE TABLE IF NOT EXISTS history(url varchar primary key, hits int);"\
				"INSERT OR IGNORE INTO history VALUES(\"%s\", 0);"\
				"UPDATE history SET hits = hits + 1 WHERE \"%s\"=url", uri, uri);
		sucess &= SQLITE_OK == sqlite3_exec(db, sql, NULL, NULL, NULL);
	}
	sqlite3_close(db);
	return sucess;
}

WebKitWebView *new_web_view(Browser *b)
{
	WebKitWebView *wv = WEBKIT_WEB_VIEW(webkit_web_view_new_with_context(b->web_context));
	webkit_settings_set_user_agent(webkit_web_view_get_settings(wv),
			USER_AGENT);
	g_signal_connect(wv, "create", G_CALLBACK(create_signal_handler), b);
	g_signal_connect(wv, "decide_policy", G_CALLBACK(decide_policy_signal_handler), b);
	g_signal_connect(wv, "load_changed", G_CALLBACK(load_changed_signal_handler), b);
	g_signal_connect(wv, "notify::title", G_CALLBACK(update_title_signal_handler), b);
	return wv;
}
