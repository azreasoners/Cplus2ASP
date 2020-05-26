#include "constants.h"

class clingo3to4
{


	public:

		enum IncrementalScope
		{
			NONE,
			BASE,
			CUMULATIVE,
			VOLATILE
		};
		
		

		int convert(const int argc, const char *argv[]);
		

		
		
	private: 
		
		bool is_incremental;
		bool is_script;
		std::vector<std::string> clauses;
		IncrementalScope current_scope;
		std::string vol_var_string;

		void setup_clauses();
		int convert_file(const char* argv[],bool stdout,std::string filename);
		int convert_stdin(const char* argv[]);
		int match_normal_rule(std::string& output, const std::string& input);
		int match_counting_literal_rule(std::string& output, const std::string& input);
		int match_clause_rule(std::string& output, const std::string& input);
		int match_rule(std::string& output, const std::string& input);
		int match_volatile_rule(std::string& output, const std::string& input);
		std::string get_file_contents(const char *filename);
		std::string remove_abs(std::string& input);
		std::string remove_sum(std::string& input);
		
		void set_incremental(bool is_incremental);
		void set_current_scope(IncrementalScope scope);
		int match_cumulative_rule(std::string& output, const std::string& input);
		bool get_incremental();
		std::string get_volatile_query();
		void set_volatile_query(const std::string& str);
		IncrementalScope get_scope();

		bool match_lua_start_rule(std::string& output, const std::string& input);
		bool match_lua_end_rule(std::string& output, const std::string& input);
		bool match_python_start_rule(std::string& output, const std::string& input);
		bool match_python_end_rule(std::string& output, const std::string& input);
		void set_script(bool is_script);
		bool get_script();
};

