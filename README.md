# HuffmanCompressor

# Compressed File Format
First byte represents the bits that appended for huffman codes

11(in binary): represents the end of the tree 
00: means go left
01: means go right
10: means the node is a leaf 

after the huffman codes comes characters that each leaf represents,
and after that comes 1 byte that reperesnt appended bits for compressed data ,
and after that byte the data that compressed using huffman codes comes. 
