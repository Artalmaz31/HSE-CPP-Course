#include "lru_cache.h"

LRUCache::LRUCache(size_t capacity) {
    capacity_ = capacity;
}

void LRUCache::Update() {
    while (Size() > capacity_ && !lru_list_.empty()) {
        cache_.erase(lru_list_.back().key);
        lru_list_.pop_back();
    }
}

size_t LRUCache::Size() const {
    return lru_list_.size() + pinned_list_.size();
}

size_t LRUCache::Capacity() const {
    return capacity_;
}

void LRUCache::Clear() noexcept {
    lru_list_.clear();
    pinned_list_.clear();
    cache_.clear();
}

std::optional<int> LRUCache::Get(const std::string& key) {
    auto it = cache_.find(key);
    if (it == cache_.end()) {
        return std::nullopt;
    }
    if (!(it->second.node_it->is_pinned)) {
        lru_list_.splice(lru_list_.begin(), lru_list_, it->second.node_it);
    }
    return it->second.value;
}

bool LRUCache::Put(const std::string& key, int value) {
    auto it = cache_.find(key);
    if (it == cache_.end()) {
        if (capacity_ == pinned_list_.size()) {
            return false;
        }
        lru_list_.emplace_front(Node{key, false});
        cache_[key] = CacheItem{value, lru_list_.begin()};
        Update();
        return true;
    } else {
        Get(key);
        it->second.value = value;
        return false;
    }
}

bool LRUCache::Erase(const std::string& key) {
    auto it = cache_.find(key);
    if (it == cache_.end()) {
        return false;
    }
    if (it->second.node_it->is_pinned) {
        pinned_list_.erase(it->second.node_it);
    } else {
        lru_list_.erase(it->second.node_it);
    }
    cache_.erase(key);
    return true;
}

bool LRUCache::Pin(const std::string& key) {
    auto it = cache_.find(key);
    if (it == cache_.end() || it->second.node_it->is_pinned) {
        return false;
    }
    pinned_list_.splice(pinned_list_.end(), lru_list_, it->second.node_it);
    it->second.node_it->is_pinned = true;
    return true;
}

bool LRUCache::Unpin(const std::string& key) {
    auto it = cache_.find(key);
    if (it == cache_.end() || !(it->second.node_it->is_pinned)) {
        return false;
    }
    lru_list_.splice(lru_list_.begin(), pinned_list_, it->second.node_it);
    it->second.node_it->is_pinned = false;
    Update();
    return true;
}

void LRUCache::Merge(LRUCache& other) {
    if (this == &other) {
        return;
    }
    for (auto it = other.pinned_list_.end(); it != other.pinned_list_.begin();) {
        --it;
        if (cache_.find(it->key) == cache_.end()) {
            pinned_list_.emplace_front(*it);
            cache_[it->key] = CacheItem{other.cache_[it->key].value, pinned_list_.begin()};
        }
    }
    for (auto it = other.lru_list_.end(); it != other.lru_list_.begin();) {
        --it;
        if (cache_.find(it->key) == cache_.end()) {
            lru_list_.emplace_front(*it);
            cache_[it->key] = CacheItem{other.cache_[it->key].value, lru_list_.begin()};
        }
    }
    Update();
}
