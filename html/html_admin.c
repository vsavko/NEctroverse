static void iohtmlAdminForm( ReplyDataPtr cnt, char *target ) {

httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, target ) );

return;
}

static void iohtmlAdminSubmit( ReplyDataPtr cnt, char *name ) {

httpPrintf( cnt, "<input type=\"submit\" value=\"%s\"></form>", name );
httpString( cnt, "<br>" );

return;
}

void iohtmlFunc_AdminMenu( ReplyDataPtr cnt ) {
	int id;
	dbUserMainDef maind;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 ) {
  return;
}
if( dbUserMainRetrieve( id, &maind ) < 0 ) {
	maind.empire = -1;
}
httpString( cnt, "<div class=\"floating-menu\">" );
httpString( cnt, "<br><table cellspacing=\"0\" cellpadding=\"0\" style=\"text-align:left;width=150px;\" background=\"files?type=image&amp;name=i36.jpg\"><tr>" );
httpString( cnt, "<td background=\"files?type=image&amp;name=i18.jpg\" width=\"150\" height=\"40\">&nbsp;</td>" );
httpString( cnt, "</tr><tr><td background=\"files?type=image&amp;name=i23.jpg\" height=\"20\"><b><font face=\"Tahoma\" size=\"2\">" );

httpPrintf( cnt, "<a href=\"%s\">Forums</a>", URLAppend( cnt, "adminforum" ) );
httpString( cnt, "</font></b></td></tr><tr><td background=\"files?type=image&amp;name=i36.jpg\"><table style=\"text-align:left;width=125px;\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><b><font face=\"Tahoma\" size=\"2\">" );
httpPrintf( cnt, "<a href=\"%s\">Users</a><br>", URLAppend( cnt, "administration" ) );

httpString( cnt, "</font></b></td></tr></table></td></tr>" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=i36.jpg\"><img height=\"15\" src=\"files?type=image&amp;name=i53.jpg\" width=\"150\"></td></tr>" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=i36.jpg\"><table cellspacing=\"0\" cellpadding=\"0\" style=\"text-align:left;width=125px;\"><tr><td><b><font face=\"Tahoma\" size=\"2\">" );

httpPrintf( cnt, "<a href=\"%s\">Back to Game</a><br>", URLAppend( cnt, "hq" ) );
	 
if( (cnt->session)->dbuser ) {
	if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) {
		httpPrintf( cnt, "<br><a href=\"%s\">Moderator panel</a>", URLAppend( cnt, "moderator" ) );
	} if( ((cnt->session)->dbuser)->level >= LEVEL_ADMINISTRATOR ) {
		httpPrintf( cnt, "<br><a href=\"%s\">Admin panel</a>", URLAppend( cnt, "administration" ) );
	}
}


httpString( cnt, "</font></b></td></tr></table></td></tr>" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=i55.jpg\" width=\"150\" height=\"20\">&nbsp;</td></tr>" );
httpString( cnt, "<tr><td background=\"files?type=image&amp;name=i56.jpg\" width=\"150\" height=\"75\">&nbsp;</td></tr>" );
httpString( cnt, "</table></div>" );
 
return;
}



void iohtmlFunc_adminforum( ReplyDataPtr cnt ) {
	int a, id;
	char *actionstring, *str0;
	dbForumForumDef forumd;
	dbUserMainDef maind;

iohtmlBase( cnt, 1 );

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
	return;
if( ((cnt->session)->dbuser)->level < LEVEL_FORUMMOD ) {
    httpString( cnt, "This account does not have adminitrator privileges" );
    httpString( cnt, "</center></body></html>" );
    return;
}
if( !( iohtmlAdminHeader( cnt, id, &maind ) ) )
	return;

iohtmlBodyInit( cnt, "Forums Administration" );

  if( ( actionstring = iohtmlVarsFind( cnt, "famforum" ) ) )
  {
    for( a = 0 ; a < dbMapBInfoStatic[MAP_EMPIRES] ; a++ )
    {
      sprintf( forumd.title, "Empire %d forum", a );
      forumd.rperms = 2;
      forumd.wperms = 2;
      forumd.flags = DB_FORUM_FLAGS_FORUMFAMILY;
      dbForumAddForum( true, &forumd );
    }
    httpString( cnt, "<i>Empire forums created</i><br><br>" );
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "crforum" ) ) )
  {
    forumd.threads = 0;
    forumd.time = 0;
    forumd.tick = 0;
    forumd.flags = 0;
    for( a = 0 ; ( a < 63 ) && ( actionstring[a] ) ; a++ );
    actionstring[a] = 0;
    memcpy( forumd.title, actionstring, a );
    if( !( str0 = iohtmlVarsFind( cnt, "crforumread" ) ) )
      goto cancel;
    if( !( sscanf( str0, "%d", &forumd.rperms ) ) )
      goto cancel;
    if( !( str0 = iohtmlVarsFind( cnt, "crforumwrite" ) ) )
      goto cancel;
    if( !( sscanf( str0, "%d", &forumd.wperms ) ) )
      goto cancel;
    forumd.flags = 0;
    dbForumAddForum( false, &forumd );
    httpString( cnt, "<i>Forum created</i><br><br>" );
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "delforum" ) ) )
  {
    if( !( sscanf( actionstring, "%d", &a ) ) )
      goto cancel;
    if( dbForumRemoveForum( false, a ) >= 0 )
      httpString( cnt, "<i>Forum deleted</i><br><br>" );
  }

  meat:

  iohtmlAdminForm( cnt, "adminforum" );
  httpString( cnt, "<input type=\"hidden\" name=\"famforum\"><br>" );
  iohtmlAdminSubmit( cnt, "Create empire forums" );

  iohtmlAdminForm( cnt, "adminforum" );
  httpString( cnt, "<input type=\"text\" name=\"crforum\" value=\"Forum name\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"crforumread\" value=\"Read permission\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"crforumwrite\" value=\"Write permission\"><br>" );
  iohtmlAdminSubmit( cnt, "Create forum" );

  iohtmlAdminForm( cnt, "adminforum" );
  httpString( cnt, "<input type=\"text\" name=\"delforum\" value=\"Forum number\"><br>" );
  iohtmlAdminSubmit( cnt, "Delete forum" );



  iohtmlBodyEnd( cnt );
  return;


  cancel:
  httpString( cnt, "<i>Command refused, make sure you didn't make an error in the input</i><br><br>" );
  goto meat;

  return;
}








void iohtmlFunc_moderator( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings;
  int id, a, b, c, x, y, num, actionid, i0, numbuild, curtime, cmd[2];
  int64_t i1;
  float fa;
  dbBuildPtr build;
  dbUserMainDef maind, main2d;
  dbUserInfoDef infod;
  dbUserFleetDef fleetd;
  dbUserFleetPtr fleetp;
  dbMainPlanetDef planetd;
  dbMainEmpireDef empired;
  dbBuildPtr buildd;
  //dbForumForumDef forumd;
  //dbForumThreadPtr pThread;
  char *actionstring, *str0, *str1;
  char COREDIR[256];
  dbUserPtr user, next;
  int64_t *newsp, *newsd;
  int *buffer;
  int *plist;
  FILE *file;


if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 ) {
    return;
}

iohtmlBase( cnt, 1 );
if( ((cnt->session)->dbuser)->level < LEVEL_MODERATOR ) {
	httpString( cnt, "This account does not have moderator privileges" );
	goto END;
}
if( !( iohtmlAdminHeader( cnt, id, &maind ) ) ) {
	goto END;
}
settings = GetSetting( "Directory" );
sprintf( COREDIR, "%s/logs/modlog.log", settings->string_value );
    if( !( file = fopen( COREDIR, "a+t" ) ) )
        return;

iohtmlBodyInit( cnt, "Primary re-draft of moderator panel" );

  httpString( cnt, "<table width=\"90%\"><tr><td width=\"30%\" align=\"center\" valign=\"top\">" );

