#include <iostream>
#include <vector>
#include <fstream>

const unsigned char LEFT = 0;
const unsigned char RIGHT = 1;
const unsigned char LEAF = 2;
const unsigned int END = 3;

unsigned int getSize(const std::string&);
void LOG(const char* msg) {
	std::cout << msg << std::endl;
}
class Node {
public:
	static int counter;
	Node* rchild;
	Node* lchild;
	char val;
	Node() : rchild(NULL), lchild(NULL), val(0) {};
	Node(Node& node) : rchild(node.rchild), lchild(node.lchild), val(node.val) {};
	Node(char value) :rchild(NULL), lchild(NULL), val(value) {
	};
	~Node();
};
int Node::counter = 0;
Node::~Node() {
	if (rchild != NULL)
	{
		counter++;
		delete rchild;
		rchild = NULL;
	}
	if (lchild != NULL)
	{
		counter++;
		delete lchild;
		lchild = NULL;
	}
}
void reverseHuffmanTreeBuilder(Node*, const char* , std::ifstream& , unsigned int);

class BinarySort {
public:
	BinarySort* left;
	BinarySort* right;
	Node* tree;
	float value;

	BinarySort(float value, char c) : left(NULL), right(NULL), value(value) , tree(new Node(c)) {
		//tree = new Huffman(c);
	};
	BinarySort(float freq,Node* bs) : left(NULL), right(NULL), value(freq) , tree(bs) {};
	BinarySort(BinarySort& bs) : left(bs.left), right(bs.right), tree(bs.tree), value(bs.value) {};
	~BinarySort() {
		if (tree != NULL)
		{
			delete tree;
			tree = NULL;
		}
		if (left == NULL && right == NULL)
			return;
		else
		{
			if (left != NULL)
			{
				delete left;
				left = NULL;
			}
			if (right != NULL)
			{
				delete right;
				right = NULL;
			}
		}
	};
	void Add(float freq, char cr);
	void Add(float freq, Node* ptree);
	BinarySort* popSmallest();
	void BuildHuffmanTree();
};
BinarySort* BinarySort::popSmallest() {
	BinarySort* temp = this;
	if (temp->left != NULL) {
		while (temp->left->left != NULL) {
			temp = temp->left;
		}
		BinarySort* temp2 = temp->left;
		if (temp->left->right == NULL)
		{
			temp->left = NULL;
			return temp2;
		}
		if (temp2->right->left != NULL) {
			BinarySort* temp3 = (*temp2).right;
			while (temp3->left->left != NULL) {
				temp3 = temp3->left;
			}
			BinarySort* replacer = temp3->left;
			temp3->left = replacer->right;
			replacer->right = temp2->right;
			temp2->right = NULL;
			temp->left = replacer;

			return temp2;
		}
		else {
			temp->left = temp2->right;
			temp2->right = NULL;
			return temp2;
		}
		

	}
	else {
		if (temp->right == NULL)
			return temp;
		BinarySort* r = new BinarySort(this->value,this->tree);
		if (temp->right->left != NULL) {
			BinarySort* temp2 = temp->right;

			while (temp2->left->left != NULL) {
				temp2 = temp2->left;
			}
			this->value = temp2->left->value;
			this->tree = temp2->left->tree;

			temp2->left = temp2->left->right;
		}
		else {
			this->tree = this->right->tree;
			this->value = this->right->value;
			this->left = this->right->left;
			this->right = this->right->right;

			

		}
		
		
		r->left = NULL;
		r->right = NULL;

		return r;

	}
	
}
void BinarySort::Add(float freq,Node* ptree) {
	BinarySort* temp = this;
	while (1) {
		if (freq < temp->value) {
			if (temp->left == NULL) {
				temp->left = new BinarySort(freq, ptree);
				return;
			}
			temp = temp->left;
		}
		else {
			if (temp->right == NULL) {
				temp->right = new BinarySort(freq, ptree);
				return;
			}
			temp = temp->right;
		}
	}
}
void BinarySort::Add(float freq, char cr) {
	BinarySort* temp = this;
	while (1) {
		if (freq < temp->value) {
			if (temp->left == NULL) {
				temp->left = new BinarySort(freq, cr);
				return;
			}
			temp = temp->left;
		}
		else {
			if (temp->right == NULL) {
				temp->right = new BinarySort(freq, cr);
				return;
			}
			temp = temp->right;
		}
	}
	
}
void BinarySort::BuildHuffmanTree() {
	while (this->left != NULL || this->right != NULL) {
		BinarySort* t1 = this->popSmallest();
		BinarySort* t2 = this->popSmallest();
		 if (t2 == this) {
			Node* merge = new Node();
			merge->lchild = this->tree;
			merge->rchild = t1->tree;
			this->tree = merge;
			this->value += t1->value;
			t1->tree = NULL;
			delete t1;
			return;
		}
		Node* merge = new Node();
		merge->lchild = t2->tree;
		merge->rchild = t1->tree;
		this->Add(t2->value+t1->value,merge);
		t1->tree = NULL;
		t2->tree = NULL;
		delete t1;
		delete t2;
	}
	
	
}

