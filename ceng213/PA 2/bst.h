#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <functional>
#include <cstddef>
#include <ostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <utility>

#include <iostream>   // you may use it for debugging your code


class DefaultBalanceCondition    // default BST, do not change
{
  public:
    bool operator() (int current_height, int ideal_height) const
    {
      return true;
    }
};


template <typename Key, typename Object,
          typename BalanceCondition=DefaultBalanceCondition,
	  typename Comparator=std::less<Key> >
class BinarySearchTree
{
  public:  /* DO NOT CHANGE */
    struct Node   // core structure of BinarySearchTree
    {
       Key key;         // unique key
       Object data;     // data to be stored, characterized by key
       Node * left;     // pointer to left subtree
       Node * right;    // pointer to right subtree
       size_t height;   // height of the node
       size_t subsize;  // size of the subtree node roots including itself

       Node(const Key &, const Object &, 
            Node *, Node *, size_t =0, size_t =1);   // Node constructor
    };

  public:  /* DO NOT CHANGE PROTOTYPES */
    BinarySearchTree();  // zero-parameter constructor
    BinarySearchTree(const std::list<std::pair<Key, Object> > &); // list is always sorted!
    ~BinarySearchTree();  // destructor

  public:  /* DO NOT CHANGE PROTOTYPES */
    void insert(const Key &, const Object &);  // insert new Key-Object
    void remove(const Key &);  // remove Node characterized by Key
    void toCompleteBST();  // convert into a complete BST

  public:  /* DO NOT CHANGE PROTOTYPES */
    Node * find(const Key &) const; // single item
    std::list<Node *> find(const Key &, const Key &) const; // range queries
    int height() const;  // return the height of the tree
    size_t size() const; // return the number of items in the tree
    bool empty() const;  //return whether the tree is empty or not
    Node * getRoot() const; // return a pointer to the root of the tree
    void print(std::ostream &) const;  // print tree structure into an output stream

  private:  /* DO NOT CHANGE DATA TYPES AND IDENTIFIERS */
    Node * root;                     // designated root
    size_t numNodes;                 // size
    Comparator isLessThan;           // operator upon which BST nodes are arranged
    BalanceCondition isBalanced;     // signals whether the BST is balanced or not
  
  private:  /* DO NOT CHANGE PROTOTYPES */
    Node * find(const Key &, Node *) const;
    int height(Node *) const;
    int subsize(Node *) const;
    void print(Node *, std::ostream &) const;

    // Define your const private utility functions below this line

  private:
    void makeEmpty(Node * &);       // utility for destructor
    void findHelp(const Key & lower, const Key & upper, Node *&r, std::list<Node *> &ranges) const;
    void changeHeight(Node *& succ, Node *& parSucc);
    void removeHeight(Node *&r, Node *&x);
    void insertHelp(const Key & k, const Object & x, Node *&r);
    void toCompleteSub(Node *&r);
    void removeHelper(const Key & k, Node *&r);
    void removeHelperRoot(const Key & k, Node *&r);
    Node * minNode(Node *&r);
    Node * findParent(Node *&r, Node *&x);
    void toVector(std::vector<Node *> &vector, Node * r);
    // Define your private utility functions below this line
    
    Node * HelpToConstruct(std::vector<std::pair<Key,Object> > tree, int start, int end, int length);
    int findroot(int length);
    Node * toCompleteHelper(std::vector<Node *> vector, int start, int end, int length);

    
  private: /* DO NOT CHANGE PROTOTYPES: compiler defaults are blocked */  
    BinarySearchTree(const BinarySearchTree &);
    const BinarySearchTree & operator=(const BinarySearchTree &);

  private:  // static utility functions
    template <typename T> //static utility function
    static const T & max(const T &, const T &);

    // Define more below this line if needed

};

