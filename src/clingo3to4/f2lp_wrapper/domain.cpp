#include "domain.h"

std::vector< std::pair<std::string,std::string> > domain::domain_list;

std::set<std::string> domain::search_domain_variables(const std::string& input)
{
	std::set<std::string> domains;
	
	// boost::regex expr("([A-Z]+)");
	boost::regex expr("([A-Z_]+)([A-Za-z0-9_]*)");

	boost::sregex_token_iterator iter(input.begin(), input.end(), expr, 0);
    boost::sregex_token_iterator end;

    for (; iter != end; ++iter)
    {
    	domains.insert(std::string(*iter));
   	}
	return domains;
}

int domain::isDomain(const std::string& input)
{
	// boost::regex expr("(#domain)([ ]+)([a-z]+)([(]){1}([A-Z_]+)([A-Za-z0-9_]*)([)]){1}");
	boost::regex expr("(#domain)([ ]+)([a-z_]){1}([A-Za-z0-9_]+)([(]){1}([A-Z_]+)([A-Za-z0-9_]*)([)]){1}");
	return regex_match(input,expr) ? 1 : 0 ;
}

std::pair<std::string,std::string> domain::get_domain_variables(const std::string& input)
{
	std::string::const_iterator start, end; 
	start = input.begin(); 
	end = input.end();
	boost::match_results<std::string::const_iterator> what; 
	// boost::regex expr("(#domain)([ ]+)([a-z]+)([(]){1}([A-Z_]+[A-Za-z0-9_]*)([)]){1}");
	boost::regex expr("(#domain)([ ]+)([a-z_]){1}([A-Za-z0-9_]+)([(]){1}([A-Z_]+)([A-Za-z0-9_]*)([)]){1}");
	boost::regex_match(start, end, what, expr);

	// std::string variable(what[what.size()-2]);
	// std::string identifier(what[what.size()-4]);

	std::string variable(what[what.size()-3]+what[what.size()-2]);
	std::string identifier(what[what.size()-6]+what[what.size()-5]);
	std::pair <std::string,std::string> domain (variable,identifier);

	return domain;
}


/*
Matches all clauses that statrt with #domain. 
*/
int domain::match_domain_rule(std::string& output, const std::string& input)
{
	//first check if the clause is a domain. If it is a domain clause, extract the identifier and variable from it and exit
	if(isDomain(input))
	{
		// The sentence is a domain. Extract Domain identfier and domain variable and put it in a list

		domain_list.push_back(get_domain_variables(input));
		// Comment out the domain as it is no longer used
		output.insert(0,COMMENT);
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
Method removes domain variables from clauses.
Call this method for every sentence.
All senstences need to be checked for domain variables
*/
void domain::remove_domain_variables(std::string& output,const std::string& input)
{

	//if the line is a comment or begins with # ignore
	if(input.compare(0,1,HASHTAG) == 0 || input.compare(0,1,COMMENT) == 0)
	{
		
		return;
	}
	else
	{
		std::set<std::string> domains = search_domain_variables(input);
		
		

		output = input ;
		if(!domains.empty())
		{
			/* Found a domain in this input.

			strategy 1:
			input types:
			1-> {loc(X,Y)}. 

				Now although X and Y can both be a global 
				variables, there is a global variable associated 
				with X but not with Y. So we only process for X
				and assume that Y is a local variable.				

			*/

			/*For each global variable detected from a string and 
			append it to the end of the current clause. Match the 
			identifier of the global variable from the domain_list vector.*/

			/*This is the generated string that needs to be appended to 
			original string */
			std::string generated_string;
			std::set<std::string>::iterator i;

			int is_domain_added = 0;

			for (i = domains.begin(); i != domains.end(); ++i)
			{
				for (int j = 0; j < domain_list.size(); ++j)
				{
					if ((*i).compare(domain_list.at(j).first) == 0)
					{
						generated_string.append(domain_list.at(j).second)
						.append(PAREN_OPEN)
						.append(domain_list.at(j).first)
						.append(PAREN_CLOSE)
						.append(COMMA);
						is_domain_added = 1;
					}
				}
			}

			/* The last clause will have a trailing comma, remove that*/
			ba::trim_right_if(generated_string,ba::is_any_of(COMMA));

			if(is_domain_added == 1)
			{
				//detect if a string has ':-' in it
				//If it doesn't then append a ':-' to the clause and
				//then append the generated string
				if(input.find(":-") == std::string::npos)
				{
					output.append(COLON_DASH)
					.append(generated_string);
					
				}
				else
				{
					output.append(COMMA)
					.append(generated_string);
					
				}
			}
		}
		else
		{
			return;
		}
	}
}