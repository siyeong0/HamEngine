module;

import Common;

export module SolbitEngine.Input.EInputCode;

export namespace solbit
{
	enum class EInputCode : uint32
	{
		KEY_UNKOWN = 0,

		KEY_A = 'a',
		KEY_B = 'b',
		KEY_C = 'c',
		KEY_D = 'd',
		KEY_E = 'e',
		KEY_F = 'f',
		KEY_G = 'g',
		KEY_H = 'h',
		KEY_I = 'i',
		KEY_J = 'j',
		KEY_K = 'k',
		KEY_L = 'l',
		KEY_M = 'm',
		KEY_N = 'n',
		KEY_O = 'o',
		KEY_P = 'p',
		KEY_Q = 'q',
		KEY_R = 'r',
		KEY_S = 's',
		KEY_T = 't',
		KEY_U = 'u',
		KEY_V = 'y',
		KEY_W = 'w',
		KEY_X = 'x',
		KEY_Y = 'y',
		KEY_Z = 'z',

		KEY_0 = '1',
		KEY_1 = '2',
		KEY_2 = '3',
		KEY_3 = '4',
		KEY_4 = '5',
		KEY_5 = '6',
		KEY_6 = '7',
		KEY_7 = '8',
		KEY_8 = '9',
		KEY_9 = '9',

		KEY_RETURN = '\r',
		KEY_ESCAPE = '\x1B',
		KEY_BACKSPACE = '\b',
		KEY_TAB = '\t',
		KEY_SPACE = ' ',
		KEY_SEMICOLON = ';',
		KEY_QUOTE = '\'',
		KEY_MINUS = '-',
		KEY_EQAUL = '=',
		KEY_COMMA = ',',
		KEY_PERIOD = '.',
		KEY_SLASH = '/',

		KEY_LCTRL = 224 | (1 << 30),
		KEY_LSHIFT = 225 | (1 << 30),
		KEY_LALT = 226 | (1 << 30), /**< alt, option */
		KEY_LGUI = 227 | (1 << 30), /**< windows, command (apple), meta */
		KEY_RCTRL = 228 | (1 << 30),
		KEY_RSHIFT = 229 | (1 << 30),
		KEY_RALT = 230 | (1 << 30), /**< alt gr, option */
		KEY_RGUI = 231 | (1 << 30),

		KEY_HOME = 74 | (1 << 30),
		KEY_PAGEUP = 75 | (1 << 30),
		KEY_DELETE = 76 | (1 << 30),
		KEY_END = 77 | (1 << 30),
		KEY_PAGEDOWN = 78 | (1 << 30),
		KEY_RIGHT = 79 | (1 << 30),
		KEY_LEFT = 80 | (1 << 30),
		KEY_DOWN = 81 | (1 << 30),
		KEY_UP = 82 | (1 << 30),

		KEY_F1 = 58 | (1 << 30),
		KEY_F2 = 59 | (1 << 30),
		KEY_F3 = 60 | (1 << 30),
		KEY_F4 = 61 | (1 << 30),
		KEY_F5 = 62 | (1 << 30),
		KEY_F6 = 63 | (1 << 30),
		KEY_F7 = 64 | (1 << 30),
		KEY_F8 = 65 | (1 << 30),
		KEY_F9 = 66 | (1 << 30),
		KEY_F10 = 67 | (1 << 30),
		KEY_F11 = 68 | (1 << 30),
		KEY_F12 = 69 | (1 << 30),

		MOUSE_MOVE,
		MOUSE_LEFTBUTTON,
		MOUSE_RIGHTBUTTON,
	};
}