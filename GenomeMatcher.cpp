#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Trie.h"
using namespace std;

class GenomeMatcherImpl
{
public:
	GenomeMatcherImpl(int minSearchLength);
	void addGenome(const Genome& genome);
	int minimumSearchLength() const;
	bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
	bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	int min_length; 
	
	vector <Genome> gene_container; 
	struct pos_gene
	{
		string name;
		int position;
	};
	Trie<pos_gene> m_geneTrie;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	min_length = minSearchLength; 
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	gene_container.push_back(genome);
	for (int i = 0; i <= genome.length()-min_length; i++)
	{
		string in1;
		int in2 = i;
		string in3 = genome.name();

		if (genome.extract(i, min_length, in1))
		{
			pos_gene a; 
			a.name = in3;
			a.position = in2;
			m_geneTrie.insert(in1,a);
		}
	}

}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return min_length;  // This compiles, but may not be correct
}


bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	
	string Fragment, Matches,exact_Fragment, out_name,match_sequence,exact_sequence;
	int mismatch=0,match_length =0,exact_length=0;
	vector<pos_gene> finder,exact_finder;
	bool exact = true;
	struct Match_cheker
	{
		string NAME;
		string sequence; 
		int length;
		int position; 
	};
	vector <Match_cheker> CHECKER; 
	Match_cheker longest;
	longest.length = 0; longest.position = 0; longest.NAME = "";
	
	if (fragment.size() < minimumLength)
		return false; 

	if (minimumLength < min_length)
		return false;

	for (int i = 0; i < min_length; i++)
		Fragment += fragment[i];
	finder = m_geneTrie.find(Fragment, exactMatchOnly);

	if (finder.empty())
	{
		return false;
	}
	if (exactMatchOnly == true)
	{
		for (int i = 0; i < finder.size(); i++)
		{
			for (int j = 0; j < gene_container.size(); j++)
			{
				if (finder[i].name == gene_container[j].name())
					exact = gene_container[j].extract(finder[i].position, fragment.size(), Matches);
			}
			if (exact == false)
				return false;

			for (int k = 0; k < minimumLength; k++)
			{
				if (Matches[k] == fragment[i])
					exact_length++;
				else 
				{
					//exact_length++;
					mismatch++;
				}
			}

			if (mismatch < 1)
			{
				for (int j = minimumLength; j < fragment.size(); j++)   //check the rest sequence
				{
					if (Matches[j] == fragment[j])
						exact_length++;
					else
						break;
				}
			}
			if (mismatch < 1)
			{
				Match_cheker ck;
				ck.NAME = finder[i].name;
				ck.sequence = "";
				ck.length = exact_length;
				ck.position = finder[i].position;
				CHECKER.push_back(ck);
			}
			exact_length = 0;  //reset them for the next for loop
			mismatch = 0;
			exact = true;
		}
	}

	if (exactMatchOnly == false)
	{
		for (int i = 0; i < finder.size(); i++)
		{
			for (int j = 0; j < gene_container.size(); j++)
			{
				if (finder[i].name == gene_container[j].name())
				{

					for (int k = 0; k < fragment.size(); k++)
					{
						if (gene_container[j].extract(finder[i].position + k, 1, Matches))
						{
							//cout << "fragment: " << fragment[k] << " matches: " << Matches << endl;
							if (fragment[k] == Matches[0] && exactMatchOnly == false)
							{
								match_sequence += Matches[0];
								match_length++;
							}

							if (fragment[k] != Matches[0] && exactMatchOnly == false)
							{
								match_sequence += Matches[0];
								mismatch++;
								match_length++;
							}
							/*
							if (fragment[k] == Matches[0] && exactMatchOnly == true)
							{
								exact_sequence += Matches[0];
								exact_length++;
								//cout << "exact length: " << exact_length << " name: " << finder[i].name << endl;
							}

							if ((fragment[k] != Matches[0] && exactMatchOnly == true) || (fragment.size() == minimumLength && exactMatchOnly == true))
							{

								if (exact_length >= minimumLength)
								{

									//cout << "pushing" << endl;
									//cout << "input length: " << exact_length << " name: " << finder[i].name << endl;
									Match_cheker cck;
									cck.NAME = finder[i].name;
									cck.sequence = exact_sequence;
									cck.length = exact_length;
									cck.position = finder[i].position;
									CHECKER.push_back(cck);
									break;
								}

							}*/


							if (mismatch == 2 || (mismatch < 2 && match_length == fragment.size()))
							{

								if (mismatch == 2)
									match_length--;

								if (match_length >= minimumLength)
								{
									Match_cheker ck;
									ck.NAME = finder[i].name;
									ck.sequence = match_sequence;
									ck.length = match_length;
									ck.position = finder[i].position;
									CHECKER.push_back(ck);

									match_sequence = "";
									match_length = 0;
									mismatch = 0;
								}

								if (match_length < minimumLength)
								{
									match_sequence = "";
									match_length = 0;
									mismatch = 0;
								}
								break;
							}
							Matches = "";
						}
					}
					Matches = "";
					//exact_sequence = "";
					//exact_length = 0;
				}
			}
		}
	}
	
	
	//////////////////////////output//////////////////////////////////////////
	CHECKER.push_back(longest);
	longest.NAME = CHECKER[0].NAME;
	for (int i = 0; i < CHECKER.size(); i++)
	{
		if (CHECKER[i].length > longest.length && CHECKER[i].NAME == longest.NAME)
		{
			longest = CHECKER[i];
		}

		if (CHECKER[i].length == longest.length && CHECKER[i].NAME == longest.NAME)
		{
			if (CHECKER[i].position < longest.position)
				longest = CHECKER[i];
		}

		if (CHECKER[i].NAME != longest.NAME)
		{
			DNAMatch ak;
			ak.genomeName = longest.NAME;
			ak.length = longest.length;
			ak.position = longest.position;
			matches.push_back(ak);

			longest.length = CHECKER[i].length;
			longest.position = CHECKER[i].position;
			longest.NAME = CHECKER[i].NAME;
		}
	}

	return true;
}


bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	if (fragmentMatchLength < min_length)
		return false;

	string relate;
	vector<DNAMatch> matcher;
	int counter = 0, g, S;
	for (int i = 0; i < fragmentMatchLength; i++)
	{
		query.extract(i*fragmentMatchLength, fragmentMatchLength, relate);

		if (findGenomesWithThisDNA(relate, fragmentMatchLength, exactMatchOnly, matcher))
		{
			for (int j = 0; j < matcher.size(); j++)
			{
				counter++;
				g = counter;
				S = query.length() / fragmentMatchLength;
				double p = ((g*1.00) / (S*1.00))*100.00;
				
				if (p >= matchPercentThreshold)
				{
					GenomeMatch genmatcher;
					genmatcher.genomeName = matcher[j].genomeName;
					genmatcher.percentMatch = p;
					results.push_back(genmatcher);
				}
			}
			relate = "";
		}
	}
	if (counter == 0)
		return false; 

	return true; 
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
	m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
	delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
	m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
	return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
} 

