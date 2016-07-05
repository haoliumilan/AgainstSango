//
//  ShiningFrame.cpp
//  test
//
//  Created by Zhang Hao on 13-4-2.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "ShiningFrame.h"
#include "Card.h"

static CCPoint gPtOutside[] = {
    CCPointMake(5,  9),CCPointMake(5, 10),CCPointMake(5, 11),CCPointMake(5, 12),CCPointMake(5, 13),
    CCPointMake(5, 14),CCPointMake(5, 15),CCPointMake(5, 16),CCPointMake(5, 17),CCPointMake(5, 18),
    CCPointMake(5, 19),CCPointMake(5, 20),CCPointMake(5, 21),CCPointMake(5, 22),CCPointMake(5, 23),
    CCPointMake(5, 24),CCPointMake(5, 25),CCPointMake(5, 26),CCPointMake(5, 27),CCPointMake(5, 28),
    CCPointMake(5, 29),CCPointMake(5, 30),CCPointMake(5, 31),CCPointMake(5, 32),CCPointMake(5, 33),
    CCPointMake(5, 34),CCPointMake(5, 35),CCPointMake(5, 36),CCPointMake(5, 37),CCPointMake(5, 38),
    CCPointMake(5, 39),CCPointMake(5, 40),CCPointMake(5, 41),CCPointMake(5, 42),CCPointMake(5, 43),
    CCPointMake(5, 44),CCPointMake(5, 45),CCPointMake(5, 46),CCPointMake(5, 47),CCPointMake(5, 48),
    CCPointMake(5, 49),CCPointMake(5, 50),CCPointMake(5, 51),CCPointMake(5, 52),CCPointMake(5, 53),
    CCPointMake(5, 54),CCPointMake(5, 55),CCPointMake(5, 56),CCPointMake(5, 57),CCPointMake(5, 58),
    CCPointMake(5, 59),CCPointMake(5, 60),CCPointMake(5, 61),CCPointMake(5, 62),CCPointMake(5, 63),
    CCPointMake(5, 64),CCPointMake(5, 65),CCPointMake(5, 66),CCPointMake(5, 67),CCPointMake(5, 68),
    CCPointMake(5, 69),CCPointMake(5, 70),CCPointMake(5, 71),CCPointMake(5, 72),CCPointMake(5, 73),
    CCPointMake(5, 74),CCPointMake(5, 75),CCPointMake(5, 76),CCPointMake(5, 77),CCPointMake(5, 78),
    CCPointMake(5, 79),CCPointMake(5, 80),CCPointMake(5, 81),CCPointMake(5, 82),CCPointMake(5, 83),
    CCPointMake(5, 84),CCPointMake(5, 85),CCPointMake(5, 86),CCPointMake(5, 87),CCPointMake(5, 88),
    CCPointMake(5, 89),CCPointMake(5, 90),CCPointMake(5, 91),CCPointMake(5, 92),CCPointMake(5, 93),
    CCPointMake(5, 94),CCPointMake(5, 95),CCPointMake(5, 96),CCPointMake(5, 97),CCPointMake(5, 98),
    
    CCPointMake(6, 99),CCPointMake(6, 100),CCPointMake(7, 101),CCPointMake(8, 101),
    
    CCPointMake(9, 102),CCPointMake(10, 102),CCPointMake(11, 102),CCPointMake(12, 102),CCPointMake(13, 102),
    CCPointMake(14, 102),CCPointMake(15, 102),CCPointMake(16, 102),CCPointMake(17, 102),CCPointMake(18, 102),
    CCPointMake(19, 102),CCPointMake(20, 102),CCPointMake(21, 102),CCPointMake(22, 102),CCPointMake(23, 102),
    CCPointMake(24, 102),CCPointMake(25, 102),CCPointMake(26, 102),CCPointMake(27, 102),CCPointMake(28, 102),
    CCPointMake(29, 102),CCPointMake(30, 102),CCPointMake(31, 102),CCPointMake(32, 102),CCPointMake(33, 102),
    CCPointMake(34, 102),CCPointMake(35, 102),CCPointMake(36, 102),CCPointMake(37, 102),CCPointMake(38, 102),
    CCPointMake(39, 102),CCPointMake(40, 102),CCPointMake(41, 102),CCPointMake(42, 102),CCPointMake(43, 102),
    CCPointMake(44, 102),CCPointMake(45, 102),CCPointMake(46, 102),CCPointMake(47, 102),CCPointMake(48, 102),
    CCPointMake(49, 102),CCPointMake(50, 102),CCPointMake(51, 102),CCPointMake(52, 102),CCPointMake(53, 102),
    CCPointMake(54, 102),CCPointMake(55, 102),CCPointMake(56, 102),CCPointMake(57, 102),CCPointMake(58, 102),
    CCPointMake(59, 102),CCPointMake(60, 102),CCPointMake(61, 102),CCPointMake(62, 102),CCPointMake(63, 102),
    CCPointMake(64, 102),CCPointMake(65, 102),CCPointMake(66, 102),CCPointMake(67, 102),CCPointMake(68, 102),
    CCPointMake(69, 102),CCPointMake(70, 102),CCPointMake(71, 102),CCPointMake(72, 102),CCPointMake(73, 102),
    CCPointMake(74, 102),CCPointMake(75, 102),CCPointMake(76, 102),CCPointMake(77, 102),CCPointMake(78, 102),
    CCPointMake(79, 102),CCPointMake(80, 102),CCPointMake(81, 102),CCPointMake(82, 102),CCPointMake(83, 102),
    CCPointMake(84, 102),CCPointMake(85, 102),CCPointMake(86, 102),CCPointMake(87, 102),CCPointMake(88, 102),
    CCPointMake(89, 102),CCPointMake(90, 102),CCPointMake(91, 102),CCPointMake(92, 102),CCPointMake(93, 102),
    CCPointMake(94, 102),CCPointMake(95, 102),CCPointMake(96, 102),CCPointMake(97, 102),CCPointMake(98, 102),
    
    CCPointMake(99, 101),CCPointMake(100, 101),CCPointMake(101, 100),CCPointMake(101, 99),
    
    CCPointMake(102, 98),CCPointMake(102, 97),CCPointMake(102, 96),CCPointMake(102, 95),CCPointMake(102, 94),
    CCPointMake(102, 93),CCPointMake(102, 92),CCPointMake(102, 91),CCPointMake(102, 90),CCPointMake(102, 89),
    CCPointMake(102, 88),CCPointMake(102, 87),CCPointMake(102, 86),CCPointMake(102, 85),CCPointMake(102, 84),
    CCPointMake(102, 83),CCPointMake(102, 82),CCPointMake(102, 81),CCPointMake(102, 80),CCPointMake(102, 79),
    CCPointMake(102, 78),CCPointMake(102, 77),CCPointMake(102, 76),CCPointMake(102, 75),CCPointMake(102, 74),
    CCPointMake(102, 73),CCPointMake(102, 72),CCPointMake(102, 71),CCPointMake(102, 70),CCPointMake(102, 69),
    CCPointMake(102, 68),CCPointMake(102, 67),CCPointMake(102, 66),CCPointMake(102, 65),CCPointMake(102, 64),
    CCPointMake(102, 63),CCPointMake(102, 62),CCPointMake(102, 61),CCPointMake(102, 60),CCPointMake(102, 59),
    CCPointMake(102, 58),CCPointMake(102, 57),CCPointMake(102, 56),CCPointMake(102, 55),CCPointMake(102, 54),
    CCPointMake(102, 53),CCPointMake(102, 52),CCPointMake(102, 51),CCPointMake(102, 50),CCPointMake(102, 49),
    CCPointMake(102, 48),CCPointMake(102, 47),CCPointMake(102, 46),CCPointMake(102, 45),CCPointMake(102, 44),
    CCPointMake(102, 43),CCPointMake(102, 42),CCPointMake(102, 41),CCPointMake(102, 40),CCPointMake(102, 39),
    CCPointMake(102, 38),CCPointMake(102, 37),CCPointMake(102, 36),CCPointMake(102, 35),CCPointMake(102, 34),
    CCPointMake(102, 33),CCPointMake(102, 32),CCPointMake(102, 31),CCPointMake(102, 30),CCPointMake(102, 29),
    CCPointMake(102, 28),CCPointMake(102, 27),CCPointMake(102, 26),CCPointMake(102, 25),CCPointMake(102, 24),
    CCPointMake(102, 23),CCPointMake(102, 22),CCPointMake(102, 21),CCPointMake(102, 20),CCPointMake(102, 19),
    CCPointMake(102, 18),CCPointMake(102, 17),CCPointMake(102, 16),CCPointMake(102, 15),CCPointMake(102, 14),
    CCPointMake(102, 13),CCPointMake(102, 12),CCPointMake(102, 11),CCPointMake(102, 10),CCPointMake(102, 9),
    
    CCPointMake(101, 8),CCPointMake(101, 7),CCPointMake(100, 6),CCPointMake(99, 6),
    
    CCPointMake(98, 5),CCPointMake(97, 5),CCPointMake(96, 5),CCPointMake(95, 5),CCPointMake(94, 5),
    CCPointMake(93, 5),CCPointMake(92, 5),CCPointMake(91, 5),CCPointMake(90, 5),CCPointMake(89, 5),
    CCPointMake(88, 5),CCPointMake(87, 5),CCPointMake(86, 5),CCPointMake(85, 5),CCPointMake(84, 5),
    CCPointMake(83, 5),CCPointMake(82, 5),CCPointMake(81, 5),CCPointMake(80, 5),CCPointMake(79, 5),
    CCPointMake(78, 5),CCPointMake(77, 5),CCPointMake(76, 5),CCPointMake(75, 5),CCPointMake(74, 5),
    CCPointMake(73, 5),CCPointMake(72, 5),CCPointMake(71, 5),CCPointMake(70, 5),CCPointMake(69, 5),
    CCPointMake(68, 5),CCPointMake(67, 5),CCPointMake(66, 5),CCPointMake(65, 5),CCPointMake(64, 5),
    CCPointMake(63, 5),CCPointMake(62, 5),CCPointMake(61, 5),CCPointMake(60, 5),CCPointMake(59, 5),
    CCPointMake(58, 5),CCPointMake(57, 5),CCPointMake(56, 5),CCPointMake(55, 5),CCPointMake(54, 5),
    CCPointMake(53, 5),CCPointMake(52, 5),CCPointMake(51, 5),CCPointMake(50, 5),CCPointMake(49, 5),
    CCPointMake(48, 5),CCPointMake(47, 5),CCPointMake(46, 5),CCPointMake(45, 5),CCPointMake(44, 5),
    CCPointMake(43, 5),CCPointMake(42, 5),CCPointMake(41, 5),CCPointMake(40, 5),CCPointMake(39, 5),
    CCPointMake(38, 5),CCPointMake(37, 5),CCPointMake(36, 5),CCPointMake(35, 5),CCPointMake(34, 5),
    CCPointMake(33, 5),CCPointMake(32, 5),CCPointMake(31, 5),CCPointMake(30, 5),CCPointMake(29, 5),
    CCPointMake(28, 5),CCPointMake(27, 5),CCPointMake(26, 5),CCPointMake(25, 5),CCPointMake(24, 5),
    CCPointMake(23, 5),CCPointMake(22, 5),CCPointMake(21, 5),CCPointMake(20, 5),CCPointMake(19, 5),
    CCPointMake(18, 5),CCPointMake(17, 5),CCPointMake(16, 5),CCPointMake(15, 5),CCPointMake(14, 5),
    CCPointMake(13, 5),CCPointMake(12, 5),CCPointMake(11, 5),CCPointMake(10, 5),CCPointMake(9, 5),
    
    CCPointMake(8, 6),CCPointMake(7, 6),CCPointMake(6, 7),CCPointMake(6, 8),
};

