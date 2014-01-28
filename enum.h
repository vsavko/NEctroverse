#ifndef ENUMINCLUDES
#define ENUMINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

//Moved from cmd.h
enum
{
CMD_SEARCH_EMPIRE,
CMD_CHANGE_KILLUSER,
CMD_SET_USERMAIN,

CMD_RETRIEVE_USERMAIN,
CMD_RETRIEVE_USERPLIST,
CMD_RETRIEVE_USERPLISTINDICES,
CMD_RETRIEVE_USERPLISTSYSTEMS,
CMD_RETRIEVE_USERPORTALS,
CMD_RETRIEVE_USERPORTALSINDICES,
CMD_RETRIEVE_USERBUILDLIST,

CMD_ADD_BUILD,
CMD_RAZE_BUILDINGS,
CMD_RAZE_PORTAL,
CMD_DISBAND_UNITS,
CMD_RETRIEVE_FLEET,

CMD_SET_RESEARCH,
CMD_EXPLORE_INFO,
CMD_EXPLORE,
CMD_FIND_SYSTEM,
CMD_FIND_PLANET,

CMD_UNSTATION_PLANET,
CMD_ATTACK_PLANET,

};


enum
{
CMD_RESSOURCE_ENERGY,
CMD_RESSOURCE_MINERAL,
CMD_RESSOURCE_CRYSTAL,
CMD_RESSOURCE_ECTROLIUM,
CMD_RESSOURCE_NUMUSED,
};

enum
{
CMD_BONUS_ENERGY,
CMD_BONUS_MINERAL,
CMD_BONUS_CRYSTAL,
CMD_BONUS_ECTROLIUM,
CMD_BONUS_FISSION,
/*CMD_BONUS_RESEARCH,
CMD_BONUS_DISEASE,
CMD_BONUS_POPULATION,*/
CMD_BONUS_NUMUSED,
};

#define CMD_RESSOURCE_TIME (CMD_RESSOURCE_NUMUSED)
#define CMD_RESSOURCE_POPULATION (CMD_RESSOURCE_NUMUSED+1)



enum
{
CMD_BUILDING_SOLAR,
CMD_BUILDING_FISSION,
CMD_BUILDING_MINING,
CMD_BUILDING_CRYSTAL,
CMD_BUILDING_REFINEMENT,
CMD_BUILDING_CITIES,
CMD_BUILDING_RESEARCH,
CMD_BUILDING_SATS,
CMD_BUILDING_SHIELD,
CMD_BLDG_NUMUSED,
};

#define CMD_BLDG_OVERBUILD 7


#define CMD_POPULATION_SIZE_FACTOR 20
#define CMD_POPULATION_BASE_FACTOR 2



enum
{
CMD_UNIT_BOMBER,
CMD_UNIT_FIGHTER,
CMD_UNIT_TRANSPORT,
CMD_UNIT_CRUISER,
CMD_UNIT_CARRIER,
CMD_UNIT_SOLDIER,
CMD_UNIT_DROID,
CMD_UNIT_GOLIATH,
CMD_UNIT_PHANTOM,

CMD_UNIT_WIZARD,
CMD_UNIT_AGENT,
CMD_UNIT_GHOST,
CMD_UNIT_EXPLORATION,
CMD_UNIT_NUMUSED,
};


#define CMD_UNIT_FLEET 9



enum
{
CMD_RESEARCH_MILITARY,
CMD_RESEARCH_CONSTRUCTION,
CMD_RESEARCH_TECH,
CMD_RESEARCH_ENERGY,
CMD_RESEARCH_POPULATION,
CMD_RESEARCH_CULTURE,
CMD_RESEARCH_OPERATIONS,
CMD_RESEARCH_NUMUSED,
};




enum
{
CMD_UNIT_STATS_AIRATTACK,
CMD_UNIT_STATS_AIRDEFENSE,
CMD_UNIT_STATS_GROUNDATTACK,
CMD_UNIT_STATS_GROUNDDEFENSE,
CMD_UNIT_STATS_SPEED,
CMD_UNIT_STATS_NETWORTH,
CMD_UNIT_STATS_NUMUSED
};

