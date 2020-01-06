#pragma once

namespace Framework::Utility {
    /**
     * @brief マップにキーが含まれているか
     */
    template <class Key, class Value>
    inline bool isExist(const std::unordered_map<Key, Value>& map, const Key& key) {
        return map.find(key) != map.end();
    }
} // namespace Framework::Utility