iohtmlAdminForm( cnt, "moderator" );
httpString( cnt, "<select name=\"player\">" );
for( user = dbUserList ; user ; user = user->next ) {
	httpPrintf( cnt, "<option value=\"%d\">%s%s</option>", user->id, user->faction, ( bitflag( user->flags, CMD_USER_FLAGS_FROZEN ) ) ? " (Frozen)" : "" );
}
httpString( cnt, "</select>" );
iohtmlAdminSubmit( cnt, "View player data" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<select name=\"empire\">" );
for( a = 0; a < dbMapBInfoStatic[MAP_EMPIRES]; a++ ) {
	if( dbEmpireGetInfo( a, &empired ) < 0 ) {
		continue;
	}
	httpPrintf( cnt, "<option value=\"%d\">", a );
	if( empired.name[0] ) {
		httpPrintf( cnt, "%s</option>", empired.name );
	} else {
		httpPrintf( cnt, "Empire #%d</option>", a );
	}
}
  httpString( cnt, "</select>" );
  iohtmlAdminSubmit( cnt, "View Empire data" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"playernews\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "View player news" );

  httpString( cnt, "<form action=\"famnews\" method=\"POST\">" );
  httpString( cnt, "<input type=\"text\" name=\"id\" value=\"empire number\"><br>" );
  iohtmlAdminSubmit( cnt, "View empire news" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"playerpts\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"playerptsnum\" value=\"Points to add\"><br>" );
  iohtmlAdminSubmit( cnt, "Add tag points" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"playertag\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"playertagname\" value=\"New tag\"><br>" );
  iohtmlAdminSubmit( cnt, "Change forum tag" );

iohtmlAdminForm( cnt, "moderator" );
httpString( cnt, "<select name=\"playerres\">" );
for( user = dbUserList ; user ; user = user->next ) {
	if( !( bitflag( user->flags, CMD_USER_FLAGS_ACTIVATED ) ) ) {
		continue;
	}
	httpPrintf( cnt, "<option value=\"%d\">%s%s</option>", user->id, user->faction, ( bitflag( user->flags, CMD_USER_FLAGS_FROZEN ) ) ? " (Frozen)" : "" );
}
httpString( cnt, "</select>" );
httpString( cnt, "<select name=\"resource\">" );
for( a = 0; a < CMD_RESSOURCE_NUMUSED; a++ ) {
	httpPrintf( cnt, "<option value=\"%d\">%s</option>", a, cmdRessourceName[a] );
}
  httpString( cnt, "</select>" );

  httpString( cnt, "<input type=\"text\" name=\"resqt\" value=\"quantity\"><br>" );
  iohtmlAdminSubmit( cnt, "Set resources" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"seemarket\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "View bids" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"clearmarket\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "Clear bids" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"seebuild\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "See build" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"clearbuild\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "Clear build" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"rmbuild\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"rmbuilditem\" value=\"build ID\"><br>" );
  iohtmlAdminSubmit( cnt, "Remove build item" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"seeresearch\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "See research" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"setfunding\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"setfundingqt\" value=\"funding\"><br>" );
  iohtmlAdminSubmit( cnt, "Set research funding" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"setresearch\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"setresearchid\" value=\"research field ID\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"setresearchqt\" value=\"research points\"><br>" );
  iohtmlAdminSubmit( cnt, "Set research points" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"setreadiness\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"setreadinessid\" value=\"readiness ID\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"setreadinessqt\" value=\"readiness %\"><br>" );
  iohtmlAdminSubmit( cnt, "Set readiness" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"seefleets\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "See fleets" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"mainfleet\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"mainfleetid\" value=\"unit ID\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"mainfleetqt\" value=\"unit QT\"><br>" );
  iohtmlAdminSubmit( cnt, "Set main fleet unit" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"clearops\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "Clear ops" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"giveplanet\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"giveplanetid\" value=\"planet ID\"><br>" );
  iohtmlAdminSubmit( cnt, "Give planet" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"setportal\" value=\"planet ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"setportalst\" value=\"1 or 0\"><br>" );
  iohtmlAdminSubmit( cnt, "Set portal" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"setbuilding\" value=\"planet ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"setbuildingid\" value=\"building type\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"setbuildingqt\" value=\"quantity\"><br>" );
  iohtmlAdminSubmit( cnt, "Set building" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"givepop\" value=\"player ID number\"><br>" );
  iohtmlAdminSubmit( cnt, "Give population" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"clearfam\" value=\"Family\"><br>" );
  iohtmlAdminSubmit( cnt, "Fix Fam" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"changestatus\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"statusid\" value=\"StatusID\"><br>" );
  iohtmlAdminSubmit( cnt, "Change player status" );

  #if FACEBOOK_SUPPORT
  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"changefbstatus\" value=\"player ID number\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"status\" value=\"Status\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"fbid\" value=\"FBID\"><br>" );
  iohtmlAdminSubmit( cnt, "Change player status" );
  #endif
