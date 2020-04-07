#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
	Trie();
	~Trie();
	void reset();
	void insert(const std::string& key, const ValueType& value);
	std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

	// C++11 syntax for preventing copying and assignment
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;

private:
	int size;
	struct node
	{
	  char key;
	  std::vector<ValueType> val;
	  std::vector<node*> children; 
	};
	node* root;

	void clear(node* cur);
	void insert_helper(const std::string& key, const ValueType& value, node* cur);
	std::vector<ValueType> find_exact(std::string key, node* cur) const;
	std::vector<ValueType> find_Nonexact(std::string key, node* cur) const;
};

#endif // TRIE_INCLUDED

template<typename ValueType>
Trie<ValueType>::Trie()
{
	root = new node; 
	size = 0; 
}

template<typename ValueType>
Trie<ValueType>::~Trie()
{
	clear(root);
}

template<typename ValueType>
void Trie<ValueType>::clear(node* cur)
{
	if (cur == nullptr)
		return; 
	for (int i = 0; i < cur->children.size(); i++)
		clear(cur->children[i]);

	delete cur; 
	size--;
}

template<typename ValueType>
void Trie<ValueType>::reset()
{
	clear(root);
	root = new node;
	size = 0;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
	insert_helper(key, value, root);
}

template<typename ValueType>
void Trie<ValueType>::insert_helper(const std::string& key, const ValueType& value, node* cur)
{
	std::string KEY = key; 
	if (KEY.size() == 0) // base case, finish all the input key
	{
		cur->val.push_back(value);
		return;
	}

	if (cur->children.size() == 0) //if not leafe in the tree, input all the key into leafe
	{
		node* a = new node; 
		a->key = KEY[0]; 
		cur->children.push_back(a); 
		size++;
		KEY.erase(0, 1);
		return insert_helper(KEY, value, a);
	}

	else
	{
		for (int i = 0; i < cur->children.size(); i++)
		{
			if (KEY[0] == cur->children[i]->key) //find the same key in the child
			{
				KEY.erase(0, 1);
				return insert_helper(KEY, value, cur->children[i]);	 //go to next child
			}

			else  //not found the child insert the rest of the input string 
			{
				node* b = new node;
				b->key = KEY[0];
				cur->children.push_back(b);
				KEY.erase(0, 1);
				size++;
				return insert_helper(KEY, value, b);
			}
		}
	}
}


template<typename ValueType>
typename std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
	std::vector<ValueType> out; 
	
	if (exactMatchOnly == true)
	{
		out = find_exact(key, root);
	}

	if (exactMatchOnly == false)
	{
		for (int i = 0; i < root->children.size(); i++) // don't need to recursion everytime
		{
			if (key[0] == root->children[i]->key)
			{
				out = find_Nonexact(key, root);
			}
		}
	}

	return out; 
}

template<typename ValueType>
typename std::vector<ValueType>  Trie<ValueType>::find_exact(std::string key, node* cur) const
{
	if (key.size() == 0)
	{
		return cur->val; 
	}

	std::vector<ValueType> out;
	for (int i = 0; i < cur->children.size(); i++)
	{
		if (key[0] == cur->children[i]->key)
		{
			std::vector<ValueType> S = find_exact(key.substr(1), cur->children[i]); //find the same key, go to next child 
			out.insert(out.begin(), S.begin(), S.end());
		}
	}

	return out;
}

template<typename ValueType>
typename std::vector<ValueType>  Trie<ValueType>::find_Nonexact(std::string key, node* cur) const
{
	if (key.size() == 0)
	{
		return cur->val;
	}
	
	std::vector<ValueType> out;
	for (int i = 0; i < cur->children.size(); i++)
	{
		if (key[0] == cur->children[i]->key)
		{
			std::vector<ValueType> S = find_Nonexact(key.substr(1), cur->children[i]);
			 out.insert(out.begin(),S.begin(),S.end());
		}

		if (key[0] != cur->children[i]->key)
		{
			std::vector<ValueType> K = find_exact(key.substr(1), cur->children[i]);
			out.insert(out.begin(), K.begin(), K.end());
		}
	}

	return out; 
}
