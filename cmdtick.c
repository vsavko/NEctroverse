int64_t cmdTickProduction[CMD_BLDG_NUMUSED];
int64_t StationedUnits[CMD_UNIT_NUMUSED];

void cmdTickGenRanks() {
	int first, num, artmax, wa, wnum;
	FILE *file, *filep;
	ConfigArrayPtr settings[2];
	dbUserMainDef maind;
	dbMainEmpirePtr empirep;
	dbMainEmpireDef empired;
	dbUserMainPtr mainp;
	dbUserPtr user;
	int64_t a, b, c, d;
	int64_t *stats;
	int *rels;
	int artefacts[ARTEFACT_NUMUSED], artsnum;
	char COREDIR[PATH_MAX];
    bool warTag = false;
    int *rel;
    int maxRelations = 0, i = 0;

  artmax = 0;
  settings[0] = GetSetting( "Directory" );
  sprintf( COREDIR, "%s/rankings/round%dfamranks.txt", settings[0]->string_value, ticks.round );
  memset( artefacts, 0, ARTEFACT_NUMUSED*sizeof(int) );
  if( !( file = fopen( COREDIR, "wb" ) ) )
    return;
  sprintf( COREDIR, "%s/rankings/round%dfamranksplain.txt", settings[0]->string_value, ticks.round);
  if( !( filep =fopen( COREDIR, "wb" ) ) )
  {
    fclose( file );
    return;
  }
  if( !( stats = malloc( 6*dbMapBInfoStatic[MAP_EMPIRES]*sizeof(int64_t) + sizeof(dbMainEmpireDef)*dbMapBInfoStatic[MAP_EMPIRES] ) ) )
  {
    fclose( file );
    return;
  }
  empirep = (void *)&stats[6*dbMapBInfoStatic[MAP_EMPIRES]];
  memset( stats, 0, 6*dbMapBInfoStatic[MAP_EMPIRES]*sizeof(int64_t) );
settings[0] = GetSetting( "Admin Empire Number" );
settings[1] = GetSetting( "Admin Empire Ommit" );
  for( b = c = num = 0 ; b < dbMapBInfoStatic[MAP_EMPIRES] ; b++ )
  {
	if( (b == (int)settings[1]->num_value) && ( (int)settings[1]->num_value ) )
		continue;
    if( dbEmpireGetInfo( b, &empirep[b] ) < 0 )
      continue;
    if( !( empirep[b].numplayers ) )
      continue;

settings[0] = GetSetting( "End Wars" );
if( (int)settings[0]->num_value > 0 ) {
	if( ( wnum = dbEmpireRelsList( b, &rels ) ) >= 0 ) {
		wnum *= 6;
		for( wa = 0 ; wa < wnum ; wa += 6 ) {
			if( rels[wa+1] == CMD_RELATION_WAR ) {
				if( (rels[wa] + (int)settings[0]->num_value) <= ticks.number ) {
					cmdExecDelRelation( b, wa / 6 );
				}
			}
			if( rels[wa+1] == CMD_RELATION_NAP && rels[wa+4] == 4 ) {
				if( (rels[wa] + rels[wa+5]*(3600/ticks.speed)) <= ticks.number ) {
				//if( (rels[wa] + rels[wa+5]) <= ticks.number ) {
					cmdExecDelRelation( b, wa / 6 );
				}
			}
		}
		if( rels ) {
			free( rels );
		}
	} else {
		error( "Getting relations list for Empire:%d", b );
	}
}

    stats[c+0] = b;
// calc NW, planets and empire artefacts
    for( a = 0 ; a < empirep[b].numplayers ; a++ )
    {
      if( dbUserMainRetrieve( empirep[b].player[a], &maind ) < 0 )
        continue;
      stats[c+1] += maind.planets;
      stats[c+2]++;
      stats[c+3] += maind.networth;
    }
    empirep[b].planets = stats[c+1];
    empirep[b].networth = stats[c+3];
    dbEmpireSetInfo( b, &empirep[b] );
    num++;

// set artefacts for empire members
    for( a = 0 ; a < empirep[b].numplayers ; a++ )
    {
      if( dbUserMainRetrieve( empirep[b].player[a], &maind ) < 0 )
        continue;
      maind.artefacts = empirep[b].artefacts;
      dbUserMainSet( empirep[b].player[a], &maind );
    }

    c += 6;
  }

// empire rankings
  first = 0;
  stats[4] = -1;
  stats[5] = -1;
  for( b = 1, c = 6 ; b < num ; b++, c += 6 )
  {
    if( ( stats[c+1] > stats[first+1] ) || ( ( stats[c+1] == stats[first+1] ) && ( stats[c+3] > stats[first+3] ) ) )
    {
      stats[first+4] = c;
      stats[c+5] = first;
      stats[c+4] = -1;
      first = c;
      continue;
    }
    a = stats[first+5];
    d = first;
    for( ; ; )
    {
      if( a == -1 )
      {
        stats[d+5] = c;
        stats[c+4] = d;
        stats[c+5] = -1;
        break;
      }
      if( ( stats[c+1] > stats[a+1] ) || ( ( stats[c+1] == stats[a+1] ) && ( stats[c+3] > stats[a+3] ) ) )
      {
        stats[c+4] = d;
        stats[c+5] = a;
        stats[d+5] = c;
        stats[a+4] = c;
        break;
      }
      d = a;
      a = stats[a+5];
    }
  }
  if( !( num ) )
    first = -1;
  fprintf( file, "<table cellspacing=\"4\"><tr><td align=\"right\">Rank</td><td>Name</td><td>Planets</td><td>Players</td><td>Networth</td></tr>" );
  for( a = first, b = 1 ; a != -1 ; b++ )
  {
    fprintf( file, "<tr><td align=\"right\" nowrap>" );



/*
    for( c = 0, d = 1 ; c < ARTEFACT_NUMUSED ; c++, d <<= 1 )
    {
      if( empirep[stats[a+0]].artefacts & d )
        fprintf( file, "<img src=\"%s\">", artefactImage[c] );
    }
*/

    artsnum = 0;
    for( c = 0, d = 1 ; c < ARTEFACT_NUMUSED ; c++, d <<= 1 )
    {
      if( empirep[stats[a+0]].artefacts & d )
      {
        artsnum++;
        artefacts[c] = 1;
      }
    }

if( artsnum > artmax )
	artmax = artsnum;

if( ( ARTEFACT_NUMUSED > 0 ) && ( artsnum == ARTEFACT_NUMUSED ) ) {
        if ( dbMapBInfoStatic[MAP_ARTITIMER] == -1 ) {
	        settings[0] = GetSetting( "Auto Victory" );
	        if( (int)settings[0]->num_value > 0 ) {
        	        dbMapBInfoStatic[MAP_ARTITIMER] = ticks.number + (int)settings[0]->num_value;
        	        dbMapBInfoStatic[MAP_TIMEMPIRE] = stats[a+0];
	                dbMapSetMain( dbMapBInfoStatic );
                }
        } else if ( ( dbMapBInfoStatic[MAP_TIMEMPIRE] == stats[a+0] ) && ( dbMapBInfoStatic[MAP_ARTITIMER] <= ticks.number ) ) {
                ticks.status = 0;
                ticks.locked = 1;
        }
} else if( dbMapBInfoStatic[MAP_TIMEMPIRE] == stats[a+0] ) {
        dbMapBInfoStatic[MAP_ARTITIMER] = -1;
        dbMapBInfoStatic[MAP_TIMEMPIRE] = -1;
        dbMapSetMain( dbMapBInfoStatic );
}

    if( ( 3*artsnum >= ARTEFACT_NUMUSED ) || ( (3*dbArtefactMax)/2 >= ARTEFACT_NUMUSED ) )
    {
	  if (! (artsnum == 4 && (empirep[stats[a+0]].artefacts & ARTEFACT_64_BIT)) ){
		  for( c = 0, d = 1 ; c < ARTEFACT_NUMUSED ; c++, d <<= 1 )
		  {
			if( empirep[stats[a+0]].artefacts & d ){
			  //fprintf( file, "<img src=\"files?type=image&name=artefact/%s\">", ArtefactTable[c]->image );
				if (d == ARTEFACT_64_BIT) continue;
				fprintf( file, "<img src=\"files?type=image&name=artefact/%s\">", artefactImage[c] );
			}
		  }
	  }
    }

	//war tag
	warTag = false; 
	 if( ( maxRelations = dbEmpireRelsList( empirep[stats[a+0]].id, &rel ) ) > 0 )
	 {
		  maxRelations *= 6;
		  for( i = 0 ; i < maxRelations ; i += 6 )
		  {
		   if( rel[i+3] & 1 )
			continue;
		   else if( rel[i+1] == CMD_RELATION_WAR ){
			warTag = true;
			break;
		   }
		  }
		  for( i = 0 ; i < maxRelations ; i += 6 )
		  {
		   if( !( rel[i+3] & 1 ) )
			continue;
		   else if( rel[i+1] == CMD_RELATION_WAR ){
			warTag = true;
			break;
		   }
		  }
	 }
 

	fprintf( file, " %lld</td><td><a href=\"empire?id=%lld\">", (long long)b, (long long)stats[a+0] );
  	if( empirep[stats[a+0]].name[0] )
      fprintf( file, "%s%s #%lld", empirep[stats[a+0]].name, warTag ? "<font color=\"#FF2020\"> @war</font>" : "", (long long)stats[a+0] );
    else
      fprintf( file, "Empire #%lld%s", (long long)stats[a+0],warTag ? "<font color=\"#FF2020\"> @war</font>" : "" );
    fprintf( file, "</a></td><td align=\"center\">%lld</td><td align=\"center\">%lld</td><td align=\"center\">%lld</td></tr>\n", (long long)stats[a+1], (long long)stats[a+2], (long long)stats[a+3] );

    /* Plain Text */
    fprintf( filep, "%lld:%lld:%lld:%lld:%lld:", (long long)b, (long long)stats[a+0], (long long)stats[a+1], (long long)stats[a+2], (long long)stats[a+3] );
    if( empirep[stats[a+0]].name[0] )
      fprintf( filep, "%s #%lld", empirep[stats[a+0]].name, (long long)stats[a+0] );
    else
      fprintf( filep, "Empire #%lld", (long long)stats[a+0] );
    fprintf( filep, "\n" );

    // moo
    empirep[stats[a+0]].rank = b;
    dbEmpireSetInfo( stats[a+0], &empirep[stats[a+0]] );




    a = stats[a+5];
  }
  fprintf( file, "</table>" );

  fprintf( file, "<br>" );
  artsnum = 0;
for( c = 0 ; c < ARTEFACT_NUMUSED ; c++ )
    artsnum |= artefacts[c];



if( artsnum ) {
	if( !(dbMapBInfoStatic[MAP_ARTITIMER] == -1 ) ) {
		if( dbEmpireGetInfo( dbMapBInfoStatic[MAP_TIMEMPIRE], &empired ) < 0 )
		      return;
		if( empired.name[0] ) {
			if( (dbMapBInfoStatic[MAP_ARTITIMER] - ticks.number) <= 0 )
				fprintf( file, "<br><br><b>All Artefacts held by: %s</b><br>", empired.name );
			else
				fprintf( file, "<br><br><b>All Artefacts held by: %s<br>Round will end in %d weeks!</b><br>", empired.name, dbMapBInfoStatic[MAP_ARTITIMER] - ticks.number );
		} else {
			if( (dbMapBInfoStatic[MAP_ARTITIMER] - ticks.number) <= 0 )
				fprintf( file, "<br><br><b>All Artefacts held by: Empire #%d</b><br>", dbMapBInfoStatic[MAP_TIMEMPIRE] );
			else
				fprintf( file, "<br><br><b>All Artefacts held by: Empire #%d<br>Round will end in %d weeks!</b><br>", dbMapBInfoStatic[MAP_TIMEMPIRE], dbMapBInfoStatic[MAP_ARTITIMER] - ticks.number );
		}
	} else {
		fprintf( file, "<br><br><b>Artefacts found</b><br>" );
	}
	fprintf( file, "<table><tr><td>" );
	for( c = 0 ; c < ARTEFACT_NUMUSED ; c++ ) {
		if( artefacts[c] )
			//fprintf( file, "<img src=\"files?type=image&name=artefact/%s\"> %s<br>", ArtefactTable[c]->image, ArtefactTable[c]->description );
			fprintf( file, "<img src=\"files?type=image&name=artefact/%s\"> %s<br>", artefactImage[c], artefactDescription[c] );
	}
	fprintf( file, "</td></tr></table><br>" );
}
dbArtefactMax = artmax;

  free( stats );
  fclose( file );
  fclose( filep );


// player rankings
  settings[0] = GetSetting( "Directory" );
  sprintf( COREDIR, "%s/rankings/round%dranks.txt", settings[0]->string_value, ticks.round );
  if( !( file = fopen( COREDIR, "wb" ) ) )
    return;
  sprintf( COREDIR, "%s/rankings/round%dranksplain.txt", settings[0]->string_value, ticks.round );
  if( !( filep = fopen( COREDIR, "wb" ) ) )
  {
    fclose( file );
    return;
  }
  for( user = dbUserList, num = 0 ; user ; user = user->next )
    num++;
  if( !( num ) || !( stats = malloc( 6*num*sizeof(int64_t) + sizeof(dbUserMainDef)*num ) ) )
  {
    fclose( file );
    return;
  }
  mainp = (void *)&stats[6*num];
  memset( stats, 0, 6*num*sizeof(int64_t) );
settings[0] = GetSetting( "Admin Empire Number" );
settings[1] = GetSetting( "Admin Empire Ommit" );
  for( b = c = 0, user = dbUserList ; user ; user = user->next )
  {
    if( !( bitflag( user->flags, CMD_USER_FLAGS_ACTIVATED ) ) )
	{
	  //printf("user %d not activated\n", user->id );
      continue;
	}
    if( dbUserMainRetrieve( user->id, &mainp[b] ) < 0 ) {
	error( "Error retreiving maind id: %d", user->id );
      continue;
	}
	if( (mainp[b].empire == (int)settings[0]->num_value) && ((int)settings[1]->num_value) )
		continue;
    stats[c+0] = user->id;
    stats[c+1] = mainp[b].planets;
    stats[c+2] = mainp[b].networth;
    stats[c+3] = b;
    b++;
    c += 6;
  }
  num = b;

  first = 0;
  stats[4] = -1;
  stats[5] = -1;
  for( b = 1, c = 6 ; b < num ; b++, c += 6 )
  {
    if( ( stats[c+1] > stats[first+1] ) || ( ( stats[c+1] == stats[first+1] ) && ( stats[c+2] > stats[first+2] ) ) )
    {
      stats[first+4] = c;
      stats[c+5] = first;
      stats[c+4] = -1;
      first = c;
      continue;
    }
    a = stats[first+5];
    d = first;
    for( ; ; )
    {
      if( a == -1 )
      {
        stats[d+5] = c;
        stats[c+4] = d;
        stats[c+5] = -1;
        break;
      }
      if( ( stats[c+1] > stats[a+1] ) || ( ( stats[c+1] == stats[a+1] ) && ( stats[c+2] > stats[a+2] ) ) )
      {
        stats[c+4] = d;
        stats[c+5] = a;
        stats[d+5] = c;
        stats[a+4] = c;
        break;
      }
      d = a;
      a = stats[a+5];
    }
  }
  if( !( num ) )
    first = -1;
  fprintf( file, "<table cellspacing=\"4\"><tr><td>Rank</td><td>Faction</td><td>Empire</td><td>Planets</td><td>Networth</td><td>Race</td></tr>" );
  for( a = first, b = 1 ; a != -1 ; b++ )
  {
    fprintf( file, "<tr><td align=\"right\">%lld</td><td><a href=\"player?id=%lld\">%s</a></td><td>\
	<a href=\"empire?id=%d\">empire #%d</a></td><td align=\"center\">%d</td><td align=\"center\">%lld\
	</td><td align=\"left\">%s</td></tr>", (long long)b, (long long)stats[a+0], mainp[stats[a+3]].faction,\
	mainp[stats[a+3]].empire, mainp[stats[a+3]].empire, mainp[stats[a+3]].planets, (long long)mainp[stats[a+3]].networth,\
	cmdRaceName[mainp[stats[a+3]].raceid] );

    fprintf( filep, "%lld:%lld:%d:%d:%lld:%s:%s\n", (long long)b, (long long)stats[a+0], mainp[stats[a+3]].empire,\
	mainp[stats[a+3]].planets, (long long)mainp[stats[a+3]].networth, mainp[stats[a+3]].faction, cmdRaceName[mainp[stats[a+3]].raceid] );



    // moo
    mainp[stats[a+3]].rank = b;
    dbUserMainSet( stats[a+0], &mainp[stats[a+3]] );



    a = stats[a+5];
  }
  fprintf( file, "</table>" );

  free( stats );
  fclose( file );
  fclose( filep );



  return;
}





