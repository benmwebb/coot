
#include "mini-mol-utils.hh"

// C-beta position
std::pair<short int, clipper::Coord_orth>
coot::cbeta_position(const coot::minimol::residue &res) {

//     For fitting Cbetas:
//     Cb pos = Ca pos + -0.94rx(hat) + 1.2ry(hat)
//     where rx is the bisector of N-Ca and C-Ca and rz is N-C and ry is 
//     rz x rx (it seems from get_ori_to_this_res in coot-coord-utils)
 
   clipper::Coord_orth pos(0.0, 0.0, 0.0);
   std::pair<short int, clipper::Coord_orth> p(0, pos);

   short int found_ca = 0;
   short int found_c  = 0;
   short int found_n  = 0;

   clipper::Coord_orth ca_pos;
   clipper::Coord_orth c_pos;
   clipper::Coord_orth n_pos;

   for (int iat=0; iat<res.atoms.size(); iat++) {
      if (res.atoms[iat].name == " CA ") {
	 found_ca = 1;
	 ca_pos = res.atoms[iat].pos;
      }
      if (res.atoms[iat].name == " C  ") {
	 found_c = 1;
	 c_pos = res.atoms[iat].pos;
      }
      if (res.atoms[iat].name == " N  ") {
	 found_n = 1;
	 n_pos = res.atoms[iat].pos;
      }
   }

   if (found_ca && found_c && found_n) {

      clipper::Coord_orth n_ca = ca_pos - n_pos;
      double n_ca_len = clipper::Coord_orth::length(ca_pos, n_pos);
      if (n_ca_len < 3.0 && n_ca_len > 1.0) { 
	 clipper::Coord_orth n_ca_unit(n_ca.x()/n_ca_len,
				       n_ca.y()/n_ca_len,
				       n_ca.z()/n_ca_len);
	 clipper::Coord_orth c_ca = ca_pos - c_pos;
	 double c_ca_len = clipper::Coord_orth::length(ca_pos, n_pos);
	 if (c_ca_len < 3.0 && c_ca_len > 1.0) { 
	    clipper::Coord_orth c_ca_unit(c_ca.x()/c_ca_len,
					  c_ca.y()/c_ca_len,
					  c_ca.z()/c_ca_len);
	    clipper::Coord_orth bis_sum = n_ca_unit + c_ca_unit;
	    double bis_sum_len =
	       clipper::Coord_orth::length(bis_sum,
					   clipper::Coord_orth(0.0,0.0,0.0));
	    clipper::Coord_orth bisector(bis_sum.x()/bis_sum_len,
					 bis_sum.y()/bis_sum_len,
					 bis_sum.z()/bis_sum_len);

	    clipper::Coord_orth rz = c_pos - n_pos;
	    double rz_len = clipper::Coord_orth::length(c_pos, n_pos);

	    clipper::Coord_orth rz_unit(rz.x()/rz_len,
					rz.y()/rz_len,
					rz.z()/rz_len);

	    clipper::Coord_orth cross_prod(clipper::Coord_orth::cross(rz_unit, bisector));

	    // 	    clipper::Coord_orth c_beta_pos = ca_pos -0.94 * bisector + 1.2 *cross_prod;
	    clipper::Coord_orth c_beta_pos = ca_pos + 0.94 * bisector + 1.2 * cross_prod;
	    p.first = 1;
	    p.second = c_beta_pos;

	    //      ry = rz * rx
	    // i.e. ry = N->C  * bisector

	    // std::cout << "Cbeta atoms found\n" << c_beta_pos.format() << std::endl;
	 }
      }
   } else {
      std::cout << "INFO:: not all atoms found\n" << found_ca << " "
		<< found_c << " " << found_n << std::endl;
   }
   return p;
} 



// Carbonyl O position return a pair, the first of which indicates
// if the O was properly positioned.  There are distance sanity
// checks applied also.  There is abstractable code.
std::pair<short int, clipper::Coord_orth>
coot::o_position(const coot::minimol::residue &res_with_CA_C,
		 const coot::minimol::residue &res_with_N) {

   // For setting the O position, the position is 1.231A from the C in
   // a direction perpendicular to N->Ca in the plane of N,C,CA.  We
   // get a vector in the plane of N,C,CA by taking the cross product
   // of N->CA and C->CA.

   clipper::Coord_orth pos(0.0, 0.0, 0.0);
   std::pair<short int, clipper::Coord_orth> p(0, pos);

   short int found_ca = 0;
   short int found_c  = 0;
   short int found_n  = 0;

   clipper::Coord_orth ca_pos;
   clipper::Coord_orth c_pos;
   clipper::Coord_orth n_pos;

   for (int iat=0; iat<res_with_N.atoms.size(); iat++) {
      if (res_with_N.atoms[iat].name == " N  ") {
	 found_n = 1;
	 n_pos = res_with_N.atoms[iat].pos;
	 break;
      }
   }

   for (int iat=0; iat<res_with_CA_C.atoms.size(); iat++) {
      if (res_with_CA_C.atoms[iat].name == " CA ") {
	 found_ca = 1;
	 ca_pos = res_with_CA_C.atoms[iat].pos;
      }
      if (res_with_CA_C.atoms[iat].name == " C  ") {
	 found_c = 1;
	 c_pos = res_with_CA_C.atoms[iat].pos;
      }
   }

   if (found_ca && found_c && found_n) {
      // We could absract this code for functions that don't come in
      // with minimol::residues

      clipper::Coord_orth n_ca = ca_pos - n_pos;
      double n_ca_d = clipper::Coord_orth::length(ca_pos, n_pos);
      if (n_ca_d < 3.0) {
	 // the vector direction and signed are determined (I should
	 // confess) by guess and fiddle.
	 clipper::Coord_orth n_ca_unit(n_ca.unit());
	 clipper::Coord_orth c_ca = ca_pos - c_pos;
	 clipper::Coord_orth c_ca_unit(c_ca.unit());

	 clipper::Coord_orth plane_vector(n_ca_unit - c_ca_unit);
	 clipper::Coord_orth plane_vector_unit(plane_vector.unit());
	 p.first = 1;
	 p.second = c_pos + 1.231 * plane_vector_unit;
      }
   }
   return p;
}
