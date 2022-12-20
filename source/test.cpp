#include<iostream>
#include<string>
#include "hh_stl_vector.h"
#include "hh_stl_alloc.h"
#include "hh_stl_list.h"
#include "hh_stl_deque.h"
#include "hh_stl_stack.h"
#include "hh_stl_queue.h"
#include "hh_stl_heap.h"
#include "hh_stl_priority_queue.h"
#include "hh_stl_slist.h"
#include "hh_stl_tree.h"
#include "hh_stl_functional.h"
#include "hh_stl_pair.h"
#include "hh_stl_set.h"
#include "hh_stl_map.h"
#include "hh_stl_pair.h"
#include "hh_stl_multiset.h"
#include "hh_stl_multimap.h"
#include "hh_stl_hash_fun.h"
#include "hh_stl_hashtable.h"
#include "hh_stl_hashset.h"
#include "hh_stl_hash_multiset.h"
#include "hh_stl_hashmap.h"
#include "hh_stl_hash_multimap.h"
#include "hh_stl_numeric.h"
#include "hh_stl_algobase.h"
#include "hh_stl_algo.h"
#include<algorithm>
#include<iterator>
#include<iostream>
#include<cmath>

int main() {
    auto op = hh_std::bind2nd(hh_std::plus<int>(), 1);
    std::cout << op(1) << std::endl;
    return 0;
}
