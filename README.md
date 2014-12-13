# swb
###[suckless,simple,simons] web browser

A minimal web browser based on webkitgtk.

## Current state
For a while I used this as my daily browser. However, webkitgtk have proven to be much
to unstable to actually use. If you decide to use this browser, prepare for segfaults from
webkitgtk.

## Why?
Because other minimal web browsers are not based on the webkit2 API.
The webkit2 API is where all the development takes place.

## Settings
Settings are changed by editing config.h, keys.h and events.h and rebuilding the browser.

One can also bind keys and events to execute scripts. See events.h and keys.h for examples.

## Ad-blocking?
I use an adblocking stylesheet and privoxy.

## Follow hints?!
This is a feature available in vimperator, pentadactyl and so on. It's great.

Default configuration works with the script from suckless surf. You could also code your own with javascript (and shell script if you'd like).
