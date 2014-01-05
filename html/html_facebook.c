
#define MAX_FB_STRING 4096

static bool is_https = false;
static char do_redi[MAXREDIRECT];

void init_string( CurlStringPtr curl_str ) {

curl_str->len = 0;
curl_str->ptr = malloc(curl_str->len+1);

if (curl_str->ptr == NULL) {
	critical( "malloc failure" );
}

curl_str->ptr[0] = '\0';

return;
}

size_t writefunc( void *ptr, size_t size, size_t nmemb, CurlStringPtr curl_str ) {
	size_t new_len = ( curl_str->len + size*nmemb );

curl_str->ptr = realloc(curl_str->ptr, new_len+1);
if (curl_str->ptr == NULL) {
	critical( "realloc failed" );
	return -3;
}
memcpy(curl_str->ptr+curl_str->len, ptr, size*nmemb);
curl_str->ptr[new_len] = '\0';
curl_str->len = new_len;

return size*nmemb;
}


void facebook_apptoken( char **token ) {
	int offset;
	char post[MAX_FB_STRING];
	CURL *curl;
	CURLcode res;

offset = 0;
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "client_id=%s", fbcfg.app_id );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&client_secret=%s", fbcfg.app_secret );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "%s", "&grant_type=client_credentials" );

curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		error( "Facebook Curl" );
	/* always cleanup */
	curl_easy_cleanup(curl);
	*token = strdup(curl_str.ptr);
	free(curl_str.ptr);
}


return;
}

void facebook_usertoken( FBTokenPtr token, char *code ) {
	int i, num, offset;
	char **split;
	char post[MAX_FB_STRING];
	CURL *curl;
	CURLcode res;

offset = 0;
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "client_id=%s", fbcfg.app_id );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&client_secret=%s", fbcfg.app_secret );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&redirect_uri=%s", do_redi );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&code=%s", code );

curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		error( "Facebook Curl" );

	/* always cleanup */
	curl_easy_cleanup(curl);
	split = str_split( curl_str.ptr, '&', &num );
		
	if ( split == NULL ) {
		info( "str_split returned NULL" );
	} else {
		for ( i = 0; i < num; i++ ) {
			if( strncmp( split[i], "access_token", strlen("access_token") ) == 0 ) {
				memcpy( token->val, (strstr( split[i], "=" )+1), sizeof(token->val) );
			} else if( strncmp( split[i], "expires", strlen("expires") ) == 0 ) {
				sscanf( (strstr( split[i], "=" )+1), "%d", &token->expire );
			}
		}
	}
	free( split );
	free(curl_str.ptr);
}

return;
}

void facebook_getdata( FBUserPtr fbdata, char *urlstring, int offset ) {
	CURL *curl;
	CURLcode res;

offset += snprintf( &urlstring[offset], (MAX_FB_STRING - offset), "&fields=%s", "id,gender,name,first_name,last_name,timezone,bio,picture,languages" );

curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, urlstring );
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		error( "Facebook Curl" );

	/* always cleanup */
	curl_easy_cleanup(curl);

	cJSON *root = cJSON_Parse(curl_str.ptr);
	if( root ) {
		cJSON *message;
		
		message = cJSON_GetObjectItem(root,"languages");
		fbdata->connected = ( message ) ? true : false;
		
		message = cJSON_GetObjectItem(root,"timezone");
		if( ( message ) ) {
			fbdata->timezone = message->valuedouble;
		}
		
		message = cJSON_GetObjectItem(root,"id");
		if( ( message ) ) {
			strncpy( fbdata->id, message->valuestring, sizeof( fbdata->id ) );
		}
		
		message = cJSON_GetObjectItem(root,"gender");
		if( ( message ) ) {
			strncpy( fbdata->gender, message->valuestring, sizeof( fbdata->gender ) );
		}
		
		message = cJSON_GetObjectItem(root,"name");
		if( ( message ) ) {
			strncpy(fbdata->full_name, message->valuestring, sizeof( fbdata->full_name ) );
		}
		
		message = cJSON_GetObjectItem(root,"first_name");
		if( ( message ) ) {
			strncpy(fbdata->first_name, message->valuestring, sizeof( fbdata->first_name ) );
		}
		
		message = cJSON_GetObjectItem(root,"last_name");
		if( ( message ) ) {
			strncpy(fbdata->last_name, message->valuestring, sizeof( fbdata->last_name ) );
		}
		
		message = cJSON_GetObjectItem(root,"bio");
		if( ( message ) ) {
			strncpy(fbdata->bio, message->valuestring, sizeof( fbdata->last_name ) );
		}
		
		message = cJSON_GetObjectItem(root,"picture");
		if( ( message ) ) {
			message = cJSON_GetObjectItem(message,"data");
			if( ( message ) ) {
				message = cJSON_GetObjectItem(message,"url");
				if( ( message ) ) {
					strncpy(fbdata->pic, message->valuestring, sizeof( fbdata->pic ) );
				}
			}
		}
		
	} else {
		error( "Bad Responce" );
	}
	cJSON_Delete(root);
	//printf( curl_str.ptr );
	free(curl_str.ptr);
}