#define CMD_UNIT_STATS_BATTLE 4




#define CMD_SATS_ATTACK 110
#define CMD_SATS_DEFENCE 450
#define CMD_SHIELD_ABSORB 1300




enum
{
CMD_FLEET_ORDER_EXPLORE,
CMD_FLEET_ORDER_STATION,
CMD_FLEET_ORDER_MOVE,
CMD_FLEET_ORDER_ATTACK,
CMD_FLEET_ORDER_RECALL,
CMD_FLEET_ORDER_CANCELED,
CMD_FLEET_ORDER_MERGE,

CMD_FLEET_ORDER_SPYTARGET,
CMD_FLEET_ORDER_OBSERVEPLANET,
CMD_FLEET_ORDER_NETWORKVIRUS,
CMD_FLEET_ORDER_INFILTRATION,
CMD_FLEET_ORDER_BIOINFECTION,
CMD_FLEET_ORDER_ENERGYTRANSFER,
CMD_FLEET_ORDER_MILITARYSAB,
CMD_FLEET_ORDER_NUKEPLANET,
CMD_FLEET_ORDER_HIGHINFIL,
CMD_FLEET_ORDER_PLANETBEACON,

CMD_FLEET_ORDER_SENSE,
CMD_FLEET_ORDER_SURVEY,
CMD_FLEET_ORDER_SHIELDING,
CMD_FLEET_ORDER_FORCEFIELD,
CMD_FLEET_ORDER_VORTEX,
CMD_FLEET_ORDER_MINDCONTROL,
CMD_FLEET_ORDER_ENERGYSURGE,

CMD_FLEET_ORDER_NUMUSED
};

#define CMD_FLEET_ORDER_FIRSTOP (7)
#define CMD_FLEET_ORDER_LASTOP (7+10)

#define CMD_FLEET_ORDER_FIRSTINCANT (7+10)
#define CMD_FLEET_ORDER_LASTINCANT (7+10+7)


#define CMD_PLANET_FLAGS_PORTAL 1
#define CMD_PLANET_FLAGS_PORTAL_BUILD 2
#define CMD_PLANET_FLAGS_HOME 4
#define CMD_PLANET_FLAGS_BEACON 8
#define CMD_PLANET_FLAGS_MEGA 16


enum
{
CMD_USER_FLAGS_ACTIVATED,
CMD_USER_FLAGS_KILLED,
CMD_USER_FLAGS_DELETED,
CMD_USER_FLAGS_NEWROUND,

CMD_USER_FLAGS_LEADER,
CMD_USER_FLAGS_COMMINISTER,
CMD_USER_FLAGS_DEVMINISTER,
CMD_USER_FLAGS_WARMINISTER,
CMD_USER_FLAGS_INDEPENDENT,

CMD_USER_FLAGS_RESERVED,
CMD_USER_FLAGS_FROZEN,
CMD_USER_FLAGS_EMAIL,

//These should always go last... as they can be flaged on and off with build.
//Turning them on and re-building can result in false-positives if these are moved.
#if FACEBOOK_SUPPORT
CMD_USER_FLAGS_FBLINK,
CMD_USER_FLAGS_FBMADE,
#endif

CMD_USER_FLAGS_NUMUSED
};

enum
{
CMD_POLITICS_LEADER,
CMD_POLITICS_COMMINISTER,
CMD_POLITICS_DEVMINISTER,
CMD_POLITICS_WARMINISTER,
CMD_POLITICS_NUMUSED
};

enum
{
CMD_BLDG_EMPIRE_RESEARCH,
CMD_BLDG_EMPIRE_R1,
CMD_BLDG_EMPIRE_R2,
CMD_BLDG_EMPIRE_R3,
CMD_BLDG_EMPIRE_R4,
CMD_BLDG_EMPIRE_R5,
CMD_BLDG_EMPIRE_R6,
CMD_BLDG_EMPIRE_R7,
CMD_BLDG_EMPIRE_NUMUSED
};

