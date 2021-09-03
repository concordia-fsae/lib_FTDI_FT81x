/*
@file    EVE_config.h
@brief   configuration information for some TFTs
@version 5.0
@date    2021-06-02
@author  Rudolph Riedel
*/

#pragma once

#ifndef EVE_CONFIG_H_
#define EVE_CONFIG_H_

/* select the settings for the TFT attached */
#if 0
/* F810 / F811 */
  #define EVE_VM810C
  #define EVE_FT810CB_HY50HD
  #define EVE_FT811CB_HY50HD

/* FT812 / F813 */
  #define EVE_ME812A
  #define EVE_ME813A
  #define EVE_ET07
  #define EVE_RVT50
  #define EVE_RVT70
  #define EVE_EVE2_29
  #define EVE_EVE2_35
  #define EVE_EVE2_35G
  #define EVE_EVE2_38
  #define EVE_EVE2_38G
  #define EVE_EVE2_43
  #define EVE_EVE2_43G
  #define EVE_EVE2_50
  #define EVE_EVE2_50G
  #define EVE_EVE2_70
  #define EVE_EVE2_70G
  #define EVE_NHD_35
  #define EVE_NHD_43
  #define EVE_NHD_43_800480
  #define EVE_NHD_50
  #define EVE_NHD_70
  #define EVE_ADAM101
  #define EVE_CFAF240400C1_030SC
  #define EVE_CFAF320240F_035T
  #define EVE_CFAF480128A0_039TC
  #define EVE_CFAF800480E0_050SC
  #define EVE_GEN4_FT81x_43
  #define EVE_GEN4_FT812_50
  #define EVE_GEN4_FT812_70
  #define EVE_GEN4_FT813_50
  #define EVE_GEN4_FT813_70
  #define EVE_SUNFLOWER
  #define EVE_PH800480

/* BT815 / BT816 */
  #define EVE_VM816C50AD
  #define EVE_PAF90
  #define EVE_RiTFT35
  #define EVE_RiTFT43
  #define EVE_RiTFT50
  #define EVE_RiTFT70
  #define EVE_EVE3_35
  #define EVE_EVE3_35G
  #define EVE_EVE3_43
  #define EVE_EVE3_43G
  #define EVE_EVE3_50
  #define EVE_EVE3_50G
  #define EVE_EVE3_70
  #define EVE_EVE3_70G
  #define EVE_EVE3x_39
  #define EVE_EVE3x_39G

/* BT817 / BT818 */
  #define EVE_RVT35H
  #define EVE_RVT43H
  #define EVE_RVT50H
  #define EVE_RVT70H
  #define EVE_RVT101H
  #define EVE_EVE4_40G
  #define EVE_EVE4_70G
  #define EVE_EVE4_101G


#endif

// #define EVE_EVE3_50G
#include "ScreenConfig.h"


/* display timing parameters below */

/* ----------- 320 x 240 ----------- */

/* EVE2-35A 320x240 3.5" Matrix Orbital, resistive, or non-touch, FT812 */
#if defined (EVE_EVE2_35)
#define Resolution_320x240

#define EVE_PCLK	(9L)
#define EVE_PCLKPOL	(0L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* EVE2-35G 320x240 3.5" Matrix Orbital, capacitive touch, FT813 */
#if defined (EVE_EVE2_35G)
#define Resolution_320x240

#define EVE_PCLK	(9L)
#define EVE_PCLKPOL	(0L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#define EVE_HAS_GT911	/* special treatment required for out-of-spec touch-controller */
#endif


/* untested */
/* EVE3-35A 320x240 3.5" Matrix Orbital, resistive, or non-touch, BT816 */
#if defined (EVE_EVE3_35)
#define Resolution_320x240

#define EVE_PCLK	(11L)
#define EVE_PCLKPOL	(0L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#endif


/* EVE3-35G 320x240 3.5" Matrix Orbital, capacitive-touch, BT815 */
#if defined (EVE_EVE3_35G)
#define Resolution_320x240

