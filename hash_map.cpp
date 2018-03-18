#include <vector>
#include <list>
#include <utility>
#include <stdexcept>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    Hash hasher;
    std::vector<std::pair<typename std::list<std::pair<const KeyType, ValueType>>::iterator, size_t>> hash_table;
    std::list<std::pair<const KeyType, ValueType>> all_elements;
    size_t element_count;

public:
    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;

    HashMap(Hash hasher = Hash()) :hasher(hasher) {
        element_count = 0;
    }

    HashMap(const HashMap& other) :hasher(other.hasher) {
        clear();
        hash_table.resize(other.size());
        auto start = other.begin();
        while (start != other.end()) {
            insert(*start);
            ++start;
        }
    }

    HashMap(HashMap&& other) :hasher(other.hasher),
        hash_table(std::move(other.hash_table)),
        all_elements(std::move(other.all_elements)),
        element_count(other.element_count) {
        other.element_count = 0;
    }

    HashMap& operator=(const HashMap& other) {
        if (this == &other)
            return *this;
        clear();
        hash_table.resize(other.size());
        auto start = other.begin();
        while (start != other.end()) {
            insert(*start);
            ++start;
        }
        return *this;
    }

    HashMap& operator=(HashMap&& other) {
        hash_table = std::move(other.hash_table);
        all_elements = std::move(other.all_elements);
        element_count = other.element_count;
        other.element_count = 0;
        return *this;
    }

    size_t size() const {
        return element_count;
    }

    bool empty() const {
        return element_count == 0;
    }

    Hash hash_function() const {
        return hasher;
    }

    iterator begin() {
        return all_elements.begin();
    }

    iterator end() {
        return all_elements.end();
    }

    const_iterator begin() const {
        return all_elements.begin();
    }

    const_iterator end() const {
        return all_elements.end();
    }

    void expand_table() {
        size_t new_size = hash_table.size() * 2 + 1;

        HashMap<KeyType, ValueType, Hash> new_hash_map(begin(), end(), new_size, hasher);


        *this = std::move(new_hash_map);
    }

    iterator find(const KeyType& key) {
        if (hash_table.size() == 0)
            return end();
        size_t hash = hasher(key) % hash_table.size();
        iterator iter = hash_table[hash].first;
        for (size_t i = 0; i != hash_table[hash].second; ++i, ++iter) {
            if (iter->first == key)
                return iter;
        }
        return end();
    }

    const_iterator find(const KeyType& key) const {
        if (hash_table.size() == 0)
            return end();
        size_t hash = hasher(key) % hash_table.size();
        const_iterator iter = hash_table[hash].first;
        for (size_t i = 0; i != hash_table[hash].second; ++i, ++iter) {
            if (iter->first == key)
                return iter;
        }
        return end();
    }

    iterator insert(const std::pair<const KeyType, ValueType>& element) {
        if (element_count == hash_table.size())
            expand_table();
        auto found = find(element.first);
        if (found != end())
            return found;
        size_t hash = hasher(element.first) % hash_table.size();
        if (hash_table[hash].second == 0) {
            hash_table[hash].first = all_elements.end();
        }
        auto new_element = all_elements.insert(hash_table[hash].first, element);
        hash_table[hash].first = new_element;
        ++hash_table[hash].second;
        ++element_count;
        return new_element;
    }

    void erase(const KeyType& key) {
        auto found = find(key);
        if (found == end())
            return;
        size_t hash = hasher(key) % hash_table.size();
        if (found == hash_table[hash].first) {
            ++hash_table[hash].first;
        }
        all_elements.erase(found);
        --hash_table[hash].second;
        --element_count;
    }

    ValueType& operator[](const KeyType& key) {
        auto found = find(key);
        if (found == end()) {
            return insert(std::pair<const KeyType, ValueType>(key, ValueType()))->second;
        }
        return found->second;
    }

    const ValueType& at(const KeyType& key) const {
        auto found = find(key);
        if (found == end())
            throw std::out_of_range("No such element in HashMap");
        return found->second;
    }

    template<class Iter>
    HashMap(Iter start, Iter finish, Hash hasher = Hash()) :hasher(hasher) {
        element_count = 0;
        while (start != finish) {
            insert(*start);
            ++start;
        }
    }

    template<class Iter>
    HashMap(Iter start, Iter finish, size_t _size, Hash hasher = Hash()) :hasher(hasher) {
        element_count = 0;
        hash_table.resize(_size);
        while (start != finish) {
            insert(*start);
            ++start;
        }
    }

    HashMap(std::initializer_list<std::pair<const KeyType, ValueType>> init_list, Hash hasher = Hash()) :
        HashMap(init_list.begin(), init_list.end(), init_list.size(), hasher) {}

    void clear() {
        hash_table.clear();
        all_elements.clear();
        element_count = 0;
    }
};
