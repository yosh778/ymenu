
#ifndef _YCSO_H_
#define _YCSO_H_

#include "YISO.h"


class YCSO : public YISO
{

private:
	
	
public:
	YCSO( string csoPath );
	~YCSO();
	
	static bool isCSO ( string filePath );
};


#endif