#define EVE_PCLK	(11L)
#define EVE_PCLKPOL	(0L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#define EVE_HAS_GT911	/* special treatment required for out-of-spec touch-controller */
#endif


/* Crystalfonts CFAF320240F-035T 320x240 3.5" , FT810 resistive touch */
#if defined (EVE_CFAF320240F_035T)
#define EVE_HSIZE	(320L)
#define EVE_VSIZE	(240L)

#define EVE_VSYNC0	(1L)
#define EVE_VSYNC1	(4L)
#define EVE_VOFFSET	(4L)
#define EVE_VCYCLE	(245L)
#define EVE_HSYNC0	(10L)
#define EVE_HSYNC1	(20L)
#define EVE_HOFFSET	(40L)
#define EVE_HCYCLE 	(510L)
#define EVE_PCLK	(8L)
#define EVE_PCLKPOL	(0L)
#define EVE_SWIZZLE	(2L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1800L)
#define EVE_GEN 2
#endif


/* untested */
/* Sunflower Arduino Shield, 320x240 3.5" from Cowfish, FT813, https://github.com/Cowfish-Studios/Cowfish_Sunflower_Shield_PCB */
/* set EVE_CS to 6 and EVE_PDN to 5 in the Arduino block in EVE_target.h */
#if defined (EVE_SUNFLOWER)
#define Resolution_320x240

#define EVE_PCLK	(9L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(2L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* NHD-3.5-320240FT-CxXx-xxx 320x240 3.5" Newhaven, resistive or capacitive, FT81x */
#if defined (EVE_NHD_35)
#define Resolution_320x240

#define EVE_PCLK	(9L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(2L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* untested */
/* 320x240 3.5" Riverdi, various options, BT815/BT816 */
#if defined (EVE_RiTFT35)
#define EVE_HSIZE	(320L)
#define EVE_VSIZE	(240L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(2L)
#define EVE_VOFFSET	(13L)
#define EVE_VCYCLE	(263L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(10L)
#define EVE_HOFFSET (70L)
#define EVE_HCYCLE 	(408L)
#define EVE_PCLK	(11L)
#define EVE_PCLKPOL (1L)
#define EVE_SWIZZLE (2L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#endif


/* untested */
/* RVT35HHBxxxxx 320x240 3.5" Riverdi, various options, BT817 */
#if defined (EVE_RVT35H)
#define EVE_HSIZE	(320L)
#define EVE_VSIZE	(240L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(4L)
#define EVE_VOFFSET	(12L)
#define EVE_VCYCLE	(260L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(4L)
#define EVE_HOFFSET (43L)
#define EVE_HCYCLE 	(371L)
#define EVE_PCLK	(12L)
#define EVE_PCLKPOL (1L)
#define EVE_SWIZZLE (0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#endif

/* ----------- 480 x 272 ----------- */

/* untested */
/* EVE2-43A 480x272 4.3" Matrix Orbital, resistive or no touch, FT812 */
#if defined (EVE_EVE2_43)
#define Resolution_480x272

#define EVE_PCLK	(6L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* EVE2-43G 480x272 4.3" Matrix Orbital, capacitive touch, FT813 */
#if defined (EVE_EVE2_43G)
#define Resolution_480x272

#define EVE_PCLK	(6L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#define EVE_HAS_GT911	/* special treatment required for out-of-spec touch-controller */
#endif


/* untested */
/* EVE3-43A 480x272 4.3" Matrix Orbital, resistive, or non-touch, BT816 */
#if defined (EVE_EVE3_43)
#define Resolution_480x272

#define EVE_PCLK	(7L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#endif


/* EVE3-43G 480x272 4.3" Matrix Orbital, capacitive-touch, BT815 */
#if defined (EVE_EVE3_43G)
#define Resolution_480x272

#define EVE_PCLK	(7L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#define EVE_HAS_GT911
#endif


/* 480x272 4.3" Riverdi, various options, BT815/BT816 */
#if defined (EVE_RiTFT43)
#define Resolution_480x272

