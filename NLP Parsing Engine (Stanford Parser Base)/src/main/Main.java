package main;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.sql.*;

import edu.stanford.nlp.ling.CoreLabel;
import edu.stanford.nlp.ling.HasWord;
import edu.stanford.nlp.ling.Sentence;
import edu.stanford.nlp.ling.TaggedWord;
import edu.stanford.nlp.parser.lexparser.LexicalizedParser;
import edu.stanford.nlp.process.CoreLabelTokenFactory;
import edu.stanford.nlp.process.PTBTokenizer;
import edu.stanford.nlp.tagger.maxent.MaxentTagger;
import edu.stanford.nlp.trees.GrammaticalStructure;
import edu.stanford.nlp.trees.PennTreebankLanguagePack;
import edu.stanford.nlp.trees.Tree;
import edu.stanford.nlp.trees.TreebankLanguagePack;
import edu.stanford.nlp.trees.TypedDependency;

public class Main {

	public static void main(String[] args) throws IOException {

		String input = "";

		LexicalizedParser lp = LexicalizedParser.loadModel("englishPCFG.caseless.ser.gz");
		TreebankLanguagePack tlp = new PennTreebankLanguagePack();
		MaxentTagger tagger = new MaxentTagger("english-left3words-distsim.tagger");
		edu.stanford.nlp.process.TokenizerFactory<CoreLabel> tf = PTBTokenizer.factory(new CoreLabelTokenFactory(), "");

		try{

			Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Jarvis","root","tremezone");
			Statement statement = connection.createStatement();

			while(true){
				ResultSet result = statement.executeQuery("SELECT * FROM `parse` WHERE `id`=1 AND `done`=0");
				while(result.next()){
					System.out.println("yo");
					if(result.getString(2) != null){
						input = result.getString(2);
						List<List<HasWord>> sentence2 = tagger.tokenizeText(new BufferedReader(new StringReader(input)));
						ArrayList<TaggedWord> tsentence = tagger.tagSentence(sentence2.get(0));
						String taggedSentence = Sentence.listToString(tsentence,false);
						String pos = taggedSentence;
						String taggedWords[] = taggedSentence.split(" ");

						ArrayList<CoreLabel> list = (ArrayList<CoreLabel>) tf.getTokenizer(new StringReader(input)).tokenize();

						Tree tree = lp.parse(list);

						GrammaticalStructure gs = tlp.grammaticalStructureFactory().newGrammaticalStructure(tree);
						Collection<TypedDependency> collection = gs.typedDependenciesCollapsed();
						ArrayList collectionsList = new ArrayList(collection);

						String parse_tree = tree.pennString();
						String dependency_string = "";

						for(Object dependency2 : collectionsList){
							dependency_string += "\n" + dependency2;
						}

						dependency_string = dependency_string.trim();

						//connection.createStatement().execute("UPDATE `parse` SET `pos`='" + pos + "',`parse_tree`='" + parse_tree + "',`dependency`='" + dependency_string + "' WHERE `id`=1");
						PreparedStatement p_statement = connection.prepareStatement("UPDATE `parse` SET `pos`=?,`parse_tree`=?,`dependency`=? WHERE `id`=1");
						p_statement.setString(1,pos);
						p_statement.setString(2,parse_tree);
						p_statement.setString(3,dependency_string);
						p_statement.execute();
						connection.createStatement().execute("UPDATE `parse` SET `done`=1,`done_2`=0 WHERE `id`=1");
					}

				}

				try{
					Thread.sleep(1000);
				}catch(Exception ex2){
					ex2.printStackTrace();
				}
			}
		}catch(SQLException ex){
			ex.printStackTrace();
		}
	}

}
