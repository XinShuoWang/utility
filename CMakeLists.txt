cmake_minimum_required(VERSION 3.1)

project(utility)

include_directories(include/utility)

# FOR arena test
add_executable(arena_test test/arena_test.cc)
target_link_libraries(arena_test pthread)

# FOR hash test
add_executable(hash_test test/hash_test.cc)

# FOR logger tets
add_executable(logger_test test/logger_test.cc)

# FOR Mutex
add_executable(mutex_test test/mutex_test.cc)

# FOR read write lock
add_executable(read_write_lock_test test/read_write_lock_test.cc)
target_link_libraries(read_write_lock_test pthread)

# FOR spin lock
add_executable(spin_lock_test test/spin_lock_test.cc)

# FOR thread pool
add_executable(thread_pool_test test/thread_pool_test.cc)
target_link_libraries(thread_pool_test pthread)

# FOR branch prediction
add_executable(branch_prediction_test test/branch_prediction_test.cc)

# FOR cpu affinity
add_executable(cpu_affinity_test test/cpu_affinity_test.cc)
target_link_libraries(cpu_affinity_test pthread)

# FOR cache prefetch
add_executable(cache_prefetch_test test/cache_prefetch_test.cc)

# FOR random generator
add_executable(random_generator_test test/random_test.cc)