#define EVE_PCLK	(7L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#endif


/* untested */
/* RVT43HLBxxxxx 480x272 4.3" Riverdi, various options, BT817 */
#if defined (EVE_RVT43H)
#define EVE_HSIZE	(480L)
#define EVE_VSIZE	(272L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(4L)
#define EVE_VOFFSET	(12L)
#define EVE_VCYCLE	(292L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(4L)
#define EVE_HOFFSET (43L)
#define EVE_HCYCLE 	(531L)
#define EVE_PCLK	(7L)
#define EVE_PCLKPOL (1L)
#define EVE_SWIZZLE (0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#endif


/* untested */
/* NHD-4.3-480272FT-CxXx-xxx 480x272 4.3" Newhaven, resistive or capacitive, FT81x */
#if defined (EVE_NHD_43)
#define Resolution_480x272

#define EVE_PCLK	(6L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
// #define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* untested */
/* 4D-Systems GEN4-FT81x-43xx 480x272 4.3",resistive or capacitive, FT812 / FT813 */
#if defined (EVE_GEN4_FT81x_43)
#define Resolution_480x272

#define EVE_PCLK	(6L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* untested */
/* Gameduino 3, 480x272 4.3",resistive touch, FT810 */
#if defined (EVE_GEN4_FT81x_43)
#define Resolution_480x272

#define EVE_PCLK	(6L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(3L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* ----------- 800 x 480 ----------- */

/* untested */
/* FTDI/BRT EVE2 modules VM810C50A-D, ME812A-WH50R and ME813A-WH50C, 800x480 5.0" */
/* 4D-Systems GEN4 FT812/FT813 5.0/7.0 */
/* NHD-4.3-800480FT-CSXP-CTP 800x480 4.3" Newhaven, capacitive touch, FT813 */
#if defined (EVE_VM810C) || defined (EVE_ME812A) || defined (EVE_ME813A) || defined (EVE_GEN4_FT812_50) || defined (EVE_GEN4_FT813_50) || defined (EVE_GEN4_FT812_70) || defined (EVE_GEN4_FT813_70) || defined (EVE_NHD_43_800480)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1800L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* untested */
/* PH800480T024-IFC03 800x480 7.0" FT813 from PowerTip */
/* PH800480T013-IFC05 800x480 7.0" FT813 from PowerTip */
#if defined (EVE_PH800480)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* untested */
/* NHD-7.0-800480FT-CxXx-xxx 800x480 7.0" Newhaven, resistive or capacitive, FT81x */
#if defined (EVE_NHD_70)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1800L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* untested */
/* Matrix Orbital EVE2 modules EVE2-50A, EVE2-70A : 800x480 5.0" and 7.0" resistive, or no touch, FT812 */
#if defined (EVE_EVE2_50) || defined (EVE_EVE2_70)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* Matrix Orbital EVE2 modules EVE2-50G, EVE2-70G : 800x480 5.0" and 7.0" capacitive touch, FT813 */
/* Crystalfonts CFAF800480E0-050SC 800x480 5.0" , FT813 capacitive touch */
#if defined (EVE_EVE2_50G) || defined (EVE_EVE2_70G) || defined (EVE_CFAF800480E0_050SC)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#define EVE_HAS_GT911	/* special treatment required for out-of-spec touch-controller */
#endif


/* untested */ /* Matrix Orbital EVE3 modules EVE3-50A, EVE3-70A : 800x480 5.0" and 7.0" resistive, or no touch, BT816 */
/* PAF90B5WFNWC01 800x480 9.0" Panasys, BT815 */
#if defined (EVE_EVE3_50) || defined (EVE_EVE3_70) || defined (EVE_PAF90)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1600L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#endif


/* Matrix Orbital EVE3 modules EVE3-50G, EVE3-70G : 800x480 5.0" and 7.0" capacitive touch, BT815 */
#if defined (EVE_EVE3_50G) || defined (EVE_EVE3_70G)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#define EVE_HAS_GT911
#endif