static CCPoint gPtInside[] = {
    CCPointMake(6, 9),CCPointMake(6, 10),CCPointMake(6, 11),CCPointMake(6, 12),CCPointMake(6, 13),
    CCPointMake(6, 14),CCPointMake(6, 15),CCPointMake(6, 16),CCPointMake(6, 17),CCPointMake(6, 18),
    CCPointMake(6, 19),CCPointMake(6, 20),CCPointMake(6, 21),CCPointMake(6, 22),CCPointMake(6, 23),
    CCPointMake(6, 24),CCPointMake(6, 25),CCPointMake(6, 26),CCPointMake(6, 27),CCPointMake(6, 28),
    CCPointMake(6, 29),CCPointMake(6, 30),CCPointMake(6, 31),CCPointMake(6, 32),CCPointMake(6, 33),
    CCPointMake(6, 34),CCPointMake(6, 35),CCPointMake(6, 36),CCPointMake(6, 37),CCPointMake(6, 38),
    CCPointMake(6, 39),CCPointMake(6, 40),CCPointMake(6, 41),CCPointMake(6, 42),CCPointMake(6, 43),
    CCPointMake(6, 44),CCPointMake(6, 45),CCPointMake(6, 46),CCPointMake(6, 47),CCPointMake(6, 48),
    CCPointMake(6, 49),CCPointMake(6, 50),CCPointMake(6, 51),CCPointMake(6, 52),CCPointMake(6, 53),
    CCPointMake(6, 54),CCPointMake(6, 55),CCPointMake(6, 56),CCPointMake(6, 57),CCPointMake(6, 58),
    CCPointMake(6, 59),CCPointMake(6, 60),CCPointMake(6, 61),CCPointMake(6, 62),CCPointMake(6, 63),
    CCPointMake(6, 64),CCPointMake(6, 65),CCPointMake(6, 66),CCPointMake(6, 67),CCPointMake(6, 68),
    CCPointMake(6, 69),CCPointMake(6, 70),CCPointMake(6, 71),CCPointMake(6, 72),CCPointMake(6, 73),
    CCPointMake(6, 74),CCPointMake(6, 75),CCPointMake(6, 76),CCPointMake(6, 77),CCPointMake(6, 78),
    CCPointMake(6, 79),CCPointMake(6, 80),CCPointMake(6, 81),CCPointMake(6, 82),CCPointMake(6, 83),
    CCPointMake(6, 84),CCPointMake(6, 85),CCPointMake(6, 86),CCPointMake(6, 87),CCPointMake(6, 88),
    CCPointMake(6, 89),CCPointMake(6, 90),CCPointMake(6, 91),CCPointMake(6, 92),CCPointMake(6, 93),
    CCPointMake(6, 94),CCPointMake(6, 95),CCPointMake(6, 96),CCPointMake(6, 97),CCPointMake(6, 98),
    
    CCPointMake(7, 98),CCPointMake(7, 99),CCPointMake(7, 100),CCPointMake(8, 100),
    
    CCPointMake(9, 101),CCPointMake(10, 101),CCPointMake(11, 101),CCPointMake(12, 101),CCPointMake(13, 101),
    CCPointMake(14, 101),CCPointMake(15, 101),CCPointMake(16, 101),CCPointMake(17, 101),CCPointMake(18, 101),
    CCPointMake(19, 101),CCPointMake(20, 101),CCPointMake(21, 101),CCPointMake(22, 101),CCPointMake(23, 101),
    CCPointMake(24, 101),CCPointMake(25, 101),CCPointMake(26, 101),CCPointMake(27, 101),CCPointMake(28, 101),
    CCPointMake(29, 101),CCPointMake(30, 101),CCPointMake(31, 101),CCPointMake(32, 101),CCPointMake(33, 101),
    CCPointMake(34, 101),CCPointMake(35, 101),CCPointMake(36, 101),CCPointMake(37, 101),CCPointMake(38, 101),
    CCPointMake(39, 101),CCPointMake(40, 101),CCPointMake(41, 101),CCPointMake(42, 101),CCPointMake(43, 101),
    CCPointMake(44, 101),CCPointMake(45, 101),CCPointMake(46, 101),CCPointMake(47, 101),CCPointMake(48, 101),
    CCPointMake(49, 101),CCPointMake(50, 101),CCPointMake(51, 101),CCPointMake(52, 101),CCPointMake(53, 101),
    CCPointMake(54, 101),CCPointMake(55, 101),CCPointMake(56, 101),CCPointMake(57, 101),CCPointMake(58, 101),
    CCPointMake(59, 101),CCPointMake(60, 101),CCPointMake(61, 101),CCPointMake(62, 101),CCPointMake(63, 101),
    CCPointMake(64, 101),CCPointMake(65, 101),CCPointMake(66, 101),CCPointMake(67, 101),CCPointMake(68, 101),
    CCPointMake(69, 101),CCPointMake(70, 101),CCPointMake(71, 101),CCPointMake(72, 101),CCPointMake(73, 101),
    CCPointMake(74, 101),CCPointMake(75, 101),CCPointMake(76, 101),CCPointMake(77, 101),CCPointMake(78, 101),
    CCPointMake(79, 101),CCPointMake(80, 101),CCPointMake(81, 101),CCPointMake(82, 101),CCPointMake(83, 101),
    CCPointMake(84, 101),CCPointMake(85, 101),CCPointMake(86, 101),CCPointMake(87, 101),CCPointMake(88, 101),
    CCPointMake(89, 101),CCPointMake(90, 101),CCPointMake(91, 101),CCPointMake(92, 101),CCPointMake(93, 101),
    CCPointMake(94, 101),CCPointMake(95, 101),CCPointMake(96, 101),CCPointMake(97, 101),CCPointMake(98, 101),
    
    CCPointMake(98, 100),CCPointMake(99, 100),CCPointMake(100, 100),CCPointMake(100, 99),
    
    CCPointMake(101, 98),CCPointMake(101, 97),CCPointMake(101, 96),CCPointMake(101, 95),CCPointMake(101, 94),
    CCPointMake(101, 93),CCPointMake(101, 92),CCPointMake(101, 91),CCPointMake(101, 90),CCPointMake(101, 89),
    CCPointMake(101, 88),CCPointMake(101, 87),CCPointMake(101, 86),CCPointMake(101, 85),CCPointMake(101, 84),
    CCPointMake(101, 83),CCPointMake(101, 82),CCPointMake(101, 81),CCPointMake(101, 80),CCPointMake(101, 79),
    CCPointMake(101, 78),CCPointMake(101, 77),CCPointMake(101, 76),CCPointMake(101, 75),CCPointMake(101, 74),
    CCPointMake(101, 73),CCPointMake(101, 72),CCPointMake(101, 71),CCPointMake(101, 70),CCPointMake(101, 69),
    CCPointMake(101, 68),CCPointMake(101, 67),CCPointMake(101, 66),CCPointMake(101, 65),CCPointMake(101, 64),
    CCPointMake(101, 63),CCPointMake(101, 62),CCPointMake(101, 61),CCPointMake(101, 60),CCPointMake(101, 59),
    CCPointMake(101, 58),CCPointMake(101, 57),CCPointMake(101, 56),CCPointMake(101, 55),CCPointMake(101, 54),
    CCPointMake(101, 53),CCPointMake(101, 52),CCPointMake(101, 51),CCPointMake(101, 50),CCPointMake(101, 49),
    CCPointMake(101, 48),CCPointMake(101, 47),CCPointMake(101, 46),CCPointMake(101, 45),CCPointMake(101, 44),
    CCPointMake(101, 43),CCPointMake(101, 42),CCPointMake(101, 41),CCPointMake(101, 40),CCPointMake(101, 39),
    CCPointMake(101, 38),CCPointMake(101, 37),CCPointMake(101, 36),CCPointMake(101, 35),CCPointMake(101, 34),
    CCPointMake(101, 33),CCPointMake(101, 32),CCPointMake(101, 31),CCPointMake(101, 30),CCPointMake(101, 29),
    CCPointMake(101, 28),CCPointMake(101, 27),CCPointMake(101, 26),CCPointMake(101, 25),CCPointMake(101, 24),
    CCPointMake(101, 23),CCPointMake(101, 22),CCPointMake(101, 21),CCPointMake(101, 20),CCPointMake(101, 19),
    CCPointMake(101, 18),CCPointMake(101, 17),CCPointMake(101, 16),CCPointMake(101, 15),CCPointMake(101, 14),
    CCPointMake(101, 13),CCPointMake(101, 12),CCPointMake(101, 11),CCPointMake(101, 10),CCPointMake(101, 9),
    
    CCPointMake(100, 9),CCPointMake(100, 8),CCPointMake(100, 7),CCPointMake(99, 7),
    
    CCPointMake(98, 6),CCPointMake(97, 6),CCPointMake(96, 6),CCPointMake(95, 6),CCPointMake(94, 6),
    CCPointMake(93, 6),CCPointMake(92, 6),CCPointMake(91, 6),CCPointMake(90, 6),CCPointMake(89, 6),
    CCPointMake(88, 6),CCPointMake(87, 6),CCPointMake(86, 6),CCPointMake(85, 6),CCPointMake(84, 6),
    CCPointMake(83, 6),CCPointMake(82, 6),CCPointMake(81, 6),CCPointMake(80, 6),CCPointMake(79, 6),
    CCPointMake(78, 6),CCPointMake(77, 6),CCPointMake(76, 6),CCPointMake(75, 6),CCPointMake(74, 6),
    CCPointMake(73, 6),CCPointMake(72, 6),CCPointMake(71, 6),CCPointMake(70, 6),CCPointMake(69, 6),
    CCPointMake(68, 6),CCPointMake(67, 6),CCPointMake(66, 6),CCPointMake(65, 6),CCPointMake(64, 6),
    CCPointMake(63, 6),CCPointMake(62, 6),CCPointMake(61, 6),CCPointMake(60, 6),CCPointMake(59, 6),
    CCPointMake(58, 6),CCPointMake(57, 6),CCPointMake(56, 6),CCPointMake(55, 6),CCPointMake(54, 6),
    CCPointMake(53, 6),CCPointMake(52, 6),CCPointMake(51, 6),CCPointMake(50, 6),CCPointMake(49, 6),
    CCPointMake(48, 6),CCPointMake(47, 6),CCPointMake(46, 6),CCPointMake(45, 6),CCPointMake(44, 6),
    CCPointMake(43, 6),CCPointMake(42, 6),CCPointMake(41, 6),CCPointMake(40, 6),CCPointMake(39, 6),
    CCPointMake(38, 6),CCPointMake(37, 6),CCPointMake(36, 6),CCPointMake(35, 6),CCPointMake(34, 6),
    CCPointMake(33, 6),CCPointMake(32, 6),CCPointMake(31, 6),CCPointMake(30, 6),CCPointMake(29, 6),
    CCPointMake(28, 6),CCPointMake(27, 6),CCPointMake(26, 6),CCPointMake(25, 6),CCPointMake(24, 6),
    CCPointMake(23, 6),CCPointMake(22, 6),CCPointMake(21, 6),CCPointMake(20, 6),CCPointMake(19, 6),
    CCPointMake(18, 6),CCPointMake(17, 6),CCPointMake(16, 6),CCPointMake(15, 6),CCPointMake(14, 6),
    CCPointMake(13, 6),CCPointMake(12, 6),CCPointMake(11, 6),CCPointMake(10, 6),CCPointMake(9, 6),
    
    CCPointMake(9, 7),CCPointMake(8, 7),CCPointMake(7, 7),CCPointMake(7, 8),
};

