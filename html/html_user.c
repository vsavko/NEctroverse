


void iohtmlFunc_account( ReplyDataPtr cnt ) {
	int a, id, race_change_time = 144;
	char *faction, *race, *desc, *block;
	char description[USER_DESC_MAX];
	dbUserMainDef maind;
	dbUserInfoDef infod;
	int tmp;

faction = iohtmlVarsFind( cnt, "newname" );
race = iohtmlVarsFind( cnt, "race" );
desc = iohtmlVarsFind( cnt, "desc" );
block = iohtmlVarsFind( cnt, "block" );

iohtmlBase( cnt, 1 );

if( ( id = iohtmlIdentify( cnt, 1 ) ) < 0 )
	return;
if( !( iohtmlHeader( cnt, id, &maind ) ) )
	return;

#if FACEBOOK_SUPPORT
iohtmlFBSDK( cnt );
#endif

if( !( dbUserInfoRetrieve( id, &infod ) ) ) {
	error( "Error in user html info, getting real info" );
	return;
}

iohtmlBodyInit( cnt, "Account Options" );

if( !( ticks.status | ticks.number ) ) {
	if( faction ) {
		if( cmdExecChangeName( id, faction ) < 0 ) {
			if( cmdErrorString )
				httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
			else
				httpString( cnt, "<i>Error encountered while changing faction name</i><br><br>" );
		} else {
			httpString( cnt, "<i>Faction name has been changed</i><br><br>" );
			dbUserMainRetrieve( id, &maind );
		}
	}
	if( race ) {
		if( ( sscanf( race, "%d", &a ) == 1 ) && ( a < CMD_RACE_NUMUSED ) ) {
			maind.raceid = a;
			httpString( cnt, "<i>Race changed</i><br><br>" );
			dbUserMainSet( id, &maind );
		}
	}
}

if( ( ticks.status | ticks.number ) && (ticks.number - maind.benefactor[3] >= race_change_time) && roundspec_define[ROUNDSPEC_RACECHANGE] == 1) {

	if( race ) {
		if( ( sscanf( race, "%d", &a ) == 1 ) && ( a < CMD_RACE_NUMUSED ) ) {
			maind.raceid = a;
			httpString( cnt, "<i>Race changed</i><br><br>" );
			maind.benefactor[3] = ticks.number;
			dbUserMainSet( id, &maind );
		}
	}
}

if( desc ) {
	iohttpForumFilter( description, desc, USER_DESC_MAX, 0 );
	iohttpForumFilter2( infod.desc, description, USER_DESC_MAX );
	if( dbUserInfoSet( id, &infod ) )
      		httpString( cnt, "<i>Description Updated</i><br><br>" );
	else
		httpString( cnt, "<i>Error encountered while changing Description</i><br><br>" );
}

if( block ) {
	if (dbUserLinkID(atoi(block)) == NULL)
		httpString( cnt, "<i>Player with this ID doesn't exist!</i><br><br>" );
	else{
		if( (tmp = dbUserAddToBlockedList( id, atoi(block) )) >= 0 )
			httpPrintf( cnt, "<i>User %d %s</i><br><br>", atoi(block), (tmp == 1 ) ?"blocked" : "unblocked" );
		else
			httpString( cnt, "<i>Error encountered while adding a user to the blocked list</i><br><br>" );
	}
}

httpString( cnt, "<table border=\"0\"><tr><td>" );
httpPrintf( cnt, "Account ID : <b>%d</b><br>", id );
#if FACEBOOK_SUPPORT
if( !( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBMADE ) ) )
	httpPrintf( cnt, "User name : <b>%s</b><br>", ((cnt->session)->dbuser)->name );
#else
httpPrintf( cnt, "User name : <b>%s</b><br>", ((cnt->session)->dbuser)->name );
#endif
httpPrintf( cnt, "User e-mail : <b>%s</b><br>", ((cnt->session)->dbuser)->email );
httpPrintf( cnt, "Faction name : <b>%s</b><br>", maind.faction );
httpPrintf( cnt, "Faction race : <b>%s</b><br>", cmdRaceName[maind.raceid] );
httpPrintf( cnt, "Tag points : <b>%d</b><br>", infod.tagpoints );

httpPrintf( cnt, "Forum tag : <b>%s</b><br>", infod.forumtag );