#define CMD_EMPIRE_POLITICS_START CMD_USER_FLAGS_LEADER
#define CMD_EMPIRE_POLITICS_END CMD_USER_FLAGS_INDEPENDENT
#define CMD_EMPIRE_POLITICS_TOTAL (CMD_EMPIRE_POLITICS_END - CMD_EMPIRE_POLITICS_START)


#define CMD_NEWS_FLAGS_MAIL 0x1
#define CMD_NEWS_FLAGS_BUILD 0x2
#define CMD_NEWS_FLAGS_AID 0x4
#define CMD_NEWS_FLAGS_FLEET 0x8
#define CMD_NEWS_FLAGS_ATTACK 0x10



enum
{
CMD_NEWS_BUILDING,
CMD_NEWS_UNIT,
CMD_NEWS_EXPLORE,
CMD_NEWS_EXPLORE_FAILED,
CMD_NEWS_STATION,
CMD_NEWS_STATION_FAILED,
CMD_NEWS_RECALL,
CMD_NEWS_ATTACK,
CMD_NEWS_ATTACK_FAILED,
CMD_NEWS_MARKET_SOLD,
CMD_NEWS_MARKET_BOUGHT,
CMD_NEWS_AID,
CMD_NEWS_GETAID,
CMD_NEWS_FAMATTACK,
CMD_NEWS_FAMATTACK_FAILED,
CMD_NEWS_MAIL,


CMD_NEWS_OPSPYTARGET,
CMD_NEWS_OPOBSERVEPLANET,
CMD_NEWS_OPNETWORKVIRUS,
CMD_NEWS_OPINFILTRATION,
CMD_NEWS_OPBIOINFECTION,
CMD_NEWS_OPENERGYTRANSFER,
CMD_NEWS_OPMILITARYSAB,
CMD_NEWS_OPNUKEPLANET,
CMD_NEWS_OPHIGHINFIL,
CMD_NEWS_OPPLANETBEACON,
CMD_NEWS_OPRESERVED1,
CMD_NEWS_OPRESERVED2,
CMD_NEWS_OPRESERVED3,

CMD_NEWS_OPSPYTARGET_TARGET,
CMD_NEWS_OPOBSERVEPLANET_TARGET,
CMD_NEWS_OPNETWORKVIRUS_TARGET,
CMD_NEWS_OPINFILTRATION_TARGET,
CMD_NEWS_OPBIOINFECTION_TARGET,
CMD_NEWS_OPENERGYTRANSFER_TARGET,
CMD_NEWS_OPMILITARYSAB_TARGET,
CMD_NEWS_OPNUKEPLANET_TARGET,
CMD_NEWS_OPHIGHINFIL_TARGET,
CMD_NEWS_OPPLANETBEACON_TARGET,
CMD_NEWS_OPRESERVED1T,
CMD_NEWS_OPRESERVED2T,
CMD_NEWS_OPRESERVED3T,

CMD_NEWS_OPCANCEL,


CMD_NEWS_SPIRRAECTRO,
CMD_NEWS_SPDARKWEB,
CMD_NEWS_SPINCANDESCENCE,
CMD_NEWS_SPBLACKMIST,
CMD_NEWS_SPWARILLUSIONS,
CMD_NEWS_SPASSAULT,
CMD_NEWS_SPPHANTOMS,
CMD_NEWS_SPRESERVED0,
CMD_NEWS_SPRESERVED1,
CMD_NEWS_SPRESERVED2,

CMD_NEWS_SPIRRAECTRO_TARGET,
CMD_NEWS_SPDARKWEB_TARGET,
CMD_NEWS_SPINCANDESCENCE_TARGET,
CMD_NEWS_SPBLACKMIST_TARGET,
CMD_NEWS_SPWARILLUSIONS_TARGET,
CMD_NEWS_SPASSAULT_TARGET,
CMD_NEWS_SPPHANTOMS_TARGET,
CMD_NEWS_SPRESERVED0T,
CMD_NEWS_SPRESERVED1T,
CMD_NEWS_SPRESERVED2T,

CMD_NEWS_SPCANCEL,


CMD_NEWS_INSENSE,
CMD_NEWS_INSURVEY,
CMD_NEWS_INSHIELDING,
CMD_NEWS_INFORCEFIELD,
CMD_NEWS_INVORTEX,
CMD_NEWS_INMINDCONTROL,
CMD_NEWS_INENERGYSURGE,
CMD_NEWS_INRESERVED0,
CMD_NEWS_INRESERVED1,
CMD_NEWS_INRESERVED2,

CMD_NEWS_INSENSE_TARGET,
CMD_NEWS_INSURVEY_TARGET,
CMD_NEWS_INSHIELDING_TARGET,
CMD_NEWS_INFORCEFIELD_TARGET,
CMD_NEWS_INVORTEX_TARGET,
CMD_NEWS_INMINDCONTROL_TARGET,
CMD_NEWS_INENERGYSURGE_TARGET,
CMD_NEWS_INRESERVED0_TARGET,
CMD_NEWS_INRESERVED1_TARGET,
CMD_NEWS_INRESERVED2_TARGET,

CMD_NEWS_INCANCEL,


CMD_NEWS_PLANET_OFFER,
CMD_NEWS_PLANET_TAKEN,
CMD_NEWS_PLANET_GIVEN,
CMD_NEWS_FLEETS_MERGE,

CMD_NEWS_DEATH,


CMD_NEWS_NUMUSED
};


