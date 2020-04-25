// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HASHTREE_HEADER
#define RAVL_HASHTREE_HEADER 1
//////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Trees"
//! lib=RavlCore
//! file="Ravl/Core/Container/Trees/HashTree.hh"

#include "Ravl/Hash.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/DList.hh"

namespace RavlN {
  
  template<class KeyT,class DataT> class HashTreeC;
  
  //! userlevel=Develop
  //: Base class for tree's.
  
  template<class KeyT,class DataT>
  class HashTreeNodeBodyC 
    : public RCBodyVC 
  {
  public:
    HashTreeNodeBodyC()
    {}
    //: Default constructor.
    
    HashTreeNodeBodyC(const DataT &ndata)
      : data(ndata)
    {}
    //: Constructor.

    HashTreeNodeBodyC(BinIStreamC &strm)
      : RCBodyVC(strm)
    { strm >> data; }
    //: Binary stream constructor.
    
    HashTreeNodeBodyC(std::istream &strm)
      : RCBodyVC(strm)
    { strm >> data; }
    //: Text stream constructor.
    
    virtual bool Save(BinOStreamC &strm) const {
      if(!RCBodyVC::Save(strm))
        return false;
      strm << data;
      return true;
    }
    //: Save to binary stream.
    
    virtual bool Save(std::ostream &strm) const {
      if(!RCBodyVC::Save(strm))
        return false;
      strm << data;
      return true;
    }
    //: Save to text stream.
    
    DataT &Data()
    { return data; }
    //: Access data.
    
    const DataT &Data() const
    { return data; }
    //: Access data.

    virtual bool IsLeaf() const
    { return true; }
    //: Test if node is a leaf in the tree.
    
    virtual std::ostream &Dump(std::ostream &out,int level = 0) const {
      out << this->data;
      return out;
    }
    //: Dump in a easly readable format.
    
  protected:
    DataT data;
  };
  
  //! userlevel=Advanced
  //: Base class for tree's.
  
  template<class KeyT,class DataT>
  class HashTreeNodeC 
    : public RCHandleVC<HashTreeNodeBodyC<KeyT,DataT> >
  {
  public:
    HashTreeNodeC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    HashTreeNodeC(HashTreeNodeBodyC<KeyT,DataT> &bod)
      : RCHandleVC<HashTreeNodeBodyC<KeyT,DataT> >(bod)
    {}
    //: Default constructor.
    
    HashTreeNodeC(const HashTreeNodeBodyC<KeyT,DataT> *bod)
      : RCHandleVC<HashTreeNodeBodyC<KeyT,DataT> >(bod)
    {}
    //: Default constructor.
    
    HashTreeNodeBodyC<KeyT,DataT> &Body()
    { return RCHandleVC<HashTreeNodeBodyC<KeyT,DataT> >::Body(); }
    //: Access body.
    
    const HashTreeNodeBodyC<KeyT,DataT> &Body() const
    { return RCHandleVC<HashTreeNodeBodyC<KeyT,DataT> >::Body(); }
    //: Access body.

  public:
    
    DataT &Data()
    { return Body().Data(); }
    //: Access data.
    
    const DataT &Data() const
    { return Body().Data(); }
    //: Access data.
    
    bool IsLeaf() const
    { return Body().IsLeaf(); }
    //: Test if node is a leaf in the tree.
    
    std::ostream &Dump(std::ostream &out,int level = 0) const 
    { return Body().Dump(out,level); } 
    //: Dump in a easly readable format.
    
  };

  
  //! userlevel=Develop
  //: Tree of hashtables.
  