return;
}


void facebook_getdata_token( FBUserPtr fbdata, FBTokenDef token ) {
	int offset;
	char post[MAX_FB_STRING];

/*
offset = 0;
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "%s", "grant_type=fb_exchange_token" );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&client_id=%s", fbcfg.app_id );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&client_secret=%s", fbcfg.app_secret );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&fb_exchange_token=%s", token );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&redirect_uri=%s", do_redi );
*/


offset = 0;
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "%s", "https://graph.facebook.com/me" );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "?access_token=%s", token.val );

facebook_getdata( fbdata, post, offset );

	
return;
}	


void facebook_getdata_id( FBUserPtr fbdata, char *userid ) {
	int offset;
	char post[MAX_FB_STRING];

offset = 0;
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "https://graph.facebook.com/%s", userid  );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "?%s", fbcfg.app_token );

facebook_getdata( fbdata, post, offset );

	
return;
}


int facebook_post_notice( char *fbid, char *fmt, ... ) {
	bool result = false;
	int offset;
	char post[MAX_FB_STRING];
	char template[MAX_FB_STRING];
	CURL *curl;
	CURLcode res;
	va_list ap;

va_start( ap, fmt );
vsnprintf( template, MAX_FB_STRING, fmt, ap );
va_end( ap );

offset = 0;
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "%s", fbcfg.app_token );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "&template=%s", template );

curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	sprintf( template, "https://graph.facebook.com/%s/notifications", fbid  );
	curl_easy_setopt(curl, CURLOPT_URL, template);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		error( "Facebook Curl" );

	/* always cleanup */
	curl_easy_cleanup(curl);
	result = ( strcmp( curl_str.ptr, "true" ) == 0 ) ? true : false;
	free(curl_str.ptr);
}

return result;
}

int facebook_unlink_app( char *userid ) {
	bool result = false;
	int offset;
	char post[MAX_FB_STRING];
	CURL *curl;
	CURLcode res;

offset = 0;
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "https://graph.facebook.com/%s/permissions", userid  );
offset += snprintf( &post[offset], (MAX_FB_STRING - offset), "?%s", fbcfg.app_token );

curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, post );
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		error( "Facebook Curl" );

	/* always cleanup */
	curl_easy_cleanup(curl);
	result = ( strcmp( curl_str.ptr, "true" ) == 0 ) ? true : false;
	free(curl_str.ptr);
}

	
return result;
}

void iohtmlFunc_facebook( ReplyDataPtr cnt ) {
	int a, id, offset = 0;
	char *error, *remove, *host;
	char *code, *fbtoke;
	char buffer[1024];
	char temp[2][32];
	dbUserPtr user;
	dbUserInfoDef infod;
	FBUserDef fbdata;
	FBTokenDef token;

if( !(strlen(fbcfg.app_id)) || !(strlen(fbcfg.app_secret)) )
	goto BAILOUT;

host = (char *)MHD_lookup_connection_value( cnt->connection, MHD_HEADER_KIND, "Host" );

#if HTTPS_SUPPORT
sprintf(temp[0], "%d", options.port[PORT_HTTP]);
sprintf(temp[1], "%d", options.port[PORT_HTTPS]);
is_https = strstr( host, temp[1] ) ? true : ( strstr( host, temp[0] ) ? false : true );
#endif

snprintf( do_redi, sizeof( do_redi ), "%s://%s/facebook", (is_https ? "https" : "http"), host  );

memset( &fbdata, 0, sizeof(FBUserDef) );
memset( &token, 0, sizeof(FBTokenDef) );
memset( &buffer, 0, sizeof(buffer) );

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_FACEBOOK );

