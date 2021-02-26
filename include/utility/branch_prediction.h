//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_BRANCH_PREDICTION_H_
#define UTILITY_INCLUDE_UTILITY_BRANCH_PREDICTION_H_

#define LIKELY(x) (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect(!!(x), 0))

#endif //UTILITY_INCLUDE_UTILITY_BRANCH_PREDICTION_H_
