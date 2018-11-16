#include "iostream"
#include "string"
#include "fstream"
#include "mysql++.h"
#include "unistd.h"
#include "Init.h"

using namespace std;
using namespace mysqlpp;

Input input;
vector<string> bin_programs;
vector<string> bin_programs_2;

Connection connection(false);

bool synonym_check(string word,string synonym){
	StoreQueryResult result = connection.query("SELECT * FROM `synonyms` WHERE `word`='" + word + "'").store();
	for(int i=0;i<result.num_rows();i++){
		string synonyms;
		vector<string> synonyms_split;

		result[i][2].to_string(synonyms);
		boost::algorithm::split_regex(synonyms_split,synonyms,boost::regex(","));
		if(find(synonyms_split.begin(),synonyms_split.end(),synonym) != synonyms_split.end()){
			return true;
		}
	}

	return false;
}

void update_bin_programs_vector(){

	string exceptions;

	ifstream read("/home/arjun/.system/bin_exceptions");
	read >> exceptions;

	FILE *command = popen("ls /usr/bin", "r");
	char buffer[1024];
	if (command) {
		bin_programs.clear();

		while ((fgets(buffer, sizeof(buffer), command))) {
			string program = boost::regex_replace(string(buffer), boost::regex("\n"), "");
			if(exceptions.find(program) == string::npos){
				boost::algorithm::to_lower(program);
				bin_programs.push_back(program);
				bin_programs_2.push_back(boost::regex_replace(program,boost::regex("[^0-9a-z]"),""));
			}
		}
	}

}