if( !( ticks.status | ticks.number ) ) {
	httpString( cnt, "<br>Change Faction name<br><i>Only available before time starts.</i><br>" );
	httpPrintf( cnt, "<form action=\"%s\"><input type=\"text\" name=\"newname\" size=\"32\" value=\"%s\"><input type=\"submit\" value=\"Change\"></form>", URLAppend( cnt, "account" ), maind.faction );
	httpPrintf( cnt, "<form action=\"%s\">Faction race - ", URLAppend( cnt, "account" ) );
	httpPrintf( cnt, "<a href=\"%s&type=races\">See races</a><br><i>Only available before time starts.</i><br><select name=\"race\">", URLAppend( cnt, "info" ) );

	for( a = 0 ; a < CMD_RACE_NUMUSED ; a++ ) {
		httpPrintf( cnt, "<option value=\"%d\"", a );
		if( a == maind.raceid )
			httpString( cnt, " selected" );
		httpPrintf( cnt, ">%s", cmdRaceName[a] );
	}
	httpString( cnt, "</select><input type=\"submit\" value=\"Change\"></form>" );
}

if( ticks.status | ticks.number && roundspec_define[ROUNDSPEC_RACECHANGE] == 1) {
	httpString( cnt, "<br><b>Round special - change faction race</b><br><i>Available every 24h.</i><br>" );
	if  (ticks.number - maind.benefactor[3] >= race_change_time){
		httpPrintf( cnt, "<form action=\"%s\">Faction race - ", URLAppend( cnt, "account" ) );
		httpPrintf( cnt, "<a href=\"%s&type=races\">See races</a><br><br><select name=\"race\">", URLAppend( cnt, "info" ) );
		for( a = 0 ; a < CMD_RACE_NUMUSED ; a++ ) {
			httpPrintf( cnt, "<option value=\"%d\"", a );
			if( a == maind.raceid )
				httpString( cnt, " selected" );
			httpPrintf( cnt, ">%s", cmdRaceName[a] );
		}
		httpString( cnt, "</select><input type=\"submit\" value=\"Change\"></form>" );
	}
	else
		httpPrintf( cnt, "Race change will be availible in %d tick(s)<br>", fmax(race_change_time - (ticks.number - maind.benefactor[1]), 0) );
}


iohttpForumFilter3( description, infod.desc, USER_DESC_MAX );
httpString( cnt, "<br>" );
httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><i>Faction description</i><br>", URLAppend( cnt, "account" ) );
httpString( cnt, "<textarea name=\"desc\" wrap=\"soft\" rows=\"4\" cols=\"64\" placeholder=\"Tell us something about yourself!\">" );
httpString( cnt, description );
httpString( cnt, "</textarea><br>" );
httpString( cnt, "<input type=\"submit\" value=\"Change\"></form><br>" );

httpString( cnt, "<br>" );
httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><i>Block/unblock user's messages</i><br>", URLAppend( cnt, "account" ) );
httpPrintf( cnt, "<input type=\"number\" style = \"width: 10em; margin-bottom: 3px;\" name=\"block\" placeholder=\"Enter user's id\" min = \"0\">" );
httpString( cnt, "<br>" );
httpString( cnt, "<input type=\"submit\" value=\"Block/unblock\"></form><br>" );

//print blocked users
int * list = NULL;
int size = 0;
if ( (size = dbUserPrintBlockedList(id, &list)) > 0){
	
	httpString( cnt, "<br>Blocked users:<br>" );
	for(int i = 0; i < size; i++){
		if( dbUserMainRetrieve( list[i], &maind ) < 0 ) {
			maind.empire = -1;
		}
		httpPrintf( cnt, "ID:%d Faction Name: %s<br>", list[i], maind.faction );
	}
}
free(list);

#if FACEBOOK_SUPPORT
if( -timediff( *localtime( &infod.fbinfo.updated ) ) >= day ) {
	facebook_update_user( (cnt->session)->dbuser );
	dbUserInfoRetrieve( ((cnt->session)->dbuser)->id, &infod );
}
iohtmlFBConnect( cnt );
if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBLINK ) ) {
httpString( cnt, "<table><tr><td>" );
httpPrintf( cnt, "<img src=\"%s\" alt=\"Facebook Icon\">", infod.fbinfo.pic );
httpString( cnt, "</td><td>" );
	httpPrintf( cnt, "Facebook ID : <b>%s</b><br>", infod.fbinfo.id );
	httpPrintf( cnt, "Facebook Name : <b>%s</b><br>", infod.fbinfo.full_name );
httpString( cnt, "</td></tr></table>" );
}
#endif

httpPrintf( cnt, "<br><a href=\"%s\">Change your password</a>", URLAppend( cnt, "changepass" ) );
httpPrintf( cnt, "<br><br><a href=\"%s\">Delete your Faction</a>", URLAppend( cnt, "delete" ) );
httpString( cnt, "</td></tr></table>" );