std::string TraverseTree2(Node* root, std::string c = "") {
	std::string temp = "";
	if (root->lchild != NULL) {
		temp += TraverseTree2(root->lchild, c + "0");
	}

	if (root->rchild != NULL) {
		if (c == "") {
			temp += TraverseTree2(root->rchild, c + "1");
		}
		else {
			temp += TraverseTree2(root->rchild, "1");
		}
	}

	if (root->lchild == NULL && root->rchild == NULL) {
		return c + "\n" + root->val;
	}
	return temp;
}
std::string TraverseTree(Node* root, std::string c = "") {
	std::string temp = "";
	if (root->lchild != NULL) {
		 temp += TraverseTree(root->lchild,c + "0" );
	}

	if (root->rchild != NULL) {
		temp += TraverseTree(root->rchild, c + "1");
	}

	if (root->lchild == NULL && root->rchild == NULL) {
		return  temp + c + "\n" + root->val;
	}
	return temp;
}


std::string Transform(std::string codeTree) {
	std::string result = "";
	std::string c = "";
	int bitCounter = 0;
	unsigned char k = 0;
	for (unsigned int i = 0; i < codeTree.length(); i++)
	{
		if (codeTree[i] == '0') {
			k = (k << 2) | LEFT;
			bitCounter += 2;
		}
		else if(codeTree[i] == '1'){
			k = (k << 2) | RIGHT;
			bitCounter += 2;
		}
		else {

			k = (k << 2) | LEAF;
			bitCounter += 2;
			i++;
			c += codeTree[i];
		}

		if (bitCounter == 8) {
			result += k;
			k = 0;
			bitCounter = 0;
		}

	}
	unsigned char emptPlace = 8 - bitCounter;
	result += (k << emptPlace) | (END << (emptPlace - 2));
	result = (char)(emptPlace - 2) + result + c;
	return result;
}
void compress(std::string& path) {
	unsigned int fileSize = 0;
	char *data = new char[1024 * 1024];
	std::string items = "";
	std::string serialized= "";
	std::string result = "";
	std::vector< int > frequency;
	unsigned int pointer = 0;

	std::ifstream myReadFile(path,std::fstream::binary);
	myReadFile.clear();

	if (myReadFile.is_open()) {
		std::streamsize bytes = myReadFile.rdbuf()->sgetn(data, 1024 * 1024);

		while (bytes > 0) {
			pointer = 0;
			fileSize += (unsigned int)bytes;
			for (size_t i = 0; i < bytes; i++)
			{
				pointer = items.find(data[i]);

				if (pointer == std::string::npos) {

					items += data[i];
					frequency.push_back(1);
				}
				else {
					frequency[pointer]++;
				}
			}
			if (myReadFile.good()) {
				myReadFile.clear();
			}
			bytes = myReadFile.rdbuf()->sgetn(data, 1024 * 1024);

		}
		myReadFile.close();

		BinarySort tree((float)frequency[0], items[0]);
		for (unsigned int i = 1; i < frequency.size(); i++)
		{
			tree.Add((float)frequency[i], items[i]);
		}

		tree.BuildHuffmanTree();
		LOG("Tree builded");

		serialized = TraverseTree2(tree.tree);
		result = Transform(serialized);

		serialized = TraverseTree(tree.tree);

		std::vector<std::string> codes;
		std::string temp = "";
		std::string items2 = "";
		unsigned int counter = 0;
		for (size_t i = 0; i < serialized.size(); i++)
		{
			if (serialized[i] == '0') {
				temp += '0';
			}
			else if (serialized[i] == '1') {
				temp += '1';
			}
			else {

				items2 += serialized[i + 1];
				i++;
				codes.push_back(temp);
				temp = "";
			}
		}

		unsigned int newSize = 0;
		for (size_t i = 0; i < items.size(); i++)
		{
			newSize += frequency[i] * (codes[items2.find(items[i])].size());
		}
		unsigned char padding = 8 - (newSize % 8);
		result += padding == 8 ? '\0' : padding;
		std::cout << "Write operation started" << std::endl;
		std::ofstream outfile;
		
		/*compressed file*/
		outfile.open("file.dat", std::ios::out | std::ios::ate | std::ios::binary);
		
		/*file header contains the huffman tree*/
		outfile.write(result.c_str(), result.size());
		
		result.clear();
		myReadFile.open(path, std::fstream::binary);
		myReadFile.clear();
		memset(data, 0, 1024 * 1024 * (sizeof data[0]));
		
		unsigned char k = 0;
		bytes = myReadFile.rdbuf()->sgetn(data, 1024 * 1024);
		if (myReadFile.is_open()) {
			while (bytes > 0) {
				pointer = 0;
				for (size_t i = 0; i < bytes; i++)
				{
					pointer = items2.find(data[i]);
					for (size_t j = 0; j < codes[pointer].size(); j++)
					{
						if (codes[pointer][j] == '0')
						{
							k <<= 1;
							counter++;
						}
						else {
							k = (k << 1) | 1;
							counter++;
						}

						if (counter == 8) {
							result += k;
							counter = 0;
							k = 0;
						}
					}

				}
				outfile.write(result.c_str(), result.size());
				result.clear();
				bytes = myReadFile.rdbuf()->sgetn(data, 1024 * 1024);				
			}
			if (counter != 0) {
				k <<= (8 - counter);
				temp.clear();
				temp = "";
				temp += k;
				outfile.write(temp.c_str(), 1);
			}
			
			outfile.close();
			myReadFile.close();

			LOG("Write operation done.");
		}
		else {
			LOG("Unable to open the file");
		}
		memset(data, 0, 1024 * 1024 * (sizeof data[0]));
		delete[] data;

	}

}