bool command(){
	StoreQueryResult result2 = connection.query("SELECT * FROM `synonyms` WHERE `synonyms` LIKE '%" + input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) + "%'").store();

	bool done = false;

	for(int i=0;i<result2.num_rows();i++){
		string word,synonyms,action;
		vector<string> synonyms_split;

		result2[i][1].to_string(word);
		result2[i][2].to_string(synonyms);
		result2[i][3].to_string(action);
		boost::algorithm::split_regex(synonyms_split,synonyms,boost::regex(","));
		
		if(find(synonyms_split.begin(),synonyms_split.end(),input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin()))) != synonyms_split.end()){
			try{
				cout << word << endl;
				if(action == ""){
					if(word == "increase"){
						if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "volume"){
							if(find(input.dependency.begin(),input.dependency.end(),"prep_of") != input.dependency.end()){
								if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"prep_of")-input.dependency.begin())) == "volume" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"prep_of")-input.dependency.begin())) == "vlc"){
									system(string("/bin/bash -c 'exec 3<>/dev/tcp/localhost/2510;echo volup >&3' &").c_str());
									done = true;
									break;
								}
							}

							if(find(input.dependency.begin(),input.dependency.end(),"nn") != input.dependency.end()){
								if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"nn")-input.dependency.begin())) == "volume" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"nn")-input.dependency.begin())) == "vlc"){
									system(string("/bin/bash -c 'exec 3<>/dev/tcp/localhost/2510;echo volup >&3' &").c_str());
									done = true;
									break;
								}
							}
						}
					}

					if(word == "reduce"){
						if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "volume"){
							if(find(input.dependency.begin(),input.dependency.end(),"prep_of") != input.dependency.end()){
								if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"prep_of")-input.dependency.begin())) == "volume" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"prep_of")-input.dependency.begin())) == "vlc"){
									system(string("/bin/bash -c 'exec 3<>/dev/tcp/localhost/2510;echo voldown >&3' &").c_str());
									done = true;
									break;
								}
							}

							if(find(input.dependency.begin(),input.dependency.end(),"nn") != input.dependency.end()){
								if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"nn")-input.dependency.begin())) == "volume" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"nn")-input.dependency.begin())) == "vlc"){
									system(string("/bin/bash -c 'exec 3<>/dev/tcp/localhost/2510;echo voldown >&3' &").c_str());
									done = true;
									break;
								}
							}
						}
					}

					if(word == "show"){
						if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "list"){
							if(find(input.dependency.begin(),input.dependency.end(),"nn") != input.dependency.end()){
								if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"nn")-input.dependency.begin())) == "list" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"nn")-input.dependency.begin())) == "music"){
									system(string("/bin/bash -c '/home/arjun/.system/music_list.sh' &").c_str());
									done = true;
									break;
								}
							}
						}

						system(("wmctrl -a " + input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) + " &").c_str());
						done = true;
					}

					if(word == "play"){
						if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "number"){
							if(find(input.dependency.begin(),input.dependency.end(),"num") != input.dependency.end()){
								for(vector<string>::size_type i=0;i<input.dependency.size();i++){
									if(input.dependency.at(i) == "num"){
										if(input.words.at(input.dependency_element_1.at(i)) == "number"){
											cout << input.words.at(input.dependency_element_2.at(i)) << endl;
											system(string("/home/arjun/.system/play_track.sh 'play track " + input.words.at(input.dependency_element_2.at(i)) + "' &").c_str());
											done = true;
											break;
										}
									}
								}
								break;
							}
						}

						short checkpoint = 0;

						for(vector<string>::size_type i=0;i<input.dependency.size();i++){
							if(input.dependency.at(i) == "nn"){
								if(input.words.at(input.dependency_element_1.at(i)) == input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin()))){
									if(input.words.at(input.dependency_element_2.at(i)) == "track" || input.words.at(input.dependency_element_2.at(i)) == "number"){
										checkpoint++;
									}
								}
							}
						}

						if(checkpoint == 2){
							system(string("/home/arjun/.system/play_track.sh 'play track " + input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) + "' &").c_str());
							done = true;
							break;
						}
					}

					if(word == "tell"){
						if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "time"){
							system(string("/home/arjun/.system/robotic_espeak $(date +'%I:%M %p'). &").c_str());
							done = true;
							break;
						}

						if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "date"){
							system(string("/home/arjun/.system/robotic_espeak $(date +'%e %B %Y'). &").c_str());
							done = true;
						}

						if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "definition"){
							for(vector<string>::size_type i=0;i<input.dependency.size();i++){
								if(input.dependency.at(i) == "prep_of"){
									if(input.words.at(input.dependency_element_1.at(i)) == "definition"){
										system(("/bin/bash -c '/home/arjun/CPP/popup/popup \"Definition of " + input.words.at(input.dependency_element_2.at(i)) + "\" \"$(wn " + input.words.at(input.dependency_element_2.at(i)) + " -over)\"' &").c_str());
										done = true;
									}
								}
							}
						}
					}

					//check for e-mails

					if(word == "check"){
						if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "e-mail" || input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "e-mails" || input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "email" || input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())) == "emails"){
							system(("firefox -new-tab http://www.gmail.com &"));
							done = true;
						}
					}
				}else{
					if(word == "open"){
						for(vector<string>::size_type i=0;i<input.dependency.size();i++){
							if(input.dependency.at(i) == "nn"){
								if(input.dependency_element_1.at(i) == input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())){
									if(input.words.at(input.dependency_element_2.at(i)) == "website"){
										system(("firefox -new-tab " + input.words.at(input.dependency_element_1.at(i)) + "&").c_str());
										done = true;
										break;
									}
								}
							}
						}
					}
					cout << (action + bin_programs.at(find(bin_programs_2.begin(),bin_programs_2.end(),boost::regex_replace(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())),boost::regex("[^0-9a-z]"),""))-bin_programs_2.begin()) + " &") << endl;
					system((action + bin_programs.at(find(bin_programs_2.begin(),bin_programs_2.end(),boost::regex_replace(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"dobj")-input.dependency.begin())),boost::regex("[^0-9a-z]"),""))-bin_programs_2.begin()) + " &").c_str());
					done = true;
					break;
				}
			}catch(exception &ex){
				cerr << "In 'void command()': " << ex.what() << endl;
			}
		}
	}

	return done;
}

