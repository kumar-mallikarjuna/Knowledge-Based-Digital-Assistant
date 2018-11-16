#ifndef INIT_H
#define INIT_H

#include "iostream"
#include "string"
#include "vector"
#include "boost/regex.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/regex.hpp"
#include "stdlib.h"

using namespace std;

class Input{
	public:
		string type,input,pos,parse_tree,dependency_string;
		vector<string> words;
		vector<string> dependency;
		vector<int> dependency_element_1,dependency_element_2;

		Input(){
		}

		Input(string *input_,string *pos_,string *parse_tree_,string *dependency_string_){
			input = *input_;
			boost::algorithm::trim(input);
			pos = *pos_;
			parse_tree = *parse_tree_;
			dependency_string = *dependency_string_;

			string parse_tree_no_spaces = boost::regex_replace(parse_tree,boost::regex("\\s+"),"");
			
			type = parse_tree_no_spaces.substr(6,parse_tree_no_spaces.find("(",6)-6);

			boost::algorithm::split_regex(words,boost::regex_replace(pos,boost::regex("/(.*?) ")," ").substr(0,boost::regex_replace(pos,boost::regex("/(.*?) ")," ").find_last_of("/")),boost::regex(" "));

			vector<string> dependency_string_split;

			boost::algorithm::split_regex(dependency_string_split,dependency_string,boost::regex("\n"));
			for(vector<string>::size_type i=0;i<dependency_string_split.size();i++){
				dependency.push_back(dependency_string_split.at(i).substr(0,dependency_string_split.at(i).find("(")));
				vector<string> dependency_segments;
				boost::algorithm::split_regex(dependency_segments,dependency_string_split.at(i).substr(dependency_string_split.at(i).find("(")+1).substr(0,dependency_string_split.at(i).substr(dependency_string_split.at(i).find("(")+1).length()-1),boost::regex(", "));

				dependency_element_1.push_back(atoi(dependency_segments.at(0).substr(dependency_segments.at(0).find_last_of("-")+1).c_str())-1);
				dependency_element_2.push_back(atoi(dependency_segments.at(1).substr(dependency_segments.at(1).find_last_of("-")+1).c_str())-1);
			}
		}
};

#endif