  template<class KeyT,class DataT>
  class HashTreeBodyC
    : public HashTreeNodeBodyC<KeyT,DataT>
  {
  public:
    HashTreeBodyC(const DataT &dat)
      : HashTreeNodeBodyC<KeyT,DataT>(dat)
    {}
    //: Constructor.
    
    HashTreeBodyC()
    {}
    //: Default constructor.
    
    HashTreeBodyC(BinIStreamC &strm)
      : HashTreeNodeBodyC<KeyT,DataT>(strm)
    { strm >> children; }
    //: Binary stream constructor.
    
    HashTreeBodyC(std::istream &strm)
      : HashTreeNodeBodyC<KeyT,DataT>(strm)
    { strm >> children; }
    //: Text stream constructor.
    
    virtual bool Save(BinOStreamC &strm) const {
      if(!HashTreeNodeBodyC<KeyT,DataT>::Save(strm))
        return false;
      strm << children;
      return true;
    }
    //: Save to binary stream.

    virtual bool Save(std::ostream &strm) const {
      if(!HashTreeNodeBodyC<KeyT,DataT>::Save(strm))
        return false;
      strm << children;
      return true;
    }
    //: Save to text stream.
    
    HashTreeNodeC<KeyT,DataT> &Node(const KeyT &key)
    { return children[key]; }
    //: Access child node.
    
    virtual bool IsLeaf() const
    { return children.IsEmpty(); }
    //: Test if node is a leaf in the tree.

    HashTreeNodeC<KeyT,DataT> Follow(const DListC<KeyT> &lst);
    //: Follow list of keys to a node.
    // If node is not found then an invalid handle is returned.
    
    bool Child(const KeyT &key,HashTreeNodeC<KeyT,DataT> &child) 
    { return children.Lookup(key,child); }
    //: lookup child in tree.
    // Returns true and updates 'child' if child is found.
    
    bool IsElm(const KeyT &key) const
    { return children.IsElm(key); }
    //! Test if a child exists.

    bool Add(const KeyT &key,const DataT &data,bool overWrite = false);
    //: Add a child with given key and data.
    // If overWrite is true the node is replaced if it already exists.
    // returns false if node is not added because it exists.
    
    bool Add(const KeyT &key,const HashTreeC<KeyT,DataT> &subtree,bool overWrite = false);
    //: Add a sub tree
    // If overWrite is true the node is replaced if it already exists.
    // returns false if node is not added because it exists.

    bool Remove(const KeyT& key);
    //: Remove a child.
    //!return: True if node successfully removed.
    
    HashC<KeyT,HashTreeNodeC<KeyT,DataT> > &Children()
    { return children; }
    //: Access table of children.

    const HashC<KeyT,HashTreeNodeC<KeyT,DataT> > &Children() const
    { return children; }
    //: Access table of children.

    virtual std::ostream &Dump(std::ostream &out,int level = 0) const {
      out << this->data << "\n";
      for(HashIterC<KeyT,HashTreeNodeC<KeyT,DataT> > it(children);it;it++){
	out << it.Key() << " "; 
	it.Data().Dump(out,level+1) << "\n";
      }
      return out;
    }
    //: Dump in a easly readable format.
    // This is a little poor at the moment, its really expecting a derived
    // class to provide a better function.
    
  protected:
    HashC<KeyT,HashTreeNodeC<KeyT,DataT> > children;
  };
  
  //! userlevel=Normal
  //: Tree of hashtables.
  
  template<class KeyT,class DataT>
  class HashTreeC
    : public HashTreeNodeC<KeyT,DataT>
  {
  public:
    HashTreeC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    explicit HashTreeC(bool)
      : HashTreeNodeC<KeyT,DataT>(*new HashTreeBodyC<KeyT,DataT>())
    {}
    //: Constructor.
    // Creates an empty tree

    HashTreeC(HashTreeNodeC<KeyT,DataT> &base)
      : HashTreeNodeC<KeyT,DataT>(dynamic_cast<HashTreeBodyC<KeyT,DataT> * >(RCHandleC<HashTreeNodeBodyC<KeyT,DataT> >::BodyPtr(base)))
    {}
    //: Base class constructor.
    
    explicit HashTreeC(const DataT &data)
      : HashTreeNodeC<KeyT,DataT>(*new HashTreeBodyC<KeyT,DataT>(data))      
    {}
    
  protected:
    HashTreeC(HashTreeBodyC<KeyT,DataT> &bod)
      : HashTreeNodeC<KeyT,DataT>(bod)
    {}
    //: Body constructor.
    
    HashTreeC(const HashTreeBodyC<KeyT,DataT> *bod)
      : HashTreeNodeC<KeyT,DataT>(bod)
    {}
    //: Body constructor.
    
    HashTreeBodyC<KeyT,DataT> &Body()
    { return static_cast<HashTreeBodyC<KeyT,DataT> & >(HashTreeNodeC<KeyT,DataT>::Body()); }
    //: Access body.

    const HashTreeBodyC<KeyT,DataT> &Body() const
    { return static_cast<const HashTreeBodyC<KeyT,DataT> & >(HashTreeNodeC<KeyT,DataT>::Body()); }
    //: Access body.
    
  public:
    HashTreeNodeC<KeyT,DataT> Follow(const DListC<KeyT> &lst) 
    { return Body().Follow(lst); }
    //: Follow list of keys to a node.
    // If node is not found then an invalid handle is returned.
    
    bool Child(const KeyT &key,HashTreeNodeC<KeyT,DataT> &child) 
      { return Body().Child(key,child); }
    //: lookup child in tree.
    // Returns true and updates 'child' if child is found.

    bool Add(const KeyT &key,const DataT &data,bool overWrite = false)
    { return Body().Add(key,data,overWrite); }
    //: Add a child with given key and data.
    // If overWrite is true the node is replaced if it already exists.
    // returns false if node is not added because it exists.

    bool IsElm(const KeyT &key) const
    { return Body().IsElm(key); }
    //! Test if a child exists.

    bool Add(const KeyT &key,const HashTreeC<KeyT,DataT> &subtree,bool overWrite = false)
    { return Body().Add(key,subtree,overWrite); }
    //: Add a sub tree
    // If overWrite is true the node is replaced if it already exists.
    // returns false if node is not added because it exists.
    
    bool Remove(const KeyT& key)
    { return Body().Remove(key); }
    //: Remove a child.
    //!return: True if node successfully removed.

    HashC<KeyT,HashTreeNodeC<KeyT,DataT> > &Children()
    { return Body().Children(); }
    //: Access table of children.

    const HashC<KeyT,HashTreeNodeC<KeyT,DataT> > &Children() const
    { return Body().Children(); }
    //: Access table of children.
    
    friend class HashTreeBodyC<KeyT,DataT>;
  };

