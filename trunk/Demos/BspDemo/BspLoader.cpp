/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/


#include "BspLoader.h"

typedef struct
{
	char	filename[1024];
	char    *buffer,*script_p,*end_p;
	int     line;
} BSPScript;

#define	MAX_INCLUDES	8
BSPScript	scriptstack[MAX_INCLUDES];
BSPScript	*script;
int			scriptline;

char    token[BSPMAXTOKEN];
bool endofscript;
bool tokenready;                     // only true if UnGetToken was just called

//
//LoadBSPFile
//

int extrasize = 100;

bool	BspLoader::LoadBSPFile( void* memoryBuffer) {
	
	BSPHeader	*header = (BSPHeader*) memoryBuffer;

	// load the file header
	if (header)
	{
		// swap the header
		SwapBlock( (int *)header, sizeof(*header) );
		
		int length = (header->lumps[BSPLUMP_SHADERS].filelen) / sizeof(BSPShader);
		m_dshaders.resize(length+extrasize);
		m_numShaders = CopyLump( header, BSPLUMP_SHADERS, &m_dshaders[0], sizeof(BSPShader) );

		length = (header->lumps[LUMP_MODELS].filelen) / sizeof(BSPModel);
		m_dmodels.resize(length+extrasize);
		m_nummodels = CopyLump( header, LUMP_MODELS, &m_dmodels[0], sizeof(BSPModel) );

		length = (header->lumps[BSPLUMP_PLANES].filelen) / sizeof(BSPPlane);
		m_dplanes.resize(length+extrasize);
		m_numplanes = CopyLump( header, BSPLUMP_PLANES, &m_dplanes[0], sizeof(BSPPlane) );

		length = (header->lumps[BSPLUMP_LEAFS].filelen) / sizeof(BSPLeaf);
		m_dleafs.resize(length+extrasize);
		m_numleafs = CopyLump( header, BSPLUMP_LEAFS, &m_dleafs[0], sizeof(BSPLeaf) );

		length = (header->lumps[BSPLUMP_NODES].filelen) / sizeof(BSPNode);
		m_dnodes.resize(length+extrasize);
		m_numnodes = CopyLump( header, BSPLUMP_NODES, &m_dnodes[0], sizeof(BSPNode) );

		length = (header->lumps[BSPLUMP_LEAFSURFACES].filelen) / sizeof(m_dleafsurfaces[0]);
		m_dleafsurfaces.resize(length+extrasize);
		m_numleafsurfaces = CopyLump( header, BSPLUMP_LEAFSURFACES, &m_dleafsurfaces[0], sizeof(m_dleafsurfaces[0]) );

		length = (header->lumps[BSPLUMP_LEAFBRUSHES].filelen) / sizeof(m_dleafbrushes[0]) ;
		m_dleafbrushes.resize(length+extrasize);
		m_numleafbrushes = CopyLump( header, BSPLUMP_LEAFBRUSHES, &m_dleafbrushes[0], sizeof(m_dleafbrushes[0]) );

		length = (header->lumps[LUMP_BRUSHES].filelen) / sizeof(BSPBrush);
		m_dbrushes.resize(length+extrasize);
		m_numbrushes = CopyLump( header, LUMP_BRUSHES, &m_dbrushes[0], sizeof(BSPBrush) );


		length = (header->lumps[LUMP_BRUSHSIDES].filelen) / sizeof(BSPBrushSide);
		m_dbrushsides.resize(length+extrasize);
		m_numbrushsides = CopyLump( header, LUMP_BRUSHSIDES, &m_dbrushsides[0], sizeof(BSPBrushSide) );

		
		length = (header->lumps[LUMP_SURFACES].filelen) / sizeof(BSPSurface);
		m_drawSurfaces.resize(length+extrasize);
		m_numDrawSurfaces = CopyLump( header, LUMP_SURFACES, &m_drawSurfaces[0], sizeof(BSPSurface) );


		length = (header->lumps[LUMP_DRAWINDEXES].filelen) / sizeof(m_drawIndexes[0]);
		m_drawIndexes.resize(length+extrasize);
		m_numDrawIndexes = CopyLump( header, LUMP_DRAWINDEXES, &m_drawIndexes[0], sizeof(m_drawIndexes[0]) );

		length = (header->lumps[LUMP_VISIBILITY].filelen) / 1;
		m_visBytes.resize(length+extrasize);
		m_numVisBytes = CopyLump( header, LUMP_VISIBILITY, &m_visBytes[0], 1 );

		length = (header->lumps[LUMP_LIGHTMAPS].filelen) / 1;
		m_lightBytes.resize(length+extrasize);
		m_numLightBytes = CopyLump( header, LUMP_LIGHTMAPS, &m_lightBytes[0], 1 );

		length = (header->lumps[BSPLUMP_ENTITIES].filelen) / 1;
		m_dentdata.resize(length+extrasize);
		m_entdatasize = CopyLump( header, BSPLUMP_ENTITIES, &m_dentdata[0], 1);

		length = (header->lumps[LUMP_LIGHTGRID].filelen) / 1;
		m_gridData.resize(length+extrasize);
		m_numGridPoints = CopyLump( header, LUMP_LIGHTGRID, &m_gridData[0], 8 );

		// swap everything
		SwapBSPFile();

		return true;

	}
	return false;
}