void cmdTickInit(){

(void)pthread_mutex_lock( &mutex );

RANDOMIZE_SEED;

return;
}


void cmdTickEnd() {

cmdTickGenRanks();

savetickconfig();

(void)pthread_mutex_unlock( &mutex );

#if IRCBOT_SUPPORT
	ConfigArrayPtr setting;
	char buffer[2][512];
setting = GetSetting( "IRC Enable" );
if( setting->num_value ) {
	setting = GetSetting( "IRC Announce" );
	if( ( setting->num_value ) && ( ticks.status ) ) {
		snprintf( buffer[0], sizeof(buffer), "Game has Ticked -- Week %d, Year %d (Tick #%d)", ticks.number % 52, ticks.number / 52, ticks.number );
		setting = GetSetting( "IRC Channel" );
		snprintf( buffer[1], 512, "#%s", setting->string_value );
		irc_cmd_notice( sysconfig.irc_session, buffer[1], buffer[0] );
	}
}
#endif

dbRegisteredInfo[DB_TOTALS_USERS_REGISTERED] = ticks.uregist;
dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED] = ticks.uactive;
dbRegisteredInfo[DB_TOTALS_USERS_ONLINE] = ticks.uonline;

return;
}




int cmdTickPlanets( int usrid, dbUserMainPtr mainp ) {
	int a, b, num, nump, nInfection;
	int *buffer;
	int *portals;
	int64_t population;
	dbMainPlanetDef planetd;
	dbMainEmpireDef empired;
	dbUserSpecOpPtr specopd;

ticks.debug_pass = 0 + 10000;


memset( mainp->totalbuilding, 0, (CMD_BLDG_NUMUSED+1)*sizeof(int64_t) );
memset( mainp->totalunit, 0, CMD_UNIT_NUMUSED*sizeof(int64_t) );
memset( cmdTickProduction, 0, CMD_BLDG_NUMUSED*sizeof(int64_t) );
memset( StationedUnits, 0, CMD_UNIT_NUMUSED*sizeof(int64_t) );

nInfection = 0;
b = dbUserSpecOpList( usrid, &specopd );
if( specopd ) {
	for(a = 0; a < b; a++) {
		if (specopd[a].type == (CMD_OPER_BIOINFECTION|0x10000)) {
			nInfection++;
		}
  	}
	free( specopd );
}

portals = 0;
nump = 0;

if( ( nump = dbUserPortalsListCoords( usrid, &portals ) ) < 0 ) {
	free( buffer );
	return NO;
}
if( ( num = dbUserPlanetListIndices( usrid, &buffer ) ) < 0 ) {
	return NO;
}

ticks.debug_pass = 1 + 10000;
population = 0;

for( a = 0 ; a < num ; a++ ) {
	dbMapRetrievePlanet( buffer[a], &planetd );
	planetd.maxpopulation = ( planetd.size * CMD_POPULATION_SIZE_FACTOR );
	planetd.maxpopulation += ( planetd.building[CMD_BUILDING_CITIES] * cmdBuildingProduction[CMD_BUILDING_CITIES] );
	planetd.maxpopulation *= ( 1.00 + 0.01 * mainp->totalresearch[CMD_RESEARCH_POPULATION] );
	
	/*if (mainp->artefacts & ARTEFACT_32_BIT){
		planetd.maxpopulation *= 1.25;
	}*/

		//ARTI CODE Super Stacker
	/*	if(mainp->artefacts & ARTEFACT_*_BIT)
			planetd.maxpopulation = (float)( ( planetd.size * CMD_POPULATION_SIZE_FACTOR ) + ( planetd.building[CMD_BUILDING_CITIES] * (CMD_POPULATION_CITIES+1000) ) );
	*/
	ticks.debug_pass = 2 + 10000;

	/*if(mainp->artefacts & ARTEFACT_16_BIT) {
		planetd.population += ceil( (( planetd.population * ( cmdRace[mainp->raceid].growth * 1.25 ) ) * ( 1.00 + 0.001 * mainp->totalresearch[CMD_RESEARCH_POPULATION] )) * pow(0.75, (float)nInfection) );
	} else {
	}*/
	
	if (mainp->artefacts &  ARTEFACT_512_BIT)
		planetd.population += ceil( (( 3 * planetd.population * ( cmdRace[mainp->raceid].growth ) ) * ( 1.00 + 0.001 * mainp->totalresearch[CMD_RESEARCH_POPULATION] )) * pow(0.75, (float)nInfection) );
	else
		planetd.population += ceil( (( planetd.population * ( cmdRace[mainp->raceid].growth ) ) * ( 1.00 + 0.001 * mainp->totalresearch[CMD_RESEARCH_POPULATION] )) * pow(0.75, (float)nInfection) );
	
	
	planetd.population = fmin( planetd.maxpopulation, planetd.population );


	if( ( planetd.flags & CMD_PLANET_FLAGS_PORTAL ) ) {
		planetd.protection = 100;
	} else {
      		planetd.protection = (int)( 100.0 * battlePortalCalc( ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, portals, nump, mainp->totalresearch[CMD_RESEARCH_PORTALS] ) );
	}

	if( planetd.construction < 0 ) {
		error( "Warning : negative construction count : %d", planetd.construction );
		planetd.construction = 0;
	}


	ticks.debug_pass = 3 + 10000;


	/* CRAP */
	for( b = 0 ; b < CMD_BLDG_NUMUSED ; b++ ) {
		if( planetd.building[b] < 0 ) {
			
				error( "Warning : negative building count : %d id %d b %d", planetd.building[b], planetd.id, b );
				planetd.building[b] = 0;
			}
	}

	dbMapSetPlanet( buffer[a], &planetd );

	ticks.debug_pass = 4 + 10000;

	population += planetd.population;

	for( b = 0 ; b < CMD_BLDG_NUMUSED ; b++ ) {
		mainp->totalbuilding[b] += planetd.building[b];
		cmdTickProduction[b] += ( cmdBuildingProduction[b] * planetd.building[b] );
	}

	for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ )
		StationedUnits[b] += planetd.unit[b];

	if( ( planetd.flags & CMD_PLANET_FLAGS_PORTAL ) )
		mainp->totalbuilding[CMD_BLDG_NUMUSED]++;


	ticks.debug_pass = 5 + 10000;

	if( planetd.special[1] ) {
		int factor;
		//if ( mainp->artefacts & ARTEFACT_2_BIT)
		//	factor = 2;
		//else
			factor = 1;
			
		if( planetd.special[0] == CMD_BONUS_ENERGY ) {
			cmdTickProduction[CMD_BUILDING_SOLAR] += ( factor * planetd.special[1] * ( planetd.building[CMD_BUILDING_SOLAR] * cmdBuildingProduction[CMD_BUILDING_SOLAR] ) ) / 100;
		} else if( planetd.special[0] == CMD_BONUS_MINERAL ) {
			cmdTickProduction[CMD_BUILDING_MINING] += ( factor * planetd.special[1] * ( planetd.building[CMD_BUILDING_MINING] * cmdBuildingProduction[CMD_BUILDING_MINING] ) ) / 100;
		} else if( planetd.special[0] == CMD_BONUS_CRYSTAL ) {
			cmdTickProduction[CMD_BUILDING_CRYSTAL] += ( factor * planetd.special[1] * ( planetd.building[CMD_BUILDING_CRYSTAL] * cmdBuildingProduction[CMD_BUILDING_CRYSTAL] ) ) / 100;
		} else if( planetd.special[0] == CMD_BONUS_ECTROLIUM ) {
			cmdTickProduction[CMD_BUILDING_REFINEMENT] += ( factor * planetd.special[1] * ( planetd.building[CMD_BUILDING_REFINEMENT] * cmdBuildingProduction[CMD_BUILDING_REFINEMENT] ) ) / 100;
		} else if( planetd.special[0] == CMD_BONUS_FISSION ) {
			cmdTickProduction[CMD_BUILDING_FISSION] += ( factor * planetd.special[1] * ( planetd.building[CMD_BUILDING_FISSION] * cmdBuildingProduction[CMD_BUILDING_FISSION] ) ) / 100;
		}
	}


	ticks.debug_pass = 6 + 10000;


	if( ( b = (int)artefactPrecense( &planetd ) ) < 0 )
		continue;

	if( dbEmpireGetInfo( mainp->empire, &empired ) < 0 )
		continue;

	empired.artefacts |= 1 << b;
	dbEmpireSetInfo( mainp->empire, &empired );


	ticks.debug_pass = 7 + 10000;
}
free( buffer );

