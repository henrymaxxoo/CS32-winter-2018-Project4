#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include<fstream>
using namespace std;

class GenomeImpl
{
public:
	GenomeImpl(const string& nm, const string& sequence);
	static bool load(istream& genomeSource, vector<Genome>& genomes);
	int length() const;
	string name() const;
	bool extract(int position, int length, string& fragment) const;
private:
	string m_name; 
	string DNAsequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	m_name = nm; 
	DNAsequence = sequence; 

}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
	string input,name,dna,dna_upper;
	bool first_gene = true;
	while (getline(genomeSource, input))
	{
		if (input[0] == '>')
		{
			if (!first_gene) {
				for (int i = 0; i < dna.size(); i++)
				{
					if (dna[i] == 'a' || dna[i] == 'A')
						dna_upper += "A";
					if (dna[i] == 'c' || dna[i] == 'C')
						dna_upper += "C";
					if (dna[i] == 't' || dna[i] == 'T')
						dna_upper += "T";
					if (dna[i] == 'g' || dna[i] == 'G')
						dna_upper += "G";
					if (dna[i] == 'n' || dna[i] == 'N')
						dna_upper += "N";
				}
				Genome new_genome(name, dna_upper);
				genomes.push_back(new_genome);
			}
			else first_gene = false;

			name = input.substr(1);  //name
			dna = "";
			dna_upper = "";
		}
		else
		{
			dna += input;  //dna sequency
		}
	}

	for (int i = 0; i < dna.size(); i++)
	{
		if (dna[i] == 'a' || dna[i] == 'A')
			dna_upper += "A";
		if (dna[i] == 'c' || dna[i] == 'C')
			dna_upper += "C";
		if (dna[i] == 't' || dna[i] == 'T')
			dna_upper += "T";
		if (dna[i] == 'g' || dna[i] == 'G')
			dna_upper += "G";
		if (dna[i] == 'n' || dna[i] == 'N')
			dna_upper += "N";
		//if(dna[i]=='a')
	}
	Genome new_genome(name, dna_upper); //add the last name and dna sequency
	genomes.push_back(new_genome);

	return true;
		
}

int GenomeImpl::length() const
{
	return DNAsequence.size();  // This compiles, but may not be correct
}

string GenomeImpl::name() const
{
	return m_name;  // This compiles, but may not be correct
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (position > DNAsequence.size())
		return false; 
	if (position + length > DNAsequence.size())
		return false;
	for (int i = position; i < (position+length); i++)
	{
		fragment += DNAsequence[i];
	}
	return true; 
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
	m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
	delete m_impl;
}

Genome::Genome(const Genome& other)
{
	m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
	GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
	delete m_impl;
	m_impl = newImpl;
	return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
	return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
	return m_impl->length();
}

string Genome::name() const
{
	return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
	return m_impl->extract(position, length, fragment);
}
