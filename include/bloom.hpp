class BloomFilter
{
    int *bit_array;
    unsigned long length_bit;

public:
    BloomFilter(unsigned long);
    ~BloomFilter();

    void set_bit(int);
    int test_bit(int);
    void add(const char *);
    bool search(char *);
};