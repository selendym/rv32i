#pragma once

// bit utils



namespace cpu_emu::util {

// FUNCTION DEFINITIONS

// Returns a bit mask with bits [`left`, `right`] (inclusive) set.
// `T_word`: an unsigned integer type of exact length; e.g., `std::uint32_t`.
// `Word_length`: an unsigned integer specifying `T_word` length; e.g., `32`.
template<
    typename T_word,
    unsigned Word_length
>
constexpr
T_word
bit_mask(
    unsigned left,
    unsigned right
)
{
    constexpr T_word mask_all = -1;

    if ( left >= Word_length || right >= Word_length ) {
        return 0;
    }

    return mask_all >> (Word_length - 1 - left)
         & mask_all << right;
}

// Returns the bits [`left`, `right`] (inclusive) from `word` shifted properly.
// `T_word`: an unsigned integer type of exact length; e.g., `std::uint32_t`.
// `Word_length`: an unsigned integer specifying `T_word` length; e.g., `32`.
template<
    typename T_word,
    unsigned Word_length
>
constexpr
T_word
bit_extract(
    T_word word,
    unsigned left,
    unsigned right
)
{
    return (word & bit_mask< T_word, Word_length >( left, right )) >> right;
}

}  // END cpu_emu::util
