#include <stdio.h>
#include <string>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

char ss[20];
char to[20];

static void parse_line ( char *s ) {
	memset ( ss, 0, 20 );
	memset ( to, 0, 20 );
	char *e;
	while ( e = strchr ( s, ',' ) ) *e = '.';
	e = strchr ( s, '\n' );
	if ( e ) *e = 0;

	for ( int i = 0; *s != 32; i++, s++ ) {
		ss[i] = *s;
	}
	s++;
	while ( *s != 32 ) s++;
	s++;
	for ( int i = 0; *s != 32; i++, s++ ) {
		to[i] = *s;
	}
	s++;

}

int main ( int argc, char **argv ) {
	if ( argc < 4 ) {
		printf ( "./parser_sub [file of subtitle] [name of movie] [sound map of movie]\n" );
		return -1;
	}

	FILE *fp = fopen ( argv[1], "r" );

	char line[512];

	char *map = argv[3];
	char *movie = argv[2];

	char *s;
	int pos = 0;
	FILE *fp_out;
	char *a;
	char buffer[1024];
	char soundname[255];
	printf ( "processing...\n" );
	while ( s = fgets ( line, 512, fp ) ) {
		if ( pos == 0 ) {
			memset ( buffer, 0, 1024 );
			a = buffer;
			char *m = strchr ( s, '\n' );
			if ( m ) *m = 0;
			char filename[255];
			memset ( soundname, 0, 255 );
			snprintf ( filename, 255, "%s.txt", s );
			snprintf ( soundname, 255, "%s.mp3", s );
			printf ( "%s\n", filename );
			fp_out = fopen ( filename, "w" );
			pos++;
			continue;
		}
		if ( pos > 0 && s[0] == '\n' ) {
			pos = 0;
			fwrite ( buffer, 1, strlen ( buffer ), fp_out );
			fclose ( fp_out );
			char args[2048];
			snprintf ( args, 2048, "ffmpeg -i %s -qscale:a 0 -c:a mp3 -map 0:%s -ss %s -to %s %s", movie, map, ss, to, soundname );
			printf ( "system: %s\n", args );
			fflush ( stdout );
			system ( args );
			continue;
		}
		if ( pos == 1 ) {
			parse_line ( s );
			pos++;
			continue;
		}
		char *ii = strchr ( s, '\r' );
		if ( ii ) *ii = ' ';

		int len = strlen ( s );
		strncpy ( a, s, len );
		a += len;
	}
}
