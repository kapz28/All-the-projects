//kapilan satkunanathan and yuanpei gao
#ifndef A1_SEQUENTIAL_LIST_HPP
#define A1_SEQUENTIAL_LIST_HPP
class SequentialList {
public:
	SequentialList();
	// Can be seen outside as SequentialList::DataType
	typedef int DataType;	

private:	
	// Befriend so tests have access to variables.
	friend class SequentialListTest;

	// MEMBER VARIABLES
	// A pointer to the block of memory allocated to store the list data.
	DataType *data_;
	// The capacity of the list
	// (i.e., the maximum number of items the list can store).
	unsigned int capacity_;
	// The number of used elements in data_.
	unsigned int size_;

	// Copy constructor. Declared private so we don't use it incorrectly.
	SequentialList(const SequentialList& rhs) {}
	// Assignment operator. Declared private so we don't use it incorrectly.
	SequentialList& operator=(const SequentialList& rhs) {}

public:
	// CONSTRUCTORS/DESTRUCTOR
	// Create a new SequentialList with the given number of elements.
	SequentialList(unsigned int cap);
	//Destroy this SequentialList, freeing all dynamically allocated memory.
	~SequentialList();
	// ACCESSORS
	// Returns the number of elements in the list.
	unsigned int size() const;
	// Returns the maximum number of elements the list can hold.
	unsigned int capacity() const;
	// Returns true if the list is empty, false otherwise.
	bool empty() const;
	// Returns true if the list is at capacity, false otherwise.
	bool full() const;
	// Returns the value at the given index in the list. If index is invalid,
	// return the value of the last element.
	DataType select(unsigned int index) const;
	// Searches for the given value, and returns the index of this value if found.
	// Returns the size of the list otherwise
	unsigned int search(DataType val) const;
	// Prints all elements in the list to the standard output.
	void print() const;
	// Returns true if the list is sorted in ascending (non-descending) order
    bool is_sorted_asc() const;
    // Returns true if the list is sorted in descending (non-ascending) order
    bool is_sorted_desc() const;	

	// MUTATORS
	// Inserts a value into the list at a given index.
	bool insert(DataType val, unsigned int index);
	// Inserts a value at the beginning of the list.
	bool insert_front(DataType val);
	// Inserts a value at the end of the list.
	bool insert_back(DataType val);
	// Deletes a value from the list at the given index.
	bool remove(unsigned int index);
	// Deletes a value from the beginning of the list.
	bool remove_front();
	// Deletes a value at the end of the list.
	bool remove_back();
	// Replaces the value at the given index with the given value.
	bool replace(unsigned int index, DataType val);	
    // Inserts a value so that the list remains sorted in ascending order
    bool insert_sorted_asc(DataType val);
    // Inserts a value so that the list remains sorted in descending order
    bool insert_sorted_desc(DataType val);
    // Sorts the list into ascending (non-descending) order
    void sort_asc();
    // Sorts the list into descending (non-ascending) order
    void sort_desc();
};
#endif