const char* BspLoader::ValueForKey( const  BSPEntity* ent, const char* key ) const {

	const BSPKeyValuePair* ep;
	
	for (ep=ent->epairs ; ep ; ep=ep->next) {
		if (!strcmp(ep->key, key) ) {
			return ep->value;
		}
	}
	return "";
}

float	BspLoader::FloatForKey( const BSPEntity *ent, const char *key ) {
	const char	*k;
	
	k = ValueForKey( ent, key );
	return float(atof(k));
}

bool 	BspLoader::GetVectorForKey( const BSPEntity *ent, const char *key, BSPVector3 vec ) {

	const char	*k;
	k = ValueForKey (ent, key);
	if (strcmp(k, ""))
	{
		sscanf (k, "%f %f %f", &vec[0], &vec[1], &vec[2]);
		return true;
	}
	return false;
}




/*
==============
ParseFromMemory
==============
*/
void BspLoader::ParseFromMemory (char *buffer, int size)
{
	script = scriptstack;
	script++;
	if (script == &scriptstack[MAX_INCLUDES])
	{
		//printf("script file exceeded MAX_INCLUDES");
	}
	strcpy (script->filename, "memory buffer" );

	script->buffer = buffer;
	script->line = 1;
	script->script_p = script->buffer;
	script->end_p = script->buffer + size;

	endofscript = false;
	tokenready = false;
}


bool BspLoader::EndOfScript (bool crossline)
{
	if (!crossline)
		//printf("Line %i is incomplete\n",scriptline);

	if (!strcmp (script->filename, "memory buffer"))
	{
		endofscript = true;
		return false;
	}

	//free (script->buffer);
	if (script == scriptstack+1)
	{
		endofscript = true;
		return false;
	}
	script--;
	scriptline = script->line;
	//printf ("returning to %s\n", script->filename);
	return GetToken (crossline);
}

/*

==============
GetToken
==============
*/
bool BspLoader::GetToken (bool crossline)
{
	char    *token_p;

	if (tokenready)                         // is a token allready waiting?
	{
		tokenready = false;
		return true;
	}

	if (script->script_p >= script->end_p)
		return EndOfScript (crossline);

//
// skip space
//
skipspace:
	while (*script->script_p <= 32)
	{
		if (script->script_p >= script->end_p)
			return EndOfScript (crossline);
		if (*script->script_p++ == '\n')
		{
			if (!crossline)
			{
				//printf("Line %i is incomplete\n",scriptline);
			}
			scriptline = script->line++;
		}
	}

	if (script->script_p >= script->end_p)
		return EndOfScript (crossline);

	// ; # // comments
	if (*script->script_p == ';' || *script->script_p == '#'
		|| ( script->script_p[0] == '/' && script->script_p[1] == '/') )
	{
		if (!crossline)
		{
			//printf("Line %i is incomplete\n",scriptline);
		}
		while (*script->script_p++ != '\n')
			if (script->script_p >= script->end_p)
				return EndOfScript (crossline);
		scriptline = script->line++;
		goto skipspace;
	}

	// /* */ comments
	if (script->script_p[0] == '/' && script->script_p[1] == '*')
	{
		if (!crossline)
		{
			//printf("Line %i is incomplete\n",scriptline);
		}
		script->script_p+=2;
		while (script->script_p[0] != '*' && script->script_p[1] != '/')
		{
			if ( *script->script_p == '\n' ) {
				scriptline = script->line++;
			}
			script->script_p++;
			if (script->script_p >= script->end_p)
				return EndOfScript (crossline);
		}
		script->script_p += 2;
		goto skipspace;
	}

//
// copy token
//
	token_p = token;

	if (*script->script_p == '"')
	{
		// quoted token
		script->script_p++;
		while (*script->script_p != '"')
		{
			*token_p++ = *script->script_p++;
			if (script->script_p == script->end_p)
				break;
			if (token_p == &token[BSPMAXTOKEN])
			{
				//printf ("Token too large on line %i\n",scriptline);
			}
		}
		script->script_p++;
	}
	else	// regular token
	while ( *script->script_p > 32 && *script->script_p != ';')
	{
		*token_p++ = *script->script_p++;
		if (script->script_p == script->end_p)
			break;
		if (token_p == &token[BSPMAXTOKEN])
		{
			//printf ("Token too large on line %i\n",scriptline);
		}
	}

	*token_p = 0;

	if (!strcmp (token, "$include"))
	{
		//GetToken (false);
		//AddScriptToStack (token);
		return false;//GetToken (crossline);
	}

	return true;
}