mainp->planets = num;
mainp->ressource[CMD_RESSOURCE_POPULATION] = population;


ticks.debug_pass = 9 + 10000;

if( portals )
	free( portals );

ticks.debug_pass = 10 + 10000;


return YES;
}





int cmdTick() {
	int a, c, d, e, num, specopnum, opvirus /*,cmd[3], i*/;
	float fb, phdecay;
	double fa, fc;
	int64_t newd[DB_USER_NEWS_BASE],/* nIllusion,*/ b;
	//int nChicks = 0, penalty;
	int marketbid[DB_MARKETBID_NUMUSED];
	int bidresult[2];
	int *plist;
	dbUserPtr user;
	dbUserMainDef maind;
	dbBuildPtr build;
	dbMainPlanetDef planetd;
	dbUserFleetDef fleetd;
	dbUserFleetPtr fleetp;
	dbUserSpecOpPtr specopd;
	dbMainEmpireDef empired;
	ConfigArrayPtr settings;


ticks.uonline = 0;
ticks.uactive = 0;
ticks.uregist = 0;
ticks.debug_pass = 0;
ticks.debug_id = 0;
time(&now);

for( a = 0 ; a < dbMapBInfoStatic[MAP_EMPIRES] ; a++ ) {
	if( dbEmpireGetInfo( a, &empired ) < 0 )
		continue;
	ticks.debug_id = a;
	empired.artefacts = 0;
	//Add decay to empire funds, decay (if any) removed before deposits from this tick.
	empired.infos[CMD_RESSOURCE_ENERGY] = fmax( 0.0, ( CMD_ENERGY_DECAY * empired.fund[CMD_RESSOURCE_ENERGY] ) );
	empired.infos[CMD_RESSOURCE_CRYSTAL] = fmax( 0.0, (  CMD_CRYSTAL_DECAY * empired.fund[CMD_RESSOURCE_CRYSTAL] ) );
	empired.fund[CMD_RESSOURCE_ENERGY] = fmax( 0.0, ( empired.fund[CMD_RESSOURCE_ENERGY] - empired.infos[CMD_RESSOURCE_ENERGY] ) );
	empired.fund[CMD_RESSOURCE_CRYSTAL] = fmax( 0.0, ( empired.fund[CMD_RESSOURCE_CRYSTAL] - empired.infos[CMD_RESSOURCE_CRYSTAL] ) );
	dbEmpireSetInfo( a, &empired );
}


ticks.debug_pass = 1;


if( ( dbMapRetrieveMain( dbMapBInfoStatic ) < 0 ) ) {
	error( "Tick error: Retriving Map Info!" );
}


// Super stacker ARTEFACT_128_BIT code!
for( a = 0 ; a < dbMapBInfoStatic[MAP_PLANETS] ; a++ )
{
  dbMapRetrievePlanet( a, &planetd );
  if( (int)artefactPrecense( &planetd ) == 7 ){
	planetd.size += 7;
	dbMapSetPlanet( a, &planetd );
	break;
  }
}

for( user = dbUserList ; user ; user = user->next ) {
	ticks.uregist++;
	ticks.debug_id = user->id;
	if( ( (now - user->lasttime) > SESSION_TIME ) && ( strlen(user->http_session) ) ) {
		if( remove_session( user->http_session ) == NO ) {
			memset( user->http_session, 0, sizeof(user->http_session) );
			dbUserSave( user->id, user );
		}
	}

	if( !( bitflag( user->flags, CMD_USER_FLAGS_ACTIVATED ) ) || ( bitflag( user->flags, CMD_USER_FLAGS_FROZEN ) ) ) {
		continue;
	}

	ticks.uactive++;
	ticks.uonline += ( ( (now - user->lasttime) < (SESSION_TIME / 4) ) && ( strlen(user->http_session) ) );

	if( dbUserMainRetrieve( user->id, &maind ) < 0 ) {
		error( "Tick error: Retriving User %d", user->id );
		continue;
	}
	if( maind.empire < 0 )
		continue;

	ticks.debug_pass = 2;


	if( ( specopnum = dbUserSpecOpList( user->id, &specopd ) )  < 0 ) {
		error( "Tick error: SpecOps User %d", user->id );
		continue;
	}
	opvirus = 0;
	/*
	//WAR  ILLUSION we recalcul each tick
	for(i=0;i<specopnum;i++) {
		if (specopd[i].type == (CMD_SPELL_WARILLUSIONS | 0x1000)) {
			fa = 0.4 + (1.2/255.0) * (float)( rand() & 255 );
			nChicks = maind.totalunit[CMD_UNIT_WIZARD];
			nIllusion = ( fa * cmdRace[maind.raceid].unit[CMD_UNIT_WIZARD] * (float)nChicks * ( 1.0 + 0.005*maind.totalresearch[CMD_RESEARCH_CULTURE] ) / cmdPsychicopDifficulty[CMD_SPELL_WARILLUSIONS] );
			penalty = cmdGetOpPenalty( maind.totalresearch[CMD_RESEARCH_CULTURE], cmdPsychicopTech[CMD_SPELL_WARILLUSIONS] );
			if( penalty )
		    	nIllusion = (float)nIllusion / ( 1.0 + 0.01*(float)penalty );
			fa = 100.0 * (float)nIllusion / (float)maind.networth;
    			a = (int)( fa * 4.5 );
    			a += a * rand()%20;
    			if (a<0)
    				a = 0;
    			specopd[i].vars[0] = a;
		} else if (specopd[i].type == (CMD_SPELL_DARKWEB | 0x1000)) {
			fa = 0.4 + (1.2/255.0) * (float)( rand() & 255 );
			nChicks = maind.totalunit[CMD_UNIT_WIZARD];
			nIllusion = ( fa * cmdRace[maind.raceid].unit[CMD_UNIT_WIZARD] * (float)nChicks * ( 1.0 + 0.005*maind.totalresearch[CMD_RESEARCH_CULTURE] ) / cmdPsychicopDifficulty[CMD_SPELL_DARKWEB] );
			penalty = cmdGetOpPenalty( maind.totalresearch[CMD_RESEARCH_CULTURE], cmdPsychicopTech[CMD_SPELL_DARKWEB] );
			if( penalty )
			    	nIllusion = (float)nIllusion / ( 1.0 + 0.01*(float)penalty );
			fa = 100.0 * (float)nIllusion / (float)maind.networth;
			a = (int)( fa * 3.5 );
    			if (a<0)
    				a = 0;
    			specopd[i].vars[0] = a;
		}
	}
	*/
	for( a = specopnum-1 ; a >= 0 ; a-- ) {
		if( specopd[a].type == ( CMD_OPER_NETWORKVIRUS | 0x10000 ) )
			opvirus++;
	}


	ticks.debug_pass = 3;


	num = dbUserBuildListReduceTime( user->id, &build );
	for( a = num-1 ; a >= 0 ; a-- ) {
		memset(&newd, 0, sizeof(int64_t)*DB_USER_NEWS_BASE);
		newd[0] = ticks.number;
		newd[1] = CMD_NEWS_FLAGS_NEW;
		if( build[a].time > 0 )
			continue;
		if( !( build[a].type >> 16 ) ) {
			dbMapRetrievePlanet( build[a].plnid, &planetd );
			if( build[a].type == CMD_BLDG_NUMUSED ) {
				// portal
				planetd.flags &= 0xFFFFFFFF - CMD_PLANET_FLAGS_PORTAL_BUILD;
				planetd.flags |= CMD_PLANET_FLAGS_PORTAL;
				dbUserPlanetSetFlags( user->id, build[a].plnid, planetd.flags );
				planetd.construction--;
			} else {
				planetd.building[ build[a].type ] += build[a].quantity;
				planetd.construction -= build[a].quantity;
			}
			dbMapSetPlanet( build[a].plnid, &planetd );
			newd[2] = CMD_NEWS_BUILDING;
			newd[5] = build[a].plnid;
			newd[6] = build[a].plnpos;
		} else {
			if( !( dbUserFleetRetrieve( user->id, 0, &fleetd ) ) )
				continue;
			fleetd.unit[ build[a].type & 0xFFFF ] += build[a].quantity;
			if( !( dbUserFleetSet( user->id, 0, &fleetd ) ) )
				continue;
			newd[2] = CMD_NEWS_UNIT;
		}
		dbUserBuildRemove( user->id, a );
		newd[3] = build[a].type;
		newd[4] = build[a].quantity;
		cmdUserNewsAdd( user->id, newd, CMD_NEWS_FLAGS_BUILD );
	}
	free( build );


	ticks.debug_pass = 4;

	// calc total of buildings, units, artefacts
	if( cmdTickPlanets( user->id, &maind ) == NO ) {
		error( "User %d Planet Tick Failed", user->id );
		return NO;
	}

	ticks.debug_pass = 5;

	// add research
	int64_t total_rc = 0;

	for( a = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ ) {
		int artiBonus = 0;
		/*if( maind.artefacts & ARTEFACT_16_BIT )
			artiBonus = maind.totalbuilding[CMD_BUILDING_FISSION] * 100;*/
		
		//if(maind.artefacts & ARTEFACT_64_BIT) 
		//	fa = ( (maind.allocresearch[a]) * ( 100 * cmdTickProduction[CMD_BUILDING_RESEARCH] * 1.20 + maind.fundresearch + artiBonus) ) / 10000.0;
		//else	
			fa = ( (maind.allocresearch[a]) * ( 100*cmdTickProduction[CMD_BUILDING_RESEARCH] + 1.2*maind.fundresearch + artiBonus) ) / 10000.0;
		
	
		
	/*		//ARTI CODE Foohon Ancestry
			if(maind.artefacts & ARTEFACT_*_BIT)
				fa += ( (double)(maind.allocresearch[a]) * (double)maind.ressource[CMD_RESSOURCE_POPULATION] ) / ( 400.0 * 100.0 );
	*/
		maind.research[a] += cmdRace[maind.raceid].researchpoints[a] * fa * specopEnlightemntCalc(user->id,CMD_ENLIGHT_RESEARCH);
		
		if( cmdRace[maind.raceid].special & CMD_RACE_SPECIAL_POPRESEARCH )
			maind.research[a] += 1.2*( (maind.allocresearch[a]) * maind.ressource[CMD_RESSOURCE_POPULATION] ) / ( 600.0 * 100.0 );
			
		maind.research[a] = fmax( 0.0, maind.research[a]);
		total_rc += maind.research[a];
	}
	
	
	/*if (ticks.speed > 59){ //only record for slow rounds
	char COREDIR[PATH_MAX];
	ConfigArrayPtr settings2;
	FILE * check_rc;
	settings2 = GetSetting( "Directory" );
	snprintf( COREDIR, sizeof(COREDIR), "%s/users/user%d/rc_check%d.txt", settings2->string_value,user->id,ticks.round);
	check_rc = fopen( COREDIR, "a" );
	//User %d Week %lld, year %lld energy %lld cry %lld 
	fprintf(check_rc, "%d %lld, %lld %lld %lld ",user->id, (long long)ticks.number % 52, (long long)ticks.number / 52, (long long)maind.ressource[CMD_RESSOURCE_ENERGY], (long long)maind.ressource[CMD_RESSOURCE_CRYSTAL] );
	//center prod: %lld fund_Rc %lld pop %lld
	fprintf(check_rc, "%lld, %lld, %lld ", (long long)cmdTickProduction[CMD_BUILDING_RESEARCH], (long long)(maind.fundresearch),  (long long)(maind.ressource[CMD_RESSOURCE_POPULATION]  / ( 600.0 * 100.0 )) );
	//total_Rc
	fprintf(check_rc, "%lld\n", (long long)total_rc );
	fclose(check_rc);
	}*/
	
	maind.fundresearch = fmax( 0.0, ( 0.9 * maind.fundresearch ) );

	ticks.debug_pass = 6;

	// SK: because of the network backbone arti, we need to calculate Tech research first
	//    int addedFromTech = 0;

	// calculate total research for tech

          //research maximum
	fa = cmdRace[maind.raceid].researchmax[CMD_RESEARCH_TECH];



	// calculate total research
	for( a = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ ) {
		/*if( a == CMD_RESEARCH_TECH ) {
			continue;
		}*/
		//research maximum
		fa = cmdRace[maind.raceid].researchmax[a];
		// CODE_ARTI
		/*evo obelisk
		if( ( maind.artefacts & ARTEFACT_64_BIT )){//& ( ( a == CMD_RESEARCH_ENERGY ) || ( a == CMD_RESEARCH_MILITARY ) ) ) {
			fa += 20.0;
		}*/
		if( ( maind.artefacts & ARTEFACT_16_BIT ) && a == CMD_RESEARCH_CULTURE){//& ( ( a == CMD_RESEARCH_ENERGY ) || ( a == CMD_RESEARCH_MILITARY ) ) ) {
			fa += 50.0;
		}
		/*
		ARTEFACT_64_BIT
		
		if( ( maind.artefacts & ARTEFACT_4_BIT ) && a == CMD_RESEARCH_MILITARY){//& ( ( a == CMD_RESEARCH_ENERGY ) || ( a == CMD_RESEARCH_MILITARY ) ) ) {
			fa += 30.0;
		}*/
	        // put this arti last, you need the other ones calculated before this one.
	         /*   //ARTI CODE network backbone
	        if(maind.artefacts & ARTEFACT_*_BIT)
	        {
	           // exclude tech research from having this bonus (otherwise there is no cap)
	           if( a != CMD_RESEARCH_TECH)
	               {
	                fa += addedFromTech;
	               }
	        }*/

		b = fa * ( 1.0 - exp( maind.research[a] / ( -10.0 * maind.networth ) ) );

		if( b > maind.totalresearch[a] ) {
			maind.totalresearch[a]++;
		} else if( b < maind.totalresearch[a] ) {
			maind.totalresearch[a]--;
		}
	}

	ticks.debug_pass = 7;


	// calc infos
	fa = ( cmdTickProduction[CMD_BUILDING_SOLAR] / specopSolarCalc( user->id ) );
	if( cmdRace[maind.raceid].special & CMD_RACE_SPECIAL_SOLARP15 ) {
		fa *= 1.15;
	}

      //ARTI CODE Ether Palace
	/*if( maind.artefacts & ARTEFACT_32_BIT ) {
		fa *= 1.30;
	}*/

	fb = cmdTickProduction[CMD_BUILDING_FISSION];
	
	/*if( maind.artefacts & ARTEFACT_32_BIT ) {
		fb *= 1.20;
	}*/
	
	fa += fb;

	//ARTI CODE Ether Garden

	if(maind.artefacts & ARTEFACT_ETHER_BIT) {
		fa *= 1.10;
	}
	
	/*if(maind.artefacts & ARTEFACT_8_BIT) {
		fa *= 0.9;
	}*/

	//ARTI CODE Ether Palace
	/*if(maind.artefacts & ARTEFACT_8_BIT) {
		fa *= 1.25;
	}*/
	
	//Enlightenment
	fa *= specopEnlightemntCalc(user->id, CMD_ENLIGHT_ENERGY);

	fb = cmdRace[maind.raceid].resource[CMD_RESSOURCE_ENERGY] * ( 1.00 + 0.01 * maind.totalresearch[CMD_RESEARCH_ENERGY] );


	maind.infos[INFOS_ENERGY_PRODUCTION] = ( fa * fb );


	settings = GetSetting( "Stockpile" );
	fc = settings->num_value * maind.infos[INFOS_ENERGY_PRODUCTION];
	fa = CMD_ENERGY_DECAY;
	//if(maind.artefacts & ARTEFACT_8_BIT) {
	//	fa /= 2.0;
	//}
	maind.infos[INFOS_ENERGY_DECAY] = fa * fmax( 0.0, ( maind.ressource[CMD_RESSOURCE_ENERGY] - fc ) );


	// meh! building upkeep
	maind.infos[INFOS_BUILDING_UPKEEP] = 0;
	for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ ) {
		if( ( a == CMD_BUILDING_SOLAR ) || ( a == CMD_BUILDING_FISSION ) ) {
			maind.infos[INFOS_BUILDING_UPKEEP] += ( maind.totalbuilding[a] * cmdBuildingUpkeep[a] * fb );
   		} else {
			maind.infos[INFOS_BUILDING_UPKEEP] += ( maind.totalbuilding[a] * cmdBuildingUpkeep[a] );
		}

	}


	ticks.debug_pass = 8;


	if( ( num = dbUserFleetList( user->id, &fleetp ) ) < 0 )
		return 0;

	ticks.debug_pass = 11;
	
	maind.infos[INFOS_UNITS_UPKEEP] = 0;
	
	for( a = 0 ; a < num ; a++ ) {
		for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ ) {
			
			if (b == 5  && (cmdRace[maind.raceid].special & CMD_RACE_SPECIAL_WOOKIEE)) //wookiee reduced soldiers upkeep
				maind.infos[INFOS_UNITS_UPKEEP] += ( ( 0.5 * fleetp[a].unit[b] * cmdUnitUpkeep[b] ) * ( (a == 0) ? 1 : cmdUnitPenalty[0] ) );
			else
				maind.infos[INFOS_UNITS_UPKEEP] += ( ( fleetp[a].unit[b] * cmdUnitUpkeep[b] ) * ( (a == 0) ? 1 : cmdUnitPenalty[0] ) );
		
			maind.totalunit[b] += fleetp[a].unit[b];
		}
	}
	for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ ) {
		
		if (b == 5  && (cmdRace[maind.raceid].special & CMD_RACE_SPECIAL_WOOKIEE)) //wookiee reduced soldiers upkeep
			maind.infos[INFOS_UNITS_UPKEEP] += ( ( 0.5* StationedUnits[b] * cmdUnitUpkeep[b] ) * cmdUnitPenalty[1] );
		else
			maind.infos[INFOS_UNITS_UPKEEP] += ( ( StationedUnits[b] * cmdUnitUpkeep[b] ) * cmdUnitPenalty[1] );
		
		maind.totalunit[b] += StationedUnits[b];
	}

	ticks.debug_pass = 12 + 10000;

	free( fleetp );

	//ARTI CODE Romulan Military Outpost
	/*if(maind.artefacts & ARTEFACT_16_BIT) {
		maind.infos[INFOS_UNITS_UPKEEP] *= 1.15;
	}*/
	
	//ARTEFACT_2_BIT check for the timer!
	int artiUpkeepReduction = dbCheckArtifactTimer(maind.empire, 10);
		if (artiUpkeepReduction > 0){
			maind.infos[INFOS_UNITS_UPKEEP] *= pow(0.9,(double)artiUpkeepReduction);
		}
	
	
	maind.infos[INFOS_POPULATION_REDUCTION] = (1.0/35.0) * maind.ressource[CMD_RESSOURCE_POPULATION];
	//Population Reduction changed to include portals + units
	maind.infos[INFOS_PORTALS_UPKEEP] = fmax( 0.0, ( pow( (maind.totalbuilding[CMD_BLDG_NUMUSED]-1), 1.2736 ) * 10000.0)/ (1.0 + (double)maind.totalresearch[CMD_RESEARCH_PORTALS]/100.0));
	maind.infos[INFOS_POPULATION_REDUCTION] = fmin( maind.infos[INFOS_POPULATION_REDUCTION], ( maind.infos[INFOS_BUILDING_UPKEEP] + maind.infos[INFOS_UNITS_UPKEEP] + maind.infos[INFOS_PORTALS_UPKEEP] ) );

	/*
	 * Network Virus Effect.
	 */
	for( a = 0 ; a < opvirus ; a++ ) {
		//maind.infos[INFOS_POPULATION_REDUCTION] -= fmax( 0.0, ( maind.infos[INFOS_POPULATION_REDUCTION] * 0.05 ) );
		maind.infos[INFOS_BUILDING_UPKEEP] *=  1.15 ;
	}

	maind.infos[INFOS_CRYSTAL_PRODUCTION] = ( ( cmdRace[maind.raceid].resource[CMD_RESSOURCE_CRYSTAL] ) * cmdTickProduction[CMD_BUILDING_CRYSTAL] ) * specopEnlightemntCalc(user->id, CMD_ENLIGHT_CRY);
		
		//ARTI CODE Crystalline Entity | reduces crystal decay by 75%
	//	if(maind.artefacts & ARTEFACT_*_BIT)
	//	fa /= 4;
	fc = settings->num_value * maind.infos[INFOS_CRYSTAL_PRODUCTION];
	fa = CMD_CRYSTAL_DECAY;
	//if(maind.artefacts & ARTEFACT_8_BIT) {
	//	fa /= 2.0;
	//}
	maind.infos[INFOS_CRYSTAL_DECAY] = fa * fmax( 0.0, ( maind.ressource[CMD_RESSOURCE_CRYSTAL] - fc ) );


    //ARTI CODE Mana Gate
		//if(maind.artefacts & ARTEFACT_MANA_BIT)
		//	maind.infos[INFOS_PORTALS_UPKEEP] /= 2;

	maind.infos[INFOS_MINERAL_PRODUCTION] = ( cmdRace[maind.raceid].resource[CMD_RESSOURCE_MINERAL] * cmdTickProduction[CMD_BUILDING_MINING] ) * specopEnlightemntCalc(user->id, CMD_ENLIGHT_MINERAL);
	maind.infos[INFOS_ECTROLIUM_PRODUCTION] = ( cmdRace[maind.raceid].resource[CMD_RESSOURCE_ECTROLIUM] * cmdTickProduction[CMD_BUILDING_REFINEMENT] ) * specopEnlightemntCalc(user->id, CMD_ENLIGHT_ECTRO);
	
	if(maind.artefacts & ARTEFACT_64_BIT)
		maind.infos[INFOS_MINERAL_PRODUCTION] *= 1.15;

	if ( dbEmpireGetInfo( maind.empire, &empired ) < 0 ) {
		error( "Tick error: Retriving empire %d", maind.empire  );
		continue;
	}
	maind.infos[INFOS_ENERGY_TAX] = fmax( 0.0, ( maind.infos[INFOS_ENERGY_PRODUCTION] * empired.taxation ) );
	maind.infos[INFOS_MINERAL_TAX] = fmax( 0.0, ( maind.infos[INFOS_MINERAL_PRODUCTION] * empired.taxation ) );
	maind.infos[INFOS_CRYSTAL_TAX] = fmax( 0.0, ( maind.infos[INFOS_CRYSTAL_PRODUCTION] * empired.taxation ) );
	maind.infos[INFOS_ECTROLIUM_TAX] = fmax( 0.0, ( maind.infos[INFOS_ECTROLIUM_PRODUCTION] * empired.taxation ) );

	maind.infos[CMD_RESSOURCE_ENERGY] = maind.infos[INFOS_ENERGY_PRODUCTION] - maind.infos[INFOS_ENERGY_DECAY] - maind.infos[INFOS_BUILDING_UPKEEP] - maind.infos[INFOS_UNITS_UPKEEP] + maind.infos[INFOS_POPULATION_REDUCTION] - maind.infos[INFOS_PORTALS_UPKEEP] - maind.infos[INFOS_ENERGY_TAX];

	maind.infos[CMD_RESSOURCE_MINERAL] = (maind.infos[INFOS_MINERAL_PRODUCTION] - maind.infos[INFOS_MINERAL_TAX]);

	maind.infos[CMD_RESSOURCE_CRYSTAL] = ( (maind.infos[INFOS_CRYSTAL_PRODUCTION] - maind.infos[INFOS_CRYSTAL_TAX] ) - maind.infos[INFOS_CRYSTAL_DECAY]);

	maind.infos[CMD_RESSOURCE_ECTROLIUM] = (maind.infos[INFOS_ECTROLIUM_PRODUCTION] - maind.infos[INFOS_ECTROLIUM_TAX]);


	empired.fund[CMD_RESSOURCE_ENERGY] = fmax( 0.0, ( empired.fund[CMD_RESSOURCE_ENERGY] + maind.infos[INFOS_ENERGY_TAX] ) );
	empired.fund[CMD_RESSOURCE_MINERAL] = fmax( 0.0, ( empired.fund[CMD_RESSOURCE_MINERAL] + maind.infos[INFOS_MINERAL_TAX] ) );
	empired.fund[CMD_RESSOURCE_CRYSTAL] = fmax( 0.0, ( empired.fund[CMD_RESSOURCE_CRYSTAL] + maind.infos[INFOS_CRYSTAL_TAX] ) );
	empired.fund[CMD_RESSOURCE_ECTROLIUM] = fmax( 0.0, ( empired.fund[CMD_RESSOURCE_ECTROLIUM] + maind.infos[INFOS_ECTROLIUM_TAX] ) );

	if( dbEmpireSetInfo( maind.empire, &empired ) < 0 ) {
		error( "Tick error: Setting Empire #%d Fund!", maind.empire );
	}


	//ARTI CODE Mineral enhancement
	/*if(maind.artefacts & ARTEFACT_64_BIT) {
		maind.infos[CMD_RESSOURCE_ECTROLIUM] *= 1.20;
	}*/
	
	/*if(maind.artefacts & ARTEFACT_8_BIT) {
		maind.infos[CMD_RESSOURCE_CRYSTAL] *= 1.20;
	}*/
	
