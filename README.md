NEctroverse Alpha in Testing!
========

NEctroverse is a recode of the game Ectroverse created by Maloeran.<br>
It natrually contains a lot of changes from the original code.

You can help test the current version at http://www.nectroverse.net<br>
You can find the original source code at https://github.com/EV-EVA/Ectroverse


To install the game:<br>
Edit config/buildflags.h to ajust optinal addins.<br>
Edit config/evconfig.ini and change as required.<br>

Additional Librarys needed for components:
* build-essential -- needed, but you should already have that.
* libpng-dev -- needed to render the galaxy map.
--Below are Optional's
* libmysqlclient-dev -- needed for mySql addition.
* libgnutls-dev -- For HTTPs support.
* libcurl4-gnutls-dev -- For Facebook support.

New addition, mail authentication!
*************************************
https://askubuntu.com/questions/522431/how-to-send-an-email-using-command-line

First of all you need to install and configure Postfix to Use Gmail SMTP on Ubuntu.

Install all necessary packages:

$ sudo apt-get install postfix mailutils libsasl2-2 ca-certificates libsasl2-modules
If you do not have postfix installed before, postfix configuration wizard will ask you some questions. Just select your server as Internet Site and for FQDN use something like mail.example.com

Then open your postfix config file:

$ sudo -H gedit /etc/postfix/main.cf
and add following lines to it:

relayhost = [smtp.gmail.com]:587
smtp_sasl_auth_enable = yes
smtp_sasl_password_maps = hash:/etc/postfix/sasl_passwd
smtp_sasl_security_options = noanonymous
smtp_tls_CAfile = /etc/postfix/cacert.pem
smtp_use_tls = yes
You might have noticed that we haven’t specified our Gmail username and password in above lines. They will go into a different file. Open/Create:

$ sudo -H gedit /etc/postfix/sasl_passwd
And add following line:

[smtp.gmail.com]:587    USERMAIL@gmail.com:PASSWORD
If you want to use your Google App’s domain, please replace @gmail.com with your @domain.com.

Fix permission and update postfix config to use sasl_passwd file:

$ sudo chmod 400 /etc/postfix/sasl_passwd
$ sudo postmap /etc/postfix/sasl_passwd
Next, validate certificates to avoid running into error. Just run following command:

$ cat /etc/ssl/certs/Thawte_Premium_Server_CA.pem | sudo tee -a /etc/postfix/cacert.pem
Finally, reload postfix config for changes to take effect:

$ sudo /etc/init.d/postfix reload
*************************************

Run:
* make 
* ./evserver

Yup, that's it...<br>
Map creation is embeded into the core.

If an invalid config is found, you will recive a shell notice.<br>
If no file exists, the game will create a blank template for your ajustment.

NEctroverse can run in forked daemon mode, which does not require a shell.<br>
This does not require any extra librarys, and will save logs via syslog feature.

KNOWN BUGS - Very annoying and need to go:
* Facebook linkage is unstable.
* ... Possibly more
* ... Join and help find them all!
* .... ?