iohtmlBodyEnd( cnt );
return;
}


void iohtmlFunc_changepass( ReplyDataPtr cnt )
{
  int a, b, id;
  dbUserMainDef maind;
  char oldpass[USER_PASS_MAX];
  char *newpass[3];


  newpass[0] = iohtmlVarsFind( cnt, "password" );
  newpass[1] = iohtmlVarsFind( cnt, "newpass1" );
  newpass[2] = iohtmlVarsFind( cnt, "newpass2" );

  iohtmlBase( cnt, 1 );
  if( ( id = iohtmlIdentify( cnt, 1 ) ) < 0 )
    return;
  if( !( iohtmlHeader( cnt, id, &maind ) ) )
    return;

  iohtmlBodyInit( cnt, "Change password" );

  if( ( newpass[0] ) && ( newpass[1] ) && ( newpass[2] ) )
  {
    for( b = 0 ; b < 3 ; b++ )
    {
      for( a = 0 ; newpass[b][a] ; a++ )
      {
        if( newpass[b][a] == '+' )
          newpass[b][a] = ' ';
        else if( ( newpass[b][a] == 10 ) || ( newpass[b][a] == 13 ) )
          newpass[b][a] = 0;
      }
    }
    if( dbUserRetrievePassword( id, oldpass ) < 0 )
      httpString( cnt, "<i>Error encountered when retrieving password.</i><br><br>" );
    else if( ( checkPass( newpass[0], oldpass )) != 0 ) //checkencrypt
      httpString( cnt, "<i>Wrong old password</i><br><br>" );
    else if( !( ioCompareExact( newpass[1], newpass[2] ) ) )
      httpString( cnt, "<i>Different new passwords? Check your typing.</i><br><br>" );
    else if( cmdExecChangePassword( id, newpass[1] ) < 0 )
    {
      if( cmdErrorString )
        httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
      else
        httpString( cnt, "<i>Error encountered when changing password.</i><br><br>" );
    }
    else
      httpString( cnt, "<i>Password has been changed.</i><br><br>" );
  }

  httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "changepass" ) );
  httpString( cnt, "Current Password:<br><input type=\"password\" name=\"password\" size=\"16\"><br><br>" );
  httpString( cnt, "New Password:<br><input type=\"password\" name=\"newpass1\" size=\"16\"><br><br>" );
  httpString( cnt, "Repeat New Password:<br><input type=\"password\" name=\"newpass2\" size=\"16\"><br><br>" );
  httpString( cnt, "<input type=\"submit\" name=\"Change\"></center>" );

  iohtmlBodyEnd( cnt );
  return;
}

void cmdEmpireNewsAdd( int famid, int id, int64_t *data );