void decompress(const char* path) {
	std::ifstream myReadFile(path, std::fstream::binary);
	myReadFile.clear();
	char* byte = new char;
	int counter = 0;
	if (myReadFile.is_open()) {
		// in binary 11 = 1011 -> 10 means leaf , 11 means end of tree
		std::streamsize byteCount = myReadFile.rdbuf()->sgetn(byte, 1);
		std::string tree = "";
		unsigned char ENDOfTree = 11 << (unsigned char)*byte; // first byte is padding of data of tree
		byteCount = myReadFile.rdbuf()->sgetn(byte, 1);
		while ((ENDOfTree & (unsigned char)*byte) != ENDOfTree) {
			tree += *byte;
			byteCount = myReadFile.rdbuf()->sgetn(byte, 1);
			if (byteCount == 0) {
				LOG("Error: Failed to read.");
			}
		}
		tree += *byte;

		/// Building Tree //////

		Node* head = new Node();
		
		const char* c_tree = tree.c_str();
		reverseHuffmanTreeBuilder(head,c_tree , myReadFile,(unsigned int)tree.size());
		std::string serialization = TraverseTree(head);

		// padding of actual data
		myReadFile.rdbuf()->sgetn(byte, 1);
		unsigned char dataPadding = *byte;

		char* data = new char[1024*1024];
		unsigned char k = 0x80;
		unsigned int z = 0;
		std::ofstream outfile;
		outfile.open("file_uncompressed.dat", std::ios::out | std::ios::ate | std::ios::binary);

		byteCount = myReadFile.rdbuf()->sgetn(data, 1024 * 1024);

		Node* temp = head;
		std::string result = "";
		while (byteCount != 0) {
			result = "";
			for (size_t i = 0; i < byteCount - 1; i++)
			{
				k = 0x80;
				for (unsigned char j = 0; j < 8; j++)
				{
					if ((k & data[i]) != 0) {
						temp = temp->rchild;
						if (temp->rchild == NULL && temp->lchild == NULL) {
							result += temp->val;
							temp = head;
						}
					}
					else
					{
						temp = temp->lchild;
						if (temp->rchild == NULL && temp->lchild == NULL) {
							result += temp->val;
							temp = head;
						}
					}
					k = k >> 1;

				}
			}
			if (byteCount < 1024 * 1024) {
				k = 0x80;
				for (unsigned char j = 0; j < (8 - dataPadding); j++)
				{
					if ((k & data[byteCount - 1]) != 0) {
						temp = temp->rchild;
						if (temp->rchild == NULL && temp->lchild == NULL) {
							z++;
							result += temp->val;
							temp = head;
						}
					}
					else
					{
						temp = temp->lchild;
						if (temp->rchild == NULL && temp->lchild == NULL) {
							z++;
							result += temp->val;
							temp = head;
						}
					}
					k = k >> 1;

				}

			}
			else {
				k = 0x80;
				for (unsigned char j = 0; j < 8; j++)
				{
					if ((k & data[byteCount - 1]) != 0) {
						temp = temp->rchild;
						if (temp->rchild == NULL && temp->lchild == NULL) {
							result += temp->val;
							temp = head;
						}
					}
					else
					{
						temp = temp->lchild;
						if (temp->rchild == NULL && temp->lchild == NULL) {
							result += temp->val;
							temp = head;
						}
					}
					k = k >> 1;

				}
			}
			outfile.write(result.c_str(), result.size());
			result.clear();
			byteCount = myReadFile.rdbuf()->sgetn(data, 1024 * 1024);
		}
		memset(data, 0, 1024 * 1024 * (sizeof data[0]));
		outfile.close();
		myReadFile.close();
		delete[] data;
	}
	else {
		std::cout << "Unable to open the file." << std::endl;
	}
	delete byte;
	

}