/* untested */
/* Bridgtek 800x480 5.0" BT816 */
#if defined (EVE_VM816C50AD)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1800L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#endif


/* RVT50xQFxxxxx 800x480 5.0" Riverdi, various options, FT812/FT813 */
/* RVT70xQFxxxxx 800x480 7.0" Riverdi, various options, FT812/FT813, tested with RVT70UQFNWC0x */
#if defined (EVE_RVT70) || defined (EVE_RVT50)
#define EVE_HSIZE	(800L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(480L)	/* Tvd Number of visible lines (in lines) - display height */

#define EVE_VSYNC0	(0L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(10L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(23L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(525L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(0L)	/* Thf Horizontal Front Porch */
#define EVE_HSYNC1	(10L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET (46L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(1056L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#define EVE_PCLK	(2L)	/* 60MHz / REG_PCLK = PCLK frequency 30 MHz */
#define EVE_PCLKPOL (1L)	/* PCLK polarity (0 = rising edge, 1 = falling edge) */
#define EVE_SWIZZLE (0L)	/* Defines the arrangement of the RGB pins of the FT800 */
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1800L)	/* touch-sensitivity */
#define EVE_GEN 2
#endif


/* untested */
/* RVT50xQBxxxxx 800x480 5.0" Riverdi, various options, BT815/BT816 */
/* RVT70xQBxxxxx 800x480 7.0" Riverdi, various options, BT815/BT816 */
#if defined (EVE_RiTFT70) || defined (EVE_RiTFT50)
#define EVE_HSIZE	(800L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(480L)	/* Tvd Number of visible lines (in lines) - display height */

#define EVE_VSYNC0	(0L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(10L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(23L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(525L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(0L)	/* Thf Horizontal Front Porch */
#define EVE_HSYNC1	(10L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET (46L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(1056L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#define EVE_PCLK	(2L)	/* 72MHz / REG_PCLK = PCLK frequency 30 MHz */
#define EVE_PCLKPOL (1L)	/* PCLK polarity (0 = rising edge, 1 = falling edge) */
#define EVE_SWIZZLE (0L)	/* Defines the arrangement of the RGB pins of the FT800 */
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1800L)	/* touch-sensitivity */
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#endif

/* untested */
/* RVT50HQBxxxxx 800x480 5.0" Riverdi, various options, BT817 */
#if defined (EVE_RVT50H)
#define EVE_HSIZE	(800L)
#define EVE_VSIZE	(480L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(4L)
#define EVE_VOFFSET	(8L)
#define EVE_VCYCLE	(496L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(4L)
#define EVE_HOFFSET (8L)
#define EVE_HCYCLE 	(816L)
#define EVE_PCLK	(3L)
#define EVE_PCLKPOL (1L)
#define EVE_SWIZZLE (0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#endif

/* untested */
/* NHD-5.0-800480FT-CxXx-xxx 800x480 5.0" Newhaven, resistive or capacitive, FT81x */
#if defined (EVE_NHD_50)
#define Resolution_800x480

#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(0L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* FT810CB-HY50HD: FT810 800x480 5.0" HAOYU */
#if defined (EVE_FT810CB_HY50HD)
#define EVE_HSIZE	(800L)
#define EVE_VSIZE	(480L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(2L)
#define EVE_VOFFSET	(13L)
#define EVE_VCYCLE	(525L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(20L)
#define EVE_HOFFSET	(64L)
#define EVE_HCYCLE 	(952L)
#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (2000L)	/* touch-sensitivity */
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* FT811CB-HY50HD: FT811 800x480 5.0" HAOYU */
#if defined (EVE_FT811CB_HY50HD)
#define EVE_HSIZE	(800L)
#define EVE_VSIZE	(480L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(2L)
#define EVE_VOFFSET	(13L)
#define EVE_VCYCLE	(525L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(20L)
#define EVE_HOFFSET	(64L)
#define EVE_HCYCLE 	(952L)
#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)	/* touch-sensitivity */
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif


