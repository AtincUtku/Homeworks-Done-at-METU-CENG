#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"
#include "ItemNotFoundException.h"
/* Do not add new libraries or files */

#define BUCKET_SIZE 2

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class K, class T>
class HashTable {
    struct Entry {
        K Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[BUCKET_SIZE];
    };

    int _capacity; // INDICATES THE TOTAL CAPACITY OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // THE HASH TABLE

    // == DEFINE HELPER METHODS & VARIABLES BELOW ==
    


public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    // COPY THE WHOLE CONTENT OF RHS INCLUDING THE KEYS THAT WERE SET AS DELETED
    HashTable(const HashTable<K, T>& rhs);
    HashTable<K, T>& operator=(const HashTable<K, T>& rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF THE LOAD FACTOR OF THE TABLE IS GREATER THAN 0.6,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(const K& key, const T& value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, THROW ItemNotFoundException()
    void Delete(const K& key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THROW ItemNotFoundException()
    T& Get(const K& key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES
    void Resize(int newCapacity);

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const; // RETURN THE TOTAL CAPACITY OF THE TABLE
    int Size() const; // RETURN THE NUMBER OF ACTIVE ITEMS
    void getKeys(K* keys); // PUT THE ACTIVE KEYS TO THE GIVEN INPUT PARAMETER
};


template <class K, class T>
HashTable<K, T>::HashTable() 
{
    _capacity = 7;
    _size = 0;
    _table = new Bucket[_capacity];
    

    
}

template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T>& rhs) 
{
    _capacity = rhs._capacity;
    _size = rhs._size;
    _table = new Bucket[_capacity];
    
    for (int i=0; i<rhs._capacity; i++)
    {
        for (int j=0; j<BUCKET_SIZE; j++)
        {
            _table[i].entries[j].Key = rhs._table[i].entries[j].Key;
            _table[i].entries[j].Value = rhs._table[i].entries[j].Value;
            _table[i].entries[j].Deleted = rhs._table[i].entries[j].Deleted;
            _table[i].entries[j].Active = rhs._table[i].entries[j].Active;
        }

    }
    
}

template <class K, class T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& rhs) 
{
    if (this->_capacity > 0)
    {
        delete [] _table;
    }
    
    this->_capacity = rhs._capacity;
    this->_size = rhs._size;
    this->_table = new Bucket[_capacity];
    
    for (int i=0; i<rhs._capacity; i++)
    {
        for (int j=0; j<BUCKET_SIZE; j++)
        {
            this->_table[i].entries[j].Key = rhs._table[i].entries[j].Key;
            this->_table[i].entries[j].Value = rhs._table[i].entries[j].Value;
            this->_table[i].entries[j].Deleted = rhs._table[i].entries[j].Deleted;
            this->_table[i].entries[j].Active = rhs._table[i].entries[j].Active;
        }
    }
    
    
    return *this;
    
}

template <class K, class T>
HashTable<K, T>::~HashTable() 
{
    _capacity = 0;
    _size = 0;
    delete [] _table;
    
}

template <class K, class T>
void HashTable<K, T>::Insert(const K& key, const T& value) 
{
    if (_table == NULL)
    {
        _capacity = NextCapacity(_capacity);
        _table = new Bucket[_capacity];
    }        
    
    
    float loadFactor = ((float) _size) / (((float) BUCKET_SIZE) * ((float) _capacity));
    
    if (loadFactor > 0.6)
    {
        int newC = NextCapacity(_capacity);
        Resize(newC);
    }
    

    int index = Hash(key) % _capacity;
    
    int i=0;
    
    while (i<_capacity)
    {
        

        if (_table[index].entries[0].Active == true && _table[index].entries[0].Deleted == false && _table[index].entries[0].Key == key)
        {
            _table[index].entries[0].Value = value;

            return;
        }
            
        else if (_table[index].entries[1].Active == true && _table[index].entries[1].Deleted == false && _table[index].entries[1].Key == key)
        {
            _table[index].entries[1].Value = value;

            return;                
        }
        
        else
        {
            if (_table[index].entries[0].Active == false || _table[index].entries[0].Deleted == true)
            {
                _table[index].entries[0].Key = key;
                _table[index].entries[0].Value = value;
                _table[index].entries[0].Deleted = false;
                _table[index].entries[0].Active = true;
                _size++;
                return;
            }
        
            else if (_table[index].entries[1].Active == false || _table[index].entries[1].Deleted == true)
            {
                _table[index].entries[1].Key = key;
                _table[index].entries[1].Value = value;
                _table[index].entries[1].Deleted = false;
                _table[index].entries[1].Active = true;
                _size++;
                return;            
            }
            
            float loadFactor = ((float) _size) / (((float) BUCKET_SIZE) * ((float) _capacity));
    
            if (loadFactor > 0.6)
            {
            int newC = NextCapacity(_capacity);
            Resize(newC);
            }
        }
        
        i++;
        
        index = (index + (i*i)) % _capacity;

    }
}

template <class K, class T>
void HashTable<K, T>::Delete(const K& key) 
{
    int index = Hash(key) % _capacity;
    
    int i=0;
    
    bool exists = false;
    
    while (i<_capacity)
    {
        if (_table[index].entries[0].Active == true && _table[index].entries[0].Deleted == false && _table[index].entries[0].Key == key)
        {
            
            _table[index].entries[0].Deleted = true;
            _table[index].entries[0].Active = false;
            _size--;
            exists = true;
            return;
        }
        
        else if (_table[index].entries[1].Active == true && _table[index].entries[1].Deleted == false  && _table[index].entries[1].Key == key)
        {
            
            _table[index].entries[1].Deleted = true;
            _table[index].entries[1].Active = false;
            _size--;
            exists = true;
            return;
        }
        
        i++;
        
        index = (index + (i*i)) % _capacity;
    }
    
    if (exists == false)
    {
        throw ItemNotFoundException();
    }
    
}

template <class K, class T>
T& HashTable<K, T>::Get(const K& key) const 
{
    int index = Hash(key) % _capacity;
    
    int i=0;
    
    bool exists = false;
    
    if (_table == NULL)
    {
        throw ItemNotFoundException();
    }
    
    while (i<_capacity)
    {
        if (_table[index].entries[0].Active == true && _table[index].entries[0].Deleted == false && _table[index].entries[0].Key == key)
        {
            exists = true;
            return _table[index].entries[0].Value;
        }
        
        else if (_table[index].entries[1].Active == true && _table[index].entries[1].Deleted == false && _table[index].entries[1].Key == key)
        {
            exists = true;
            return _table[index].entries[1].Value;
        }
        
        i++;
        
        index = (index + (i*i)) % Capacity();
    }
    

    throw ItemNotFoundException();

    
    
    
    
    
}




template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) 
{
    Entry *backup;
    backup = new Entry[_size];
    int prev_s = _size;
    
    int i=0;
    
    while (i<_size)
    {
        for (int j=0; j<BUCKET_SIZE; j++)
        {
            for (int k=0; k<_capacity; k++)
            {
                if (_table[k].entries[j].Active == true)
                {
                    backup[i] = _table[k].entries[j];
                    i++;
                }
            }
        }
    }
    
    delete [] _table;
    _table = NULL;
    
    _size = 0;
    _capacity = newCapacity;
    
    _table = new Bucket[newCapacity];
    
    int x = 0;
    
    while (x<prev_s)
    {
        Insert(backup[x].Key, backup[x].Value);
        x++;
    }
    
    delete [] backup;
}





template <class K, class T>
int HashTable<K, T>::Capacity() const {
    return _capacity;
}

template <class K, class T>
int HashTable<K, T>::Size() const {
    return _size;
}


template <class K, class T>
void HashTable<K, T>::getKeys(K* keys) {
    int index = 0;

    for (int i = 0; i < _capacity; i++) {
        Bucket& bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}

#endif