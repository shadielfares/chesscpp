#pragma once
#include <cstdint>

// pseudo-legal targets for the single piece on `square`. king-safety is
// filtered later in legal_moves; castling path-safety is handled here.
uint64_t pseudo_targets(int square, bool white);