bool ShiningFrame::init()
{
//    for (int i = 0; i < 90; i++) {
//        printf("CCPointMake(6, %d),", i+9);
//        if ((i+1) % 5 == 0) {
//            printf("\n");
//        }
//    }
//    
//    for (int i = 0; i < 90; i++) {
//        printf("CCPointMake(%d, 101),", i+9);
//        if ((i+1) % 5 == 0) {
//            printf("\n");
//        }
//    }
//    
//    for (int i = 0; i < 90; i++) {
//        printf("CCPointMake(101, %d),", 98-i);
//        if ((i+1) % 5 == 0) {
//            printf("\n");
//        }
//    }
//    
//    for (int i = 0; i < 90; i++) {
//        printf("CCPointMake(%d, 6),", 98-i);
//        if ((i+1) % 5 == 0) {
//            printf("\n");
//        }
//    }
    
    
    m_iPtCnt = sizeof(gPtOutside) / sizeof(CCPoint);
    m_idx = 0;
    m_idx2 = m_iPtCnt/2;
    
    m_lstCard.clear();
    
    scheduleUpdate();
    
    return true;
}

void ShiningFrame::draw()
{
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    
    for (list<Card*>::iterator it = m_lstCard.begin(); it != m_lstCard.end(); ++it)
    {
        Card* pCard = *it;
        
        CCPoint pt;
        pt.x = pCard->getPosition().x + pCard->getCardPos().x - pCard->getContentSize().width * 0.5;
        pt.y = pCard->getPosition().y + pCard->getCardPos().y - pCard->getContentSize().height * 0.5;
        
        drawFrameBg(pt);
        
        drawMovHead(pt);
        
        drawMovTail(pt);
    }

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
}

