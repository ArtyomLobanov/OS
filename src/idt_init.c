#include <handlers.h>
#include <memory.h>
#include <desc.h>
#include <ioport.h>
#include <PIC.h>
#include <serial.h>



unsigned long long byte_mask = (1 << 8) - 1;
unsigned long long magic_for_idt = (1 << 15) + (14 << 8);
struct desc_table_ptr ptr_idt = {0, 0};
unsigned char idt[16 * 256];
void (*listeners[256])(int n);

unsigned char get_byte(unsigned long long value, int n) {
	return (value >> (n * 8)) & byte_mask;
}

void load_idt_item(unsigned char* place, void (*f)()) {
	unsigned long long adress = (unsigned long long) f;
	place[0] = get_byte(adress, 0);
	place[1] = get_byte(adress, 1);
	place[2] = get_byte(KERNEL_CS, 0);
	place[3] = get_byte(KERNEL_CS, 1);		
	place[4] = get_byte(magic_for_idt, 0);
	place[5] = get_byte(magic_for_idt, 1);
	place[6] = get_byte(adress, 2);
	place[7] = get_byte(adress, 3);
	place[8] = get_byte(adress, 4);
	place[9] = get_byte(adress, 5);
	place[10] = get_byte(adress, 6);
	place[11] = get_byte(adress, 7);
	place[12] = 0;
	place[13] = 0;
	place[14] = 0;		
	place[15] = 0;
}

void set_listener(int iterrupt_number, void (*f)(int)) {
	listeners[iterrupt_number] = f;
}

void handle_interrupt(int n) {
	print("Work with interrupt №");
	printInt(n);
	print("\n");
	if (n >= 32 && n < 48) {
		master_EOI();
	}
	if (n >= 40 && n < 48) {
		slave_EOI();
	}
	if (listeners[n] != 0) {
		listeners[n](n);
	}
}

void ld_all();

void idt_init() {
	ld_all();
	for (int i = 0; i < 256; i++) {
		listeners[i] = 0;
	}
	ptr_idt.size = 256 * 16 - 1;
	ptr_idt.addr = (unsigned long long)(&idt);
	write_idtr(&ptr_idt);
}

#define ld(i) load_idt_item(&idt[i * 16], handler_ ## i )
void ld_all() {
ld(0);
ld(1);
ld(2);
ld(3);
ld(4);
ld(5);
ld(6);
ld(7);
ld(8);
ld(9);
ld(10);
ld(11);
ld(12);
ld(13);
ld(14);
ld(15);
ld(16);
ld(17);
ld(18);
ld(19);
ld(20);
ld(21);
ld(22);
ld(23);
ld(24);
ld(25);
ld(26);
ld(27);
ld(28);
ld(29);
ld(30);
ld(31);
ld(32);
ld(33);
ld(34);
ld(35);
ld(36);
ld(37);
ld(38);
ld(39);
ld(40);
ld(41);
ld(42);
ld(43);
ld(44);
ld(45);
ld(46);
ld(47);
ld(48);
ld(49);
ld(50);
ld(51);
ld(52);
ld(53);
ld(54);
ld(55);
ld(56);
ld(57);
ld(58);
ld(59);
ld(60);
ld(61);
ld(62);
ld(63);
ld(64);
ld(65);
ld(66);
ld(67);
ld(68);
ld(69);
ld(70);
ld(71);
ld(72);
ld(73);
ld(74);
ld(75);
ld(76);
ld(77);
ld(78);
ld(79);
ld(80);
ld(81);
ld(82);
ld(83);
ld(84);
ld(85);
ld(86);
ld(87);
ld(88);
ld(89);
ld(90);
ld(91);
ld(92);
ld(93);
ld(94);
ld(95);
ld(96);
ld(97);
ld(98);
ld(99);
ld(100);
ld(101);
ld(102);
ld(103);
ld(104);
ld(105);
ld(106);
ld(107);
ld(108);
ld(109);
ld(110);
ld(111);
ld(112);
ld(113);
ld(114);
ld(115);
ld(116);
ld(117);
ld(118);
ld(119);
ld(120);
ld(121);
ld(122);
ld(123);
ld(124);
ld(125);
ld(126);
ld(127);
ld(128);
ld(129);
ld(130);
ld(131);
ld(132);
ld(133);
ld(134);
ld(135);
ld(136);
ld(137);
ld(138);
ld(139);
ld(140);
ld(141);
ld(142);
ld(143);
ld(144);
ld(145);
ld(146);
ld(147);
ld(148);
ld(149);
ld(150);
ld(151);
ld(152);
ld(153);
ld(154);
ld(155);
ld(156);
ld(157);
ld(158);
ld(159);
ld(160);
ld(161);
ld(162);
ld(163);
ld(164);
ld(165);
ld(166);
ld(167);
ld(168);
ld(169);
ld(170);
ld(171);
ld(172);
ld(173);
ld(174);
ld(175);
ld(176);
ld(177);
ld(178);
ld(179);
ld(180);
ld(181);
ld(182);
ld(183);
ld(184);
ld(185);
ld(186);
ld(187);
ld(188);
ld(189);
ld(190);
ld(191);
ld(192);
ld(193);
ld(194);
ld(195);
ld(196);
ld(197);
ld(198);
ld(199);
ld(200);
ld(201);
ld(202);
ld(203);
ld(204);
ld(205);
ld(206);
ld(207);
ld(208);
ld(209);
ld(210);
ld(211);
ld(212);
ld(213);
ld(214);
ld(215);
ld(216);
ld(217);
ld(218);
ld(219);
ld(220);
ld(221);
ld(222);
ld(223);
ld(224);
ld(225);
ld(226);
ld(227);
ld(228);
ld(229);
ld(230);
ld(231);
ld(232);
ld(233);
ld(234);
ld(235);
ld(236);
ld(237);
ld(238);
ld(239);
ld(240);
ld(241);
ld(242);
ld(243);
ld(244);
ld(245);
ld(246);
ld(247);
ld(248);
ld(249);
ld(250);
ld(251);
ld(252);
ld(253);
ld(254);
ld(255);
}