/* untested */
/* G-ET0700G0DM6 800x480 7.0" Glyn */
#if defined (EVE_ET07)
#define EVE_HSIZE	(800L)
#define EVE_VSIZE	(480L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(2L)
#define EVE_VOFFSET	(35L)
#define EVE_VCYCLE	(525L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(128L)
#define EVE_HOFFSET (203L)
#define EVE_HCYCLE 	(1056L)
#define EVE_PCLKPOL (1L)
#define EVE_SWIZZLE (0L)
#define EVE_PCLK	(2L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* ----------- 1024 x 600 ----------- */

/* ADAM101-LCP-SWVGA-NEW 1024x600 10.1" Glyn, capacitive, FT813 */
#if defined (EVE_ADAM101)
#define EVE_HSIZE	(1024L)
#define EVE_VSIZE	(600L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(1L)
#define EVE_VOFFSET	(1L)
#define EVE_VCYCLE	(720L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(1L)
#define EVE_HOFFSET	(1L)
#define EVE_HCYCLE 	(1100L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_PCLK	(2L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 2
#endif

/* untested */
/* RVT70HSBxxxxx 1024x600 7.0" Riverdi, various options, BT817 */
#if defined (EVE_RVT70H)
#define EVE_HSIZE	(1024L)
#define EVE_VSIZE	(600L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(10L)
#define EVE_VOFFSET	(23L)
#define EVE_VCYCLE	(635L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(70L)
#define EVE_HOFFSET (160L)
#define EVE_HCYCLE 	(1344L)
#define EVE_PCLK	(1L) /* 1 = use second PLL for pixel-clock in BT817 / BT818 */
#define EVE_PCLK_FREQ (51000000L) /* EVE_PCLK needs to be set to 1 for this to take effect */
#define EVE_PCLKPOL (1L)
#define EVE_SWIZZLE (0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#endif


/* untested */
/* EVE4x-70G IPS 1024x600 7" Matrix Orbital, capacitive touch, BT817 */
#if defined (EVE_EVE4_70G)
#define EVE_HSIZE (1024L)
#define EVE_VSIZE (600L)

#define EVE_VSYNC0 (0L)
#define EVE_VSYNC1 (10L)
#define EVE_VOFFSET (23L)
#define EVE_VCYCLE (632L)
#define EVE_HSYNC0 (0L)
#define EVE_HSYNC1 (70L)
#define EVE_HOFFSET (160L)
#define EVE_HCYCLE (1344L)
#define EVE_PCLK (1L) /* 1 = use second PLL for pixel-clock in BT817 / BT818 */
#define EVE_PCLK_FREQ (51000000L) /* 51MHz - value for EVE_cmd_pclkfreq -> 60 FPS */
#define EVE_PCLKPOL (1L)
#define EVE_SWIZZLE (0L)
#define EVE_CSPREAD (0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#define EVE_HAS_GT911
#endif


#if defined (BT817_TEST1)
#define EVE_HSIZE	(1024L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(600L)	/* Tvd Number of visible lines (in lines) - display height */

#define EVE_VSYNC0	(1L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(2L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(25L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(626L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(16L)	/* Thf Horizontal Front Porch */
#define EVE_HSYNC1	(17L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET (177L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(1597L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#define EVE_PCLKPOL (1L)	/* PCLK polarity (0 = rising edge, 1 = falling edge) */
#define EVE_SWIZZLE (3L)	/* Defines the arrangement of the RGB pins */
#define EVE_PCLK	(1L)	/* 1 = use second PLL for pixel-clock in BT817 / BT818 */
#define EVE_CSPREAD	(0L)	/* helps with noise, when set to 1 fewer signals are changed simultaneously, reset-default: 1 */
#define EVE_TOUCH_RZTHRESH (1200L)	/* touch-sensitivity */
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
//#define EVE_PCLK_FREQ (51000000L)	/* 51MHz - value for EVE_cmd_pclkfreq */
#endif


