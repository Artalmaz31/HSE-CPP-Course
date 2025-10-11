#pragma once

#include <cstddef>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>

struct Node {
    std::string key;
    bool is_pinned;
};

struct CacheItem {
    int value;
    std::list<Node>::iterator node_it;
};

class LRUCache {
public:
    explicit LRUCache(size_t capacity);

    size_t Size() const;
    size_t Capacity() const;

    void Clear() noexcept;

    std::optional<int> Get(const std::string& key);
    bool Put(const std::string& key, int value);
    bool Erase(const std::string& key);

    bool Pin(const std::string& key);
    bool Unpin(const std::string& key);

    void Merge(LRUCache& other);

private:
    size_t capacity_;
    std::list<Node> lru_list_;
    std::list<Node> pinned_list_;
    std::unordered_map<std::string, CacheItem> cache_;
    
    void Update();
};