#define CMD_NEWS_FLAGS_NEW 1

#define CMD_NEWS_EXPIRE_TIME (52*4)

#define CMD_NEWS_NUMOPBEGIN (CMD_NEWS_OPSPYTARGET)
#define CMD_NEWS_NUMOPEND (CMD_NEWS_OPPLANETBEACON)
#define CMD_NEWS_NUMOPTARGETBEGIN (CMD_NEWS_OPSPYTARGET_TARGET)
#define CMD_NEWS_NUMOPTARGETEND (CMD_NEWS_OPPLANETBEACON_TARGET)

#define CMD_NEWS_NUMSPBEGIN (CMD_NEWS_SPIRRAECTRO)
#define CMD_NEWS_NUMSPEND (CMD_NEWS_SPPHANTOMS)
#define CMD_NEWS_NUMSPTARGETBEGIN (CMD_NEWS_SPIRRAECTRO_TARGET)
#define CMD_NEWS_NUMSPTARGETEND (CMD_NEWS_SPPHANTOMS_TARGET)

#define CMD_NEWS_NUMINBEGIN (CMD_NEWS_INSENSE)
#define CMD_NEWS_NUMINEND (CMD_NEWS_INENERGYSURGE)
#define CMD_NEWS_NUMINTARGETBEGIN (CMD_NEWS_INSENSE_TARGET)
#define CMD_NEWS_NUMINTARGETEND (CMD_NEWS_INENERGYSURGE_TARGET)




#define CMD_FLEETS_TRAVEL_SPEED (2)



#define CMD_FLEET_FLAGS_MOVED 1
#define CMD_FLEET_FLAGS_RMERGE 2



enum
{
LEVEL_USER,
LEVEL_FORUMMOD,
LEVEL_MODERATOR,
LEVEL_ADMINISTRATOR,
LEVEL_DIETY
};


#define CMD_ENERGY_DECAY (0.005)
#define CMD_CRYSTAL_DECAY (0.02)





#define CMD_RELATION_ALLY (0)
#define CMD_RELATION_NAP (1)
#define CMD_RELATION_WAR (2)
#define CMD_RELATION_ALLY_OFFER (0x10)
#define CMD_RELATION_NAP_OFFER (0x20)


enum
{
CMD_READY_FLEET,
CMD_READY_PSYCH,
CMD_READY_AGENT,

CMD_READY_NUMUSED
};

enum
{
DB_TOTALS_USERS_REGISTERED,
DB_TOTALS_USERS_ACTIVATED,
DB_TOTALS_USERS_ONLINE,
DB_TOTALS_USERS_NUMUSED,
};

