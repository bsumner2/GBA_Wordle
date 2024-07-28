
@{{BLOCK(WordleLetters)

@=======================================================================
@
@	WordleLetters, 216x8@4, 
@	+ palette 256 entries, not compressed
@	+ 27 tiles not compressed
@	Total size: 512 + 864 = 1376
@
@	Time-stamp: 2024-07-23, 04:05:15
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global WordleLettersTiles		@ 864 unsigned chars
	.hidden WordleLettersTiles
WordleLettersTiles:
	.word 0x33333333,0x32222223,0x32222223,0x32222223,0x32222223,0x32222223,0x32222223,0x33333333
	.word 0x33333333,0x32211223,0x32122123,0x32122123,0x32111123,0x32122123,0x32122123,0x33333333
	.word 0x33333333,0x32211123,0x32122123,0x32211123,0x32122123,0x32122123,0x32211123,0x33333333
	.word 0x33333333,0x32211223,0x32122123,0x32222123,0x32222123,0x32122123,0x32211223,0x33333333
	.word 0x33333333,0x32211123,0x32122123,0x32122123,0x32122123,0x32122123,0x32211123,0x33333333
	.word 0x33333333,0x32111123,0x32222123,0x32211123,0x32222123,0x32222123,0x32111123,0x33333333
	.word 0x33333333,0x32111123,0x32222123,0x32211123,0x32222123,0x32222123,0x32222123,0x33333333
	.word 0x33333333,0x32211223,0x32122123,0x32222123,0x32112123,0x32122123,0x32111223,0x33333333

	.word 0x33333333,0x32122123,0x32122123,0x32111123,0x32122123,0x32122123,0x32122123,0x33333333
	.word 0x33333333,0x32211123,0x32221223,0x32221223,0x32221223,0x32221223,0x32211123,0x33333333
	.word 0x33333333,0x32111123,0x32212223,0x32212223,0x32212223,0x32212123,0x32221123,0x33333333
	.word 0x33333333,0x32122123,0x32122123,0x32212123,0x32121123,0x32122123,0x32122123,0x33333333
	.word 0x33333333,0x32222123,0x32222123,0x32222123,0x32222123,0x32222123,0x32111123,0x33333333
	.word 0x33333333,0x32212123,0x32121213,0x32121213,0x32121213,0x32121213,0x32121213,0x33333333
	.word 0x33333333,0x32122123,0x32121123,0x32121123,0x32112123,0x32112123,0x32122123,0x33333333
	.word 0x33333333,0x32211223,0x32122123,0x32122123,0x32122123,0x32122123,0x32211223,0x33333333

	.word 0x33333333,0x32211123,0x32122123,0x32122123,0x32211123,0x32222123,0x32222123,0x33333333
	.word 0x33333333,0x32211123,0x32122213,0x32122213,0x32121213,0x32212213,0x32121123,0x33333333
	.word 0x33333333,0x32211123,0x32122123,0x32112123,0x32211123,0x32122123,0x32122123,0x33333333
	.word 0x33333333,0x32111223,0x32222123,0x32211223,0x32122223,0x32122123,0x32211223,0x33333333
	.word 0x33333333,0x32111113,0x32221223,0x32221223,0x32221223,0x32221223,0x32221223,0x33333333
	.word 0x33333333,0x32122123,0x32122123,0x32122123,0x32122123,0x32122123,0x32211223,0x33333333
	.word 0x33333333,0x32122213,0x32122213,0x32122213,0x32212123,0x32212123,0x32221223,0x33333333
	.word 0x33333333,0x32122213,0x32121213,0x32121213,0x32121213,0x32121213,0x32212123,0x33333333

	.word 0x33333333,0x32122213,0x32212123,0x32221223,0x32221223,0x32212123,0x32122213,0x33333333
	.word 0x33333333,0x32122213,0x32212123,0x32221223,0x32221223,0x32221223,0x32221223,0x33333333
	.word 0x33333333,0x32111123,0x32122223,0x32212223,0x32221223,0x32222123,0x32111123,0x33333333

	.section .rodata
	.align	2
	.global WordleLettersPal		@ 512 unsigned chars
	.hidden WordleLettersPal
WordleLettersPal:
	.hword 0x0000,0x7F79,0x1884,0x5AB3,0x1CA8,0x18EC,0x1D51,0x11DB
	.hword 0x329B,0x4F1D,0x112A,0x1CE6,0x38E7,0x4186,0x71AB,0x0000
	.hword 0x7E6C,0x46E1,0x1CA2,0x2100,0x47C0,0x1AED,0x3646,0x2B93
	.hword 0x15A9,0x732B,0x41F0,0x35AD,0x294B,0x450E,0x18D5,0x315B
	.hword 0x5DFA,0x4BBF,0x02BF,0x033F,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x2906,0x0C42,0x20A4,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x7FFF,0x1441,0x3D00,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(WordleLetters)
