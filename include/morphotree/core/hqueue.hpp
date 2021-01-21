#pragma once 

#include "morphotree/core/types.hpp"
#include "morphotree/core/alias.hpp"

#include <queue>
#include <vector> 
#include <array>

namespace morphotree 
{

  // hierarchical queues implemented by red black tree.
  template<class Key, class Value>
  class HQueue
  {
  public:
    using KeyType = Key;
    using ValueType = Value;

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
    NodeType* rfind(NodeType *node, KeyType k);

  private:
    NodeType *root_;
  };

  // hierarchical queue
  template<class Value>
  class HQueue<uint8, Value>
  {
  public:
    using KeyType = uint8;
    using ValueType = Value;

    HQueue();
    
    void insert(KeyType k, ValueType v);
    ValueType pop(KeyType k);
    ValueType front(KeyType k) const;
    inline bool isEmpty() const { return numberOfElements == 0; } ;

  private: // methods
    ValueType popAnElement(KeyType k);
    bool findClosestNonEmptyQueue(KeyType k, KeyType &foundValue) const;

  private:
    std::array<std::queue<ValueType>, 256> queues_;
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
  typename HQueue<Key, Value>::ValueType HQueue<Key, Value>::pop(KeyType k)
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
  typename HQueue<Key, Value>::ValueType HQueue<Key, Value>::front(KeyType k) const
  {
    NodeType *node = find(k);
    if (node == nullptr)
      return ValueType();
    else
      return node->queue.front();
  }

  template<class Key, class Value>
  typename HQueue<Key, Value>::NodeType* HQueue<Key, Value>::find(KeyType k)
  {
    return rfind(root_, k);
  }

  template<class Key, class Value>
  typename HQueue<Key, Value>::NodeType* HQueue<Key, Value>::rfind(NodeType *node, KeyType k)
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

  // =====================[ IMPLEMENTATION ] ===========================================
  // Hierarchical Queue using simple array

  template<class Value>
  HQueue<uint8, Value>::HQueue()
    :numberOfElements{0}
  {}
    
  template<class Value>
  void HQueue<uint8, Value>::insert(KeyType k, ValueType v)
  {
    queues_[k].push(v);
    numberOfElements++;
  }

  template<class Value>
  typename HQueue<uint8, Value>::ValueType HQueue<uint8, Value>::pop(KeyType k)
  {
    if (!isEmpty()) { 
      uint8 closestQueue = 0;
      if (findClosestNonEmptyQueue(k, closestQueue)) {
        return popAnElement(closestQueue);
      }
    }
    return HQueue<uint8, ValueType>::ValueType();
  }

  template<class Value>
  bool HQueue<uint8, Value>::findClosestNonEmptyQueue(KeyType k, KeyType &foundValue) const 
  {
    if (!queues_[k].empty()) {
      foundValue = k;
      return true;
    }
    else {
      int32 l = k-1;
      int32 u = k+1;

      while (0 <= l && u < 256) {
        if (!queues_[l].empty()) {
          foundValue = l;
          return true;
        }
        if (!queues_[u].empty()) {
          foundValue = u;
          return true;
        }
        u++;
        l--;
      }

      while (0 <= l)
      {
        if (!queues_[l].empty()) {
          foundValue = l;
          return true;
        }
        l--;
      }
      
      while (u < 256)
      {
        if (!queues_[u].empty()){
          foundValue = u;
          return true;
        }
        u++;
      }
    }
    return false;
  }


  template<class Value>
  typename HQueue<uint8, Value>::ValueType HQueue<uint8, Value>::popAnElement(KeyType k)
  {
    HQueue<uint8, Value>::ValueType v = queues_[k].front();
    numberOfElements--;
    queues_[k].pop();
    return v;
  }

  template<class Value>
  typename HQueue<uint8, Value>::ValueType HQueue<uint8, Value>::front(KeyType k) const
  {
    if (!isEmpty()) {
      uint8 closestQueue = 0;
      if (findClosestNonEmptyQueue(k, closestQueue)) {
        return queues_[closestQueue];
      }
    }
    return HQueue<uint8, Value>::ValueType();
  }

} 