void iohtmlFunc_delete( ReplyDataPtr cnt )
{
  int a, b, c, id;
  dbUserMainDef maind;
  dbUserInfoDef infod;
  char *deletestring, *deathstring, *passwordString;
  int64_t newsd[DB_USER_NEWS_BASE];
  char oldpass[USER_PASS_MAX];
	
  deletestring = iohtmlVarsFind( cnt, "delete" );
  deathstring = iohtmlVarsFind( cnt, "death" );
  passwordString = iohtmlVarsFind( cnt, "password" );
  
  iohtmlBase( cnt, 1 );
  if( ( id = iohtmlIdentify( cnt, 1 ) ) < 0 )
    return;
  if( !( iohtmlHeader( cnt, id, &maind ) ) )
    return;
if( !( dbUserInfoRetrieve( id, &infod ) ) ) {
	error( "Error getting info" );
	return;
}
  iohtmlBodyInit( cnt, "Delete faction" );

  c = 3600*48;
  if( !( ticks.status | ticks.number ) )
    c = 120;
  a = time( 0 );
  
	//---- cant delete while in war
	int num = 0, war = 0, i, famid = -1 ;
	int *rels;
	 
	if ( ( dbUserMainRetrieve( id, &maind ) >= 0 ) && ( maind.empire != -1 ) )
		famid = maind.empire;

	if (famid > -1)
	{
		num = dbEmpireRelsList(famid, &rels );
		num <<= 6;
		
		for( i = 0 ; i < num ; i += 6 )
		{
			printf("I:%d %d\n",i,rels[i+1]);
		 if( rels[i+1] == CMD_RELATION_WAR ){
			war = 1;
			break;
		 }
		}
	}
	free( rels );
	//----
  
  if ( war == 0 )
  {
	  if( ( infod.createtime+c > a ) && ( ((cnt->session)->dbuser)->level < LEVEL_MODERATOR ) )
	  {
		b = infod.createtime+c - a;
		httpString( cnt, "You must wait 48 hours after the creation of an account to delete it, or 2 minutes if time has not started yet.<br>" );
		httpPrintf( cnt, "<b>There are %d hours and %d minutes left!</b><br><br>", b/3600, (b/60)%60 );
		iohtmlBodyEnd( cnt );
		return;
	  }
  }
  else
  {
	 	httpString( cnt, "You can't delete while in a war!<br>" );
		iohtmlBodyEnd( cnt );
		return; 
  }


  //cant delete for 24hrs after round start
 // if(((ticks.number < 144)&&(ticks.number > 0))||(ticks.number))
 // {
 // 	svSendPrintf(cnt, "You must wait 24 hours after round start to delete your account. There are %d hours and %d minutes left.<br><br>", 23 - ticks.number/6, 59-(ticks.number%6)*10);
 // 	iohttpBodyEnd( cnt );
 //   return;
 // }
 

if( deletestring ) {
	if (!passwordString){
		httpPrintf( cnt, "Enter your password!<br>" );
		iohtmlBodyEnd( cnt );
		return;
	}

	if( dbUserRetrievePassword( id, oldpass ) < 0 ){
		httpString( cnt, "<i>Error encountered when retrieving password.</i><br><br>" );
		iohtmlBodyEnd( cnt );
		return;
	}
	else if( ( checkPass( passwordString, oldpass )) != 0 ){ //checkencrypt
	  httpString( cnt, "<i>Wrong old password</i><br><br>" );
	  iohtmlBodyEnd( cnt );
	  return;
	}
		
	a = b = 0;
	sscanf( deletestring, "%d", &a );
	sscanf( deathstring, "%d", &b );
	newsd[0] = ticks.number;
	newsd[1] = CMD_NEWS_FLAGS_NEW;
	newsd[2] = CMD_NEWS_DEATH;
	newsd[3] = b;
	newsd[4] = id;
	newsd[5] = maind.empire;
	memcpy( &newsd[6], maind.faction, strlen(maind.faction) );
	if( ( a == 0 ) && ( cmdUserDelete( id ) >= 0 ) ) {
		httpPrintf( cnt, "Faction and account deleted!<br>" );
	cmdEmpireNewsAdd( newsd[5], newsd[4], newsd );
	} else if( ( a == 1 ) && ( cmdExecUserDeactivate( id, 0 ) >= 0 ) ) {
		httpPrintf( cnt, "Account deactivated!<br>" );
	cmdEmpireNewsAdd( newsd[5], newsd[4], newsd );
	} else {
		httpPrintf( cnt, "Error encountered while deleting faction<br>" );
	}
	iohtmlBodyEnd( cnt );
	return;
}

  httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "delete" ) );
  httpString( cnt, "<table><tr><td>Current Password:<br><input type=\"password\" name=\"password\" size=\"16\"><br><br>" );
  httpString( cnt, "Please specify the kind of deletion<br><select name=\"delete\">" );
  httpString( cnt, "<option value=\"1\">Keep the account to join a different empire" );
  httpString( cnt, "<option value=\"0\">Delete the faction and the account" );
  httpString( cnt, "</select><br><br>How should your emperor career end?<br><select name=\"death\">" );
  for( a = 0; cmdDeathString[a]; a++ ) {
  	httpPrintf( cnt, "<option value=\"%d\">%s</option>", a, cmdDeathString[a] );
  }
  httpString( cnt, "</select><br><br><input type=\"submit\" value=\"Delete\"></td></tr></table></form>" );

  iohtmlBodyEnd( cnt );
  return;
}


void iohtmlFunc_logout( ReplyDataPtr cnt ) {

if( (cnt->session)->dbuser != NULL ) {
	dbRegisteredInfo[DB_TOTALS_USERS_ONLINE]--;
}

if( remove_session( (cnt->session)->sid ) == NO ) {
	critical( "Unable to remove user session, this really shoulden't be able to happen." );
	cnt->session = get_session( SESSION_HTTP, NULL );
	iohtmlFunc_front( cnt, "%s", "An error, has occured with the logout. To be safe please close your browser.!" );
} else {
	cnt->session = get_session( SESSION_HTTP, NULL );
	iohtmlFunc_front( cnt, "%s", "You have safley loged out!" );
}

return;
}

void iohtmlFunc_expired( ReplyDataPtr cnt ) {

remove_session( (cnt->session)->sid );
cnt->session = get_session( SESSION_HTTP, NULL );
iohtmlFunc_login( cnt, "Expired Session" );

return;
}

