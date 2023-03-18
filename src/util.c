#include "util.h"


//==============================================================================
// STC64 PRNG, I have extracted it from STC,
// thus this part is under MIT.
//
/* MIT License
 *
 * Copyright (c) 2022 Tyge Løvset, NORCE, www.norceresearch.no
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


/* Global random() */
stc64_rng stc64_global = {{
    0x26aa069ea2fb1a4d, 0x70c72c95cd592d04,
    0x504f333d3aa0b359, 0x9e3779b97f4a7c15,
    0x6a09e667a754166b
}};

/* rng.state[4] must be odd */
stc64_rng stc64_with_seq(uint64_t seed, uint64_t seq) {
    stc64_rng rng = {{seed+0x26aa069ea2fb1a4d, seed+0x70c72c95cd592d04,
                      seed+0x504f333d3aa0b359, seed, seq<<1 | 1}};
    for (int i = 0; i < 6; ++i) stc64_random(&rng);
    return rng;
}

//==============================================================================