void reverseHuffmanTreeBuilder(Node* root , const char * tree , std::ifstream& ifs, unsigned int Size = 0) {
	static unsigned int index = 0;
	static unsigned int size = Size;
	static unsigned char/* Double bit*/ dbNumber = 0;
	static unsigned int readCount = 0;
	unsigned char temp = 0;
	
	if (dbNumber == 4) {
		dbNumber = 0;
		index++;
	}
	temp = ((0xC0 >> dbNumber * 2) & (unsigned char)tree[index]) >> (3 - dbNumber) * 2;
	dbNumber++;


	if (temp == LEFT) {
		root->lchild = new Node();
		reverseHuffmanTreeBuilder(root->lchild, tree, ifs);
		
		if (dbNumber == 4) {
			dbNumber = 0;
			index++;
		}
		temp = ((0xC0 >> dbNumber * 2) & (unsigned char)tree[index]) >> (3 - dbNumber) * 2;
		dbNumber++;
	}

	if (temp == RIGHT)
	{
		root->rchild = new Node();
		reverseHuffmanTreeBuilder(root->rchild, tree, ifs);
	}
	else if (temp == LEAF) {
		char* leaf = new char;
		ifs.rdbuf()->sgetn(leaf, 1);
		readCount++;
		root->val = *leaf;
		delete leaf;
	}
	else {
		index = size;
	}
	

}

unsigned int getSize(const std::string& address) {
	std::ifstream fstrm(address, std::ios_base::in | std::ios_base::binary);
	fstrm.seekg(0, std::ios_base::end);
	std::streamoff filesize = fstrm.tellg();
	return (unsigned int)filesize;
}
int main(int argc , char* argv[])
{

	std::cout <<"Compressing: " <<argv[1] << std::endl;
	std::string path = argv[1];


	compress(path);
	LOG("Decompression started");
	decompress("file.dat");
	LOG("Decompression ended");
	std::cin.get();
}