  //! userlevel=Normal
  //: Iterate through a tree.
  
  template<class KeyT,class DataT>
  class HashTreeIterC
    : public HashIterC<KeyT, HashTreeNodeC<KeyT,DataT> >
  {
  public:
    HashTreeIterC()
    {}
    //: Default constructor.
    
    HashTreeIterC(const HashTreeC<KeyT,DataT> &tree)
      : HashIterC<KeyT, HashTreeNodeC<KeyT,DataT> >(tree.Children())
    {}
    //: Constructor from tree.
    
    HashTreeC<KeyT,DataT> Tree()
    { return HashTreeC<KeyT,DataT>(this->Data()); }
    //: Access current node as tree.

    bool Go() {
      HashTreeC<KeyT,DataT> tree(this->Data()); 
      if(!tree.IsValid())
	return false;
      HashIterC<KeyT, HashTreeNodeC<KeyT,DataT> >::operator=(tree.Children());
      return true;
    }
    //: Go down the current node in the tree.
  };

  
  template<class KeyT,class DataT>
  std::ostream &operator<<(std::ostream &s,const HashTreeC<KeyT,DataT> &tree) {
    if(!tree.IsValid()) {
      DataT dummy;
      HashC<KeyT,HashTreeNodeC<KeyT,DataT> > tab;
      s << dummy << ' ' << tab;
    } else
      s << tree.Data() << ' ' << tree.Children();
    return s;
  }
  
  template<class KeyT,class DataT>
  std::istream &operator>>(std::istream &s,HashTreeC<KeyT,DataT> &tree) {
    s >> tree.Data() >> tree.Children();
    return s;
  }
  
  template<class KeyT,class DataT>
  std::ostream &operator<<(std::ostream &s,const HashTreeNodeC<KeyT,DataT> &node) {
    HashTreeC<KeyT,DataT> tree(const_cast<HashTreeNodeC<KeyT,DataT> & >(node));
    // Always save nodes as full hash tree's
    if(tree.IsValid())
      return s << tree;
    if(node.IsValid())
      s << node.Data();
    else {
      DataT dummy;
      s << dummy;
    }
    static HashC<KeyT,HashTreeNodeC<KeyT,DataT> > empty; // Empty tree.
    s << ' ' << empty;
    return s;
  }
  
  template<class KeyT,class DataT>
  std::istream &operator>>(std::istream &s,HashTreeNodeC<KeyT,DataT> &tree) {
    HashTreeC<KeyT,DataT> ht;
    // Always read in as a full HashTree.
    s >> ht;
    tree = ht;
    return s;
  }
  
  //: Add a child with given key and data.
  // returns false if child exists.
  
  template<class KeyT,class DataT>
  bool HashTreeBodyC<KeyT,DataT>::Add(const KeyT &key,const DataT &data,bool overWrite) {
    HashTreeNodeC<KeyT,DataT> &child = children[key];
    if(child.IsValid()  && !overWrite)
      return false; // it already exits!
    child = HashTreeC<KeyT,DataT>(data);
    return true;
  }

  //: Add a child with given key and data.
  // returns false if child exists.
  
  template<class KeyT,class DataT>
  bool HashTreeBodyC<KeyT,DataT>::Add(const KeyT &key,const HashTreeC<KeyT,DataT> &data,bool overWrite) {
    HashTreeNodeC<KeyT,DataT> &child = children[key];
    if(child.IsValid() && !overWrite)
      return false; // it already exits!
    child = data;
    return true;
  }
  
  //: Remove a child.
  //!return: True if node successfully removed.

  template<class KeyT,class DataT>
  bool HashTreeBodyC<KeyT,DataT>::Remove(const KeyT& key)
  {
    if (children.Lookup(key))
      return children.Del(key);
    return false;
  }

  //: Follow list of keys to a node.
  // If node is not found then an invalid handle is returned.
  
  template<class KeyT,class DataT>
  HashTreeNodeC<KeyT,DataT> HashTreeBodyC<KeyT,DataT>::Follow(const DListC<KeyT> &lst) {
    HashTreeNodeC<KeyT,DataT> ret;
    HashTreeC<KeyT,DataT> at(*this);
    DLIterC<KeyT> it(lst);
    for(;it && at.IsValid();it++) {
      if(!at.Child(*it,ret))
	return HashTreeNodeC<KeyT,DataT>(); // Child not found.
      at = HashTreeC<KeyT,DataT>(ret);
    }
    if(it) // Did we reach the end of the path ?
      return HashTreeNodeC<KeyT,DataT>(); // Child not found.
    return ret;
  }
  
}


#endif
