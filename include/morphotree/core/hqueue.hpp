#pragma once 

#include "morphotree/core/types.hpp"
#include "morphotree/core/alias.hpp"

#include <queue>
#include <vector> 

namespace morphotree 
{

  // hierarchical queues implemented by red black tree.
  template<class Key, class Value>
  class HQueue
  {
  public:
    using KeyType = typename Key;
    using ValueType = typename Value;


  private:
    class HQNode
    {
    public:
      HQNode *left;
      HQNode *right;
      KeyType key;
      std::queue<ValueType> queue;
    };

    using NodeType = typename HQueue<KeyType, ValueType>::HQNode;
  public:
    HQueue();

    void insert(KeyType k, ValueType v);
    ValueType pop(KeyType k);
    ValueType front(KeyType k) const;
    inline bool isEmpty() const { return root_ == nullptr; }

  private:
    NodeType* find(KeyType k);
    NodeType* rfind(NodeType *node, KeyType k)

  private:
    NodeType *root_;
  };

  // hierarchical queue
  template<uint8, class Value>
  class HQueue
  {
  public:
    using KeyType = uint8;
    using ValueType = typename Key;

    HQueue();
    
    void insert(KeyType k, ValueType v);
    ValueType pop(KeyType k);
    ValueType front(KeyType k) const;
    inline bool isEmpty() const { return numberOfElements == 0; } ;

  private:
    std::vector<std::queue<ValueType>> queues_;
    uint32 numberOfElements;
  };

  // =====================[ IMPLEMENTATION ] ===========================================
  // Hierarchical Queue implemented by Red and Black Queue
  template<class Key, class Value>
  HQueue<Key, Value>::HQueue()
  {}

  template<class Key, class Value>
  void HQueue<Key, Value>::insert(KeyType k, ValueType v)
  {
    NodeType *node = find(k);
    if (node == nullptr)
      createQueue(k);
    else
      node->queue.push(v);
  }

  template<class Key, class Value>
  HQueue<Key, Value>::ValueType HQueue<Key, Value>::pop(KeyType k)
  {
    NodeType *node = find(k);
    
    if (node == nullptr)
      return ValueType();
    else {
      Value v = node->queue.front();
      node->queue.pop();
      if (node->queue.empty())
        removeQueue(node);
    }
  }

  template<class Key, class Value>
  HQueue<Key, Value>::ValueType HQueue<Key, Value>::front(KeyType k) const
  {
    NodeType *node = find(k);
    if (node == nullptr)
      return ValueType();
    else
      return node->queue.front();
  }

  template<class Key, class Value>
  HQueue<Key, Value>::NodeType* HQueue<Key, Value>::find(KeyType k)
  {
    return rfind(root_, k);
  }

  template<class Key, class Value>
  HQueue<Key, Value>::NodeType* HQueue<Key, Value>::rfind(NodeType *node, KeyType k)
  {
    if (node == nullptr)
      return nullptr;
    else if (node->key < k)
      return rfind(node->right);
    else if (node->key > k)
      return rfind(node->left);
    else 
      return node;
  }
} 