
char *iohttpVarsRaw;


int iohttpVarsSize;



char *iohttpVarsFind( char *id )
{
  int a;
  char *src;
  if( !( iohttpVarsRaw ) )
    return 0;
  src = iohttpVarsRaw;
  for( ; ; )
  {
    for( a = 0 ; ; a++ )
    {
      if( !( id[a] ) && ( src[a] == '=' ) )
        return( src + a + 1 );
      if( src[a] != id[a] )
        break;
      if( !( src[a] ) )
        return 0;
    }
    for( ; ; src++ )
    {
      if( src[0] == '&' )
      {
        src++;
        break;
      }
      if( !( src[0] ) )
        return 0;
    }
  }
  return 0;
}



void iohttpVarsCut()
{
  char *src;
  if( !( iohttpVarsRaw ) )
    return;
  src = iohttpVarsRaw;
  for( ; ; src++ )
  {
    if( src[0] == '&' )
      src[0] = 0;
    else if( src[0] == 0 )
      return;
  }
  return;
}



int iohttpVarsInit( ReplyDataPtr cnt )
{
  int num, num2;
  if( 0 == strcmp( (cnt->connection)->method, "POST") )
    iohttpVarsRaw = (cnt->connection)->read_buffer;
    
printf("%s\n", cnt->connection->url );
printf("%s\n", cnt->connection->method );
printf("%s\n", cnt->connection->read_buffer );
//printf("%s\n", cnt->connection->write_buffer );
    
    char** tokens;
    tokens = str_split( (cnt->connection)->read_buffer, '&', &num);

if ( (tokens) && (num) ) {
	char** tokens2;
        int i, i2;
	for (i = 0; i < num ; i++) {
		printf("%d\n",num);
		tokens2 = str_split( *(tokens + i), '=', &num2);
		printf("%d\n",num2);
			for (i2 = 0; i2 < num2 ; i2++) {
				printf("month=%d[%s]\n", i2, *(tokens2 + i2));
			}
	}
	printf("\n");
}

 /* else
    iohttpVarsRaw = iohttp->query_string;*/
  return 1;
}



char *iohtmlVarsFind( ReplyDataPtr cnt, char *id ) {
	int a;
	char *value;


for( a = 0; a < (cnt->session)->posts; a++ ) {
	if( ( (cnt->session)->key[a] ) && ( strcmp( id, (cnt->session)->key[a] ) == 0 ) ) {
		value = (cnt->session)->value[a];
		(cnt->session)->key[a] = NULL;
		(cnt->session)->value[a] = NULL;
		return value;
	}
}

value = (char *)MHD_lookup_connection_value(cnt->connection, MHD_GET_ARGUMENT_KIND, id);
if( ( value ) && strlen(value) ) {
	return value;
}

return 0;
}

char *iohtmlHeaderFind( ReplyDataPtr cnt, char *id ) {

return (char *)MHD_lookup_connection_value(cnt->connection, MHD_HEADER_KIND, id);
}

char *iohtmlCookieFind( ReplyDataPtr cnt, char *id ) {

return (char *)MHD_lookup_connection_value(cnt->connection, MHD_COOKIE_KIND, id);
}

int iohtmlCookieAdd( ReplyDataPtr cnt, char *name, char *value, ... ) {
	char cstr[256];
	char text[256];
	va_list ap;

va_start( ap, value );
vsnprintf( text, 256, value, ap );
va_end( ap );

snprintf(cstr, sizeof (cstr), "%s=%s", name, text);

strncpy((cnt->cookies).value[(cnt->cookies).num], cstr, sizeof(cstr) );
(cnt->cookies).num = (cnt->cookies).num + 1;
	

return (cnt->cookies).num;
}



int iohtmlVarsMapcoords( ReplyDataPtr cnt, int *coords )
{

char *string = iohtmlVarsFind( cnt, "sectorzoom" );

  if( !( string ) )
    return 0;
  if( sscanf( string, "%d", &coords[0] ) != 1 )
    return 0;
  string = ioCompareFindWords( string, "," );
  if( !( string ) )
    return 0;
  if( sscanf( string, "%d", &coords[1] ) != 1 )
    return 0;
    
    
  return 1;
}