/*	//ARTI CODE Ectrolim enhancement
		if(maind.artefacts & ARTEFACT_xxxx_BIT)
	{
		maind.infos[CMD_RESSOURCE_ECTROLIUM] *= 1.50;
	}	*/
	ticks.debug_pass = 9;


	// fleets decay?
	a = 65536*2;
	b = -1;


	if( ( maind.ressource[CMD_RESSOURCE_ENERGY] + maind.infos[CMD_RESSOURCE_ENERGY] ) < 0 ) {
		a = -65536*3;
		b = CMD_UNIT_GHOST;
	}

	// readiness
	for( c = 0 ; c < CMD_READY_NUMUSED ; c++ ) {
		if( a < 0 ) {
			if( maind.readiness[c] > ( -65536*200 - a ) ) {
				maind.readiness[c] += a;
			} else if( maind.readiness[c] > -65536*200 ) {
				maind.readiness[c] = -65536*200;
			}
		} else {
			maind.readiness[c] += a;
		}

// CODE_ARTEFACT
	int check_readiness;
	if (c == 0)
		check_readiness = maind.benefactor[1];
	else
		check_readiness = 0;
	
	
    if( ( maind.artefacts & ARTEFACT_4_BIT ) )
    {
      if( maind.readiness[c] > 65536*(115+check_readiness) ) //alter this to increase or decrease the readiness, currently increases to 115%
        maind.readiness[c] = 65536*(115+check_readiness);	//alter this to increase or decrease the readiness, currently increases to 115%
	}
    else
    {
      if( maind.readiness[c] > 65536*(100+check_readiness) )
        maind.readiness[c] = 65536*(100+check_readiness);
   }
   
 //CODE_ARTEFACT

		/*if( maind.readiness[c] > 65536*100 ) {
			maind.readiness[c] = 65536*100;
		}*/
		if( maind.readiness[c] < -65536*500 ) {
			maind.readiness[c] = -65536*500;
		}

	}

	ticks.debug_pass = 10;


	// calculate phantoms decay rate
	phdecay = 0.20;
	if( maind.totalunit[CMD_UNIT_WIZARD] ) {
		fa = (double)maind.totalunit[CMD_UNIT_PHANTOM] / (double)maind.totalunit[CMD_UNIT_WIZARD];
		if( fa < 0.05 ) {
			phdecay = 0.01;
		} else {
		        fa = (double)pow( (double)( (1.0/0.05) * fa ), 2.4 );
		        phdecay = 0.01*fa;
			if( phdecay > 0.20 ) {
				phdecay = 0.20;
			}
		}
	}

	// fleets
	dbUserMainSet( user->id, &maind );
	fleetp = 0;
	num = dbUserFleetList( user->id, &fleetp );
	if( fleetp ) {
		for( a = num-1 ; a >= 0 ; a-- ) {
			e = 0;
			if( a == 0 ) {
				if( !( dbUserFleetRetrieve( user->id, 0, &fleetp[0] ) ) )
					return 1;
			}
			for( c = b ; c >= 0 ; c-- ) {
				d = (int)ceil( (float)fleetp[a].unit[c] * 0.02 );
				fleetp[a].unit[c] -= d;
				maind.totalunit[c] -= d;
				e |= d;
			}

			if( fleetp[a].unit[CMD_UNIT_PHANTOM] ) {
				d = (int)ceil( (float)fleetp[a].unit[CMD_UNIT_PHANTOM] * phdecay );
				fleetp[a].unit[CMD_UNIT_PHANTOM] -= d;
        			maind.totalunit[CMD_UNIT_PHANTOM] -= d;
        			e |= d;
			}

			if( ( a ) && !( fleetp[a].unit[CMD_UNIT_EXPLORATION] || fleetp[a].unit[CMD_UNIT_CRUISER] || fleetp[a].unit[CMD_UNIT_CARRIER] || fleetp[a].unit[CMD_UNIT_PHANTOM] || fleetp[a].unit[CMD_UNIT_AGENT] || fleetp[a].unit[CMD_UNIT_GHOST] ) ) {
				dbUserFleetRemove( user->id, a );
				continue;
			}
			fleetd.flags &= 0xFFFFFFFF - CMD_FLEET_FLAGS_MOVED;
			if( fleetp[a].time > 0 ) {
				fleetd.flags |= CMD_FLEET_FLAGS_MOVED;
				fleetp[a].time--;
				dbUserFleetSet( user->id, a, &fleetp[a] );
			} else if( e ) {
				dbUserFleetSet( user->id, a, &fleetp[a] );
			}
			if( ( a != 0 ) && ( fleetp[a].time == 0 ) ) {
				cmdFleetAction( &fleetp[a], user->id, a, 1 );
			}
		}
		free( fleetp );
	}
	dbUserMainRetrieve( user->id, &maind );


	ticks.debug_pass = 11;
	
	if (cmdRace[maind.raceid].special & CMD_RACE_SPECIAL_WOOKIEE){
		maind.infos[INFOS_ENERGY_INTEREST] = fmin (0.005 * maind.ressource[CMD_RESSOURCE_ENERGY], maind.infos[INFOS_ENERGY_PRODUCTION]);
		maind.infos[INFOS_MINERAL_INTEREST] = fmin (0.005 * maind.ressource[CMD_RESSOURCE_MINERAL], maind.infos[INFOS_MINERAL_PRODUCTION]);
		maind.infos[INFOS_CRYSTAL_INTEREST] = fmin (0.005 * maind.ressource[CMD_RESSOURCE_CRYSTAL], maind.infos[INFOS_CRYSTAL_PRODUCTION]);
		maind.infos[INFOS_ECTROLIUM_INTEREST] = fmin (0.005 * maind.ressource[CMD_RESSOURCE_ECTROLIUM], maind.infos[INFOS_ECTROLIUM_PRODUCTION]);
	}
	else{
		maind.infos[INFOS_ENERGY_INTEREST] = 0;
		maind.infos[INFOS_MINERAL_INTEREST] = 0;
		maind.infos[INFOS_CRYSTAL_INTEREST] = 0;
		maind.infos[INFOS_ECTROLIUM_INTEREST] = 0;
	}

	if (cmdRace[maind.raceid].special & CMD_RACE_SPECIAL_WOOKIEE){
		maind.infos[INFOS_ENERGY_INCOME] += maind.infos[INFOS_ENERGY_INTEREST];	
		maind.infos[INFOS_MINERAL_INCOME] += maind.infos[INFOS_MINERAL_INTEREST];
		maind.infos[INFOS_CRYSTAL_INCOME] += maind.infos[INFOS_CRYSTAL_INTEREST];
		maind.infos[INFOS_ECTROLIUM_INCOME] += maind.infos[INFOS_ECTROLIUM_INTEREST];	
	}
		
	// income
	for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ ) {
		maind.ressource[a] = fmax( 0.0, ( maind.ressource[a] + maind.infos[a] ) );
	}
	

	// units decay on planets + networth calcualtions
	plist = 0;
	num = dbUserPlanetListIndices( user->id, &plist );
	fa = 0;
	fb = 3;
	maind.networth = 0;
	if( plist ) {
		for( a = 0 ; a < num ; a++ ) {
			if( dbMapRetrievePlanet( plist[a], &planetd ) > 0 ) {
				e = 0;
				if( planetd.unit[CMD_UNIT_PHANTOM] ) {
					planetd.unit[CMD_UNIT_PHANTOM] -= (int)ceil( phdecay * (float)(planetd.unit[CMD_UNIT_PHANTOM]) );
					e |= 1;
				}
				for( c = b ; c >= 0 ; c-- ) {
					d = (int)ceil( (float)planetd.unit[c] * 0.02 );
					planetd.unit[c] -= d;
					maind.totalunit[c] -= d;
					e |= d;
				}
				if( e ) {
					dbMapSetPlanet( plist[a], &planetd );
				}
				//Networth Segment
				if( planetd.special[1] ) {
					switch ( planetd.special[0] ) {
						case CMD_BONUS_ENERGY:
							maind.networth += ( planetd.special[1] * 1.25 );
						break;
					
						case CMD_BONUS_MINERAL:
							maind.networth += ( planetd.special[1] * 1.45 );
						break;
					
						case CMD_BONUS_CRYSTAL:
							maind.networth += ( planetd.special[1] * 2.25 );
						break;
					
						case CMD_BONUS_ECTROLIUM:
							maind.networth += ( planetd.special[1] * 1.65 );
						break;
					
						case CMD_BONUS_FISSION:
							maind.networth += ( planetd.special[1] * 5 );
						break;

						default:
						break;
					}
				}
				if( planetd.flags & CMD_PLANET_FLAGS_MEGA ) {
					fa += planetd.size;
					fb++;
				} else {
					maind.networth += ( planetd.size * 1.75 );
				}
			}
		}
		if( fa ) {
			maind.networth += ( fa * fb );
		}
		free( plist );
	}

	ticks.debug_pass = 12;


	for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
		maind.networth += maind.totalunit[a] * cmdUnitStats[a][CMD_UNIT_STATS_NETWORTH];
	}
	for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ ) {
		/*if( maind.artefacts & ARTEFACT_16_BIT && a == CMD_BUILDING_FISSION)
			continue;*/
		//else
		maind.networth += maind.totalbuilding[a] * cmdBuildingNetworth[a];
	}

	maind.networth += (0.005 * maind.ressource[CMD_RESSOURCE_POPULATION]);

	for( a = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ )
		maind.networth += (0.001 * maind.research[a]);
	
	/*if(maind.artefacts & ARTEFACT_4_BIT)
		maind.networth *= 0.8;*/

	// spec ops
	if( specopd ) {
		for( a = specopnum-1 ; a >= 0 ; a-- ) {
			specopd[a].time--;
			if( specopd[a].time <= 0 ) {
				if(specopd[a].type == CMD_OPER_PLANETBEACON) {
					dbMapRetrievePlanet(specopd[a].plnid, &planetd);
					planetd.flags = planetd.flags & ~CMD_PLANET_FLAGS_BEACON;
					dbMapSetPlanet(specopd[a].plnid, &planetd);
				}
	
				dbUserSpecOpRemove( user->id, a );
				continue;
			}
			dbUserSpecOpSet( user->id, a, &specopd[a] );
		}
		free( specopd );
	}


	ticks.debug_pass = 13;

	dbUserMainSet( user->id, &maind );
}