#if defined (BT817_TEST2)
#define EVE_HSIZE	(1024L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(600L)	/* Tvd Number of visible lines (in lines) - display height */

#define EVE_VSYNC0	(1L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(2L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(25L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(626L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(16L)	/* Thf Horizontal Front Porch */
#define EVE_HSYNC1	(17L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET (177L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(1597L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#define EVE_PCLKPOL (1L)	/* PCLK polarity (0 = rising edge, 1 = falling edge) */
#define EVE_SWIZZLE (3L)	/* Defines the arrangement of the RGB pins */
#define EVE_PCLK	(1L)	/* 1 = use second PLL for pixel-clock in BT817 / BT818 */
#define EVE_CSPREAD	(0L)	/* helps with noise, when set to 1 fewer signals are changed simultaneously, reset-default: 1 */
#define EVE_TOUCH_RZTHRESH (1200L)	/* touch-sensitivity */
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#define EVE_PCLK_FREQ (51000000L)	/* 51MHz - value for EVE_cmd_pclkfreq */
#endif


#if defined (BT817_TEST3)
#define EVE_HSIZE	(1024L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(600L)	/* Tvd Number of visible lines (in lines) - display height */

#define EVE_VSYNC0	(1L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(2L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(25L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(626L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(16L)	/* Thf Horizontal Front Porch */
#define EVE_HSYNC1	(17L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET (177L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(1344L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#define EVE_PCLKPOL (1L)	/* PCLK polarity (0 = rising edge, 1 = falling edge) */
#define EVE_SWIZZLE (3L)	/* Defines the arrangement of the RGB pins */
#define EVE_PCLK	(1L)	/* 1 = use second PLL for pixel-clock in BT817 / BT818 */
#define EVE_CSPREAD	(0L)	/* helps with noise, when set to 1 fewer signals are changed simultaneously, reset-default: 1 */
#define EVE_TOUCH_RZTHRESH (1200L)	/* touch-sensitivity */
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#define EVE_PCLK_FREQ (51000000L)	/* 51MHz - value for EVE_cmd_pclkfreq */
#endif

/* ----------- 1280 x 800 ------------ */

/* untested */
/* EVE4-101G 1280x800 10.1" Matrix Orbital, capacitive touch, BT817 */
#if defined (EVE_EVE4_101G)
#define EVE_HSIZE	(1280L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(800L)	/* Tvd Number of visible lines (in lines) - display height */
#define EVE_VSYNC0	(11L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(12L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(22L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(823L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(78L)	/* Thf Horizontal Front Porch */
#define EVE_HSYNC1	(80L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET (158L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(1440L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#define EVE_PCLK	(1L)	/* 1 = use second PLL for pixel-clock in BT817 / BT818 */
#define EVE_PCLK_FREQ (71000000L)	/* 71MHz - value for EVE_cmd_pclkfreq */
#define EVE_PCLKPOL (0L)	/* PCLK polarity (0 = rising edge, 1 = falling edge) */
#define EVE_SWIZZLE (3L)	/* Defines the arrangement of the RGB pins */
#define EVE_CSPREAD	(0L)	/* helps with noise, when set to 1 fewer signals are changed simultaneously, reset-default: 1 */
#define EVE_TOUCH_RZTHRESH (1200L)	/* touch-sensitivity */
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#define EVE_HAS_GT911
#endif


/* untested */
/* RVT101HVBxxxxx 1280x800 7.0" Riverdi, various options, BT817 */
#if defined (EVE_RVT101H)
#define EVE_HSIZE	(1280L)
#define EVE_VSIZE	(800L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(10L)
#define EVE_VOFFSET	(23L)
#define EVE_VCYCLE	(830L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(20L)
#define EVE_HOFFSET (88L)
#define EVE_HCYCLE 	(1440L)
#define EVE_PCLK	(1L) /* 1 = use second PLL for pixel-clock in BT817 / BT818 */
#define EVE_PCLK_FREQ (71000000L) /* EVE_PCLK needs to be set to 1 for this to take effect */
#define EVE_PCLKPOL (1L)
#define EVE_SWIZZLE (0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#endif