enum
{
MAIL_IN,
MAIL_OUT,
};

enum
{
CMD_OPER_SPYTARGET,
CMD_OPER_OBSERVEPLANET,
CMD_OPER_NETWORKVIRUS,
CMD_OPER_INFILTRATION,
CMD_OPER_BIOINFECTION,
CMD_OPER_ENERGYTRANSFER,
CMD_OPER_MILITARYSAB,
CMD_OPER_NUKEPLANET,
CMD_OPER_HIGHINFIL,
CMD_OPER_PLANETBEACON,

CMD_AGENTOP_NUMUSED
};

enum
{
CMD_SPELL_IRRAECTRO,
CMD_SPELL_DARKWEB,
CMD_SPELL_INCANDESCENCE,
CMD_SPELL_BLACKMIST,
CMD_SPELL_WARILLUSIONS,
CMD_SPELL_ASSAULT,
CMD_SPELL_PHANTOMS,

CMD_PSYCHICOP_NUMUSED
};

enum
{
CMD_INCANT_SENSE,
CMD_INCANT_SURVEY,
CMD_INCANT_SHIELDING,
CMD_INCANT_FORCEFIELD,
CMD_INCANT_VORTEX,
CMD_INCANT_MINDCONTROL,
CMD_INCANT_ENERGYSURGE,

CMD_GHOSTOP_NUMUSED
};



enum
{
CMD_RACE_HARKS,
CMD_RACE_MANTI,
CMD_RACE_FOOHONS,
CMD_RACE_SPACE,
CMD_RACE_DREAM,
/*CMD_RACE_FURTIFON,
CMD_RACE_SAMSONITES,
CMD_RACE_ULTI,*/
CMD_RACE_TOTAL
};


#define CMD_RACE_NUMUSED CMD_RACE_TOTAL



#define CMD_RACE_SPECIAL_POPRESEARCH 1
#define CMD_RACE_SPECIAL_SOLARP15 2
#define CMD_RACE_SPECIAL_FRP25 4
#define CMD_RACE_SPECIAL_IDW 8
#define CMD_RACE_SPECIAL_CULPROTECT 16


//Frount page ENUMS
enum {
FMENU_NONE,
FMENU_MAIN,
FMENU_REGISTER,
FMENU_FORUM,
FMENU_FAQ,
FMENU_GSTART,
FMENU_NOTICES,
FMENU_RANKS,
FMENU_SERVER,
#if FACEBOOK_SUPPORT
FMENU_FACEBOOK,
#endif
FMENU_TOTAL,
};

//moved from db.h

enum
{
MAP_SIZEX,
MAP_SIZEY,
MAP_SYSTEMS,
MAP_PLANETS,
MAP_MEGASYS,
MAP_EMPIRES,
MAP_EMEMBERS,
MAP_CAPACITY,
MAP_ARTITIMER,
MAP_TIMEMPIRE,
MAP_TOTAL_INFO,
};

enum
{
MAP_SYSTEM_FLAG_MEGA,
MAP_SYSTEM_FLAG_PLAIN,
MAP_SYSTEM_FLAG_EMPIRE,
MAP_SYSTEM_FLAGS_NUMUSED,
};

#define DB_USER_NEWS_SIZE 384
#define DB_USER_NEWS_BASE 40

enum
{
DB_MARKETBID_ACTION,
DB_MARKETBID_RESSOURCE,
DB_MARKETBID_PRICE,
DB_MARKETBID_QUANTITY,
DB_MARKETBID_USERID,

DB_MARKETBID_NUMUSED
};

#define DB_MARKETBID_BIDID 4
#define DB_MARKET_RANGE 251
#define DB_MARKET_BIDSOFF ( 8 + 3*2*DB_MARKET_RANGE*12 )


#define DB_FORUM_NAME_SIZE 64


#define DB_FORUM_FLAGS_POSTERROR 1
#define DB_FORUM_FLAGS_FORUMUNUSED 1
#define DB_FORUM_FLAGS_FORUMFAMILY 2
#define DB_FORUM_FLAGS_THREADFREE 1
#define DB_FORUM_FLAGS_THREAD_LOCK 0xEFEF

