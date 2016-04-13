
#ifndef MAKE_ENHANCED_LIGAND_TOOLS
int main(int argc, char **argv) {return 0;}
#else 
#include "cod-atom-types.hh"
#include "rdkit-interface.hh"
#include "coot-utils/coot-coord-utils.hh"
#include "utils/coot-utils.hh"

#include "bond-record-container-t.hh"

// rdkit_mol is not const because there is no const beginAtoms() operator.
// 
void write_types(RDKit::RWMol &rdkm) {

   if (false) {
      for (unsigned int iat=0; iat<rdkm.getNumAtoms(); iat++) {
	 try {
	    std::string name;
	    RDKit::ATOM_SPTR at_p = rdkm[iat];
	    at_p->getProp("name", name);
	    std::cout << iat << "   \"" << name << "\"\n";
	 }
	 catch (const KeyErrorException &err) {
	    std::cout << "caught no-name exception in rdkit_mol H-block" << std::endl;
	 }
      }
   }

   cod::atom_types_t t;
   std::vector<std::string> v = t.get_cod_atom_types(rdkm);

   const RDKit::PeriodicTable *tbl = RDKit::PeriodicTable::getTable();
   for (unsigned int iat=0; iat<v.size(); iat++) {
      std::string name;
      try {
	 RDKit::ATOM_SPTR at_p = rdkm[iat];
	 at_p->getProp("name", name);

	 int n = at_p->getAtomicNum();
	 std::string atom_ele = tbl->getElementSymbol(n);
      
	 // Acedrg writes out: index element name COD-type, we should match that.
	 std::cout << " " << std::right << std::setw(3) << iat << "    "
		   << atom_ele << "    " << name << "     " << v[iat]
		   << "\n";
      }
      catch (const KeyErrorException &err) { }
   }
}

void molecule_from_ccd_pdbx(const std::string &comp_id,
			    const std::string &file_name) {

   coot::protein_geometry geom;
   int read_number = 0;
   geom.init_refmac_mon_lib(file_name, read_number++);

   bool idealised_flag = true;
   mmdb::Manager *mol = geom.mol_from_dictionary(comp_id, idealised_flag);

   if (! mol) {
      std::cout << "Null mol from mol_from_dictionary() for " <<  comp_id << std::endl;
   } else {
      mmdb::Residue *residue_p = coot::util::get_first_residue(mol);

      if (! residue_p) {
	 // pretty strange
	 std::cout << "Null residue from mol from mol_from_dictionary() for "
		   << comp_id << std::endl;
      } else {
	 try {
	    RDKit::RWMol rdkm = coot::rdkit_mol_sanitized(residue_p, geom);
	    write_types(rdkm);
	 }

	 catch(const std::runtime_error &rte) {
	    std::cout << "error::" << rte.what() << std::endl;
	 }
      }
   }
}

void molecule_from_comp_id(const std::string &comp_id) {
   try {
      coot::protein_geometry geom;
      // geom.init_standard();

      // std::string three_letter_code = "001"; // nice test
      // std::string three_letter_code = "0PY"; // simple
      // std::string three_letter_code = "06C"; // simplest
	 
      bool idealised_flag = true;
      mmdb::Manager *mol = geom.mol_from_dictionary(comp_id, idealised_flag);

      if (! mol) {
	 std::cout << "Null mol from mol_from_dictionary() for " <<  comp_id << std::endl;
      } else {
	 
	 mmdb::Residue *residue_p = coot::util::get_first_residue(mol);

	 if (! residue_p) {
	    // pretty strange
	    std::cout << "Null residue from mol from mol_from_dictionary() for "
		      << comp_id << std::endl;
	 } else { 

	    RDKit::RWMol rdkm = coot::rdkit_mol_sanitized(residue_p, geom);
	    coot::debug_rdkit_molecule(&rdkm);

	    write_types(rdkm);
	       
	 }
      }
   }
   catch (const std::runtime_error &rte) {
      std::cout << "test-cod-atom-types caught exception: " << rte.what() << std::endl;
   }
}

