#include "phalanx.h"

#define LREHASH 8

tpbook Learn;

typedef struct
{	unsigned hashboard;
	short depth, value;
} tlearn;


void wlearn( int depth, int value )
{

int size, pos;
int i;
int bdepth, bpos;
tlearn cell;

if( Flag.learn == 0 || Learn.f == NULL || depth <= 400 ) return;

size = Learn.filesize/sizeof(tlearn);
pos = G[Counter].hashboard % size;

if( fseek( Learn.f, pos*sizeof(tlearn), SEEK_SET ) != 0 ) return;
myfread( &cell, sizeof(tlearn), Learn.f );

bdepth = -1000;
bpos   = pos;

for( i=0; i<LREHASH; i++ )
{
	if( fseek( Learn.f, ((pos+i)%size)*sizeof(tlearn), SEEK_SET ) != 0 )
		return;
	myfread( &cell, sizeof(tlearn), Learn.f );

	if( cell.hashboard == 0 || cell.hashboard == G[Counter].hashboard )
	{ bpos = pos+i; bdepth = cell.depth; break; }

	if( cell.depth > bdepth )
	{ bdepth = cell.depth; bpos = pos+i; }
}

if( cell.hashboard == G[Counter].hashboard && cell.depth > depth )
	return;

if( fseek( Learn.f, (bpos%size)*sizeof(tlearn), SEEK_SET ) != 0 )
	return;

cell.hashboard = G[Counter].hashboard;
cell.depth = depth;
cell.value = value;

myfwrite( &cell, sizeof(tlearn), Learn.f );

}



int rlearn( void )
{

int size, pos;
int i;
tlearn cell;

if( Flag.learn == 0 || Learn.f == NULL ) return NOVALUE;

size = Learn.filesize/sizeof(tlearn);
pos = G[Counter].hashboard % size;

for( i=0; i<LREHASH; i++ )
{
	if( fseek( Learn.f, ((pos+i)%size)*sizeof(tlearn), SEEK_SET ) != 0 )
		return NOVALUE;
	myfread( &cell, sizeof(tlearn), Learn.f );

	if( cell.hashboard == 0 ) return NOVALUE;

	if( cell.hashboard == G[Counter].hashboard )
	{
		if( cell.depth >= Depth ) return cell.value;
		else return NOVALUE;
	}
}

return NOVALUE;

}


