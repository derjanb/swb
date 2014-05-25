#define _XOPEN_SOURCE
#include <webkit2/webkit2.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sqlite3.h>
#include "helpers.h"
#include "swb.h"
#include "config.h"

char *read_url(char *buf, Browser *b)
{
	gchar *command = READ_URL_CMD(webkit_web_view_get_uri(GET_CURRENT_WEB_VIEW(b)));
	FILE *url_command_stream = popen(command, "r");
	g_free(command);

	char *url = NULL;

	if(url_command_stream != NULL)
	{
		strcpy(buf,"http://");
		if(fgets(buf+strlen("http://"), BUF_LEN-1-strlen("http://"), url_command_stream) !=NULL)
		{
			if(strncmp("http", buf+strlen("http://"), strlen("http")) == 0)
			{
				url = buf+strlen("http://");
			}
			else
			{
				url = buf;
			}
		}
	}
	pclose(url_command_stream);
	return url;
}

void set_tab_title(Browser *b, WebKitWebView *wv, const gchar *title)
{
	gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(b->notebook),
			GTK_WIDGET(wv),
			title);
}

bool save_history(Browser *b, const gchar *url)
{
	sqlite3 *db;
	bool sucess = sqlite3_open(HISTORY_FILE, &db) == 0;
	if(sucess)
	{
		char sql[BUF_LEN];
		sprintf(sql, "CREATE TABLE IF NOT EXISTS history(url varchar primary key, hits int);"\
				"INSERT OR IGNORE INTO history VALUES(\"%s\", 0);"\
				"UPDATE history SET hits = hits + 1 WHERE \"%s\"=url", url, url);
		sucess &= SQLITE_OK == sqlite3_exec(db, sql, NULL, NULL, NULL);
	}
	sqlite3_close(db);
	return sucess;
}
