/*
 * SPDX-FileCopyrightText: 2020-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*******************************************************************************
 * NOTICE
 * The hal is not public api, don't use in application code.
 * See readme.md in hal/include/hal/readme.md
 ******************************************************************************/

// The LL layer for ESP32-S2 I2S register operations

#pragma once

#include <stdbool.h>
#include "hal/misc.h"
#include "soc/i2s_periph.h"
#include "soc/i2s_struct.h"
#include "hal/i2s_types.h"


#ifdef __cplusplus
extern "C" {
#endif

// Get I2S hardware instance with giving i2s num
#define I2S_LL_GET_HW(num)             (((num) == 0) ? (&I2S0) : NULL)

#define I2S_LL_BASE_CLK                (2 * APB_CLK_FREQ)

#define I2S_LL_BCK_MAX_PRESCALE  (64)

#define I2S_LL_MCLK_DIVIDER_BIT_WIDTH  (6)
#define I2S_LL_MCLK_DIVIDER_MAX        ((1 << I2S_LL_MCLK_DIVIDER_BIT_WIDTH) - 1)

#define I2S_LL_EVENT_RX_EOF         BIT(9)
#define I2S_LL_EVENT_TX_EOF         BIT(12)
#define I2S_LL_EVENT_RX_DSCR_ERR    BIT(13)
#define I2S_LL_EVENT_TX_DSCR_ERR    BIT(14)
#define I2S_INTR_MAX                (UINT32_MAX)

#define I2S_LL_TX_EVENT_MASK        I2S_LL_EVENT_TX_EOF
#define I2S_LL_RX_EVENT_MASK        I2S_LL_EVENT_RX_EOF

/* I2S clock configuration structure */
typedef struct {
    uint16_t mclk_div; // I2S module clock divider, Fmclk = Fsclk /(mclk_div+b/a)
    uint16_t a;
    uint16_t b;        // The decimal part of module clock divider, the decimal is: b/a
} i2s_ll_mclk_div_t;

/**
 * @brief Enable DMA descriptor owner check
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param en whether to enable owner check
 */
static inline void i2s_ll_dma_enable_owner_check(i2s_dev_t *hw, bool en)
{
    hw->lc_conf.check_owner = en;
}

/**
 * @brief Enable DMA descriptor write back
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param en whether to enable write back
 */
static inline void i2s_ll_dma_enable_auto_write_back(i2s_dev_t *hw, bool en)
{
    hw->lc_conf.out_auto_wrback = en;
}

/**
 * @brief I2S DMA generate EOF event on data in FIFO poped out
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param en True to enable, False to disable
 */
static inline void i2s_ll_dma_enable_eof_on_fifo_empty(i2s_dev_t *hw, bool en)
{
    hw->lc_conf.out_eof_mode = en;
}

/**
 * @brief I2S module general init, enable I2S clock.
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_enable_clock(i2s_dev_t *hw)
{
    if (hw->clkm_conf.clk_en == 0) {
        hw->clkm_conf.clk_sel = 2;
        hw->clkm_conf.clk_en = 1;
        hw->conf2.val = 0;
    }
}

/**
 * @brief I2S module disable clock.
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_disable_clock(i2s_dev_t *hw)
{
    if (hw->clkm_conf.clk_en == 1) {
        hw->clkm_conf.clk_en = 0;
    }
}

/**
 * @brief I2S tx msb right enable
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable tx msb right
 */
static inline void i2s_ll_tx_enable_msb_right(i2s_dev_t *hw, bool enable)
{
    hw->conf.tx_msb_right = enable;
}

/**
 * @brief I2S rx msb right enable
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable rx msb right
 */
static inline void i2s_ll_rx_enable_msb_right(i2s_dev_t *hw, bool enable)
{
    hw->conf.rx_msb_right = enable;
}

/**
 * @brief I2S tx right channel first
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable send right channel first
 */
static inline void i2s_ll_tx_enable_right_first(i2s_dev_t *hw, bool enable)
{
    hw->conf.tx_right_first = enable;
}

/**
 * @brief I2S rx right channel first
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable receive right channel first
 */
static inline void i2s_ll_rx_enable_right_first(i2s_dev_t *hw, bool enable)
{
    hw->conf.rx_right_first = enable;
}

/**
 * @brief I2S tx fifo module force enable
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable tx fifo module
 */
static inline void i2s_ll_tx_force_enable_fifo_mod(i2s_dev_t *hw, bool enable)
{
    hw->fifo_conf.tx_fifo_mod_force_en = enable;
}

/**
 * @brief I2S rx fifo module force enable
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable rx fifo module
 */
static inline void i2s_ll_rx_force_enable_fifo_mod(i2s_dev_t *hw, bool enable)
{
    hw->fifo_conf.rx_fifo_mod_force_en = enable;
}

/**
 * @brief Enable I2S TX slave mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param slave_en Set true to enable slave mode
 */
static inline void i2s_ll_tx_set_slave_mod(i2s_dev_t *hw, bool slave_en)
{
    hw->conf.tx_slave_mod = slave_en;
}

/**
 * @brief Enable I2S RX slave mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param slave_en Set true to enable slave mode
 */
static inline void i2s_ll_rx_set_slave_mod(i2s_dev_t *hw, bool slave_en)
{
    hw->conf.rx_slave_mod = slave_en;
}

/**
 * @brief Reset TX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_reset(i2s_dev_t *hw)
{
    hw->conf.tx_reset = 1;
    hw->conf.tx_reset = 0;
}

/**
 * @brief Reset RX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_reset(i2s_dev_t *hw)
{
    hw->conf.rx_reset = 1;
    hw->conf.rx_reset = 0;
}

/**
 * @brief Reset TX FIFO
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_reset_fifo(i2s_dev_t *hw)
{
    hw->conf.tx_fifo_reset = 1;
    hw->conf.tx_fifo_reset = 0;
}

/**
 * @brief Reset RX FIFO
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_reset_fifo(i2s_dev_t *hw)
{
    hw->conf.rx_fifo_reset = 1;
    hw->conf.rx_fifo_reset = 0;
}

/**
 * @brief Set TX source clock
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param src I2S source clock
 */
static inline void i2s_ll_tx_clk_set_src(i2s_dev_t *hw, i2s_clock_src_t src)
{
    hw->clkm_conf.clk_sel = (src == I2S_CLK_SRC_APLL) ? 1 : 2;
}

/**
 * @brief Set RX source clock
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param src I2S source clock
 */
static inline void i2s_ll_rx_clk_set_src(i2s_dev_t *hw, i2s_clock_src_t src)
{
    hw->clkm_conf.clk_sel = (src == I2S_CLK_SRC_APLL) ? 1 : 2;
}

/**
 * @brief Set I2S tx bck div num
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set tx bck div num
 */
static inline void i2s_ll_tx_set_bck_div_num(i2s_dev_t *hw, uint32_t val)
{
    hw->sample_rate_conf.tx_bck_div_num = val;
}

/**
 * @brief Configure I2S TX module clock divider
 * @note mclk on ESP32S2 is shared by both TX and RX channel
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param sclk system clock, 0 means use apll
 * @param mclk module clock
 * @param mclk_div integer part of the division from sclk to mclk
 */
static inline void i2s_ll_tx_set_mclk(i2s_dev_t *hw, uint32_t sclk, uint32_t mclk, uint32_t mclk_div)
{
    int ma = 0;
    int mb = 0;
    int denominator = 1;
    int numerator = 0;

    uint32_t freq_diff = abs((int)sclk - (int)(mclk * mclk_div));
    if (!freq_diff) {
        goto finish;
    }
    float decimal = freq_diff / (float)mclk;
    // Carry bit if the decimal is greater than 1.0 - 1.0 / (63.0 * 2) = 125.0 / 126.0
    if (decimal > 125.0 / 126.0) {
        mclk_div++;
        goto finish;
    }
    uint32_t min = ~0;
    for (int a = 2; a <= I2S_LL_MCLK_DIVIDER_MAX; a++) {
        int b = (int)(a * (freq_diff / (double)mclk) + 0.5);
        ma = freq_diff * a;
        mb = mclk * b;
        if (ma == mb) {
            denominator = a;
            numerator = b;
            goto finish;
        }
        if (abs((mb - ma)) < min) {
            denominator = a;
            numerator = b;
            min = abs(mb - ma);
        }
    }
finish:
    HAL_FORCE_MODIFY_U32_REG_FIELD(hw->clkm_conf, clkm_div_num, mclk_div);
    hw->clkm_conf.clkm_div_b = numerator;
    hw->clkm_conf.clkm_div_a = denominator;
}

/**
 * @brief Configure I2S module clock divider
 * @note mclk on ESP32 is shared by both TX and RX channel
 *       mclk = sclk / (mclk_div + b/a)
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param mclk_div integer part of the division from sclk to mclk
 * @param a Denominator of decimal part
 * @param b Numerator of decimal part
 */
static inline void i2s_ll_set_raw_mclk_div(i2s_dev_t *hw, uint32_t mclk_div, uint32_t a, uint32_t b)
{
    HAL_FORCE_MODIFY_U32_REG_FIELD(hw->clkm_conf, clkm_div_num, mclk_div);
    hw->clkm_conf.clkm_div_b = b;
    hw->clkm_conf.clkm_div_a = a;
}

/**
 * @brief Set I2S rx bck div num
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set rx bck div num
 */
static inline void i2s_ll_rx_set_bck_div_num(i2s_dev_t *hw, uint32_t val)
{
    hw->sample_rate_conf.rx_bck_div_num = val;
}

/**
 * @brief Configure I2S RX module clock divider
 * @note mclk on ESP32S2 is shared by both TX and RX channel
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param sclk system clock, 0 means use apll
 * @param mclk module clock
 * @param mclk_div integer part of the division from sclk to mclk
 */
static inline void i2s_ll_rx_set_mclk(i2s_dev_t *hw, uint32_t sclk, uint32_t mclk, uint32_t mclk_div)
{
    i2s_ll_tx_set_mclk(hw, sclk, mclk, mclk_div);
}

/**
 * @brief Enable interrupt by mask
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param mask Interrupt event mask
 * @param en true to enable, false to disable
 */
static inline void i2s_ll_enable_intr(i2s_dev_t *hw, uint32_t mask, bool en)
{
    if (en) {
        hw->int_ena.val |= mask;
    } else {
        hw->int_ena.val &= ~mask;
    }
}

/**
 * @brief Enable TX interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_enable_intr(i2s_dev_t *hw)
{
    hw->int_ena.out_eof = 1;
}

/**
 * @brief Disable TX interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_disable_intr(i2s_dev_t *hw)
{
    hw->int_ena.out_eof = 0;
}

/**
 * @brief Enable RX interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_enable_intr(i2s_dev_t *hw)
{
    hw->int_ena.in_suc_eof = 1;
}

/**
 * @brief Disable RX interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_disable_intr(i2s_dev_t *hw)
{
    hw->int_ena.in_suc_eof = 0;
}

/**
 * @brief Get interrupt status register address
 *
 * @param hw Peripheral I2S hardware instance address.
 * @return interrupt status register address
 */
static inline volatile void *i2s_ll_get_intr_status_reg(i2s_dev_t *hw)
{
    return &hw->int_st;
}

/**
 * @brief Get I2S interrupt status
 *
 * @param hw Peripheral I2S hardware instance address.
 * @return
 *        - module interrupt status
 */
static inline uint32_t i2s_ll_get_intr_status(i2s_dev_t *hw)
{
    return hw->int_st.val;
}

/**
 * @brief Get DMA interrupt status register address
 */
static inline volatile void *i2s_ll_get_interrupt_status_reg(i2s_dev_t *hw)
{
    return (volatile void *)(&hw->int_st);
}

/**
 * @brief Clear I2S interrupt status
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param clr_mask Interrupt mask to be cleared.
 */
static inline void i2s_ll_clear_intr_status(i2s_dev_t *hw, uint32_t clr_mask)
{
    hw->int_clr.val = clr_mask;
}

/**
 * @brief Reset dma out
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_reset_dma(i2s_dev_t *hw)
{
    hw->lc_conf.out_rst = 1;
    hw->lc_conf.out_rst = 0;
}

/**
 * @brief Reset dma in
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_reset_dma(i2s_dev_t *hw)
{
    hw->lc_conf.in_rst = 1;
    hw->lc_conf.in_rst = 0;
}

/**
 * @brief Enable I2S TX STD mode
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_enable_std(i2s_dev_t *hw)
{
    hw->conf2.val = 0;
}

/**
 * @brief Enable I2S RX STD mode
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_enable_std(i2s_dev_t *hw)
{
    hw->conf2.val = 0;
}

/**
 * @brief Enable TX PDM mode.
 * @note  ESP32-S2 doesn't support pdm
 *        This function is used to be compatible with those support pdm
 *
 * @param hw Peripheral I2S hardware instance address (ignored)
 */
static inline void i2s_ll_tx_enable_pdm(i2s_dev_t *hw)
{
    // Remain empty
}

/**
 * @brief Enable RX PDM mode.
 * @note  ESP32-S2 doesn't support pdm
 *        This function is used to be compatible with those support pdm
 *
 * @param hw Peripheral I2S hardware instance address (ignored)
 */
static inline void i2s_ll_rx_enable_pdm(i2s_dev_t *hw)
{
    // Remain empty
}

/**
 * @brief Start out link
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_start_out_link(i2s_dev_t *hw)
{
    hw->out_link.start = 1;
}

/**
 * @brief Set I2S out link address
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set out link address
 */
static inline void i2s_ll_set_out_link_addr(i2s_dev_t *hw, uint32_t val)
{
    hw->out_link.addr = val;
}

/**
 * @brief Start TX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_start(i2s_dev_t *hw)
{
    hw->conf.tx_start = 1;
}

/**
 * @brief Start RX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_start(i2s_dev_t *hw)
{
    hw->conf.rx_start = 1;
}

/**
 * @brief Configure TX DMA descriptor address and start TX DMA
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param link_addr DMA descriptor link address.
 */
static inline void i2s_ll_tx_start_link(i2s_dev_t *hw, uint32_t link_addr)
{
    i2s_ll_set_out_link_addr(hw, link_addr);
    i2s_ll_start_out_link(hw);
}

/**
 * @brief Configure RX DMA descriptor address and start TX DMA
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param link_addr DMA descriptor link address.
 */
static inline void i2s_ll_rx_start_link(i2s_dev_t *hw, uint32_t link_addr)
{
    hw->in_link.addr = link_addr;
    hw->in_link.start = 1;
}

/**
 * @brief Stop TX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_stop(i2s_dev_t *hw)
{
    hw->conf.tx_start = 0;
}

/**
 * @brief Stop RX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_stop(i2s_dev_t *hw)
{
    hw->conf.rx_start = 0;
}

/**
 * @brief Stop out link
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_stop_link(i2s_dev_t *hw)
{
    hw->out_link.stop = 1;
}

/**
 * @brief Stop in link
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_stop_link(i2s_dev_t *hw)
{
    hw->in_link.stop = 1;
}

/**
 * @brief Get I2S out eof descriptor address
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param eof_addr Pointer to accept out eof des address
 */
static inline void i2s_ll_tx_get_eof_des_addr(i2s_dev_t *hw, uint32_t *eof_addr)
{
    *eof_addr = hw->out_eof_des_addr;
}

/**
 * @brief Get I2S in eof descriptor address
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param eof_addr Pointer to accept in eof des address
 */
static inline void i2s_ll_rx_get_eof_des_addr(i2s_dev_t *hw, uint32_t *eof_addr)
{
    *eof_addr = hw->in_eof_des_addr;
}

/**
 * @brief Configure the received length to trigger in_suc_eof interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param eof_num the byte length to trigger in_suc_eof interrupt
 */
static inline void i2s_ll_rx_set_eof_num(i2s_dev_t *hw, uint32_t eof_num)
{
    hw->rx_eof_num = eof_num;
}

/**
 * @brief Congfigure TX chan bit and audio data bit, on ESP32-S2, sample_bit should equals to data_bit
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param chan_bit The chan bit width
 * @param data_bit The audio data bit width
 */
static inline void i2s_ll_tx_set_sample_bit(i2s_dev_t *hw, uint8_t chan_bit, int data_bit)
{
    hw->fifo_conf.tx_fifo_mod = (chan_bit <= I2S_DATA_BIT_WIDTH_16BIT ? 0 : 2);
    hw->sample_rate_conf.tx_bits_mod = data_bit;
}

/**
 * @brief Congfigure RX chan bit and audio data bit, on ESP32-S2, sample_bit should equals to data_bit
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param chan_bit The chan bit width
 * @param data_bit The audio data bit width
 */
static inline void i2s_ll_rx_set_sample_bit(i2s_dev_t *hw, uint8_t chan_bit, int data_bit)
{
    hw->fifo_conf.rx_fifo_mod = (chan_bit <= I2S_DATA_BIT_WIDTH_16BIT ? 0 : 2);
    hw->sample_rate_conf.rx_bits_mod = data_bit;
}

/**
 * @brief Enable I2S DMA
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param ena Set true to enable DMA
 */
static inline void i2s_ll_enable_dma(i2s_dev_t *hw, bool ena)
{
    hw->fifo_conf.dscr_en = ena;
}

/**
 * @brief Set I2S TX to philip standard
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_set_format_philip(i2s_dev_t *hw)
{
    hw->conf.tx_short_sync = 0;
    hw->conf.tx_msb_shift = 1;
}

/**
 * @brief Set I2S RX to philip standard
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_set_format_philip(i2s_dev_t *hw)
{
    hw->conf.rx_short_sync = 0;
    hw->conf.rx_msb_shift = 1;
}

/**
 * @brief Set I2S TX to MSB Alignment Standard
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_set_format_msb_align(i2s_dev_t *hw)
{
    hw->conf.tx_short_sync = 0;
    hw->conf.tx_msb_shift = 0;
}

/**
 * @brief Set I2S RX to MSB Alignment Standard
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_set_format_msb_align(i2s_dev_t *hw)
{
    hw->conf.rx_short_sync = 0;
    hw->conf.rx_msb_shift = 0;
}

/**
 * @brief Set I2S TX to PCM short standard
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_set_pcm_short(i2s_dev_t *hw)
{
    hw->conf.tx_short_sync = 1;
    hw->conf.tx_msb_shift = 0;
}

/**
 * @brief Set I2S RX to PCM short standard
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_set_pcm_short(i2s_dev_t *hw)
{
    hw->conf.rx_short_sync = 1;
    hw->conf.rx_msb_shift = 0;
}

/**
 * @brief Set I2S TX to PCM long standard
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_set_pcm_long(i2s_dev_t *hw)
{
    hw->conf.tx_short_sync = 0;
    hw->conf.tx_msb_shift = 0;
}

/**
 * @brief Set I2S RX to PCM long standard
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_set_pcm_long(i2s_dev_t *hw)
{
    hw->conf.rx_short_sync = 0;
    hw->conf.rx_msb_shift = 0;
}

/**
 * @brief Configure TX WS signal width
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param width WS width in BCK cycle
 */
static inline void i2s_ll_tx_set_ws_width(i2s_dev_t *hw, int width)
{
    hw->conf.tx_short_sync = width == 1 ? 1 : 0;
}

/**
 * @brief Configure RX WS signal width
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param width WS width in BCK cycle
 */
static inline void i2s_ll_rx_set_ws_width(i2s_dev_t *hw, int width)
{
    hw->conf.rx_short_sync = width == 1 ? 1 : 0;
}

/**
 * @brief Enable TX MSB shift, the data will be launch at the first BCK clock
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param msb_shift_enable Set true to enable MSB shift
 */
static inline void i2s_ll_tx_enable_msb_shift(i2s_dev_t *hw, bool msb_shift_enable)
{
    hw->conf.tx_msb_shift = msb_shift_enable;
}

/**
 * @brief Enable RX MSB shift, the data will be launch at the first BCK clock
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param msb_shift_enable Set true to enable MSB shift
 */
static inline void i2s_ll_rx_enable_msb_shift(i2s_dev_t *hw, bool msb_shift_enable)
{
    hw->conf.rx_msb_shift = msb_shift_enable;
}

/**
 * @brief Set I2S tx chan mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param slot_mask select slot to send data
 * @param is_mono is mono mode
 */
static inline void i2s_ll_tx_select_std_slot(i2s_dev_t *hw, i2s_std_slot_mask_t slot_mask, bool is_mono)
{
    if (is_mono) {
        switch (slot_mask)
        {
        case I2S_STD_SLOT_RIGHT:
            hw->conf_chan.tx_chan_mod = 3;
            break;
        case I2S_STD_SLOT_LEFT:
            hw->conf_chan.tx_chan_mod = 4;
            break;
        case I2S_STD_SLOT_BOTH:
            hw->conf_chan.tx_chan_mod = 1; // 1 & 2 has same effect
            break;
        default:
            break;
        }
    } else {
        switch (slot_mask)
        {
        case I2S_STD_SLOT_RIGHT:
            hw->conf_chan.tx_chan_mod = 1;
            break;
        case I2S_STD_SLOT_LEFT:
            hw->conf_chan.tx_chan_mod = 2;
            break;
        case I2S_STD_SLOT_BOTH:
            hw->conf_chan.tx_chan_mod = 0;
            break;
        default:
            break;
        }
    }
}

/**
 * @brief Set I2S rx chan mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param slot_mask select slot to receive data
 */
static inline void i2s_ll_rx_select_std_slot(i2s_dev_t *hw, i2s_std_slot_mask_t slot_mask, bool is_msb_right)
{
    switch (slot_mask)
    {
    case I2S_STD_SLOT_RIGHT:
        hw->conf_chan.rx_chan_mod = is_msb_right ? 1 : 2;
        break;
    case I2S_STD_SLOT_LEFT:
        hw->conf_chan.rx_chan_mod = is_msb_right ? 2 : 1;
        break;
    case I2S_STD_SLOT_BOTH:
        hw->conf_chan.rx_chan_mod = 0;
        break;
    default:
        break;
    }
}

/**
 * @brief Set I2S tx bits mod
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set tx bits mod
 */
static inline void i2s_ll_tx_set_bits_mod(i2s_dev_t *hw, uint32_t val)
{
    hw->sample_rate_conf.tx_bits_mod = val;
}

/**
 * @brief Set I2S tx dma equal
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set tx dma equal
 */
static inline void i2s_ll_tx_enable_dma_equal(i2s_dev_t *hw, bool en)
{
    hw->conf.tx_dma_equal = en;
}

/**
 * @brief Enable TX mono mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param mono_ena Set true to enable mono mde.
 */
static inline void i2s_ll_tx_enable_mono_mode(i2s_dev_t *hw, bool mono_ena)
{
    int data_bit = hw->sample_rate_conf.tx_bits_mod;
    hw->fifo_conf.tx_fifo_mod = data_bit <= I2S_DATA_BIT_WIDTH_16BIT ? mono_ena : 2 + mono_ena;
    hw->conf.tx_dma_equal = mono_ena;
}

/**
 * @brief Enable RX mono mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param mono_ena Set true to enable mono mde.
 */
static inline void i2s_ll_rx_enable_mono_mode(i2s_dev_t *hw, bool mono_ena)
{
    int data_bit = hw->sample_rate_conf.rx_bits_mod;
    hw->fifo_conf.rx_fifo_mod = data_bit <= I2S_DATA_BIT_WIDTH_16BIT ? mono_ena : 2 + mono_ena;
    hw->conf.rx_dma_equal = mono_ena;
}

/**
 * @brief Enable I2S loopback mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param loopback_en Set true to share BCK and WS signal for tx module and rx module.
 */
static inline void i2s_ll_share_bck_ws(i2s_dev_t *hw, bool loopback_en)
{
    hw->conf.sig_loopback = loopback_en;
}

/**
 * @brief Enable I2S LCD mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable LCD mode.
 */
static inline void i2s_ll_enable_lcd(i2s_dev_t *hw, bool enable)
{
    hw->conf2.lcd_en = enable;
}

/**
 * @brief Set whether to continue I2S signal on bus when TX FIFO is empty
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param en whether to stop when tx fifo is empty
 */
static inline void i2s_ll_tx_stop_on_fifo_empty(i2s_dev_t *hw, bool en)
{
    hw->conf1.tx_stop_en = en;
}

/**
 * @brief Set whether to bypass the internal PCM module
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param bypass whether to bypass the PCM module
 */
static inline void i2s_ll_tx_bypass_pcm(i2s_dev_t *hw, bool bypass)
{
    hw->conf1.tx_pcm_bypass = bypass;
}

#ifdef __cplusplus
}
#endif
