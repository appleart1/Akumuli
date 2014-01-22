/**
 * PRIVATE HEADER
 *
 * Data structures for main memory storage.
 *
 * Copyright (c) 2013 Eugene Lazin <4lazin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */


#pragma once
#include "page.h"

#include <cpp-btree/btree_map.h>

#include <tuple>
#include <vector>
#include <memory>

namespace Akumuli {

class Generation {
    Generation(Generation const& other);
public:
    //! TTL
    TimeDuration ttl_;

    //! Max generation size
    size_t max_size_;

    //! Container type
    typedef btree::btree_multimap<std::tuple<TimeStamp, ParamId>, EntryOffset> MapType;

    //! Dictionary
    MapType data_;

    //! Starting index
    uint32_t starting_index_;

    //! Normal c-tor
    Generation(TimeDuration ttl, size_t max_size, uint32_t starting_index) noexcept;

    //! Move c-tor
    Generation(Generation && other) noexcept;

    //! Add item to cache
    void add(TimeStamp ts, ParamId param, EntryOffset  offset) noexcept;

    /** Search for elements.
     *  @param ts time stamp
     *  @param pid parameter id
     *  @param results destination array
     *  @param results_len size of the destination array
     *  @param skip number of elements to skip
     *  @return number of returned elements x is there any elements remaining
     */
    std::pair<size_t, bool> find(TimeStamp ts, ParamId pid, EntryOffset* results, size_t results_len, size_t skip) noexcept;

    /** Get the oldest timestamp of the generation.
     *  If generation is empty - return false, true otherwise.
     */
    bool get_oldest_timestamp(TimeStamp* ts) const noexcept;
};


class Cache {
    PageHeader* page_;
    TimeDuration ttl_;
    // Index structures
    std::vector<Generation> gen_;
public:
    Cache(TimeDuration ttl, PageHeader* page);

    void add_entry(const Entry& entry, EntryOffset offset) noexcept;

    void add_entry(const Entry2& entry, EntryOffset offset) noexcept;
};

}