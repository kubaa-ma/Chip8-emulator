/*
    By a uninix3
*/

#pragma once

#define clear_bit(_m, _b) (_m) &= ~(1 << (_b))
#define set_bit(_m, _b) (_m) |= (1 << (_b))
#define toggle_bit(_m, _b) (_m) ^= (1 << (_b))
#define read_bit(_m, _b) ((_m) & (1 << (_b)))

#define clear_mask(_m, _b) (_m) &= ~(_b)
#define set_mask(_m, _b) (_m) |= (_b)
#define toggle_mask(_m, _b) (_m) ^= (_b)
#define read_mask(_m, _b) ((_m) & (_b))