#endif


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::Node::
Node(const K & _k, const O & _d, Node * _l, Node * _r, size_t _h, size_t _s)
  : key(_k), data(_d), left(_l), right(_r), height(_h), subsize(_s)
{
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::BinarySearchTree()
  : root(NULL), numNodes(0)  //comparator(C() ??)
{
}


// IMPLEMENT
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::BinarySearchTree(const std::list<std::pair<K,O> > & datalist)
   : root(NULL), numNodes(0)  // change it as you'd like
{
    std::vector<std::pair<K,O> > tree;
    for (auto const &i: datalist)
    {
        tree.push_back(i);
    }
    
    int length = datalist.size();
    numNodes = length;
    
    root = HelpToConstruct(tree, 0, length-1, length);
    
   
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::HelpToConstruct(std::vector<std::pair<K,O> > tree, int start, int end, int length)
{
    if (start > end)
    {
        return NULL;
    }
    
    if (start == end)
    {
        Node *x = new Node(tree[start].first, tree[start].second, NULL, NULL, 0, 1);
        return x;
    }
    
    int i = findroot(length);
    
    int head = i+start;
    
    Node *r = new Node(tree[head].first, tree[head].second, NULL, NULL);
    
    r->height = std::log2(length);
    r->subsize = length;
    
    r->left = HelpToConstruct(tree, start, head-1, (head - start));
    r->right = HelpToConstruct(tree, head+1, end, (end - head));
    
    return r;
     
    
}

template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::findroot(int length)
{
    int n=0;
    
    while (std::pow(2, n) <= length)
    {
        n++;
    }
    
    n = n - 1;
    
    int m = std::pow(2, n);
    int r = length - (m - 1);
    int lt, rt;
    
    if (r <= m/2)
    {
        lt = (m - 2) / 2 + r;
        rt = (m - 2) / 2;
    }
    
    else if (r > m/2)
    {
        lt = (m - 2) / 2 + m / 2;
        rt = (m - 2) / 2 + r - m / 2;
    }
    
    return lt;
}



// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::toCompleteBST()
{
    std::vector<Node *> vector;
    toVector(vector, root);
    
    int length = numNodes;
     
    root = toCompleteHelper(vector, 0, length-1, length);
}

template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::toVector(std::vector<Node *> &vector, Node * r)
{
    if (r == NULL)
    {
        return;
    }
    
    toVector(vector, r->left);
    vector.push_back(r);
    toVector(vector, r->right);
    
}

template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::toCompleteHelper(std::vector<Node *> vector, int start, int end, int length)
{

    if (start > end)
    {
        return NULL;
    }
    
    if (start == end)
    {
        Node *r;
        r = vector[start];
        r->height = 0;
        r->subsize = 1;
        r->left = NULL;
        r->right = NULL;
        return r;
    }
    
    int i = findroot(length);
    
    int head = i + start;
    Node *r;
    r = vector[head];
    r->height = std::log2(length);
    r->subsize = length;
 
    
    r->left = toCompleteHelper(vector, start, head-1, (head - start));
    r->right = toCompleteHelper(vector, head+1, end, (end - head));
    
    return r;
    
}

// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::insert(const K & k, const O & x)
{
    Node *r;
    r = find(k);
    
    if (r == NULL)
    {
        
        numNodes++;
        insertHelp(k, x, root);

        
    }
    
    else
    {
        r->data = x;
    }
}



template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::insertHelp(const K & k, const O & x, Node *&r)
{
    if (r == NULL)
    {
        r = new Node(k, x, NULL, NULL);
        return;
        
    }
    
    else if (isLessThan(k, r->key))
    {
        
        insertHelp(k, x, r->left);
        r->subsize = subsize(r) + 1;
        
  

    }
    
    else if (isLessThan(r->key, k))
    {
       
        insertHelp(k, x, r->right);
        r->subsize = subsize(r) + 1;
        
       
    }
    
    r->height = max(height(r->left), height(r->right)) + 1;
    r->subsize = subsize(r->left) + subsize(r->right) + 1;
    
    if (subsize(r) > 1)
    {
            
        if (!isBalanced(height(r), std::log2(subsize(r))))
        {

            toCompleteSub(r);
        
        }
    }
    
    
}


template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::toCompleteSub(Node *&r)
{
    
    
    std::vector<Node *> bonapetit;
    toVector(bonapetit, r);

    int length = subsize(r);
     
    r = toCompleteHelper(bonapetit, 0, length-1, length);    
}




// IMPLEMENT
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::remove(const K & k)
{
    if (empty())
    {
        return;
    }
    
    else if (find(k) == NULL)
    {
        return;
    }
    
    else if (find(k) == root)
    {
        numNodes = numNodes - 1;
        removeHelperRoot(k, root);
    }
    
    else
    {
        numNodes = numNodes - 1;
        removeHelper(k, root);
        
    }
    
    
    
}
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::changeHeight(Node *& succ, Node *& parSucc)
{
    if (succ == NULL)
    {
        return;
    }
    
    if (parSucc == NULL)
    {
        return;
    }
    
    if (succ == parSucc)
    {
        
        return;
    }
    succ->height = std::max(height(succ->left), height(succ->right)) + 1;
    changeHeight(succ->left, parSucc);
    
}
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::removeHelperRoot(const K & k, Node *&r)
{
    if (r == NULL)
    {
        return;
    }
    
    else 
    {
        if ((r->left == NULL) || (r->right == NULL))
        {
            Node *del;
            
            if ((r->left == NULL) && (r->right == NULL))
            {
                del = r;
                delete del;
                root = NULL;
                r = root;
            }
        
        
            else if (r->right == NULL)
            {
                del = r;
                Node *tmp = r->left;
                
                
                delete del;
                root = tmp;
                r = root;
            }
        
            else if (r->left == NULL)
            {
                del = r;
                Node *tmp = r->right;
                
                delete del;
                
                root = tmp;
                r = root;
            }
        }
        
        else if ((r->left != NULL) && (r->right != NULL))
        {
            Node *succ = minNode(r->right);
            Node *parSucc = findParent(root, succ);
            Node *del = r;
            Node *leftc = del->left;
            
            if (parSucc == del)
            {

                
                succ->left = leftc;
                delete del;
                
                root = succ;
                r = root;
                r->height = std::max(height(r->left), height(r->right)) + 1;
            }   
            else
            {
                Node *rightc = del->right;
                Node *schild;
                
                if (succ->right != NULL)
                {
                    schild = succ->right;
                  
                }
                
                else if (succ->right == NULL)
                {
                    schild = NULL;
                }
                
  
                
                succ->left = leftc;
                succ->right = rightc;
                
                if (schild != NULL)
                {
                    parSucc->left = schild;
                }
                
                else if (schild == NULL)
                {
                    parSucc->left = NULL;
                }
                root = succ;
                r = root;
                 

                
                delete del; 
                
               r->height = std::max(height(r->left), height(r->right)) + 1;
                changeHeight(r->right, parSucc);  
               
            
            }
            
            
           
            
        }
    }
    
    if (r == NULL)
    {
        return;
    }
    
    
    
    r->subsize = 1 + subsize(r->left) + subsize(r->right);
    r->height = std::max(height(r->left), height(r->right)) + 1;
    if (subsize(r) > 1)
    {    
        if (!isBalanced(height(r), std::log2(subsize(r))))
        {
            toCompleteSub(r);

        }    

    }
}


template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::removeHelper(const K & k, Node *&r)
{
    if (r == NULL)
    {
        return;
    }
    
    if (isLessThan(k, r->key))
    {
        
        removeHelper(k, r->left);
        r->subsize = subsize(r) - 1;
        
        
    }
    
    else if (isLessThan(r->key, k))
    {
        
        removeHelper(k, r->right);
        r->subsize = subsize(r) - 1;
        
        
    }
    
    else
    {
        
        if ((r->left == NULL) || (r->right == NULL))
        {
            Node *del;
            
            if ((r->left == NULL) && (r->right == NULL))
            {
                del = r;
                Node *parent = findParent(root, del);
                
                if (parent->left == del)
                {
                    parent->left = NULL;
                }
                
                else if (parent->right == del)
                {
                    parent->right = NULL;
                }
                
                
                delete del;
                
                
                
            }
            
            else if (r->right == NULL)
            {
                del = r;
                
                Node *parent = findParent(root, del);
                
                
                if(parent->left == del)
                {
                    Node *tmp = del->left;
                    parent->left = tmp;
                    r = tmp;
                    
                }
                
                else if (parent->right == del)
                {
                    Node *tmp = del->left;
                    parent->right = tmp;
                    r = tmp;
                    
                }
                
                
                
                
                
                
                delete del;
                
                
                
            }
            
            else if (r->left == NULL)
            {
                del = r;
                
                Node *parent = findParent(root, del);
                
                if(parent->left == del)
                {
                    Node *tmp2 = del->right;
                    parent->left = tmp2;
                    r = tmp2;
                }
                
                else if (parent->right == del)
                {
                    Node *tmp2 = del->right;
                    parent->right = tmp2;
                    r = tmp2;
                }
                
                
               
                
                
                delete del;
                
                
                
            }
        }
        
        else if ((r->left != NULL) && (r->right != NULL))
        {
            Node *succ = minNode(r->right);
            Node *parSucc = findParent(root, succ);
            Node *del = r;
            Node *leftc = del->left;
            Node *parent = findParent(root, del);
            
            if (parSucc == del)
            {
                if (parent->left == del)
                {
                    parent->left = succ;
                }
                
                else if (parent->right == del)
                {
                    parent->right = succ;
                }
                r = succ;
                succ->left = leftc;
                r->height = std::max(height(r->left), height(r->right)) + 1;
                delete del;
                
               
                  
            }
            
            else
            {
                Node *rightc = del->right;
                Node *schild;
                
                if (succ->right != NULL)
                {
                    schild = succ->right;
                    
                }
                
                else if (succ->right == NULL)
                {
                    schild = NULL;
                }
                
                if (parent->left == del)
                {
                    parent->left = succ;
                }
                
                else if (parent->right == del)
                {
                    parent->right = succ;
                }
                
                succ->left = leftc;
                succ->right = rightc;
                
                if (schild != NULL)
                {
                    parSucc->left = schild;
                }
                
                else if (schild == NULL)
                {
                    parSucc->left = NULL;
                }
                r = succ;
                
                delete del;

                   
                parSucc->height = std::max(height(parSucc->left), height(parSucc->right)) + 1;
                r->height = std::max(height(r->left), height(r->right)) + 1;
                changeHeight(r->right, parSucc);        
  
                
            }
            
        
             
            
            
        }
    }
    
    
    
    if (r == NULL)
    {
        return;
    }
    
    r->subsize = 1 + subsize(r->left) + subsize(r->right);
    r->height = std::max(height(r->left), height(r->right)) + 1;
    
    if (subsize(r) > 1)
    {    
        if (!isBalanced(height(r), std::log2(subsize(r))))
        {
            toCompleteSub(r);

        }    

    }
    
}

template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::removeHeight(Node *&r, Node *&x)
{
    if (r == NULL)
    {
        return;
    }
    
    if (x == NULL)
    {
        return;
    }
    
    if (r == x)
    {
        return;
    }
    
    else if (isLessThan(r->key, x->key))
    {
        removeHeight(r->right, x);
    }
    
    else if (isLessThan(x->key, r->key))
    {
        removeHeight(r->left, x);
    }
    
    
    r->height = std::max(height(r->left), height(r->right));
    
    
    
    
}



template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::findParent(Node *&r, Node *&x)
{
    if (r == NULL)
    {
        return NULL;
    }
    
    if (r->left == NULL && r->right == NULL)
    {
        return NULL;
    }
    
    if ((r->left != NULL && r->left == x) || (r->right != NULL && r->right == x))
    {
        return r;
    }
    
    if (isLessThan(x->key, r->key))
    {
        return findParent(r->left, x);
    }
    
    if (isLessThan(r->key, x->key))
    {
        return findParent(r->right, x);
    }
    
    
    
}


template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::minNode(Node *&r)
{
    if (r->left == NULL)
    {
        return r;
    }
    r->subsize = subsize(r) - 1;
    
    minNode(r->left);
    
    
}

// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::find(const K & key) const
{
  return find(key, root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::find(const K & key, Node * t) const
{
  if (t == NULL)
  {
    return NULL;
  }
  else if (isLessThan(key, t->key))
  {
    return find(key, t->left);
  }
  else if (isLessThan(t->key, key))
  {
    return find(key, t->right);
  }
  else //found
  {
    return t;
  }
}


// IMPLEMENT
template <typename K, typename O, typename B, typename C>
std::list<typename BinarySearchTree<K,O,B,C>::Node *>
BinarySearchTree<K,O,B,C>::find(const K & lower, const K & upper) const
{
    std::list<Node *> ranges;
    Node *r;
    r = getRoot();
    
    findHelp(lower, upper, r, ranges);
    return ranges;
}




template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::findHelp(const K & lower, const K & upper, Node *&r, std::list<Node *> &ranges) const
{
    
    
    
    if (r == NULL)
    {
        return;
    }
    
    else if (isLessThan(lower, r->key) && isLessThan(upper, r->key))
    {
        findHelp(lower, upper, r->left, ranges);
    }

    else if (isLessThan(r->key, lower) && isLessThan(r->key, upper))
    {
        findHelp(lower, upper, r->right, ranges);
    }
    
        
    else
    {
        findHelp(lower, upper, r->left, ranges);
        ranges.push_back(r);
        findHelp(lower, upper, r->right, ranges);
    }
    
    
    
    
}

// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::getRoot() const
{
  return root;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::height() const
{
  return height(root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::height(Node * t) const
{
  return (t == NULL) ? -1 : t->height;
}


// private utility
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::subsize(Node * t) const
{
  return (t == NULL) ? 0 : t->subsize;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
size_t
BinarySearchTree<K,O,B,C>::size() const
{
  return numNodes;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
bool
BinarySearchTree<K,O,B,C>::empty() const
{
  return numNodes == 0;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::makeEmpty(Node * & t)
{
  if (t != NULL)
  {
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;

    --numNodes;
  }
  
  t = NULL;
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::~BinarySearchTree()
{
  makeEmpty(root);
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::print(std::ostream & out) const
{
  print(root, out);
  out << '\n';
}


// DO NOT CHANGE
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::print(Node * t, std::ostream & out) const
{
  if (t != NULL && t->left != NULL) 
  {
    out << '[';
    print( t->left, out );
  }
  else if (t != NULL && t->left == NULL && t->right != NULL)
  {
    out << "[";
  }

  if (t != NULL)
  {
    if (t->left == NULL && t->right == NULL)
    {
      out << '(' << (t->key) << ')';
    }
    else if (t->left != NULL || t->right != NULL)
    {
      out << '{' << (t->key) << ",H" << t->height << ",S" << t->subsize << '}';
    }
  }
  
  if (t != NULL && t->right != NULL)
  {
    print( t->right, out );
    out << ']';
  }
  else if (t != NULL && t->left != NULL && t->right == NULL)
  {
    out << "]";
  }
}


// do not change
template <typename K, typename O, typename B, typename C>
template <typename T>
const T &
BinarySearchTree<K,O,B,C>::max(const T & el1, const T & el2)
{
  return el1 > el2 ? el1 : el2;
}