bool question(){
	bool done = false;

	if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"nsubj")-input.dependency.begin())) == "what" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"nsubj")-input.dependency.begin())) == "time"){
		system(string("/home/arjun/.system/robotic_espeak $(date +'%I:%M %p'). &").c_str());
		done = true;
	}

	if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"nsubj")-input.dependency.begin())) == "what" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"nsubj")-input.dependency.begin())) == "date"){
		system(string("/home/arjun/.system/robotic_espeak $(date +'%e %B %Y'). &").c_str());
		done = true;
	}

	if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"nsubj")-input.dependency.begin())) == "what" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"nsubj")-input.dependency.begin())) == "today"){
		if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"nn")-input.dependency.begin())) == "today" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"nn")-input.dependency.begin())) == "date"){
			system(string("/home/arjun/.system/robotic_espeak $(date +'%e %B %Y'). &").c_str());
			done = true;
		}
	}

	if(input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"nsubj")-input.dependency.begin())) == "what" && input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"nsubj")-input.dependency.begin())) == "definition"){
		for(vector<string>::size_type i=0;i<input.dependency.size();i++){
			if(input.dependency.at(i) == "prep_of"){
				if(input.words.at(input.dependency_element_1.at(i)) == "definition"){
					system(("/bin/bash -c '/home/arjun/CPP/popup/popup \"Definition of " + input.words.at(input.dependency_element_2.at(i)) + "\" \"$(wn " + input.words.at(input.dependency_element_2.at(i)) + " -over)\"' &").c_str());
					done = true;
				}
			}
		}
	}

	return done;
}

