/*
 * minesweeper_images.c
 *
 *  Created on: Dec 2, 2020
 *      Author: breit
 */
#include <minesweeper_images.h>

//
//  Image data for bombimage
//

const uint8_t Bitmap_Bomb[] =
{
 0x1E, //    ####
 0x13, //    #  ##
 0x07, //      ###
 0x7E, //  ######
 0xFF, // ########
 0xFF, // ########
 0xFF, // ########
 0xFF, // ########
 0x7E, //  ######
 0x3C, //   ####
// 0x07, //      ###
// 0x31, //   ##   #
// 0x17, //    # ###
// 0x7F, //  #######
// 0xFE, // #######
// 0xFF, // ########
// 0xFF, // ########
// 0xFE, // #######
// 0x7E, //  ######
// 0x3C, //   ####

};

const uint8_t Bitmap_Flag[] =
{
 0x08, //     #
 0x38, //   ###
 0xF8, // #####
 0xF8, // #####
 0x38, //   ###
 0x08, //     #
 0x08, //     #
 0x3E, //   #####
 0x3E, //   #####
 0xFF, // ########
};

const uint8_t Bitmap_Mouse[] =
{
 0xFF, 0xC0, // ##########
 0xFF, 0x80, // #########
 0xFF, 0x00, // ########
 0xFE, 0x00, // #######
 0xFC, 0x00, // ######
 0xFE, 0x00, // #######
 0xF7, 0x00, // #### ###
 0xE3, 0x80, // ###   ###
 0xC1, 0xC0, // ##     ###
 0x80, 0xC0, // #       ##
};

const uint8_t Bitmap_One[] =
{
 0x78, //  ####
 0xF8, // #####
 0xF8, // #####
 0x38, //   ###
 0x38, //   ###
 0x38, //   ###
 0x38, //   ###
 0x38, //   ###
 0xFC, // ######
 0xFC, // ######
};

const uint8_t Bitmap_Two[] =
{
 0x38, //   ###
 0x7C, //  #####
 0xFE, // #######
 0xEE, // ### ###
 0x1E, //    ####
 0x3C, //   ####
 0x78, //  ####
 0xF0, // ####
 0xFE, // #######
 0xFE, // #######
};

const uint8_t Bitmap_Three[] =
{
 0x38, //   ###
 0x7C, //  #####
 0xEE, // ### ###
 0xCE, // ##  ###
 0x1C, //    ###
 0x1C, //    ###
 0xCE, // ##  ###
 0xEE, // ### ###
 0x7C, //  #####
 0x38, //   ###
};

const uint8_t Bitmap_Four[] =
{
 0x1C, //    ###
 0x3C, //   ####
 0x7C, //  #####
 0xEC, // ### ##
 0xCC, // ##  ##
 0xFF, // ########
 0xFF, // ########
 0xFF, // ########
 0x0C, //     ##
 0x0C, //     ##
};

const uint8_t Bitmap_Five[] =
{
 0xFF, // ########
 0xFF, // ########
 0xC0, // ##
 0xFC, // ######
 0x7E, //  ######
 0x07, //      ###
 0x83, // #     ##
 0xE7, // ###  ###
 0x7E, //  ######
 0x3C, //   ####
};

const uint8_t Bitmap_Six[] =
{
 0x7E, //  ######
 0xFF, // ########
 0xE3, // ###   ##
 0xE0, // ###
 0xFE, // #######
 0xE3, // ###   ##
 0xE3, // ###   ##
 0xE3, // ###   ##
 0xFF, // ########
 0x7E, //  ######
};

const uint8_t Bitmap_Seven[] =
{
 0xFF, // ########
 0xFF, // ########
 0x07, //      ###
 0x07, //      ###
 0x0E, //     ###
 0x1C, //    ###
 0x38, //   ###
 0x70, //  ###
 0xE0, // ###
 0xC0, // ##
};

const uint8_t Bitmap_Eight[] =
{
 0x3C, //   ####
 0x7E, //  ######
 0xE7, // ###  ###
 0x66, //  ##  ##
 0x3C, //   ####
 0x3C, //   ####
 0x66, //  ##  ##
 0xE7, // ###  ###
 0x7E, //  ######
 0x3C, //   ####
};
