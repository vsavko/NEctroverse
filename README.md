NEctroverse Alpha TBA!
========

NEctroverse is a recode of the game Ectroverse created by Maloeran.<br>
It natrually contains a lot of changes from the original code.

You can play the current Ectroverse at http://www.ectroverse.org<br>
You can find the original source code at https://github.com/ectroverse/evsource


To install the game:<br>
Edit evconfig.ini and change as required.<br>

Additional Librarys needed for components:
* build-essential -- needed, but you should already have that.
* libpng-dev -- needed to render the galaxy map.
--Below are Optional's
* libmysqlclient-dev -- needed for mySql addition.
* libgnutls-dev -- For HTTPs support.
* libcurl4-gnutls-dev -- For Facebook support.

Run:
* make 
* ./evserver

Yup, that's it...<br>
Map creation is embeded into the core.

NEctroverse can run in forked daemon mode, which does not require a shell.<br>
This does not require any extra librarys, and will save logs via syslog feature.

If port setting in evconfig.ini is invalid or blanked out the server will default to port 9990.

KNOWN BUGS - Very annoying and need to go:
* ... ?