/* ----------- non-standard ----------- */

/* untested */
/* EVE2-29A 320x102 2.9" 1U Matrix Orbital, non-touch, FT812 */
#if defined (EVE_EVE2_29)
#define EVE_HSIZE	(320L)
#define EVE_VSIZE	(102L)

#define EVE_VSYNC0	(0L)
#define EVE_VSYNC1	(2L)
#define EVE_VOFFSET	(156L)
#define EVE_VCYCLE	(262L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(10L)
#define EVE_HOFFSET	(70L)
#define EVE_HCYCLE 	(408L)
#define EVE_PCLKPOL	(0L)
#define EVE_SWIZZLE	(0L)
#define EVE_PCLK	(9L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* Crystalfonts CFAF240400C1-030SC 240x400 3.0" , FT811 capacitive touch */
#if defined (EVE_CFAF240400C1_030SC)
#define EVE_HSIZE	(240L)
#define EVE_VSIZE	(400L)

#define EVE_VSYNC0	(4L)
#define EVE_VSYNC1	(6L)
#define EVE_VOFFSET	(8L)
#define EVE_VCYCLE	(409L)
#define EVE_HSYNC0	(10L)
#define EVE_HSYNC1	(20L)
#define EVE_HOFFSET	(40L)
#define EVE_HCYCLE 	(489L)
#define EVE_PCLKPOL	(0L)
#define EVE_SWIZZLE	(2L)
#define EVE_PCLK	(5L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* EVE2-38A 480x116 3.8" 1U Matrix Orbital, resistive touch, FT812 */
#if defined (EVE_EVE2_38)
#define EVE_HSIZE	(480L)
#define EVE_VSIZE	(272L)

#define EVE_VSYNC0	(152L)
#define EVE_VSYNC1	(10L)
#define EVE_VOFFSET	(12L)
#define EVE_VCYCLE	(292L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(41L)
#define EVE_HOFFSET	(43L)
#define EVE_HCYCLE 	(548L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_PCLK	(6L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif


/* EVE2-38G 480x116 3.8" 1U Matrix Orbital, capacitive touch, FT813 */
#if defined (EVE_EVE2_38G)
#define EVE_HSIZE	(480L)
#define EVE_VSIZE	(272L)

#define EVE_VSYNC0	(152L)
#define EVE_VSYNC1	(10L)
#define EVE_VOFFSET	(12L)
#define EVE_VCYCLE	(292L)
#define EVE_HSYNC0	(0L)
#define EVE_HSYNC1	(41L)
#define EVE_HOFFSET	(43L)
#define EVE_HCYCLE 	(548L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_PCLK	(6L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#define EVE_HAS_GT911	/* special treatment required for out-of-spec touch-controller */
#endif


/* untested */
/* EVE3x-39A 480x128 3.9" 1U Matrix Orbital, resistive touch, BT816 */
#if defined (EVE_EVE3x_39)
#define EVE_HSIZE	(480L)
#define EVE_VSIZE	(272L)

#define EVE_VSYNC0	(8L)
#define EVE_VSYNC1	(11L)
#define EVE_VOFFSET	(15L)
#define EVE_VCYCLE	(288L)
#define EVE_HSYNC0	(44L)
#define EVE_HSYNC1	(28L)
#define EVE_HOFFSET	(44L)
#define EVE_HCYCLE 	(524L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_PCLK	(7L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#endif


/* untested */
/* EVE3x-39G 480x128 3.9" 1U Matrix Orbital, capacitive touch, BT815 */
#if defined (EVE_EVE3x_39G)
#define EVE_HSIZE	(480L)
#define EVE_VSIZE	(272L)

#define EVE_VSYNC0	(8L)
#define EVE_VSYNC1	(11L)
#define EVE_VOFFSET	(15L)
#define EVE_VCYCLE	(288L)
#define EVE_HSYNC0	(44L)
#define EVE_HSYNC1	(28L)
#define EVE_HOFFSET	(44L)
#define EVE_HCYCLE 	(524L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_PCLK	(7L)
#define EVE_CSPREAD	(1L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 3
#define EVE_HAS_GT911
#endif