ticks.debug_pass = 14;


// bids energy decay
marketbid[DB_MARKETBID_ACTION] = 0;
for( marketbid[DB_MARKETBID_RESSOURCE] = 0 ; marketbid[DB_MARKETBID_RESSOURCE] < 3 ; marketbid[DB_MARKETBID_RESSOURCE]++ ) {
	for( marketbid[DB_MARKETBID_PRICE] = 0 ; marketbid[DB_MARKETBID_PRICE] < DB_MARKET_RANGE ; marketbid[DB_MARKETBID_PRICE]++ ) {
		b = dbMarketListStart( marketbid );
		while( (unsigned int)b < 0x1000000 ) {
			c = dbMarketListNext( b, bidresult );
			a = (int)ceil( CMD_ENERGY_DECAY * (float)bidresult[0] );
			if( (bidresult[0]-a) > 0 ) {
				dbUserMarketQuantity( bidresult[1], b, bidresult[0] - a );
				dbMarketSetQuantity( marketbid, b, bidresult[0] - a, a );
			} else {
				dbUserMarketRemove( bidresult[1], b );
				dbMarketRemove( marketbid, b );
			}
			b = c;
		}
	}
}


ticks.debug_pass = 15;


// bids crystal decay
marketbid[DB_MARKETBID_ACTION] = 1;
marketbid[DB_MARKETBID_RESSOURCE] = CMD_RESSOURCE_CRYSTAL-1;
for( marketbid[DB_MARKETBID_PRICE] = 0 ; marketbid[DB_MARKETBID_PRICE] < DB_MARKET_RANGE ; marketbid[DB_MARKETBID_PRICE]++ ) {
	b = dbMarketListStart( marketbid );
	while( (unsigned int)b < 0x1000000 ) {
		c = dbMarketListNext( b, bidresult );
		a = (int)ceil( CMD_CRYSTAL_DECAY * (float)bidresult[0] );
		if( (bidresult[0]-a) > 0 ) {
			dbUserMarketQuantity( bidresult[1], b, bidresult[0] - a );
			dbMarketSetQuantity( marketbid, b, bidresult[0] - a, a );
		} else {
			dbUserMarketRemove( bidresult[1], b );
			dbMarketRemove( marketbid, b );
		}
		b = c;
	}
}


ticks.debug_pass = 16;


return YES;
}


