//;.section .bootstrap, "ax"
.code64
.extern handle_interrupt
.global generate
.global cli_command
.global sti_command


cli_command:
cli
ret

sti_command:
sti
ret

generate:
int $40
ret

trash:
.long 0x00

#define handler(i) \
.global handler_ ## i; \
handler_ ## i :; \
push $0; \
push %RAX; \
push %RBX; \
push %RCX; \
push %RDX; \
push %RBP; \
push %RDI; \
push %RSI; \
push %R9; \
push %R10; \
push %R11; \
push %R12; \
push %R13; \
push %R14; \
push %R15; \
movq $ ## i, %rdi; \
cld;\
call handle_interrupt; \
pop %R15; \
pop %R14; \
pop %R13; \
pop %R12; \
pop %R11; \
pop %R10; \
pop %R9; \
pop %RSI; \
pop %RDI; \
pop %RBP; \
pop %RDX; \
pop %RCX; \
pop %RBX; \
pop %RAX; \
pop trash; \
iretq; \

#define handlerError(i) \
.global handler_ ## i; \
handler_ ## i :; \
push %RAX; \
push %RBX; \
push %RCX; \
push %RDX; \
push %RBP; \
push %RDI; \
push %RSI; \
push %R9; \
push %R10; \
push %R11; \
push %R12; \
push %R13; \
push %R14; \
push %R15; \
movq $ ## i, %rdi; \
cld;\
call handle_interrupt; \
pop %R15; \
pop %R14; \
pop %R13; \
pop %R12; \
pop %R11; \
pop %R10; \
pop %R9; \
pop %RSI; \
pop %RDI; \
pop %RBP; \
pop %RDX; \
pop %RCX; \
pop %RBX; \
pop %RAX; \
pop trash; \
iretq; \

handler(0)
handler(1)
handler(2)
handler(3)
handler(4)
handler(5)
handler(6)
handler(7)
handlerError(8)
handler(9)
handlerError(10)
handlerError(11)
handlerError(12)
handlerError(13)
handlerError(14)
handler(15)
handler(16)
handlerError(17)
handler(18)
handler(19)
handler(20)
handler(21)
handler(22)
handler(23)
handler(24)
handler(25)
handler(26)
handler(27)
handler(28)
handler(29)
handler(30)
handler(31)
handler(32)
handler(33)
handler(34)
handler(35)
handler(36)
handler(37)
handler(38)
handler(39)
handler(40)
handler(41)
handler(42)
handler(43)
handler(44)
handler(45)
handler(46)
handler(47)
handler(48)
handler(49)
handler(50)
handler(51)
handler(52)
handler(53)
handler(54)
handler(55)
handler(56)
handler(57)
handler(58)
handler(59)
handler(60)
handler(61)
handler(62)
handler(63)
handler(64)
handler(65)
handler(66)
handler(67)
handler(68)
handler(69)
handler(70)
handler(71)
handler(72)
handler(73)
handler(74)
handler(75)
handler(76)
handler(77)
handler(78)
handler(79)
handler(80)
handler(81)
handler(82)
handler(83)
handler(84)
handler(85)
handler(86)
handler(87)
handler(88)
handler(89)
handler(90)
handler(91)
handler(92)
handler(93)
handler(94)
handler(95)
handler(96)
handler(97)
handler(98)
handler(99)
handler(100)
handler(101)
handler(102)
handler(103)
handler(104)
handler(105)
handler(106)
handler(107)
handler(108)
handler(109)
handler(110)
handler(111)
handler(112)
handler(113)
handler(114)
handler(115)
handler(116)
handler(117)
handler(118)
handler(119)
handler(120)
handler(121)
handler(122)
handler(123)
handler(124)
handler(125)
handler(126)
handler(127)
handler(128)
handler(129)
handler(130)
handler(131)
handler(132)
handler(133)
handler(134)
handler(135)
handler(136)
handler(137)
handler(138)
handler(139)
handler(140)
handler(141)
handler(142)
handler(143)
handler(144)
handler(145)
handler(146)
handler(147)
handler(148)
handler(149)
handler(150)
handler(151)
handler(152)
handler(153)
handler(154)
handler(155)
handler(156)
handler(157)
handler(158)
handler(159)
handler(160)
handler(161)
handler(162)
handler(163)
handler(164)
handler(165)
handler(166)
handler(167)
handler(168)
handler(169)
handler(170)
handler(171)
handler(172)
handler(173)
handler(174)
handler(175)
handler(176)
handler(177)
handler(178)
handler(179)
handler(180)
handler(181)
handler(182)
handler(183)
handler(184)
handler(185)
handler(186)
handler(187)
handler(188)
handler(189)
handler(190)
handler(191)
handler(192)
handler(193)
handler(194)
handler(195)
handler(196)
handler(197)
handler(198)
handler(199)
handler(200)
handler(201)
handler(202)
handler(203)
handler(204)
handler(205)
handler(206)
handler(207)
handler(208)
handler(209)
handler(210)
handler(211)
handler(212)
handler(213)
handler(214)
handler(215)
handler(216)
handler(217)
handler(218)
handler(219)
handler(220)
handler(221)
handler(222)
handler(223)
handler(224)
handler(225)
handler(226)
handler(227)
handler(228)
handler(229)
handler(230)
handler(231)
handler(232)
handler(233)
handler(234)
handler(235)
handler(236)
handler(237)
handler(238)
handler(239)
handler(240)
handler(241)
handler(242)
handler(243)
handler(244)
handler(245)
handler(246)
handler(247)
handler(248)
handler(249)
handler(250)
handler(251)
handler(252)
handler(253)
handler(254)
handler(255)
handler(256)


