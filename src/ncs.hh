
namespace coot { 

   // Maybe we want a mainchain/side chain split here...
   class ncs_residue_info_t {
   public:
     float mean_diff;
     float n_weighted_atoms;
     int resno; 
     bool filled;
     std::string inscode;
     int serial_number;
     int target_resno; 
     int target_serial_number;
     std::string target_inscode;
     ncs_residue_info_t() {
       mean_diff = -1;
       n_weighted_atoms = 0;
       filled = 0;
     }
     ncs_residue_info_t(int resno_in, const std::string &ins_code_in, int serial_number_in,
			int target_resno_in, const std::string &target_ins_code_in, int target_serial_number_in) {
       filled = 1;
       resno = resno_in;
       inscode = ins_code_in;
       serial_number = serial_number_in;
       target_resno = target_resno_in;
       target_inscode = target_ins_code_in;
       target_serial_number = target_serial_number_in;
     }
   };

   class ncs_chain_difference_t {
   public:
     std::string peer_chain_id;
     std::vector<ncs_residue_info_t> residue_info;
     ncs_chain_difference_t() {
     }
     ncs_chain_difference_t(const std::string &peer_chain_id_in,
			    const std::vector<ncs_residue_info_t> &residue_info_in) {
       peer_chain_id = peer_chain_id_in;
       residue_info = residue_info_in;
     }
   };

   class ncs_differences_t {
   public:
     std::string target_chain_id;
     std::vector<ncs_chain_difference_t> diffs;
     unsigned int size() const { return diffs.size(); }
     ncs_differences_t() {}
     ncs_differences_t(const std::string &target_chain_id_in, 
		       std::vector<ncs_chain_difference_t> diffs_in) {
       target_chain_id = target_chain_id_in;
       diffs = diffs_in;
     }
   };

   class ncs_matrix_info_t {
   public: 
     bool state; 
     clipper::RTop_orth rtop;
     std::vector<int> residue_matches;
     ncs_matrix_info_t() {
       state = 0;
     }
     ncs_matrix_info_t(bool state_in, clipper::RTop_orth rtop_in, 
		       std::vector<int> residue_matches_in) {
       state = state_in;
       rtop = rtop_in;
       residue_matches = residue_matches_in;
     }
   };

}
