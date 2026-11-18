#include <OpenGL/gl.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define VAC_SIZE 32
#define VAC_TOTAL_PIXELS (VAC_SIZE * VAC_SIZE) /* 1024 */
#define VAC_STIPPLE_BYTES (VAC_TOTAL_PIXELS / 8) /* 128 bytes */

typedef struct {
    uint16_t rank_matrix[VAC_SIZE][VAC_SIZE];
} VacStipple;

/* Simple deterministic xorshift32 PRNG for Fisher-Yates shuffle */
static uint32_t vac_xorshift32(uint32_t *state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

/* Precompute toroidal (wrap-around) Gaussian energy kernel */
static void vac_build_gaussian_lut(double sigma, double lut[VAC_SIZE][VAC_SIZE]) {
    double two_sigma_sq = 2.0 * sigma * sigma;
    for (int dy = 0; dy < VAC_SIZE; ++dy) {
        int wy = (dy < VAC_SIZE - dy) ? dy : (VAC_SIZE - dy);
        for (int dx = 0; dx < VAC_SIZE; ++dx) {
            int wx = (dx < VAC_SIZE - dx) ? dx : (VAC_SIZE - dx);
            double dist_sq = (double)(wx * wx + wy * wy);
            lut[dy][dx] = exp(-dist_sq / two_sigma_sq);
        }
    }
}

/* Find pixel with highest cluster energy (among 1s) or lowest void energy (among 0s) */
static int vac_find_extreme(const uint8_t grid[VAC_TOTAL_PIXELS],
                            const double energy[VAC_TOTAL_PIXELS],
                            uint8_t target_state,
                            int find_max) {
    int best_idx = -1;
    double best_val = find_max ? -1e30 : 1e30;

    for (int i = 0; i < VAC_TOTAL_PIXELS; ++i) {
        if (grid[i] == target_state) {
            if ((find_max && energy[i] > best_val) || (!find_max && energy[i] < best_val)) {
                best_val = energy[i];
                best_idx = i;
            }
        }
    }
    return best_idx;
}

/* Incrementally update energy field when toggling a single pixel */
static void vac_update_energy(double energy[VAC_TOTAL_PIXELS],
                              const double lut[VAC_SIZE][VAC_SIZE],
                              int idx,
                              double sign) {
    int px = idx % VAC_SIZE;
    int py = idx / VAC_SIZE;
    for (int y = 0; y < VAC_SIZE; ++y) {
        int dy = abs(y - py);
        for (int x = 0; x < VAC_SIZE; ++x) {
            int dx = abs(x - px);
            energy[y * VAC_SIZE + x] += sign * lut[dy][dx];
        }
    }
}

/* Initialize the 32x32 Void-and-Cluster rank matrix (call once at startup) */
void vac_generate_matrix(VacStipple *vac, double sigma, uint32_t seed) {
    double lut[VAC_SIZE][VAC_SIZE];
    uint8_t grid[VAC_TOTAL_PIXELS];
    double energy[VAC_TOTAL_PIXELS];
    int indices[VAC_TOTAL_PIXELS];

    if (seed == 0) seed = 42; /* xorshift32 requires a non-zero seed */

    vac_build_gaussian_lut(sigma, lut);
    memset(grid, 0, sizeof(grid));
    memset(energy, 0, sizeof(energy));

    /* 1. Seed ~10% initial minority pixels (102 pixels) via Fisher-Yates shuffle */
    int initial_ones = VAC_TOTAL_PIXELS / 10;
    for (int i = 0; i < VAC_TOTAL_PIXELS; ++i) {
        indices[i] = i;
    }
    for (int i = VAC_TOTAL_PIXELS - 1; i > 0; --i) {
        int j = (int)(vac_xorshift32(&seed) % (uint32_t)(i + 1));
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }
    for (int i = 0; i < initial_ones; ++i) {
        grid[indices[i]] = 1;
        vac_update_energy(energy, lut, indices[i], +1.0);
    }

    /* 2. Relax initial binary pattern (break tightest cluster, fill largest void) */
    while (1) {
        int tightest_cluster = vac_find_extreme(grid, energy, 1, 1);
        grid[tightest_cluster] = 0;
        vac_update_energy(energy, lut, tightest_cluster, -1.0);

        int largest_void = vac_find_extreme(grid, energy, 0, 0);
        if (largest_void == tightest_cluster) {
            grid[tightest_cluster] = 1;
            vac_update_energy(energy, lut, tightest_cluster, +1.0);
            break;
        }
        grid[largest_void] = 1;
        vac_update_energy(energy, lut, largest_void, +1.0);
    }

    uint8_t initial_grid[VAC_TOTAL_PIXELS];
    double initial_energy[VAC_TOTAL_PIXELS];
    memcpy(initial_grid, grid, sizeof(grid));
    memcpy(initial_energy, energy, sizeof(energy));

    /* 3. Phase 1: Remove 1s down to 0 to assign ranks [initial_ones - 1 .. 0] */
    for (int rank = initial_ones - 1; rank >= 0; --rank) {
        int tightest = vac_find_extreme(grid, energy, 1, 1);
        grid[tightest] = 0;
        vac_update_energy(energy, lut, tightest, -1.0);
        vac->rank_matrix[tightest / VAC_SIZE][tightest % VAC_SIZE] = (uint16_t)rank;
    }

    /* 4. Phase 2: Insert 1s up to 1024 to assign ranks [initial_ones .. 1023] */
    memcpy(grid, initial_grid, sizeof(grid));
    memcpy(energy, initial_energy, sizeof(energy));
    for (int rank = initial_ones; rank < VAC_TOTAL_PIXELS; ++rank) {
        int largest_void = vac_find_extreme(grid, energy, 0, 0);
        grid[largest_void] = 1;
        vac_update_energy(energy, lut, largest_void, +1.0);
        vac->rank_matrix[largest_void / VAC_SIZE][largest_void % VAC_SIZE] = (uint16_t)rank;
    }
}

/* Build 128-byte (1024-bit) stipple mask for glPolygonStipple */
void vac_build_mask(const VacStipple *vac, float alpha, GLubyte out_mask[VAC_STIPPLE_BYTES]) {
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;

    int threshold = (int)lroundf(alpha * (float)VAC_TOTAL_PIXELS);

    for (int y = 0; y < VAC_SIZE; ++y) {
        for (int byte_idx = 0; byte_idx < 4; ++byte_idx) {
            GLubyte byte_val = 0;
            int base_x = byte_idx * 8;
            for (int bit = 0; bit < 8; ++bit) {
                if (vac->rank_matrix[y][base_x + bit] < threshold) {
                    byte_val |= (GLubyte)(1u << (7 - bit)); /* OpenGL MSB-first default */
                }
            }
            out_mask[y * 4 + byte_idx] = byte_val;
        }
    }
}
