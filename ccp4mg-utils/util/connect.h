/*
     util/connect.h: CCP4MG Molecular Graphics Program
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


#ifndef _CCPMG_CONNECT_
#define _CCPMG_CONNECT_

#include "cartesian.h"
#include <string>

class SimpleConnection{
 public:
  Cartesian first;
  Cartesian second;
  std::string label;
  SimpleConnection();
  SimpleConnection(const Cartesian &first_in, const Cartesian &second_in, const std::string &label_in="");
  ~SimpleConnection();
  	
};

#endif