//Artifact ENUMS
enum
{
ARTEFACT_CITI,
ARTEFACT_GEM,
ARTEFACT_CRAZ,
ARTEFACT_CRUI,

ARTEFACT_FAST,
ARTEFACT_LARG,
ARTEFACT_RICH,
ARTEFACT_PORT,

ARTEFACT_ETHER,

ARTEFACT_NUMUSED
};


#define ARTEFACT_1_BIT 1
#define ARTEFACT_2_BIT 2
#define ARTEFACT_4_BIT 4
#define ARTEFACT_8_BIT 8

#define ARTEFACT_16_BIT 16
#define ARTEFACT_32_BIT 32
#define ARTEFACT_64_BIT 64
#define ARTEFACT_128_BIT 128

#define ARTEFACT_ETHER_BIT 256

//#define ARTEFACT_TERO_BIT 512  //When 10 arti

//File type
enum {
FILE_IMAGEDIR,
FILE_FUNCTION,
FILE_HTML,
FILE_CSS,
FILE_JAVA,
};

enum {
HTTP_IBASE,
HTTP_AVATAR,
HTTP_OSTYPE,
HTTP_FILETYPE,
HTTP_SMILIES,

HTTP_DIR_TOTAL,
};

enum {
INFOS_ENERGY_INCOME,
INFOS_MINERAL_INCOME,
INFOS_CRYSTAL_INCOME,
INFOS_ECTROLIUM_INCOME,

INFOS_ENERGY_PRODUCTION,
INFOS_ENERGY_DECAY,

INFOS_BUILDING_UPKEEP,
INFOS_POPULATION_REDUCTION,
INFOS_PORTALS_UPKEEP,
INFOS_UNITS_UPKEEP,


INFOS_MINERAL_PRODUCTION,

INFOS_CRYSTAL_PRODUCTION,
INFOS_CRYSTAL_DECAY,

INFOS_ECTROLIUM_PRODUCTION,

INFOS_ENERGY_TAX,
INFOS_MINERAL_TAX,
INFOS_CRYSTAL_TAX,
INFOS_ECTROLIUM_TAX,
INFOS_TOTAL_NUMUSED,
};

enum
{
PORT_HTTP,
#if HTTPS_SUPPORT
PORT_HTTPS,
#endif
PORT_TOTAL
};

enum
{
MODE_DAEMON,
MODE_FORKED
};

enum
{
CONFIG_IRC,
CONFIG_TICKS,
CONFIG_SYSTEM,
CONFIG_BANNED,
};


enum
{
SESSION_IRC,
SESSION_HTTP,
};

#define MAXIPRECORD (6) //It's really 5.

enum
{
NULL_START,
SIGNALS_SIGHUP,
SIGNALS_SIGINT,
SIGNALS_SIGQUIT,
SIGNALS_SIGILL,
SIGNALS_SIGTRAP,
SIGNALS_SIGABRT,
SIGNALS_SIGBUS,
SIGNALS_SIGFPE,
SIGNALS_SIGKILL,
SIGNALS_SIGUSR1,
SIGNALS_SIGSEGV,
SIGNALS_SIGUSR2,
SIGNALS_SIGPIPE,
SIGNALS_SIGALRM,
SIGNALS_SIGTERM,
SIGNALS_SIGSTKFLT,
SIGNALS_SIGCHLD,
SIGNALS_SIGCONT,
SIGNALS_SIGSTOP,
SIGNALS_SIGTSTP,
SIGNALS_SIGTTIN,
SIGNALS_SIGTTOU,

SIGNALS_SIGURG,
SIGNALS_SIGXCPU,
SIGNALS_SIGXFSZ,
SIGNALS_SIGVTALRM,
SIGNALS_SIGPROF,
SIGNALS_SIGWINCH,
SIGNALS_SIGIO,
SIGNALS_SIGPWR,
SIGNALS_SIGSYS,



SIGNALS_NUMUSED,
};

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#endif
