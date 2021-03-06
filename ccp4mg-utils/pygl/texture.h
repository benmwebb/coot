/*
     pygl/texture.h: CCP4MG Molecular Graphics Program
     Copyright (C) 2001-2008 University of York, CCLRC

     This library is free software: you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public License
     version 3, modified in accordance with the provisions of the 
     license to address the requirements of UK law.
 
     You should have received a copy of the modified GNU Lesser General 
     Public License along with this library.  If not, copies may be 
     downloaded from http://www.ccp4.ac.uk/ccp4license.php
 
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU Lesser General Public License for more details.
*/


#ifndef __TEXTURE_HELP__
#define __TEXTURE_HELP__

#define __GL_HELP__
#ifdef __APPLE_CC__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

enum {NEAREST, LINEAR, MIPMAP};
unsigned int load_texture(const image_info &iinfo, int style);
void set_texture_coord(GLfloat x, GLfloat y, int textured);

#endif