void ShiningFrame::drawFrameBg(CCPoint& pt)
{
    glColor4f(0.3, 0.3, 0.3, 1.0);
    
    ccVertex2F *newPoints = new ccVertex2F[m_iPtCnt];
    
    for( unsigned int i= 0; i < m_iPtCnt; i++){
        newPoints[i].x = gPtOutside[i].x + pt.x;
        newPoints[i].y = gPtOutside[i].y + pt.y;
    }
    
    glVertexPointer(2, GL_FLOAT, 0, newPoints);
    glDrawArrays(GL_POINTS, 0, (GLsizei)m_iPtCnt);
    
    for( unsigned int i= 0; i < m_iPtCnt; i++){
        newPoints[i].x = gPtInside[i].x + pt.x;
        newPoints[i].y = gPtInside[i].y + pt.y;
    }
    
    glVertexPointer(2, GL_FLOAT, 0, newPoints);
    glDrawArrays(GL_POINTS, 0, (GLsizei)m_iPtCnt);
    
    delete[] newPoints;
}

void ShiningFrame::drawMovHead(CCPoint& pt)
{
    for (int i = 0; i < 10; i++) {
        glColor4f(0.2 + 0.08 * (10 - i), 0.2 + 0.08 * (10 - i), 0.2 + 0.08 * (10 - i), 1.0);
        
        ccVertex2F p;

        int idx = (m_idx + i) % m_iPtCnt;
        p.x = gPtOutside[idx].x + pt.x;
        p.y = gPtOutside[idx].y + pt.y;
        glVertexPointer(2, GL_FLOAT, 0, &p);
        glDrawArrays(GL_POINTS, 0, 1);
        
        p.x = gPtInside[idx].x + pt.x;
        p.y = gPtInside[idx].y + pt.y;
        glVertexPointer(2, GL_FLOAT, 0, &p);
        glDrawArrays(GL_POINTS, 0, 1);
        
        
        idx = (m_idx2 + i) % m_iPtCnt;
        p.x = gPtOutside[idx].x + pt.x;
        p.y = gPtOutside[idx].y + pt.y;
        glVertexPointer(2, GL_FLOAT, 0, &p);
        glDrawArrays(GL_POINTS, 0, 1);
        
        p.x = gPtInside[idx].x + pt.x;
        p.y = gPtInside[idx].y + pt.y;
        glVertexPointer(2, GL_FLOAT, 0, &p);
        glDrawArrays(GL_POINTS, 0, 1);
    }
}

