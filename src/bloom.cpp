#include <iostream>
#include "../include/bloom.hpp"
#include "../include/hash.hpp"
using namespace std;

BloomFilter::BloomFilter(unsigned long bytes)
{
    int length_int = bytes / 4; 
    bit_array = new int[length_int];
    length_bit = bytes * 8;
    for (int i = 0; i < length_int; i++)
    {
        bit_array[i] = 0;
    }
}

BloomFilter::~BloomFilter()
{
    delete[] bit_array;
}

void BloomFilter::set_bit(int k) // Find the position of the bit and set it to 1
{
    (bit_array[(k / 32)] = bit_array[(k / 32)] | (1 << (k % 32)));
}

int BloomFilter::test_bit(int k)
{
    return (bit_array[(k / 32)] & (1 << (k % 32)));
}

void BloomFilter::add(const char *id) // Add given id to the bloom filter
{
    int K = 16;
    for (int i = 0; i < K; i++)
    {
        set_bit(hash_i((unsigned char *)id, i) % length_bit);
    }
}

bool BloomFilter::search(char *id) // Check if id is in the bloom filter
{
    int K = 16;
    for (int i = 0; i < K; i++)
    {
        if (!test_bit(hash_i((unsigned char *)id, i) % length_bit))
            return false;
    }
    return true;
}