char *BspLoader::copystring(const char *s)
{
	char	*b;
	b = (char*) malloc( strlen(s)+1);
	strcpy (b, s);
	return b;
}

void BspLoader::StripTrailing( char *e ) {
	char	*s;

	s = e + strlen(e)-1;
	while (s >= e && *s <= 32)
	{
		*s = 0;
		s--;
	}
}
/*
=================
ParseEpair
=================
*/
BSPKeyValuePair *BspLoader::ParseEpair( void ) {
	BSPKeyValuePair	*e;

	e = (struct BSPPair*) malloc( sizeof(BSPKeyValuePair));
	memset( e, 0, sizeof(BSPKeyValuePair) );
	
	if ( strlen(token) >= BSPMAX_KEY-1 ) {
		//printf ("ParseEpar: token too long");
	}
	e->key = copystring( token );
	GetToken( false );
	if ( strlen(token) >= BSPMAX_VALUE-1 ) {

		//printf ("ParseEpar: token too long");
	}
	e->value = copystring( token );

	// strip trailing spaces that sometimes get accidentally
	// added in the editor
	StripTrailing( e->key );
	StripTrailing( e->value );

	return e;
}


/*
================
ParseEntity
================
*/
bool	BspLoader::ParseEntity( void ) {
	BSPKeyValuePair		*e;
	BSPEntity	*mapent;

	if ( !GetToken (true) ) {
		return false;
	}

	if ( strcmp (token, "{") ) {

		//printf ("ParseEntity: { not found");
	}

	BSPEntity bla;
	bla.brushes = 0;
	bla.epairs = 0;
	bla.firstDrawSurf = 0;
	bla.origin[0] = 0.f;
	bla.origin[1] = 0.f;
	bla.origin[2] = 0.f;
	bla.patches = 0;

	m_entities.push_back(bla);
	mapent = &m_entities[m_entities.size()-1];
	m_num_entities++;

	do {
		if ( !GetToken (true) ) {
			//printf("ParseEntity: EOF without closing brace");
		}
		if ( !strcmp (token, "}") ) {
			break;
		}
		e = (struct BSPPair*)ParseEpair ();
		e->next = mapent->epairs;
		mapent->epairs = e;
	} while (1);
	
	return true;
}

/*
================
ParseEntities

Parses the dentdata string into entities
================
*/
void BspLoader::ParseEntities( void ) {
	m_num_entities = 0;
	m_entities.clear();

	ParseFromMemory( &m_dentdata[0], m_entdatasize );

	while ( ParseEntity () ) {
	}	
}



int BspLoader::getMachineEndianness()
{
   long int i = 1;
   const char *p = (const char *) &i;
   if (p[0] == 1)  // Lowest address contains the least significant byte
	   return BSP_LITTLE_ENDIAN;
   else
	   return BSP_BIG_ENDIAN;
}

short   BspLoader::LittleShort (short l)
{
	if (machineEndianness() == BSP_BIG_ENDIAN)
	{
		unsigned char    b1,b2;

		b1 = l&255;
		b2 = (l>>8)&255;

		return (b1<<8) + b2;
	}
	//little endian
	return l;
}

short   BspLoader::BigShort (short l)
{
	if (machineEndianness() == BSP_BIG_ENDIAN)
	{
		return l;
	}

	unsigned char   b1,b2;

	b1 = l&255;
	b2 = (l>>8)&255;

	return (b1<<8) + b2;



}


int    BspLoader::LittleLong (int l)
{
	if (machineEndianness() == BSP_BIG_ENDIAN)
	{
		unsigned char    b1,b2,b3,b4;

		b1 = l&255;
		b2 = (l>>8)&255;
		b3 = (l>>16)&255;
		b4 = (l>>24)&255;

		return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
	}

	//little endian
	return l;

}

int    BspLoader::BigLong (int l)
{
	if (machineEndianness() == BSP_BIG_ENDIAN)
	{
		return l;
	}


	unsigned char    b1,b2,b3,b4;

	b1 = l&255;
	b2 = (l>>8)&255;
	b3 = (l>>16)&255;
	b4 = (l>>24)&255;

	return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;

}


float	BspLoader::LittleFloat (float l)
{
	if (machineEndianness() == BSP_BIG_ENDIAN)
	{
		union {unsigned char b[4]; float f;} in, out;
		
		in.f = l;
		out.b[0] = in.b[3];
		out.b[1] = in.b[2];
		out.b[2] = in.b[1];
		out.b[3] = in.b[0];
		
		return out.f;
	}

	//little endian
	return l;
}

