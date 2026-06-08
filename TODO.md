# todo

## decision
native raylib GUI, not CLI/web. real piece textures, mouse drag-to-move, pure C++ (no JS/embind).

changed my mind from "keep it simple, CLI-only": a terminal board looks really bad to actually play,
and i wanted something demoable. raylib gives real textures + mouse without an API or frontend stack.
note: API + custom chess platform still parked, not worth the time now.

## done
- move gen, all 6 pieces (src/pieces/); sliders/knight/king take the piece set as a param so they're reusable.
- legal filtering: squares_attacked_by -> in_check -> legal_moves (make/unmake king-safety). board_ops.cpp
- make_move: captures, en passant, castling, promotion (+ under-promo), turn enforcement.
- raylib GUI: textured drag-to-move, legal-target dots, illegal drops rejected, promotion picker.
- game over (no legal move) closes the app. has_legal_move() in board_ops.

## next
- proper end screen instead of closing: show mate (in_check) vs stalemate, winner.

## remaining
- 16-bit Move encoding (from/to/flags, the MoveInfo stub) + perft + search. flag-driven special moves replace state inference; under-promotion choice rides in the flags.
- draws: threefold repetition, 50-move rule, insufficient material.
- broken test infra: chess_tests target only compiles tests/test.pawn.cpp (main lives in tests/runner.cpp, unwired); new tests/test.*.cpp files not in CMake.
- en passant generators still return std::vector (the 3 raw pawn fns); could retire once everything uses pawn_moves / the Move encoding.