int main(int argc,char *argv[]){

	cout << "Start the parser, you must." << endl;

	connection.connect("<>","localhost","root","<>");

	update_bin_programs_vector();

	while(1){
		StoreQueryResult result_input = connection.query("SELECT * FROM `input` WHERE `id`=1").store();
		string result_input_output;
		result_input[0][1].to_string(result_input_output);

		if(result_input_output != ""){
			string input_no_spaces = boost::regex_replace(result_input_output,boost::regex("\\s"),"");
			int matches = 0;
			vector<string> input_split,matches_names;
			vector<int> matches_sizes;
			boost::algorithm::split_regex(input_split,result_input_output,boost::regex("\\s"));

			bool do_ = true;

			//			for(vector<string>::size_type i=0;i<input_split.size();i++){
			//				if(find(bin_programs_2.begin(),bin_programs_2.end(),input_split.at(i)) != bin_programs_2.end()){
			//					do_ = false;
			//					break;
			//				}
			//			}

			if(do_){
				if(input_split.size() > 1){
					for(vector<string>::size_type i=0;i<bin_programs_2.size();i++){
						if(input_no_spaces.find(bin_programs_2.at(i)) != string::npos){
							matches_names.push_back(bin_programs_2.at(i));
							matches_sizes.push_back(bin_programs_2.at(i).length());
							matches++;
						}
					}

					if(matches > 0){
						int max_size = *max_element(matches_sizes.begin(),matches_sizes.end());
						string program_name = matches_names.at(find(matches_sizes.begin(),matches_sizes.end(),max_size)-matches_sizes.begin());
						vector<vector<string>::size_type> elements_position;
						for(vector<string>::size_type i=0;i<input_split.size();i++){
							if(program_name.find(input_split.at(i)) != string::npos){
								elements_position.push_back(i);
							}
						}

						for(vector<vector<string>::size_type>::size_type i=0;i<elements_position.size();i++){
							input_split.at(elements_position.at(i)) = "";
						}

						try{
							input_split.at(elements_position.at(elements_position.size()-1)) = bin_programs.at(find(bin_programs_2.begin(),bin_programs_2.end(),program_name)-bin_programs_2.begin());
						}catch(exception &ex){
							cerr << "In 'int main(int argc,char *argv[])': " << ex.what() << endl;
						}

						result_input_output = "";

						for(string word : input_split){
							if(word != "")
								result_input_output += word + " ";
						}

						boost::algorithm::trim(result_input_output);
					}
				}
			}

			cout << result_input_output << endl;

			bool done = false;

			if(input_split.size() == 1){
				StoreQueryResult result_synonyms = connection.query("SELECT * FROM `synonyms` WHERE `synonyms` LIKE '%" + result_input_output + "%'").store();
				for(int i=0;i<result_synonyms.num_rows();i++){
					string word,synonyms,action;
					vector<string> synonyms_split;

					result_synonyms[i][1].to_string(word);
					result_synonyms[i][2].to_string(synonyms);
					result_synonyms[i][3].to_string(action);
					boost::algorithm::split_regex(synonyms_split,synonyms,boost::regex(","));

					if(find(synonyms_split.begin(),synonyms_split.end(),result_input_output) != synonyms_split.end()){
						if(word == "play"){
							system(string("qdbus org.mpris.MediaPlayer2.vlc /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Play &").c_str());
							done = true;
						}

						if(word == "pause"){
							system(string("qdbus org.mpris.MediaPlayer2.vlc /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Pause &").c_str());
							done = true;
						}
					}
				}
			}

			connection.query("UPDATE `parse` SET `input`='" + result_input_output + ".',`done`=0,`done_2`=0").execute();
			while(1){
				StoreQueryResult result = connection.query("SELECT * FROM `parse` WHERE `id`=1 AND `done`=1 AND `done_2`=0").store();
				string input_string,pos,parse_tree,dependency_string;
				if(result.num_rows() > 0){
					result[0][1].to_string(input_string);
					result[0][2].to_string(pos);
					result[0][3].to_string(parse_tree);
					result[0][4].to_string(dependency_string);

					input = Input(&input_string,&pos,&parse_tree,&dependency_string);

					if(input.type == "S"){
						if(find(input.dependency.begin(),input.dependency.end(),"dobj") != input.dependency.end()){
							done = command();
						}else{
							if(find(input.dependency.begin(),input.dependency.end(),"acomp") != input.dependency.end()){
								if(synonym_check("look",input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"acomp")-input.dependency.begin())))){
									if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"acomp")-input.dependency.begin())) == "alive"){
										system("xset -display :0.0 dpms force on");
										cout << "Looking alive." << endl;
										done = true;
									}
								}
							}

							if(find(input.dependency.begin(),input.dependency.end(),"prep_for") != input.dependency.end()){
								if(synonym_check("check",input.words.at(input.dependency_element_1.at(find(input.dependency.begin(),input.dependency.end(),"prep_for")-input.dependency.begin())))){
									if(input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"prep_for")-input.dependency.begin())) == "e-mail" || input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"prep_for")-input.dependency.begin())) == "e-mails" || input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"prep_for")-input.dependency.begin())) == "email" || input.words.at(input.dependency_element_2.at(find(input.dependency.begin(),input.dependency.end(),"prep_for")-input.dependency.begin())) == "emails"){
										system(("firefox -new-tab http://www.gmail.com &"));
										done = true;
									}
								}
							}
						}
					}
					if(input.type == "SQ"){
						string first_word = input.words.at(0);
						boost::to_lower(first_word);
						if(first_word == "can" || first_word == "would" || first_word == "will"){
							done = command();
						}
					}
					if(input.type == "SBARQ"){
						if(find(input.dependency.begin(),input.dependency.end(),"nsubj") != input.dependency.end()){
							done = question();
						}
					}

					connection.query("UPDATE `parse` SET `done_2`=1 WHERE `id`=1").execute();
					connection.query("UPDATE `input` SET `input`='',`done`='" + to_string(int(done)) + "' WHERE `id`=1").execute();

					break;
				}

				usleep(100);
			}
		}

		usleep(1000);
	}

	return 0;
}