float	BspLoader::BigFloat (float l)
{
	if (machineEndianness() == BSP_BIG_ENDIAN)
	{
		return l;
	}
	//little endian
	union {unsigned char b[4]; float f;} in, out;
	
	in.f = l;
	out.b[0] = in.b[3];
	out.b[1] = in.b[2];
	out.b[2] = in.b[1];
	out.b[3] = in.b[0];
	
	return out.f;
}






//
// SwapBlock
// If all values are 32 bits, this can be used to swap everything
//

void BspLoader::SwapBlock( int *block, int sizeOfBlock ) {
	int		i;

	sizeOfBlock >>= 2;
	for ( i = 0 ; i < sizeOfBlock ; i++ ) {
		block[i] = LittleLong( block[i] );
	}
}

//
// CopyLump
//

int BspLoader::CopyLump( BSPHeader	*header, int lump, void *dest, int size ) {
	int		length, ofs;

	length = header->lumps[lump].filelen;
	ofs = header->lumps[lump].fileofs;
	
	//if ( length % size ) {
	//	printf ("LoadBSPFile: odd lump size");
	//}

	memcpy( dest, (unsigned char *)header + ofs, length );

	return length / size;
}




//
// SwapBSPFile
//

void BspLoader::SwapBSPFile( void ) {
	int				i;
	
	// models	
	SwapBlock( (int *) &m_dmodels[0], m_nummodels * sizeof( m_dmodels[0] ) );

	// shaders (don't swap the name)
	for ( i = 0 ; i < m_numShaders ; i++ ) {
		m_dshaders[i].contentFlags = LittleLong( m_dshaders[i].contentFlags );
		m_dshaders[i].surfaceFlags = LittleLong( m_dshaders[i].surfaceFlags );
	}

	// planes
	SwapBlock( (int *)&m_dplanes[0], m_numplanes * sizeof( m_dplanes[0] ) );
	
	// nodes
	SwapBlock( (int *)&m_dnodes[0], m_numnodes * sizeof( m_dnodes[0] ) );

	// leafs
	SwapBlock( (int *)&m_dleafs[0], m_numleafs * sizeof( m_dleafs[0] ) );

	// leaffaces
	SwapBlock( (int *)&m_dleafsurfaces[0], m_numleafsurfaces * sizeof( m_dleafsurfaces[0] ) );

	// leafbrushes
	SwapBlock( (int *)&m_dleafbrushes[0], m_numleafbrushes * sizeof( m_dleafbrushes[0] ) );

	// brushes
	SwapBlock( (int *)&m_dbrushes[0], m_numbrushes * sizeof( m_dbrushes[0] ) );

	// brushsides
	SwapBlock( (int *)&m_dbrushsides[0], m_numbrushsides * sizeof( m_dbrushsides[0] ) );

	// vis
	((int *)&m_visBytes)[0] = LittleLong( ((int *)&m_visBytes)[0] );
	((int *)&m_visBytes)[1] = LittleLong( ((int *)&m_visBytes)[1] );


	// drawindexes
	SwapBlock( (int *)&m_drawIndexes[0], m_numDrawIndexes * sizeof( m_drawIndexes[0] ) );

	// drawsurfs
	SwapBlock( (int *)&m_drawSurfaces[0], m_numDrawSurfaces * sizeof( m_drawSurfaces[0] ) );

}





bool BspLoader::findVectorByName(float* outvec,const char* name)
{
	const char *cl;
	BSPVector3 origin;
	
	bool found = false;

	ParseEntities();

	for ( int i = 1; i < m_num_entities; i++ ) {
		cl = ValueForKey (&m_entities[i], "classname");
		if ( !strcmp( cl, "info_player_start" ) ) {
				GetVectorForKey( &m_entities[i], "origin", origin );
				found = true;
				break;
			}
		if ( !strcmp( cl, "info_player_deathmatch" ) ) {
				GetVectorForKey( &m_entities[i], "origin", origin );
				found = true;
				break;
			}
	}

	if (found)
	{
		outvec[0] = origin[0];
		outvec[1] = origin[1];
		outvec[2] = origin[2];
	}
	return found;
}
  


const BSPEntity * BspLoader::getEntityByValue( const char* name, const char* value)
{
	const BSPEntity* entity = NULL;

	for ( int i = 1; i < m_num_entities; i++ ) {

		const BSPEntity& ent = m_entities[i];

		const char* cl = ValueForKey (&m_entities[i], name);
		if ( !strcmp( cl, value ) ) {
			entity = &ent;
			break;
		}
	}
	return entity;
}