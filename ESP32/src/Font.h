#ifndef FONTDATA_H
#define FONTDATA_H

#define FONT_SIZE 2


MD_MAX72XX::fontType_t numeric7Se[] PROGMEM =
    {
        0,                          // 0
        3, 62, 34, 62,              // 1 - '0'
        3, 36, 62, 32,              // 2 - '1'
        3, 58, 42, 46,              // 3 - '2'
        3, 42, 42, 62,              // 4 - '3'
        3, 14, 8, 62,               // 5 - '4'
        3, 46, 42, 58,              // 6 - '5'
        3, 62, 42, 58,              // 7 - '6'
        3, 6, 2, 62,                // 8 - '7'
        3, 62, 42, 62,              // 9 - '8'
        3, 46, 42, 62,              // 10 - '9'
        0,                          // 11
        0,                          // 12
        0,                          // 13
        0,                          // 14
        0,                          // 15
        0,                          // 16
        0,                          // 17
        0,                          // 18
        0,                          // 19
        0,                          // 20
        0,                          // 21
        0,                          // 22
        0,                          // 23
        0,                          // 24
        0,                          // 25
        0,                          // 26
        0,                          // 27
        0,                          // 28
        0,                          // 29
        0,                          // 30
        0,                          // 31
        1, 0,                       // 32 - 'Space'
        3, 66, 127, 64,             // 33 - '!'
        0,                          // 34 - '"'
        0,                          // 35 - '#'
        0,                          // 36 - '$'
        0,                          // 37 - '%'
        0,                          // 38 - '&'
        0,                          // 39 - '''
        0,                          // 40 - '('
        0,                          // 41 - ')'
        0,                          // 42 - '*'
        0,                          // 43 - '+'
        0,                          // 44 - ','
        0,                          // 45 - '-'
        1, 64,                      // 46 - '.'
        0,                          // 47 - '/'
        4, 127, 65, 65, 127,        // 48 - '0'
        4, 0, 66, 127, 64,          // 49 - '1'
        4, 121, 73, 73, 79,         // 50 - '2'
        4, 99, 73, 73, 119,         // 51 - '3'
        4, 15, 8, 8, 127,           // 52 - '4'
        4, 79, 73, 73, 121,         // 53 - '5'
        4, 127, 73, 73, 121,        // 54 - '6'
        4, 3, 1, 1, 127,            // 55 - '7'
        4, 119, 73, 73, 119,        // 56 - '8'
        4, 79, 73, 73, 127,         // 57 - '9'
        1, 20,                      // 58 - ':'
        0,                          // 59 - ';'
        0,                          // 60 - '<'
        0,                          // 61 - '='
        0,                          // 62 - '>'
        0,                          // 63 - '?'
        5, 195, 231, 255, 231, 195, // 64 - '@'
        5, 126, 9, 9, 9, 126,       // 65 - 'A'
        5, 127, 73, 73, 73, 54,     // 66 - 'B'
        5, 62, 65, 65, 65, 65,      // 67 - 'C'
        5, 127, 65, 65, 65, 62,     // 68 - 'D'
        4, 127, 73, 73, 73,         // 69 - 'E'
        4, 127, 9, 9, 9,            // 70 - 'F'
        5, 127, 65, 65, 73, 121,    // 71 - 'G'
        5, 127, 8, 8, 8, 127,       // 72 - 'H'
        3, 65, 127, 65,             // 73 - 'I'
        5, 32, 64, 64, 64, 63,      // 74 - 'J'
        5, 127, 8, 20, 34, 65,      // 75 - 'K'
        4, 127, 64, 64, 64,         // 76 - 'L'
        5, 127, 2, 4, 2, 127,       // 77 - 'M'
        5, 127, 6, 8, 48, 127,      // 78 - 'N'
        5, 62, 65, 65, 65, 62,      // 79 - 'O'
        4, 127, 9, 9, 6,            // 80 - 'P'
        6, 62, 65, 65, 97, 126, 64, // 81 - 'Q'
        4, 127, 25, 41, 70,         // 82 - 'R'
        5, 70, 73, 73, 73, 49,      // 83 - 'S'
        5, 1, 1, 127, 1, 1,         // 84 - 'T'
        5, 63, 64, 64, 64, 63,      // 85 - 'U'
        5, 31, 32, 64, 32, 31,      // 86 - 'V'
        5, 127, 32, 16, 32, 127,    // 87 - 'W'
        5, 99, 20, 8, 20, 99,       // 88 - 'X'
        5, 1, 2, 124, 2, 1,         // 89 - 'Y'
        5, 97, 81, 73, 69, 67,      // 90 - 'Z'
        0,                          // 91 - '['
        0,                          // 92 - '\'
        0,                          // 93 - ']'
        0,                          // 94 - '^'
        0,                          // 95 - '_'
        1, 1,                       // 96 - '`'
        4, 116, 84, 84, 124,        // 97 - 'a'
        4, 127, 68, 68, 120,        // 98 - 'b'
        4, 124, 68, 68, 68,         // 99 - 'c'
        4, 120, 68, 68, 127,        // 100 - 'd'
        4, 124, 84, 84, 92,         // 101 - 'e'
        3, 4, 127, 5,               // 102 - 'f'
        4, 188, 164, 164, 252,      // 103 - 'g'
        4, 127, 4, 4, 120,          // 104 - 'h'
        1, 125,                     // 105 - 'i'
        3, 128, 132, 253,           // 106 - 'j'
        4, 127, 16, 40, 68,         // 107 - 'k'
        3, 65, 127, 64,             // 108 - 'l'
        5, 124, 4, 120, 4, 124,     // 109 - 'm'
        4, 124, 4, 4, 120,          // 110 - 'n'
        4, 56, 68, 68, 56,          // 111 - 'o'
        4, 252, 36, 36, 60,         // 112 - 'p'
        4, 60, 36, 36, 252,         // 113 - 'q'
        4, 120, 4, 4, 8,            // 114 - 'r'
        4, 92, 84, 84, 116,         // 115 - 's'
        3, 4, 127, 68,              // 116 - 't'
        4, 60, 64, 64, 124,         // 117 - 'u'
        5, 28, 32, 64, 32, 28,      // 118 - 'v'
        5, 60, 64, 32, 64, 60,      // 119 - 'w'
        5, 68, 40, 16, 40, 68,      // 120 - 'x'
        4, 156, 144, 144, 252,      // 121 - 'y'
        5, 68, 100, 84, 76, 68,     // 122 - 'z'
        0,                          // 123 - '{'
        1, 127,                     // 124 - '|'
        0,                          // 125
        0,                          // 126
        0,                          // 127
        0,                          // 128
        0,                          // 129
        0,                          // 130
        0,                          // 131
        0,                          // 132
        0,                          // 133
        0,                          // 134
        0,                          // 135
        0,                          // 136
        0,                          // 137
        0,                          // 138
        0,                          // 139
        0,                          // 140
        0,                          // 141
        0,                          // 142
        0,                          // 143
        0,                          // 144
        0,                          // 145
        0,                          // 146
        0,                          // 147
        0,                          // 148
        0,                          // 149
        0,                          // 150
        0,                          // 151
        0,                          // 152
        0,                          // 153
        0,                          // 154
        0,                          // 155
        0,                          // 156
        0,                          // 157
        0,                          // 158
        0,                          // 159
        0,                          // 160
        0,                          // 161
        0,                          // 162
        0,                          // 163
        0,                          // 164
        0,                          // 165
        0,                          // 166
        0,                          // 167
        0,                          // 168
        0,                          // 169
        0,                          // 170
        0,                          // 171
        0,                          // 172
        0,                          // 173
        0,                          // 174
        0,                          // 175
        0,                          // 176
        0,                          // 177
        0,                          // 178
        0,                          // 179
        0,                          // 180
        0,                          // 181
        0,                          // 182
        0,                          // 183
        0,                          // 184
        0,                          // 185
        0,                          // 186
        0,                          // 187
        0,                          // 188
        0,                          // 189
        0,                          // 190
        0,                          // 191
        0,                          // 192
        0,                          // 193
        0,                          // 194
        0,                          // 195
        0,                          // 196
        0,                          // 197
        0,                          // 198
        0,                          // 199
        0,                          // 200
        0,                          // 201
        0,                          // 202
        0,                          // 203
        0,                          // 204
        0,                          // 205
        0,                          // 206
        0,                          // 207
        0,                          // 208
        0,                          // 209
        0,                          // 210
        0,                          // 211
        0,                          // 212
        0,                          // 213
        0,                          // 214
        0,                          // 215
        0,                          // 216
        0,                          // 217
        0,                          // 218
        0,                          // 219
        0,                          // 220
        0,                          // 221
        0,                          // 222
        0,                          // 223
        0,                          // 224
        0,                          // 225
        0,                          // 226
        0,                          // 227
        0,                          // 228
        0,                          // 229
        0,                          // 230
        0,                          // 231
        0,                          // 232
        0,                          // 233
        0,                          // 234
        0,                          // 235
        0,                          // 236
        0,                          // 237
        0,                          // 238
        0,                          // 239
        0,                          // 240
        0,                          // 241
        0,                          // 242
        0,                          // 243
        0,                          // 244
        0,                          // 245
        0,                          // 246
        0,                          // 247
        0,                          // 248
        0,                          // 249
        0,                          // 250
        0,                          // 251
        0,                          // 252
        0,                          // 253
        0,                          // 254
        0,                          // 255
};


#endif