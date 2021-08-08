//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_INDEX_MAP_H
#define TENSAT_INDEX_MAP_H

template<class T>
class RawTable
{

};

template<class K, class V>
class Bucket
{

};

template<class K, class V>
class IndexMapCore
{
    RawTable<size_t> indices;
    std::vector<Bucket<K, V>> entries;
};

template<class K, class V, class S>
class IndexMap
{
public:
    IndexMapCore<K, V> map;
    S hash_builder;
    template<class Q>
    V get_mut(Q q)
    {
        // TODO: should return std::optional<ref>
        return v;
    }

    void insert(const K &k, const V &v)
    {
        // TODO:
    }

    bool is_empty()
    {
        return false;
    }

    std::optional<V> operator[](const K& k)
    {
        // TODO:if not exist, return std::nullopt
        return v;
    }
    V v;
};
IndexMap<std::string, int, int> DefaultIndexMap()
{
    return {};
}


#endif //TENSAT_INDEX_MAP_H