error = iohtmlVarsFind( cnt, "error" );
code = iohtmlVarsFind( cnt, "code" );
fbtoke = iohtmlVarsFind( cnt, "fblogin_token" );
remove = iohtmlVarsFind( cnt, "remove" );

if( ( code ) || ( fbtoke ) ){
	if( code )
		facebook_usertoken( &token, code );
	else
		strncpy( token.val, fbtoke, sizeof( token.val ) );
	if( token.val ) {
		facebook_getdata_token( &fbdata, token );
		if( !( fbdata.id ) ) {
			error( "No ID in Responce" );
			goto BAILOUT;
		}
	} else {
		offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s", "Invalid Login, Did you just reload this page?" );
	}
} else if( error) {
	offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s", "Error Detected<br>" );
	offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s<br>", error );
	offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s<br>", iohtmlVarsFind( cnt, "error_reason" ) );
	offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s<br>", iohtmlVarsFind( cnt, "error_description" ) );
} else {
	httpString( cnt, "Invalid" );
	goto BAILOUT;
}


id = dbUserFBSearch( fbdata.id );

if( buffer[0] )
	httpString( cnt, buffer );

if( id >= 0 ) {
	user = dbUserLinkID( id );
	if( dbUserLinkDatabase( cnt, id ) < 0 )
		goto BAILOUT;

	if( dbSessionSet( (cnt->session)->dbuser, (cnt->session)->sid ) < 0 )
		goto BAILOUT;

	dbUserInfoRetrieve( id, &infod );
	infod.lasttime = time( 0 );
	if( (cnt->connection)->addr->sa_family == AF_INET )
	for( a = (MAXIPRECORD-2); a >= 0 ; a-- ) {
		if( strcmp(inet_ntoa( infod.sin_addr[a] ),"0.0.0.0") ) {
			memcpy( &(infod.sin_addr[a+1]), &(infod.sin_addr[a]), sizeof(struct in_addr) );
		}
	}
	memcpy( &(infod.sin_addr[0]), &(((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr), sizeof(struct in_addr) );
	fbdata.token = token;
	infod.fbinfo = fbdata;
	dbUserInfoSet( id, &infod );
	redirect( cnt, "/" );
	httpPrintf( cnt, "<b>Welcome <i>%s</i></b><br><br>", user->faction );
	httpString( cnt, "You should be redirected back to the main screen shortly<br>" );
	httpString( cnt, "<a href=\"/\">Click here if it takes too long</a><br>" );
} else if( fbdata.id[0] ) {
	if( remove ) {
		goto REMOVELINK;
	} else {
		goto LINKWITHFB;
	}
} else {
	redirect( cnt, "/" );
	httpString( cnt, "Invalid Request, reloading to main screen.<br>" );
	httpString( cnt, "<a href=\"/\">Click here if it takes too long</a><br>" );
}

goto BAILOUT;

LINKWITHFB:

if( ( (cnt->session)->dbuser ) && ( user = (cnt->session)->dbuser ) ) {
	bitflag_add( &user->flags, cmdUserFlags[CMD_USER_FLAGS_FBLINK] );
	strcpy( user->fbid, fbdata.id );
	dbUserSave( user->id, user );
	dbUserInfoRetrieve( user->id, &infod );
	fbdata.token = token;
	infod.fbinfo = fbdata;
	dbUserInfoSet( user->id, &infod );
	facebook_post_notice( fbdata.id, "Welcome @[%s]\nYou have linked with user: %s\nThanks for deciding to join our game...\nWe hope that you will enjoy it.", fbdata.id, user->name );
	redirect( cnt, "/main?page=account" );
	httpPrintf( cnt, "<b>Facebook ID %s now linked with User %s</b><br><br>", user->fbid, user->name );
	httpString( cnt, "You should be redirected back to your account screen shortly<br>" );
	httpString( cnt, "<a href=\"/main?page=account\">Click here if it takes too long</a><br>" );
} else {
	httpString( cnt, "<script type=\"text/javascript\">" );
	httpString( cnt, "$(document).ready(function(){" );
	httpString( cnt, "$(\"select\").change(function(){" );
	httpString( cnt, "$( \"select option:selected\").each(function(){" );
	httpString( cnt, "if($(this).attr(\"value\")==\"login\"){" );
	httpString( cnt, "$(\".hidebox\").hide();" );
	httpString( cnt, "$(\".login\").show();" );
	httpString( cnt, "}" );
	httpString( cnt, "if($(this).attr(\"value\")==\"create\"){" );
	httpString( cnt, "$(\".hidebox\").hide();" );
	httpString( cnt, "$(\".create\").show();" );
	httpString( cnt, "}" );
	httpString( cnt, "if($(this).attr(\"value\")==\"remove\"){" );
	httpString( cnt, "$(\".hidebox\").hide();" );
	httpString( cnt, "$(\".remove\").show();" );
	httpString( cnt, "}" );
	httpString( cnt, "});" );
	httpString( cnt, "}).change();" );
	httpString( cnt, "});" );
	httpString( cnt, "</script>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "You have authorised this game to access your Facebook data<br>" );
	httpString( cnt, "However no account was found for this ID... what would you like to do?<br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "<div><select>" );
	httpString( cnt, "<option>Select Option...</option>" );
	httpString( cnt, "<option value=\"create\">Create New Account</option>" );
	httpString( cnt, "<option value=\"login\">Login to Existing</option>" );
	httpString( cnt, "<option value=\"remove\">Remove Game from Facebook</option>" );
        httpString( cnt, "</select></div>" );

	httpString( cnt, "<div class=\"login hidebox\">" );
	httpString( cnt, "<form action=\"/login\" method=\"POST\">" );
	httpPrintf( cnt, "<input type=\"hidden\" name=\"fblogin_token\" value=\"%s\">", token.val );
	httpString( cnt, "Name<br><input type=\"text\" name=\"name\"><br>" );
	httpString( cnt, "<br>Password<br><input type=\"password\" name=\"pass\"><br>" );
	httpString( cnt, "<br><input type=\"submit\" value=\"OK\"></form>" );
	httpString( cnt, "</div>" );

	httpString( cnt, "<div class=\"create hidebox\">" );
	httpString( cnt, "<form action=\"/register\" method=\"POST\">" );
	httpPrintf( cnt, "<input type=\"hidden\" name=\"fblogin_token\" value=\"%s\">", token.val );
	httpString( cnt, "Please choose a Faction name<br>" );
	httpString( cnt, "<input type=\"text\" name=\"Faction\"><br>" );
	httpString( cnt, "(This is what others will see you as in-game)<br>" );
	httpString( cnt, "<br><input type=\"submit\" value=\"Proceed\"></form>" );
	httpString( cnt, "</div>" );

	httpString( cnt, "<div class=\"remove hidebox\">" );
	httpString( cnt, "<form action=\"/facebook\" method=\"POST\">" );
	httpPrintf( cnt, "<input type=\"hidden\" name=\"fblogin_token\" value=\"%s\">", token.val );
	httpPrintf( cnt, "<input type=\"hidden\" name=\"remove\" value=\"%s\">", fbdata.id );
	httpString( cnt, "<input type=\"submit\" value=\"Remove Facebook Permissions\"></form>" );
	httpString( cnt, "</div>" );
}
goto BAILOUT;

REMOVELINK:

if( facebook_unlink_app( fbdata.id ) ) {
	httpString( cnt, "As requested, your link with Facebook has been removed.<br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "<a href=\"https://www.facebook.com\">Goto Facebook</a> - <a href=\"http://www.google.com\">Goto Google<br>" );
} else {
	httpString( cnt, "Oh dear, Facebook didn't respond as we'd expect...<br>" );
	httpString( cnt, "You'll have to manually check to ensure permissions were removed." );
}

BAILOUT:

iohtmlFunc_endhtml( cnt );

return;
}

void iohtmlFBConnect( ReplyDataPtr cnt ) {
	char *host;
	#if HTTPS_SUPPORT
	char temp[2][32];
	#endif

if( ( strlen( fbcfg.app_id ) <= 0 ) || ( strlen( fbcfg.app_secret ) <= 0 ) )
	return;

if( ( !( (cnt->session)->dbuser ) || ( ((cnt->session)->dbuser) && !( bitflag( ((cnt->session)->dbuser)->flags, cmdUserFlags[CMD_USER_FLAGS_FBLINK] ) )) ) ) {

	httpString( cnt, "<form action=\"https://www.facebook.com/dialog/oauth\" method=\"GET\" target=\"_top\">" );
	httpPrintf( cnt, "<input type=\"hidden\" name=\"client_id\" value=\"%s\">", fbcfg.app_id );

	host = (char *)MHD_lookup_connection_value( cnt->connection, MHD_HEADER_KIND, "Host" );

	#if HTTPS_SUPPORT
	sprintf(temp[0], "%d", options.port[PORT_HTTP]);
	sprintf(temp[1], "%d", options.port[PORT_HTTPS]);
	is_https = strstr( host, temp[1] ) ? true : ( strstr( host, temp[0] ) ? false : true );
	#endif

	sprintf( logString, "%s://%s/facebook", (is_https ? "https" : "http"), host  );

	httpPrintf( cnt, "<input type=\"hidden\" name=\"redirect_uri\" value=\"%s\">", logString );
	httpString( cnt, "<input type=\"image\" src=\"files?type=image&name=facebook.gif\" alt=\"Facebook Connect\">" );
	httpString( cnt, "</form>" );
} else if ( ((cnt->session)->dbuser) && ( bitflag( ((cnt->session)->dbuser)->flags, cmdUserFlags[CMD_USER_FLAGS_FBLINK] ) ) ) {
	httpString( cnt, "<b>This Account is linked with Facebook.</b><br>" );
}


return;
}

void facebook_update_user( dbUserPtr user ) {
	dbUserInfoDef infod;
	FBUserDef fbdata;

if( ( user ) && bitflag( user->flags, cmdUserFlags[CMD_USER_FLAGS_FBLINK] ) ) {
	facebook_getdata_id( &fbdata, user->fbid );
	fbdata.updated = *gettime( time(0), false );
	if( !( fbdata.connected ) ) {
		memset( &user->fbid, 0, sizeof(user->fbid) );
		bitflag_remove( &user->flags, cmdUserFlags[CMD_USER_FLAGS_FBLINK] );
		dbUserSave( user->id, user );
	} else {
		dbUserInfoRetrieve( user->id, &infod );
		infod.fbinfo = fbdata;
		dbUserInfoSet( user->id, &infod );	
	}
}

return;
}

void iohtmlFBSDK( ReplyDataPtr cnt ) {

httpString( cnt, "<div id=\"fb-root\"></div>" );
httpString( cnt, "<script>" );
httpString( cnt, "window.fbAsyncInit=function(){" );
httpString( cnt, "FB.init({" );
httpString( cnt, "appId:'110861965600284'," );
httpString( cnt, "status:true," );
httpString( cnt, "cookie:true," );
httpString( cnt, "xfbml:true" );
httpString( cnt, "});" );
/*
httpString( cnt, "FB.Event.subscribe('auth.authResponseChange',function(response){" );
httpString( cnt, "if(response.status==='connected'){" );
httpString( cnt, "redirect_forlink();" );
httpString( cnt, "}" );
httpString( cnt, "});" );
httpString( cnt, "};" );
*/
httpString( cnt, "(function(d){" );
httpString( cnt, "var js,id='facebook-jssdk',ref=d.getElementsByTagName('script')[0];" );
httpString( cnt, "if(d.getElementById(id)){return;}" );
httpString( cnt, "js=d.createElement('script');js.id=id;js.async=true;" );
httpString( cnt, "js.src=\"//connect.facebook.net/en_US/all.js\";" );
httpString( cnt, "ref.parentNode.insertBefore(js,ref);" );
httpString( cnt, "}(document));" );
/*
httpString( cnt, "function redirect_forlink(){" );
httpString( cnt, "location.href=\"/facebook?fblogin_token=\"+FB.getAuthResponse()['accessToken'];" );
httpString( cnt, "}" );
*/
httpString( cnt, "</script>" );

return;
}