void ShiningFrame::drawMovTail(CCPoint& pt)
{
    for (int i = 0; i < 50; i++) {
        glColor4f(0.2 + 0.016 * (50 - i), 0.2 + 0.016 * (50 - i), 0.2 + 0.016 * (50 - i), 1.0);
        
        ccVertex2F p;
        
        int idx = (m_iPtCnt + m_idx - i) % m_iPtCnt;
        p.x = gPtOutside[idx].x + pt.x;
        p.y = gPtOutside[idx].y + pt.y;
        glVertexPointer(2, GL_FLOAT, 0, &p);
        glDrawArrays(GL_POINTS, 0, 1);
        
        p.x = gPtInside[idx].x + pt.x;
        p.y = gPtInside[idx].y + pt.y;
        glVertexPointer(2, GL_FLOAT, 0, &p);
        glDrawArrays(GL_POINTS, 0, 1);
        
        idx = (m_iPtCnt + m_idx2 - i) % m_iPtCnt;
        p.x = gPtOutside[idx].x + pt.x;
        p.y = gPtOutside[idx].y + pt.y;
        glVertexPointer(2, GL_FLOAT, 0, &p);
        glDrawArrays(GL_POINTS, 0, 1);
        
        p.x = gPtInside[idx].x + pt.x;
        p.y = gPtInside[idx].y + pt.y;
        glVertexPointer(2, GL_FLOAT, 0, &p);
        glDrawArrays(GL_POINTS, 0, 1);
    }
}

void ShiningFrame::update(ccTime dt)
{
    m_idx += 3;
    if (m_idx >= m_iPtCnt) {
        m_idx = 0;
    }
    
    m_idx2 += 3;
    if (m_idx2 >= m_iPtCnt) {
        m_idx2 = 0;
    }
}

void ShiningFrame::addCard(Card* pCard)
{
    m_lstCard.push_back(pCard);
}

void ShiningFrame::rmvCard(Card* pCard)
{
    for (list<Card*>::iterator it = m_lstCard.begin(); it != m_lstCard.end(); ++it)
    {
        if (*it == pCard){
            m_lstCard.erase(it);
            break;
        }
    }
}

void ShiningFrame::clrCardList()
{
    m_lstCard.clear();
}