/*
  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"forumlid\" value=\"Forum ID\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"threadlid\" value=\"Thread ID\"><br>" );
  iohtmlAdminSubmit( cnt, "Lock Thread" );

  iohtmlAdminForm( cnt, "moderator" );
  httpString( cnt, "<input type=\"text\" name=\"forumuid\" value=\"Forum ID\"><br>" );
  httpString( cnt, "<input type=\"text\" name=\"threaduid\" value=\"Thread ID\"><br>" );
  iohtmlAdminSubmit( cnt, "Unlock Thread" );
*/

  httpString( cnt, "</td><td width=\"60%\" align=\"left\" valign=\"top\">" );

	id = iohtmlIdentify( cnt, 0 );
  if( id != -1 )
  {
    dbUserMainRetrieve( id, &main2d );
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "player" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( !( user = dbUserLinkID( actionid ) ) )
      goto iohtmlFunc_moderatorL0;
    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserInfoRetrieve( actionid, &infod ) < 0 )
      goto iohtmlFunc_moderatorL0;
    httpPrintf( cnt, "<b>Player ID %d</b><br><br>", actionid );
    if( ((cnt->session)->dbuser)->level >= LEVEL_ADMINISTRATOR )
    httpPrintf( cnt, "User Login : %s <br>", user->name );
    httpPrintf( cnt, "Faction name : %s <br>", maind.faction );
    httpString( cnt, "Last Login : " );
	//routine to show how long it has been since a player was online
	curtime = time( 0 );
	b = curtime - infod.lasttime;
	if( b < 5*minute ){
		httpString( cnt, " [online]<br>" );
	}
	else{
		if( b >= day )
		{
		  httpPrintf( cnt, "%ldd ", b/day );
		  b %= day;
		}
		if( b >= hour )
		{
		  httpPrintf( cnt, "%ldh ", b/hour );
		  b %= hour;
		}
		if( b >= minute )
		  httpPrintf( cnt, "%ldm<br>", b/minute );
	}



    httpPrintf( cnt, "Forum tag : %s<br>", infod.forumtag );
    httpPrintf( cnt, "Tag points : %d<br>", infod.tagpoints );
    httpPrintf( cnt, "User level : %d<br><br>", user->level );
    httpPrintf( cnt, "Planets : %d<br>", maind.planets );
    httpPrintf( cnt, "Networth : %lld<br>", (long long)maind.networth );
    httpPrintf( cnt, "Empire : #%d<br>", maind.empire );
    httpPrintf( cnt, "Artefacts : 0x%x<br>", maind.artefacts );

    for( a = 0; a < CMD_RESSOURCE_NUMUSED ; a++ ) {
      httpPrintf( cnt, "%s : %lld<br>", cmdRessourceName[a], (long long)maind.ressource[a] );
    }
    for( a = 0; a < CMD_RESEARCH_NUMUSED ; a++ ) {
      httpPrintf( cnt, "%s : %lld %%<br>", cmdResearchName[a], (long long)maind.totalresearch[a] );
    }
    for( a = 0 ; a < CMD_READY_NUMUSED ; a++ ) {
      httpPrintf( cnt, "%s readiness : %d %%<br>", cmdReadyName[a], maind.readiness[a] >> 16 );
    }
	for( a = 0 ; a < CMD_READY_NUMUSED ; a++ ) {
      httpPrintf( cnt, "%s readiness : %d %%<br>", cmdReadyName[a], maind.readiness[a] >> 16 );
    }
	
	char * infoscouncil[] ={
		"ENERGY_INCOME",
		"MINERAL_INCOME",
		"CRYSTAL_INCOME",
		"ECTROLIUM_INCOME",
		"ENERGY_PRODUCTION",
		"ENERGY_DECAY",
		"ENERGY_INTEREST",
		"BUILDING_UPKEEP",
		"POPULATION_REDUCTION",
		"PORTALS_UPKEEP",
		"UNITS_UPKEEP",
		"MINERAL_PRODUCTION",
		"MINERAL_INTEREST",
		"CRYSTAL_PRODUCTION",
		"CRYSTAL_DECAY",
		"CRYSTAL_INTEREST",
		"ECTROLIUM_PRODUCTION",
		"ECTROLIUM_INTEREST",
		"ENERGY_TAX",
		"MINERAL_TAX",
		"CRYSTAL_TAX",
		"ECTROLIUM_TAX",
		"BENEFACTORS_NUM",
		"BENEFACTORS_REDUCTION",
		"AUTOFUND_RESEARCH",
		"AUTOFUND_EMPIRE"
	};
	
	for (a = 0; a < INFOS_TOTAL_NUMUSED ; a++){
	httpPrintf( cnt, "%s: %lld<br>",  infoscouncil[a], (long long)maind.infos[a] );		
	}
	
	for( a = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ ) {
	httpPrintf( cnt, "%s %lld<br>", cmdBuildingName[a], (long long)maind.totalbuilding[a] );
	}
	
    fprintf( file, "%s > view player info of player %s\n", main2d.faction, maind.faction);
  }
  
  if( ( actionstring = iohtmlVarsFind( cnt, "empire" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbEmpireGetInfo( actionid, &empired ) < 0 )
	goto iohtmlFunc_moderatorL0;
	
    httpPrintf( cnt, "<b>Empire ID %d</b><br><br>", actionid );

    httpPrintf( cnt, "Planets : %d<br>", empired.planets );
    httpPrintf( cnt, "Networth : %lld<br>", (long long)empired.networth );
    httpPrintf( cnt, "Artefacts : 0x%x<br>", empired.artefacts );

    for( a = 0; a < CMD_RESSOURCE_NUMUSED ; a++ )
      httpPrintf( cnt, "%s : %lld<br>", cmdRessourceName[a], (long long)empired.fund[a] );
    for( b = 0; b < CMD_EMPIRE_POLITICS_TOTAL; b++ ) {
	httpPrintf( cnt, "%d : %d<br>", empired.politics[b], b );
    }
    for( a = 0 ; a < empired.numplayers ; a++ ) {
	    if( !( user = dbUserLinkID( empired.player[a] ) ) )
		    continue;
	    httpPrintf( cnt, "%d : %s<br>", a, user->faction );
    }
    fprintf( file, "%s > view empire info of %d\n", main2d.faction, actionid );
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "playernews" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( !( user = dbUserLinkID( actionid ) ) )
      goto iohtmlFunc_moderatorL0;
    httpPrintf( cnt, "Current date : Week %d, year %d<br>", ticks.number % 52, ticks.number / 52 );
    if( ticks.status )
      httpPrintf( cnt, "%d seconds before tick<br>", (int)( ticks.next - time(0) ) );
    else
      httpPrintf( cnt, "Time frozen<br>" );
    num = dbUserNewsList( actionid, &newsp );
    newsd = newsp;
    if( !( num ) )
      httpString( cnt, "<b>No reports</b>" );
    for( a = 0 ; a < num ; a++, newsd += DB_USER_NEWS_BASE )
      iohtmlNewsString( cnt, newsd );
    if( newsp )
      free( newsp );
    cmd[0] = CMD_RETRIEVE_USERMAIN;
    cmd[1] = actionid;
    cmdExecute( cnt, cmd, &maind, 0 );
    fprintf( file, "%s > view news of player %s \n",main2d.faction, maind.faction);
  }
#if FACEBOOK_SUPPORT
if( ( actionstring = iohtmlVarsFind( cnt, "changefbstatus" ) ) ) {
	if( sscanf( actionstring, "%d", &actionid ) != 1 )
		goto iohtmlFunc_moderatorL0;
	if( !( user = dbUserLinkID( actionid ) ) )
		goto iohtmlFunc_moderatorL0;
	if( !( str0 = iohtmlVarsFind( cnt, "status" ) ) )
		goto iohtmlFunc_moderatorL0;
	if( !( str1 = iohtmlVarsFind( cnt, "fbid" ) ) )
		goto iohtmlFunc_moderatorL0;
	if( sscanf( str0, "%d", &a ) != 1 )
		goto iohtmlFunc_moderatorL0;
	if( a == 0 ) {
		bitflag_remove( &user->flags, CMD_USER_FLAGS_FBLINK );
		bitflag_remove( &user->flags, CMD_USER_FLAGS_FBMADE );
		httpPrintf( cnt, "<b>%s Facebook Link -- Removed</b>", user->faction );
	} else if ( a == 1 ) {
		bitflag_add( &user->flags, CMD_USER_FLAGS_FBLINK );
		strcpy(user->fbid, str1 );
		httpPrintf( cnt, "<b>%s Facebook Link -- Added to ID: %s</b>", user->faction, user->fbid );
	} else if ( a == 2 ) {
		bitflag_add( &user->flags, CMD_USER_FLAGS_FBLINK );
		bitflag_add( &user->flags, CMD_USER_FLAGS_FBMADE );
		strcpy( user->fbid, str1 );
		sprintf( COREDIR, "FBUSER%s", user->fbid );
		strcpy( user->name, COREDIR );
		httpPrintf( cnt, "<b>%s Facebook Link -- Added to ID %s, With Made Flag.</b>", user->faction, user->fbid  );
	}
	dbUserSave( user->id, user );
	facebook_update_user( user );
	fprintf( file, "%s > ajust fb status of player %s \n",main2d.faction, maind.faction);
}
#endif
  if( ( actionstring = iohtmlVarsFind( cnt, "playerpts" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "playerptsnum" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &a ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserInfoRetrieve( actionid, &infod ) < 0 )
      goto iohtmlFunc_moderatorL0;
    infod.tagpoints += a;
    sprintf( infod.forumtag, "%s", cmdTagFind( infod.tagpoints ) );
    dbUserInfoSet( actionid, &infod );
    httpPrintf( cnt, "The tag points of <b>%s</b> have been increased by <b>%d</b> for a total of <b>%d</b>, tag set to <b>%s</b><br><br>", maind.faction, a, infod.tagpoints, infod.forumtag );
    fprintf( file, "%s > Tag points of player %s have been increased by %d tot a total of %d \n",main2d.faction, maind.faction,a, infod.tagpoints);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "playertag" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "playertagname" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( !( user = dbUserLinkID( actionid ) ) )
      goto iohtmlFunc_moderatorL0;
    if( ( user->level >= ((cnt->session)->dbuser)->level ) && ( id != actionid ) )
    {
      httpString( cnt, "Permission denied<br><br>" );
      goto iohtmlFunc_moderatorL1;
    }
    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserInfoRetrieve( actionid, &infod ) < 0 )
      goto iohtmlFunc_moderatorL0;
    infod.forumtag[31] = 0;
    iohttpForumFilter( infod.forumtag, str0, 31, 1 );
    dbUserInfoSet( actionid, &infod );
    httpPrintf( cnt, "Tag of <b>%s</b> changed to <b>%s</b><br><br>", maind.faction, infod.forumtag );
    fprintf( file, "%s > Tag of player %s changed to %s \n",main2d.faction, maind.faction, infod.forumtag);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "playerres" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "resource" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( !( str1 = iohtmlVarsFind( cnt, "resqt" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str1, "%" SCNd64, &i1 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( (unsigned int)i0 >= 4 )
      goto iohtmlFunc_moderatorL0;
    maind.ressource[i0] = i1;
    dbUserMainSet( actionid, &maind );
    httpPrintf( cnt, "Resource %d of %d changed to %lld.<br><br>", i0, actionid, (long long)i1 );
    fprintf( file, "%s > Resource %d of player %s changed to %lld \n",main2d.faction,i0, maind.faction, (long long)i1);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "seemarket" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    b = dbUserMarketList( actionid, &buffer );
    httpString( cnt, "<b>Bids</b><br>" );
    if( b <= 0 )
      httpString( cnt, "None<br>" );
    for( a = c = 0 ; a < b ; a++, c += 5 )
    {
      if( !( buffer[c+DB_MARKETBID_ACTION] ) )
        httpString( cnt, "Buying" );
      else
        httpString( cnt, "Selling" );
      httpPrintf( cnt, " %d %s at %d<br>", buffer[c+DB_MARKETBID_QUANTITY], cmdRessourceName[buffer[c+DB_MARKETBID_RESSOURCE]+1], buffer[c+DB_MARKETBID_PRICE] );
    }
    httpString( cnt, "<br>" );
    if( buffer )
      free( buffer );
    fprintf( file, "%s > market of player %s viewed \n",main2d.faction, maind.faction);

  }

  if( ( actionstring = iohtmlVarsFind( cnt, "clearmarket" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( ( c = dbUserMarketList( actionid, &buffer ) ) >= 0 )
    {
      for( a = b = 0 ; a < c ; a++, b += 5 )
      {
        dbUserMarketRemove( actionid, buffer[b+DB_MARKETBID_BIDID] );
        dbMarketRemove( &buffer[b], buffer[b+DB_MARKETBID_BIDID] );
      }
      free( buffer );
    }
    httpPrintf( cnt, "Bids of %d cleared.<br>", actionid );
    fprintf( file, "%s > market of player %s cleared \n",main2d.faction, maind.faction);

  }

  if( ( actionstring = iohtmlVarsFind( cnt, "seebuild" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( ( numbuild = dbBuildList( true, actionid, &build ) ) < 0 )
      goto iohtmlFunc_moderatorL0;
    for( a = c = 0 ; a < numbuild ; a++ )
    {
      if( build[a].type >> 16 )
        continue;
      httpPrintf( cnt, "<b>%d</b> %d %s in %d weeks at <a href=\"planet?id=%d\">%d,%d:%d</a><br>", a, build[a].quantity, cmdBuildingName[ build[a].type & 0xFFFF ], build[a].time, build[a].plnid, ( build[a].plnpos >> 8 ) & 0xFFF, build[a].plnpos >> 20, build[a].plnpos & 0xFF );
    }
    for( a = c = 0 ; a < numbuild ; a++ )
    {
      if( !( build[a].type >> 16 ) )
        continue;
      httpPrintf( cnt, "<b>%d</b> %d %s in %d weeks<br>", a, build[a].quantity, cmdUnitName[ build[a].type & 0xFFFF ], build[a].time );
    }
    free( build );
    fprintf( file, "%s > build of player %s viewed \n",main2d.faction, maind.faction);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "rmbuild" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "rmbuilditem" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    a = dbUserBuildRemove( actionid, i0 );
    httpPrintf( cnt, "Removing build item %d from player %d, return code : %d.<br>", i0, actionid, a );

    fprintf( file, "%s >buildings in build of player %s CLEARED \n",main2d.faction, maind.faction);

  }

  if( ( actionstring = iohtmlVarsFind( cnt, "seeresearch" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    httpPrintf( cnt, "Funding : %lld<br>", (long long)maind.fundresearch );
    httpString( cnt, "<table width=\"90%%\" cellspacing=\"8\">" );
    for( a = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ )
      httpPrintf( cnt, "<tr><td nowrap><b>%s</b></td><td nowrap>%lld Points</td><td nowrap>%lld%%</td></tr>", cmdResearchName[a], (long long)maind.research[a], (long long)maind.totalresearch[a] );
    httpString( cnt, "</table>" );
    fprintf( file, "%s >research of player %s viewed \n",main2d.faction, maind.faction);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "setfunding" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "setfundingqt" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    maind.fundresearch = i0;
    dbUserMainSet( actionid, &maind );
    httpPrintf( cnt, "Research funding of %d set to %lld.", actionid, (long long)maind.fundresearch );
    fprintf( file, "%s >research funding of player %s set to %lld\n",main2d.faction, maind.faction, (long long)maind.fundresearch);

  }

  if( ( actionstring = iohtmlVarsFind( cnt, "setresearch" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "setresearchid" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( !( str1 = iohtmlVarsFind( cnt, "setresearchqt" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str1, "%" SCNd64, &i1 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( (unsigned int)i0 >= CMD_RESEARCH_NUMUSED )
      goto iohtmlFunc_moderatorL0;
    maind.research[i0] = i1;

    for( a = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ )
    {
      fa = cmdRace[maind.raceid].researchmax[a];
      maind.totalresearch[a] = fa * ( 1.0 - exp( (double)maind.research[a] / ( -10.0 * (double)maind.networth ) ) );
    }

    dbUserMainSet( actionid, &maind );
    httpPrintf( cnt, "Research points of player %d in research field %d set to %lld.", actionid, i0, (long long)i1 );
    fprintf( file, "%s >research points of player %s in research field %d set to %lld \n",main2d.faction, maind.faction , i0, (long long)i1);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "setreadiness" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "setreadinessid" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( !( str1 = iohtmlVarsFind( cnt, "setreadinessqt" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str1, "%" SCNd64, &i1 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( (unsigned int)i0 >= 3 )
      goto iohtmlFunc_moderatorL0;
    maind.readiness[i0] = i1 << 16;
    dbUserMainSet( actionid, &maind );
    httpPrintf( cnt, "Readiness %d of player %d set to %lld.", i0, actionid, (long long)i1 );
    fprintf( file, "%s >Readiness %d of player %s set to %lld \n",main2d.faction, i0, maind.faction, (long long)i1);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "seefleets" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( ( num = dbUserFleetList( actionid, &fleetp ) ) < 0 )
      goto iohtmlFunc_moderatorL0;
    httpString( cnt, "<b>Main fleet</b><br><br>" );
    for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
    {
      if( fleetp[0].unit[a] )
      {
        httpPrintf( cnt, "%d %s<br>", fleetp[0].unit[a], cmdUnitName[a] );
        b = 1;
      }
    }
    httpString( cnt, "<br><b>Travelling fleets</b><br><br>" );
    if( num == 1 )
      httpString( cnt, "None<br>" );
    else
    {
      httpString( cnt, "<table width=\"90%%\" cellspacing=\"5\"><tr><td width=\"50%\">Units</td><td width=\"50%\">Position</td></tr>" );
      for( a = 1 ; a < num ; a++ )
      {
        httpString( cnt, "<tr><td>" );
        for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ )
        {
          if( fleetp[a].unit[b] )
            httpPrintf( cnt, "%d %s<br>", fleetp[a].unit[b], cmdUnitName[b] );
        }
        cmdFleetGetPosition( &fleetp[a], &x, &y );
        httpPrintf( cnt, "<td>%d,%d</td></tr>", x, y );
      }
      httpString( cnt, "</table>" );
    }
    fprintf( file, "%s >fleet of player %s viewed \n",main2d.faction, maind.faction);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "mainfleet" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "mainfleetid" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( !( str1 = iohtmlVarsFind( cnt, "mainfleetqt" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str1, "%" SCNd64, &i1 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbUserFleetRetrieve( actionid, 0, &fleetd ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( (unsigned int)i0 >= CMD_UNIT_NUMUSED )
      goto iohtmlFunc_moderatorL0;
    fleetd.unit[i0] = i1;
    dbUserFleetSet( actionid, 0, &fleetd );
    httpPrintf( cnt, "Unit %d of player %d set to %lld.", i0, actionid, (long long)i1 );
    fprintf( file, "%s >unit %d of player %s set to %lld \n",main2d.faction, i0, maind.faction, (long long)i1);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "clearops" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    a = dbUserSpecOpEmpty( actionid );
    httpPrintf( cnt, "Cleared spec ops on %d : %d.", actionid, a );
    fprintf( file, "%s >ops on/of player %s cleared \n",main2d.faction, maind.faction);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "giveplanet" ) ) )
  {
  	if( !( str0 = iohtmlVarsFind( cnt, "giveplanetid" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;

    if( dbUserMainRetrieve( actionid, &maind ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( dbMapRetrievePlanet( i0, &planetd ) < 0 )
      goto iohtmlFunc_moderatorL0;

    if( planetd.owner != -1 )
    {
      dbUserPlanetRemove( planetd.owner, i0 );
      if( planetd.construction )
      {
        b = dbUserBuildList( planetd.owner, &buildd );
        for( a = b-1 ; a >= 0 ; a-- )
        {
          if( buildd[a].plnid == actionid )
            dbUserBuildRemove( planetd.owner, a );
        }
        if( buildd )
          free( buildd );
      }
    }

    maind.planets++;
    memset( planetd.building, 0, CMD_BLDG_NUMUSED*sizeof(int64_t) );
    memset( planetd.unit, 0, CMD_UNIT_NUMUSED*sizeof(int64_t) );
    planetd.maxpopulation = (float)( planetd.size * CMD_POPULATION_SIZE_FACTOR );
    planetd.construction = 0;
    planetd.population = planetd.size * CMD_POPULATION_BASE_FACTOR;
    planetd.owner = actionid;

    dbUserPlanetAdd( actionid, i0, planetd.system, planetd.position, 0 );
    dbMapSetPlanet( i0, &planetd );
    dbUserMainSet( actionid, &maind );
    httpPrintf( cnt, "Planet %d transfered to %d", i0, actionid );
    fprintf( file, "%s >planet %d transferred to player %s \n",main2d.faction,i0 ,maind.faction);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "setportal" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "setportalst" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( dbMapRetrievePlanet( actionid, &planetd ) < 0 )
      goto iohtmlFunc_moderatorL0;
    if( planetd.owner == -1 )
      goto iohtmlFunc_moderatorL0;
    planetd.flags = planetd.flags & ( 0xFFFFFFFF - CMD_PLANET_FLAGS_PORTAL - CMD_PLANET_FLAGS_PORTAL_BUILD );
    if( i0 )
      planetd.flags |= CMD_PLANET_FLAGS_PORTAL;
    dbMapSetPlanet( actionid, &planetd );
    dbUserPlanetSetFlags( planetd.owner, actionid, planetd.flags );
    httpPrintf( cnt, "Planet %d portal set to %d", actionid, i0 );

    fprintf( file, "%s >planet %d portal set to %d \n",main2d.faction, actionid, i0);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "setbuilding" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "setbuildingid" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( !( str1 = iohtmlVarsFind( cnt, "setbuildingqt" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str1, "%" SCNd64, &i1 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( (unsigned int)i0 >= CMD_BLDG_NUMUSED )
      goto iohtmlFunc_moderatorL0;
    if( dbMapRetrievePlanet( actionid, &planetd ) < 0 )
      goto iohtmlFunc_moderatorL0;
    planetd.building[i0] = i1;
    dbMapSetPlanet( actionid, &planetd );
    httpPrintf( cnt, "Number of %s on %d set to %lld", cmdBuildingName[i0], actionid, (long long)i1 );
    fprintf( file, "%s >Number of building %s on %d set to %lld \n",main2d.faction, cmdBuildingName[i0], actionid, (long long)i1);
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "givepop" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    num = dbUserPlanetListIndices( actionid, &plist );
    for( a = 0 ; a < num ; a++ )
    {
      dbMapRetrievePlanet( plist[a], &planetd );
      planetd.population = planetd.maxpopulation;
      dbMapSetPlanet( plist[a], &planetd );
    }
    if( plist )
      free( plist );
    httpPrintf( cnt, "Gave population to %d", actionid );
    fprintf( file, "%s >Gave population to %s \n",main2d.faction, maind.faction);
  }


  if( ( actionstring = iohtmlVarsFind( cnt, "clearfam" ) ) )
  {
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    dbEmpireGetInfo( actionid, &empired );
    a = 0;
    for( user = dbUserList ; user ; user = next ) {
	next = user->next;
	dbUserMainRetrieve( user->id, &maind );
	if( maind.empire == actionid ) {
		empired.player[a] = user->id;
		a++;
	}
    }
    empired.numplayers = a;
    dbEmpireSetInfo( actionid, &empired );
    
    httpPrintf( cnt, "fam %d cleared", actionid );
  }

  if( ( actionstring = iohtmlVarsFind( cnt, "changestatus" ) ) )
  {
    if( !( str0 = iohtmlVarsFind( cnt, "statusid" ) ) )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( actionstring, "%d", &actionid ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &i0 ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( ( user = dbUserLinkID( actionid ) ) )
    {
		if( i0 == 1 )
	bitflag_add( &user->flags, CMD_USER_FLAGS_ACTIVATED );
		if( i0 == 2 )
        bitflag_add( &user->flags, CMD_USER_FLAGS_KILLED );
		if( i0 == 3 )
        bitflag_add( &user->flags, CMD_USER_FLAGS_DELETED );
		if( i0 == 4 )
        bitflag_add( &user->flags, CMD_USER_FLAGS_NEWROUND );
		dbUserSave( actionid, user );
    }
    httpPrintf( cnt, "changed status of %d", actionid );
  }
  /*
  if( ( actionstring = iohtmlVarsFind( cnt, "forumlid" ) ) )
  {
  	if( !( str0 = iohtmlVarsFind( cnt, "threadlid" ) ) )
    {
     	httpString(cnt, "No thread specified<br>");
      goto iohtmlFunc_moderatorL0;
    }

		//b = forum id
		//c = thread id
		if( sscanf( actionstring, "%d", &b ) != 1 )
      goto iohtmlFunc_moderatorL0;
    if( sscanf( str0, "%d", &c ) != 1 )
      goto iohtmlFunc_moderatorL0;

		a = dbForumListThreads(b, c, c+1, &forumd, &pThread);
		if(a)
		{
			printf("Thread %d locked\n", c);
			pThread[0].flags = DB_FORUM_FLAGS_THREAD_LOCK;
			//Remove the thread to add it locked mode
			dbForumRemoveThread( b, c );
			dbForumAddThread( b, pThread);
		}

  }

  if( ( actionstring = iohtmlVarsFind( cnt, "forumuid" ) ) )
  {
  	if( !( str0 = iohtmlVarsFind( cnt, "threaduid" ) ) )
    {
     	httpString(cnt, "No thread specified<br>");
      goto iohtmlFunc_moderatorL0;
    }



  }
	*/

  fclose( file );
goto iohtmlFunc_moderatorL1;
iohtmlFunc_moderatorL0:
httpString( cnt, "Command refused<br>" );
iohtmlFunc_moderatorL1:
httpString( cnt, "</td></tr></table>" );
END:
iohtmlBodyEnd( cnt );
return;
}



void iohtmlFunc_administration( ReplyDataPtr cnt )
{
  int a, b, c, cmd[2], id;
  int *buffer;
  int *plnlist;
  char buff[DEFAULT_BUFFER];
  char *action[33];
  dbForumForumDef forumd;
  dbUserFleetDef fleetd;
  dbUserMainDef maind;
  dbUserInfoDef infod;
  dbMainSystemDef systemd;
  dbMainPlanetDef planetd;
  dbUserPtr user;

  iohtmlBase( cnt, 1 );

  if( ( id = iohtmlIdentify( cnt, 0 ) ) < 0 )
    goto iohtmlFunc_admin_mainL0;
  if( ((cnt->session)->dbuser)->level < LEVEL_ADMINISTRATOR )
    goto iohtmlFunc_admin_mainL0;
  if( !( iohtmlAdminHeader( cnt, id, &maind ) ) )
	return;
  action[0] = iohtmlVarsFind( cnt, "reloadfiles" );
  action[1] = iohtmlVarsFind( cnt, "forums" );
  action[2] = iohtmlVarsFind( cnt, "shutdown" );
  action[3] = iohtmlVarsFind( cnt, "forumdel" );
  action[4] = iohtmlVarsFind( cnt, "forumcreate" );
  action[5] = iohtmlVarsFind( cnt, "setmod" );
  action[6] = iohtmlVarsFind( cnt, "delplayer" );
  action[7] = iohtmlVarsFind( cnt, "deactivate" );
  action[8] = iohtmlVarsFind( cnt, "toggletime" );
  action[9] = iohtmlVarsFind( cnt, "giveenergy" );
  action[10] = iohtmlVarsFind( cnt, "crap" );
  action[11] = iohtmlVarsFind( cnt, "inactives" );
  action[12] = iohtmlVarsFind( cnt, "newround" );
  action[13] = iohtmlVarsFind( cnt, "resettags" );
  action[14] = iohtmlVarsFind( cnt, "changename" );
  action[15] = iohtmlVarsFind( cnt, "newname" );
  action[16] = iohtmlVarsFind( cnt, "unexplored" );
  action[17] = iohtmlVarsFind( cnt, "clearnews" );
  action[18] = iohtmlVarsFind( cnt, "findartefacts" );
  action[19] = iohtmlVarsFind( cnt, "resetpsychics" );
  action[20] = iohtmlVarsFind( cnt, "fixtags" );
  action[21] = iohtmlVarsFind( cnt, "findbonuses" );
  action[22] = iohtmlVarsFind( cnt, "genranks" );
  action[23] = iohtmlVarsFind( cnt, "descs" );
  action[24] = iohtmlVarsFind( cnt, "newround2" );
  action[25] = iohtmlVarsFind( cnt, "forumdelauthor" );
  action[26] = iohtmlVarsFind( cnt, "forumdelip" );
  action[27] = iohtmlVarsFind( cnt, "setfmod" );
  action[28] = iohtmlVarsFind( cnt, "setplay" );
  action[29] = iohtmlVarsFind( cnt, "deletecons");
  action[30] = iohtmlVarsFind( cnt, "blankenergy" );
  action[31] = iohtmlVarsFind( cnt, "resetpass" );
  action[32] = iohtmlVarsFind( cnt, "findancient" );
  action[33] = iohtmlVarsFind( cnt, "findbig" );
  action[34] = iohtmlVarsFind( cnt, "showops" );
  action[35] = iohtmlVarsFind( cnt, "showbnned" );
  action[36] = iohtmlVarsFind( cnt, "banmail" );
  action[37] = iohtmlVarsFind( cnt, "banIpAndEmail" );
  //action[38] = iohtmlVarsFind( cnt, "convertPasswords" );
  action[39] = iohtmlVarsFind( cnt, "findaAll" );

iohtmlBodyInit( cnt, "Primary re-draft of Administration panel" );

  httpString( cnt, "<table width=\"90%\"><tr><td width=\"30%\" align=\"center\" valign=\"top\">" );
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"reloadfiles\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Reload HTTP files" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"forums\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Create empire forums" );

  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"shutdown\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Shutdown" );

  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"blankenergy\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Wipe Energy Production" );

  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"forumdel\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Delete forum" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"forumcreate\" value=\"Forum name\">" );
  iohtmlAdminSubmit( cnt, "Create forum" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"setmod\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Set Moderator" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"setfmod\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Set Forum Moderator" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"setplay\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Set Player" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"delplayer\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Delete Player" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"deactivate\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Deactivate Player" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"toggletime\" value=\"1\">" );
  if( ticks.status == true ) {
  	iohtmlAdminSubmit( cnt, "Freeze Time Flow" );
  } else if ( ticks.number > 0 ) {
  	iohtmlAdminSubmit( cnt, "Resume Time Flow" );
  } else {
    	iohtmlAdminSubmit( cnt, "Start Time Flow" );
  }
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"giveenergy\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Reload HTTP files" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"deletecons\" value=\"Planet ID\">" );
  iohtmlAdminSubmit( cnt, "Give Energy" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"crap\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Clear Market" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"inactives\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Delete inactives" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"newround\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Deactivate all for new round" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"resettags\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Reset tags" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"changename\" value=\"ID\">" );
  httpString( cnt, "<input type=\"text\" name=\"changetype\" value=\"0\">" );
  httpString( cnt, "<input type=\"text\" name=\"newname\" value=\"New name\">" );
  iohtmlAdminSubmit( cnt, "Change name" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"unexplored\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Recount unexplored planets" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"clearnews\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Clear all news" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"findartefacts\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Find artefacts" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"resetpsychics\" value=\"ID\">" );
  iohtmlAdminSubmit( cnt, "Reset psychics" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"fixtags\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Fix tags" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"findbonuses\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Find bonuses" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"findbig\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Find big planets" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"genranks\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Gen ranks" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"descs\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Fix descs" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"newround2\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Deactivate all for new round, no records" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"forumdelauthor\" value=\"string\">" );
  iohtmlAdminSubmit( cnt, "Delete forum author" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"forumdelip\" value=\"IP\">" );
  iohtmlAdminSubmit( cnt, "Delete forum ip" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"resetpass\" value=\"Command\">" );
  iohtmlAdminSubmit( cnt, "Reset a User Password" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"findancient\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Find ancient" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"showops\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Show ops" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"showbnned\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Show banned mail users" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"banmail\" value=\"user ID\">" );
  httpString( cnt, "<input type=\"text\" name=\"banmailType\" value=\"0\">" );
  iohtmlAdminSubmit( cnt, "Ban/unban user's mail and forums use, 0 - not banned, 1 - banned" );
  
  iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"text\" name=\"banIpAndEmail\" value=\"user ID\">" );
  iohtmlAdminSubmit( cnt, "Ban user's ip and e-mail" );
  
  /*iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"convertPasswords\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "convert passwords into hashes" ); //only use if passwords are stored as plain texts!!! 
															 //Otherwise users wont be able to login anymore!
															 
															 
 */
 
   iohtmlAdminForm( cnt, "administration" );
  httpString( cnt, "<input type=\"hidden\" name=\"findaAll\" value=\"1\">" );
  iohtmlAdminSubmit( cnt, "Find all" );

httpString( cnt, "</td><td width=\"60%\" align=\"left\" valign=\"top\">" );


if( action[0] ) {
	if( UnLoadForumList() == NO )  {
		loghandle(LOG_CRIT, false, "%s", "Forum Smiley List Unload Failed, now exiting..." );
		httpString( cnt, "<i>Forum Smileys can not unload</i><br><br>" );
	} else if( LoadForumList() == NO )  {
		loghandle(LOG_CRIT, false, "%s", "Forum Smiley List Initation Failed, now exiting..." );
		httpString( cnt, "<i>Forum Smileys Re-Initalization Failed</i><br><br>" );
	} else {
		httpString( cnt, "<i>Forum Smileys Re-Initalized</i><br><br>" );
	}
}

  if( action[1] )
  {
    forumd.threads = 0;
    forumd.time = 0;
    forumd.tick = 0;
    forumd.flags = 0;

/*

    sprintf( forumd.title, "News" );
    forumd.rperms = 0xFFF;
    forumd.wperms = 0;
    dbForumAddForum( &forumd, 0, 0 );

    sprintf( forumd.title, "Ideas" );
    forumd.rperms = 0xFFF;
    forumd.wperms = 0xFFF;
    dbForumAddForum( &forumd, 0, 0 );

    sprintf( forumd.title, "Bugs" );
    forumd.rperms = 0xFFF;
    forumd.wperms = 0xFFF;
    dbForumAddForum( &forumd, 0, 0 );

    sprintf( forumd.title, "General" );
    forumd.rperms = 0xFFF;
    forumd.wperms = 0xFFF;
    dbForumAddForum( &forumd, 0, 0 );

    sprintf( forumd.title, "Questions" );
    forumd.rperms = 0xFFF;
    forumd.wperms = 0xFFF;
    dbForumAddForum( &forumd, 0, 0 );

*/

    for( a = 0 ; a < dbMapBInfoStatic[MAP_EMPIRES] ; a++ )
    {
      sprintf( forumd.title, "Empire %d forum", a );
      forumd.rperms = 2;
      forumd.wperms = 2;
      forumd.flags = DB_FORUM_FLAGS_FORUMFAMILY;
      dbForumAddForum( true, &forumd );
    }

    httpString( cnt, "<i>Forums created</i><br><br>" );
  }

if( action[2] ) {
	httpString( cnt, "<i>Server Shutdown has been Iniated!!</i><br><br>" );
	info( "Admin: Shutting down!" );
	sysconfig.shutdown = true;
}

  if( action[3] )
  {
    if( sscanf( action[3], "%d", &a ) == 1 )
    {
      if( dbForumRemoveForum( false, a ) >= 0 )
        httpString( cnt, "<i>Forum deleted</i><br><br>" );
    }
  }

  if( action[4] )
  {
    memset( &forumd, 0, sizeof(dbForumForumDef) );
    time( &forumd.time );
    forumd.lastid = -1;
    iohttpForumFilter( forumd.title, action[4], 64, 0 );
    forumd.rperms = 0xFFF;
    forumd.wperms = 0xFFF;
    if( dbForumAddForum( false, &forumd ) >= 0 )
      httpPrintf( cnt, "Public forum %s added<br><br>", action[4] );
  }

  if( action[5] )
  {
    if( sscanf( action[5], "%d", &a ) == 1 )
    {
      dbUserInfoRetrieve( a, &infod );
      sprintf( infod.forumtag, "Moderator" );
      if( ( user = dbUserLinkID( a ) ) )
      {
        user->level = LEVEL_MODERATOR;
        dbUserSave( a, user );
      }
      dbUserInfoSet( a, &infod );
      httpPrintf( cnt, "Player %d set to moderator<br><br>", a );
    }
  }

  if( action[27] )
  {
    if( sscanf( action[27], "%d", &a ) == 1 )
    {
      dbUserInfoRetrieve( a, &infod );
      sprintf( infod.forumtag, "Forum Moderator" );
      if( ( user = dbUserLinkID( a ) ) )
      {
        user->level = LEVEL_FORUMMOD;
        dbUserSave( a, user );
      }
      dbUserInfoSet( a, &infod );
      httpPrintf( cnt, "Player %d set to forum moderator<br><br>", a );
    }
  }

  if( action[28] )
  {
    if( sscanf( action[28], "%d", &a ) == 1 )
    {
      dbUserInfoRetrieve( a, &infod );
      sprintf( infod.forumtag, "Player" );
      if( ( user = dbUserLinkID( a ) ) )
      {
        user->level = LEVEL_USER;
        dbUserSave( a, user );
      }
      dbUserInfoSet( a, &infod );
      httpPrintf( cnt, "Player %d set to player<br><br>", a );
    }
  }

  //Remove all building in construction on that plnid
	if( action[29] )
  {
  	if(sscanf(action[29], "%d", &a) == 1)
  	{
  		if(dbMapRetrievePlanet( a, &planetd )>0)
  		{
  			planetd.construction = 0;
        dbMapSetPlanet( a, &planetd );
  		}
  	}
  }

  if( action[6] )
  {
    if( sscanf( action[6], "%d", &a ) == 1 )
    {
      cmd[0] = CMD_CHANGE_KILLUSER;
      cmd[1] = a;
      if( cmdExecute( cnt, cmd, 0, 0 ) >= 0 )
        httpPrintf( cnt, "Player %d deleted<br><br>", a );
    }
  }

  if( action[7] )
  {
    if( sscanf( action[7], "%d", &a ) == 1 )
    {
      cmdExecUserDeactivate( a, CMD_USER_FLAGS_DELETED );
      httpPrintf( cnt, "Player %d deactivated<br><br>", a );
    }
  }

  if( action[8] )
  {
    ticks.locked = ticks.status ? true : false;
    ticks.status = !ticks.status;
    httpPrintf( cnt, "Time flow status : %d<br><br>", ticks.status );
  }

  if( action[9] )
  {
    if( sscanf( action[9], "%d", &a ) == 1 )
    {
      dbUserMainRetrieve( a, &maind );
      maind.ressource[CMD_RESSOURCE_ENERGY] += (int64_t)(200000);
      dbUserMainSet( a, &maind );
      httpPrintf( cnt, "User %d now got %lld energy<br><br>", a, (long long)maind.ressource[CMD_RESSOURCE_ENERGY] );
    }
  }

  if( action[10] )
  {
    for( user = dbUserList ; user ; user = user->next )
    {
      dbUserMainRetrieve( user->id, &maind );
      b = dbUserMarketList( user->id, &buffer );
      for( a = c = 0 ; a < b ; a++, c += 5 )
      {

      if( !( buffer[c+DB_MARKETBID_ACTION] ) )
        httpString( cnt, "Buying" );
      else
        httpString( cnt, "Selling" );
      httpPrintf( cnt, " %d %s at %d - <a href=\"market?rmbid=%d\">Withdraw bid</a><br>", buffer[c+DB_MARKETBID_QUANTITY], cmdRessourceName[buffer[c+DB_MARKETBID_RESSOURCE]+1], buffer[c+DB_MARKETBID_PRICE], buffer[c+DB_MARKETBID_BIDID] );


        if( !( buffer[c+DB_MARKETBID_ACTION] ) )
          maind.ressource[CMD_RESSOURCE_ENERGY] += (int64_t)(buffer[c+DB_MARKETBID_QUANTITY] * buffer[c+DB_MARKETBID_PRICE]);
        else
          maind.ressource[buffer[c+DB_MARKETBID_RESSOURCE]+1] += (int64_t)(buffer[c+DB_MARKETBID_QUANTITY]);
      }
      if( buffer )
        free( buffer );
      dbUserMainSet( user->id, &maind );
      dbUserMarketReset( user->id );
    }
    dbMarketReset();
    httpPrintf( cnt, "market crap<br><br>" );
  }

  if( action[11] )
  {
    time( &now );
    for( user = dbUserList ; user ; user = user->next )
    {
      if( dbUserInfoRetrieve( user->id, &infod ) < 0 )
        continue;
      //after 90 day + one day per 100 tagpoints? you are inactive
      a = 90*day + ( 24*60*60*(int)sqrt( (double)( infod.tagpoints / 100 ) ) );
      //a = 90*day; //90 days
      //printf("curr = %d, last = %d ", curtime, user->lasttime);
      if( (now - user->lasttime) < a )
        continue;
      if( user->level >= LEVEL_MODERATOR )
        continue;
      info("Removing inactive account #%d", user->id);
      cmdUserDelete( user->id );
    }
    httpPrintf( cnt, "All inactives removed<br><br>" );
  }

  if( action[12] )
  {
  	/*curtime = time( 0 );
    for( user = dbUserList ; user ; user = user->next )
      cmdExecUserDeactivate( user->id, CMD_USER_FLAGS_NEWROUND );*/
      
    if( pthread_create(&sysconfig.regen_thread,NULL,cmdEndofRound,NULL) == 0 ){
    httpPrintf( cnt, "All accounts deactivated<br><br>" );
	sysconfig.shutdown = true; //to prevent double calling of round reset
	}
    else
    httpPrintf( cnt, "Something bad happened...<br><br>" );
  }

  if( action[13] )
  {
    for( user = dbUserList ; user ; user = user->next )
    {
      if( !( dbUserInfoRetrieve( user->id, &infod ) ) )
        continue;
      if( user->level == LEVEL_USER )
        sprintf( infod.forumtag, "Player" );
      else if( user->level == LEVEL_MODERATOR )
        sprintf( infod.forumtag, "Moderator" );
      else
        sprintf( infod.forumtag, "Administrator" );
      dbUserInfoSet( user->id, &infod );
    }
    httpPrintf( cnt, "All forum tags reseted<br><br>" );
  }

  if( action[14] )
  {
    if( sscanf( action[14], "%d", &a ) == 1 )
    {
    if( sscanf( iohtmlVarsFind( cnt, "changetype" ), "%d", &b ) < 0 )
	return;
    if( !( action[15] ) )
	return;
    if( b == 0 ) {
      if( !( dbUserInfoRetrieve( a, &infod ) ) )
        return;
      iohttpForumFilter( infod.faction, action[15], USER_NAME_MAX, 0 );
      dbUserInfoSet( a, &infod );
      httpPrintf( cnt, "Player %d name changed for %s<br><br>", a, infod.faction );
      } else {
      if( ( user = dbUserLinkID( a ) ) ) {
      		snprintf( user->name, USER_NAME_MAX, "%s", action[15] );
      		dbUserSave( a, user );
		httpPrintf( cnt, "Player %d login changed for %s<br><br>", a, user->name );
      } else {
	httpPrintf( cnt, "No link to user %d<br><br>", a );
      }
      
      }
    }
  }

  if( action[16] )
  {
    for( a = 0 ; a < dbMapBInfoStatic[MAP_SYSTEMS] ; a++ )
    {
      dbMapRetrieveSystem( a, &systemd );
      systemd.unexplored = 0;
      for( b = 0 ; b < systemd.numplanets ; b++ )
      {
        dbMapRetrievePlanet( systemd.indexplanet + b, &planetd );
        if( planetd.owner == -1 )
          systemd.unexplored++;
      }
      dbMapSetSystem( a, &systemd );
    }
  }

  if( action[17] )
  {
    for( user = dbUserList ; user ; user = user->next )
      dbUserNewsEmpty( user->id );
    httpPrintf( cnt, "Cleared news<br><br>" );
  }

  if( action[18] )
  {
    for( a = 0 ; a < dbMapBInfoStatic[MAP_PLANETS] ; a++ )
    {
      dbMapRetrievePlanet( a, &planetd );
      if( ( b = (int)artefactPrecense( &planetd ) ) < 0 )
        continue;
      httpPrintf( cnt, "Artefact %d : %d,%d:%d ( %d )<br>", b, ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF, a );
    }
    httpPrintf( cnt, "<br>" );
  }

  if( action[19] )
  {
    if( sscanf( action[19], "%d", &a ) == 1 )
    {
      if( !( dbUserFleetRetrieve( a, 0, &fleetd ) ) )
        return;
      fleetd.unit[CMD_UNIT_WIZARD] = 1000;
      dbUserFleetSet( a, 0, &fleetd );
      httpPrintf( cnt, "User %d now reset psychics to 1000.<br><br>", a );
    }
  }

  if( action[20] )
  {
    for( user = dbUserList ; user ; user = user->next )
    {
      if( user->level != LEVEL_USER )
        continue;
      dbUserInfoRetrieve( user->id, &infod );
      sprintf( infod.forumtag, "%s", cmdTagFind( infod.tagpoints ) );
      dbUserInfoSet( user->id, &infod );
    }
    dbMarketReset();
    httpPrintf( cnt, "Tags set<br><br>" );
  }

  if( action[21] )
  {
    for( a = b = 0 ; a < dbMapBInfoStatic[MAP_PLANETS] ; a++ )
    {
      dbMapRetrievePlanet( a, &planetd );
      if( !( planetd.special[1] ) )
        continue;
      httpPrintf( cnt, "Bonus +%d%% %s : %d,%d:%d ( %d )<br>", planetd.special[1], cmdBonusName[ planetd.special[0] ], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF, a );
      b++;
    }
    httpPrintf( cnt, "Total : %d planets<br><br>", b );
  }

  if( action[22] )
  {
//    iohtmlGenRanks();
    httpPrintf( cnt, "Ranks generated<br><br>" );
  }

  if( action[23] )
  {
    for( user = dbUserList ; user ; user = user->next )
    {
      dbUserInfoRetrieve( user->id, &infod );
      for( a = 0 ; ( a < USER_DESC_MAX ) && ( infod.desc[a] ) ; a++ )
      {
        if( infod.desc[a] >= 128 )
        {
          infod.desc[0] = 0;
          dbUserInfoSet( user->id, &infod );
          break;
        }
      }
    }
    httpPrintf( cnt, "Descs removed<br><br>" );
  }

  if( action[24] )
  {
    for( user = dbUserList ; user ; user = user->next )
      cmdExecUserDeactivate( user->id, CMD_USER_FLAGS_NEWROUND );
    httpPrintf( cnt, "All accounts deactivated<br><br>" );
  }

  if( action[25] )
  {
    a = iohttpForumCleanAuthor( false, action[25] );
    httpPrintf( cnt, "Deleted author %s ; %d threads<br><br>", action[25], a );
  }

  if( action[26] )
  {
    a = iohttpForumCleanIP( false, action[26] );
    httpPrintf( cnt, "Deleted ip %s ; %d threads<br><br>", action[26], a );
  }

	if( action[30] )
  {
  	if( sscanf( action[30], "%d", &a ) == 1 ) {
  	if( ( user = dbUserLinkID( a ) ) ) {
	if( ( b = dbUserPlanetListIndices( user->id, &plnlist ) ) > 0 )
      {
        for( a = 0 ; a < b ; a++ )
        {
          if( dbMapRetrievePlanet( plnlist[a], &planetd ) < 0 )
            continue;
          planetd.building[CMD_BUILDING_SOLAR] = 0;
          planetd.building[CMD_BUILDING_FISSION] = 0;
          dbMapSetPlanet( plnlist[a], &planetd );
        }
      }
              httpPrintf( cnt, "Energy Production of User %s ID:%d Wiped;", user->faction, user->id );
      }
      if( plnlist )
        free( plnlist );
      }

  }

if( action[31] ) {
	if( sscanf( action[31], "%d", &a ) == 1 ) {
		if( ( user = dbUserLinkID( a ) ) ) {
			snprintf(buff, USER_PASS_MAX, "N%X%X", (unsigned int)random(), (unsigned int)random() );
			if( dbUserInfoRetrieve( user->id, &infod ) < 0 ) {
				httpString( cnt, "<i>Error getting user</i><br><br>" );
			} else {
    				snprintf( infod.password, USER_PASS_MAX, "%s", buff ); //hashencrypt
    			}
			if( dbUserInfoSet( user->id, &infod ) < 0 ) {
				httpString( cnt, "<i>Error encountered when changing password.</i><br><br>" );
       			} else {
				httpPrintf( cnt, "User: \'%s\' password changed to \'%s\'.<br><br>", user->name, buff );
			}
		} else {
			httpString( cnt, "Error occured" );
		}
	}
}

  if( action[32] )
  {
    for( a = b = 0 ; a < dbMapBInfoStatic[MAP_PLANETS] ; a++ )
    {
      dbMapRetrievePlanet( a, &planetd );
      if( planetd.special[0] != CMD_BONUS_ANCIENTCIV )
        continue;
      httpPrintf( cnt, "Bonus +%d%% %s : %d,%d:%d ( %d ) size : %d <br>", planetd.special[1], cmdBonusName[ planetd.special[0] ], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF, a, planetd.size );
      b++;
    }
    httpPrintf( cnt, "Total : %d planets<br><br>", b );
  }
  
    if( action[33] )
  {
    for( a = b = 0 ; a < dbMapBInfoStatic[MAP_PLANETS] ; a++ )
    {
      dbMapRetrievePlanet( a, &planetd );
      if( planetd.size < 400 )
        continue;
      httpPrintf( cnt, " %d,%d:%d ( %d ) %d <br>",( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF, a,  planetd.size  );
      b++;
    }
    httpPrintf( cnt, "Total : %d planets<br><br>", b );
  }
  
  if( action[34] )
  {
	if( sscanf( action[34], "%d", &a ) == 1 )
		showops(cnt, a);
  }
  
    if( action[35] )
  {
	if( sscanf( action[35], "%d", &a ) == 1 ){
    
		for( user = dbUserList ; user ; user = user->next )
		{
		  dbUserMainRetrieve( user->id, &maind );
		  httpPrintf( cnt, "User %d mail %d set<br><br>", user->id, maind.benefactor[4]);
		}
	}
  }
  
    if( action[36] )
  {
	if( sscanf( action[36], "%d", &a ) == 1 ){
		if( sscanf( iohtmlVarsFind( cnt, "banmailType" ), "%d", &b ) < 0 )
		return;
	
		user = dbUserLinkID( a );
		dbUserMainRetrieve( user->id, &maind );
		maind.benefactor[4] = b;
		dbUserMainSet( a, &maind );
		
	}

   }
   
    if( action[37] )
  {
	if( sscanf( action[37], "%d", &a ) == 1 ){

		FILE *file;
		char COREDIR[PATH_MAX];
		ConfigArrayPtr settings;
		settings = GetSetting( "Directory" );
		sprintf( COREDIR, "%s/logs/banList.log", settings->string_value );
		user = dbUserLinkID( a );
		if( ( file = fopen( COREDIR, "a" ) ) ) {
			fprintf( file, "%s|%s|%s\n", user -> name,user->email,user->ip);
			fclose( file );
		}
		 httpPrintf( cnt, "User %d e-mail %s and ip %s banned<br><br>", user->id,user->email,user->ip);
	}

   }
   
  /*if( action[38] )
  {
	if( sscanf( action[38], "%d", &a ) == 1 ){
    
		for( user = dbUserList ; user ; user = user->next )
		{
			if( dbUserInfoRetrieve( user->id, &infod ) < 0 ) {
				httpString( cnt, "<i>Error getting user</i><br><br>" );
			}
			
			char * tmp = generatePassHash(infod.password);

			snprintf( infod.password, strlen(tmp) + 1, "%s", tmp );
		  
			if( dbUserInfoSet( user->id, &infod ) < 0 ) {
				httpString( cnt, "<i>Error encountered when changing password.</i><br><br>" );
			} 
			else {
				httpPrintf( cnt, "User: \'%s\' password changed to \'%s\'.<br><br>", user->name, infod.password );
				httpPrintf( cnt, "%s\n", tmp);
			}
		}
	}
  }*/
  
    if( action[39] )
  {
    for( a =  0 ; a < dbMapBInfoStatic[MAP_PLANETS] ; a++ )
    {
      dbMapRetrievePlanet( a, &planetd );
      httpPrintf( cnt, "Bonus +%d%% %s : %d,%d:%d ( %d ) size : %d <br>", planetd.special[1], cmdBonusName[ planetd.special[0] ], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF, a, planetd.size );
    }
    httpPrintf( cnt, "Total : %d planets<br><br>", a );
  }
  
  
goto END;

iohtmlFunc_admin_mainL0:
httpString( cnt, "You do not have administrator privileges." );

END:
httpString( cnt, "</td></tr></table>" );
iohtmlBodyEnd( cnt );
return;
}