void molecule_from_SMILES(const std::string &smiles_string) {

   std::string s = smiles_string;
   // std::string s = "COc1ccc(cc1O[C@H]1C[C@@H]2CC[C@H]1C2)C1CNC(=O)NC1";
   // s = "CC12CC[C@@]3(C1)CC(=C)CC3C(C2C(C)(C)C)C(C)(C)C"; // bird
   // s = "O[V]1(O)(O)ONC(=[O]1)c1ccccc1";
   // s = "C#O";
   s = "[H]/N=C(\\N)/NCCC[C@@H](C(=O)N[C@@H](CCc1ccccc1)/C=C/S(=O)(=O)c2ccccc2)NC(=O)N3CC[NH+](CC3)C";
   
   RDKit::RWMol *rdkmp = RDKit::SmilesToMol(s);
   coot::mogulify_mol(*rdkmp);

   { 
      bool includeStereo = true;
      bool kekulize = false;
      std::string sdf_file_name = "smiles-string.mdl";
      int confId = -1;
      RDDepict::compute2DCoords(*rdkmp, NULL, true);
      std::cout << "confId: " << confId << std::endl;
      RDKit::MolToMolFile(*rdkmp, sdf_file_name, includeStereo, confId, kekulize);
   }
   
   coot::debug_rdkit_molecule(rdkmp);
   RDKit::RWMol rdkm(*rdkmp);

   cod::atom_types_t t;
   std::vector<std::string> v = t.get_cod_atom_types(rdkm);

   std::cout << "PE-TYPES:: -------- got " << v.size() << " atoms " << std::endl;
   for (unsigned int i=0; i<v.size(); i++)
      std::cout << "   " << i << " " << v[i] << "" << std::endl;

}

void read_tables(const std::string &tables_dir_name) {

   cod::bond_record_container_t brc(tables_dir_name);

   if (false) {  // debug
      unsigned int n_records = 10;
      for (unsigned int i=0; i<n_records; i++)
	 std::cout << "   " << brc.bonds[i] << std::endl;
   }

   brc.write("bonds.tab"); // writes atom-indices.tab too atm

   std::cout << "----------- reading " << std::endl;
   cod::bond_record_container_t brc_read;
   brc_read.read("atom-indices.tab", "bonds.tab");
   std::cout << "----------- done reading " << std::endl;

   brc_read.check();
   
}

#include "coot-utils/residue-and-atom-specs.hh"

#include "coords/mmdb-extras.h"
#include "coords/mmdb.h"

void
validate(const std::string &comp_id,
	 const std::string &chain_id,
	 int res_no,
	 const std::string &pdb_file_name,
	 const std::string &cif_file_name) {

   cod::bond_record_container_t brc; // perhaps we need a better constructor here
   brc.read("atom-indices.tab", "bonds.tab");
   atom_selection_container_t asc = get_atom_selection(pdb_file_name, true, false);

   coot::residue_spec_t residue_spec(chain_id, res_no, "");

   mmdb::Residue *res = coot::util::get_residue(residue_spec, asc.mol);

   if (res) { 

      coot::protein_geometry geom;
      int read_number = 0;
      geom.init_refmac_mon_lib(cif_file_name, read_number++);

      std::pair<bool, coot::dictionary_residue_restraints_t> p = 
	 geom.get_monomer_restraints(comp_id);

      if (p.first) {

	 coot::dictionary_residue_restraints_t rest = p.second;
	 brc.validate(res, rest);
      }
   }
}


int main(int argc, char **argv) {

   int status = 0;

   if (argc > 1) {
      if (argc == 2) {
	 std::string s = argv[1];
	 if (s.length() == 3)
	    molecule_from_comp_id(s);
	 else
	    molecule_from_SMILES(s);
      }
      
      if (argc == 3) {

	 std::string comp_id   = argv[1];
	 std::string file_name = argv[2];

	 if (comp_id == "tables") {
	    read_tables(file_name);
	 } else {
	    molecule_from_ccd_pdbx(comp_id, file_name);
	 }
      }

      if (argc == 7) {

	 std::string residue = argv[1];
	 if (residue == "residue") {
	    std::string comp_id       = argv[2];
	    std::string chain_id      = argv[3];
	    std::string res_no_str    = argv[4];
	    std::string pdb_file_name = argv[5];
	    std::string cif_file_name = argv[6];

	    try {
	       int res_no = coot::util::string_to_int(res_no_str);
	       validate(comp_id, chain_id, res_no, pdb_file_name, cif_file_name);
	    }
	    catch (const std::runtime_error &rte) {
	    }
	 }
      }
   }
   return status;
}

#endif // MAKE_ENHANCED_LIGAND_TOOLS