/* Crystalfonts CFAF480128A0-039TC 480x128 3.9" , FT811 capacitive touch */
#if defined (EVE_CFAF480128A0_039TC)
#define EVE_HSIZE	(480L)
#define EVE_VSIZE	(128L)

#define EVE_VSYNC0	(4L)
#define EVE_VSYNC1	(5L)
#define EVE_VOFFSET	(8L)
#define EVE_VCYCLE	(137L)
#define EVE_HSYNC0	(24L)
#define EVE_HSYNC1	(35L)
#define EVE_HOFFSET	(41L)
#define EVE_HCYCLE 	(1042L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_PCLK	(7L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_GEN 2
#endif

/* untested */
/* note: timing parameters from Matrix Orbital, does not use the second pll, 58,64 FPS */
/* EVE4-40G 720x720 4.0" Matrix Orbital, capacitive touch, BT817 */
#if defined (EVE_EVE4_40G)
#define EVE_HSIZE	(720L)
#define EVE_VSIZE	(720L)

#define EVE_VSYNC0	(16)
#define EVE_VSYNC1	(18L)
#define EVE_VOFFSET	(35L)
#define EVE_VCYCLE	(756L)
#define EVE_HSYNC0	(46L)
#define EVE_HSYNC1	(48L)
#define EVE_HOFFSET	(91)
#define EVE_HCYCLE 	(812L)
#define EVE_PCLK	(2L)
#define EVE_PCLKPOL	(1L)
#define EVE_SWIZZLE	(0L)
#define EVE_CSPREAD	(0L)
#define EVE_TOUCH_RZTHRESH (1200L)
#define EVE_HAS_CRYSTAL
#define EVE_GEN 4
#define EVE_HAS_GT911
#endif

/* ------ Common Timings ------ */

#if defined (Resolution_320x240)
#define EVE_HSIZE	(320L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(240L)	/* Tvd Number of visible lines (in lines) - display height */

#define EVE_VSYNC0	(0L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(2L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(18L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(262L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(0L)	 /* (40L)	// Thf Horizontal Front Porch */
#define EVE_HSYNC1	(10L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET	(70L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(408L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#endif

#if defined (Resolution_480x272)
#define EVE_HSIZE	(480L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(272L)	/* Tvd Number of visible lines (in lines) - display height */

#define EVE_VSYNC0	(0L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(10L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(12L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(292L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(0L)	 /* (40L)	// Thf Horizontal Front Porch */
#define EVE_HSYNC1	(41L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET	(43L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(548L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#endif

#if defined (Resolution_800x480)
#define EVE_HSIZE	(800L)	/* Thd Length of visible part of line (in PCLKs) - display width */
#define EVE_VSIZE	(480L)	/* Tvd Number of visible lines (in lines) - display height */

#define EVE_VSYNC0	(0L)	/* Tvf Vertical Front Porch */
#define EVE_VSYNC1	(3L)	/* Tvf + Tvp Vertical Front Porch plus Vsync Pulse width */
#define EVE_VOFFSET	(32L)	/* Tvf + Tvp + Tvb Number of non-visible lines (in lines) */
#define EVE_VCYCLE	(525L)	/* Tv Total number of lines (visible and non-visible) (in lines) */
#define EVE_HSYNC0	(0L)	 /* (40L)	// Thf Horizontal Front Porch */
#define EVE_HSYNC1	(48L)	/* Thf + Thp Horizontal Front Porch plus Hsync Pulse width */
#define EVE_HOFFSET	(88L)	/* Thf + Thp + Thb Length of non-visible part of line (in PCLK cycles) */
#define EVE_HCYCLE 	(928L)	/* Th Total length of line (visible and non-visible) (in PCLKs) */
#endif


#endif /* EVE_CONFIG_H */
