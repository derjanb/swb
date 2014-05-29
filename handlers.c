#define _XOPEN_SOURCE
#include <webkit2/webkit2.h>
#include "swb.h"
#include "helpers.h"
#include "handlers.h"
#include "config.h"
#include "navigation.h"
#include "keys.h"
#include "events.h"

gboolean key_press_event_handler(GtkWidget *window,
		GdkEvent *event,
		gpointer user_data)
{
	DEBUG("key_press_event_signal_handler entered");

	Browser *b = (Browser *)user_data;
	gboolean handled = FALSE;

	guint keyval;

	gdk_event_get_keyval(event, &keyval);

	if(b->mode == COMMAND || keyval == GDK_KEY_Escape)
	{
		for(int i = 0; i < sizeof(keys)/sizeof(Key); ++i)
		{
			if(keyval == keys[i].keyval)
			{
				handled = TRUE;
				keys[i].func(b, keys[i].arg);
			}
		}
	}
	DEBUGARG("key_press_event_signal_handler returns %d", handled);
	return handled;
}

GtkWidget* create_signal_handler(WebKitWebView *web_view,
		gpointer user_data)
{
	DEBUG("create_signal_handler entered");

	Browser *b=(Browser *)user_data;

	WebKitWebView *wv = new_tab(b);

	DEBUG("create_signal_handler returns");
	return GTK_WIDGET(wv);
}

gboolean decide_policy_signal_handler(WebKitWebView *web_view,
		WebKitPolicyDecision *decision,
		WebKitPolicyDecisionType type,
		gpointer user_data)
{
	DEBUG("decide_policy_signal_handler entered");

	gboolean handled = FALSE;

	switch(type)
	{
		case WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION:
			webkit_policy_decision_use(decision);
			handled = TRUE;
			break;
		default:
			break;
	}
	DEBUGARG("decide_policy_signal_handler returns: %d", handled);
	return handled;
}

void update_title_signal_handler(GObject *gobj, GParamSpec *p, gpointer user_data)
{
	Browser *b = (Browser *)user_data;
	WebKitWebView *wv = (WebKitWebView *)gobj;
	if(webkit_web_view_get_title(wv)!=NULL)
	{
		set_tab_title(b, wv, webkit_web_view_get_title(wv));
	}
}



void load_changed_signal_handler(WebKitWebView *wv,
		WebKitLoadEvent load_event,
		gpointer user_data)
{
	DEBUG("load_changed_signal_handler entered");

	Browser *b = (Browser *)user_data;

	for(int i=0; i < sizeof(events)/sizeof(Event); ++i)
	{
		events[i].func(b, events[i].arg);
	}

	switch (load_event)
	{
		case WEBKIT_LOAD_STARTED:
			{
				set_tab_title(b, wv, "Loading");
				break;
			}
		case WEBKIT_LOAD_FINISHED:
			{
				const char *uri = webkit_web_view_get_uri(wv);
				save_history(b, uri);
				break;
			}
		default:
			{
				break;
			}
	}

	DEBUG("load_changed_signal_handler returned");
}
