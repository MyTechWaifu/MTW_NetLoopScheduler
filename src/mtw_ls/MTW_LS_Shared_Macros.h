// SPDX-License-Identifier: Apache-2.0
#pragma once
/*
 Preprocessor-only implementation of SHARED_CONFIG. The numbered expansion
 chain supports up to 254 rows without a run-time registration loop. These
 helpers generate declarations, checks and the sole descriptor/dirty arrays.
 They create no class and generate no per-variable object wrapper.

 An internal strong ownership symbol rejects two configuration definitions.
 The constexpr address list only validates duplicate storage at compilation.
 Ordinary generated address comparisons let the optimizer resolve named
 set/commit calls while retaining a complete lookup for run-time references.
*/
#define TLS_NLS_CAT_(a, b) a##b
#define TLS_NLS_CAT(a, b) TLS_NLS_CAT_(a, b)
#define TLS_NLS_FIRST(a, ...) a
#define TLS_NLS_ARITY_(_1, _2, _3, N, ...) N
#define TLS_NLS_ARITY(...) TLS_NLS_ARITY_(__VA_ARGS__, 3, 2, 1, 0)
#define TLS_NLS_UNPAREN(...) __VA_ARGS__
#define TLS_NLS_DECL_1(id, name)
#define TLS_NLS_DECL_2(id, name, type) type name = {};
#define TLS_NLS_DECL_3(id, name, type, value) type name = value;
#define TLS_NLS_DECL_A(id, ...) TLS_NLS_CAT(TLS_NLS_DECL_, TLS_NLS_ARITY(__VA_ARGS__))(id, __VA_ARGS__)
#define TLS_NLS_DECL_B(id, ...) TLS_NLS_DECL_A(id, __VA_ARGS__)
#define TLS_NLS_DECL(id, tuple) TLS_NLS_DECL_B(id, TLS_NLS_UNPAREN tuple)
#define TLS_NLS_CHECK(id, tuple) \
    static_assert((id) > 0 && (id) < 255, "Shared number must be in 1..254"); \
    static_assert(sizeof(TLS_NLS_FIRST tuple) == 1 || sizeof(TLS_NLS_FIRST tuple) == 2 || \
                  sizeof(TLS_NLS_FIRST tuple) == 4 || sizeof(TLS_NLS_FIRST tuple) == 8, \
                  "Shared scalar must occupy 1, 2, 4 or 8 bytes");
#define TLS_NLS_ITEM(id, tuple) \
    {static_cast<uint8_t*>(static_cast<void*>(&(TLS_NLS_FIRST tuple))), sizeof(TLS_NLS_FIRST tuple)},
#define TLS_NLS_ADDRESS(id, tuple) &(TLS_NLS_FIRST tuple),
#define TLS_NLS_MATCH(id, tuple) \
    if (tls_nls_value_address == &(TLS_NLS_FIRST tuple) && \
        tls_nls_value_size == sizeof(TLS_NLS_FIRST tuple)) { \
        return (id) - 1u; \
    }
#define TLS_NLS_INC_1 2
#define TLS_NLS_INC_2 3
#define TLS_NLS_INC_3 4
#define TLS_NLS_INC_4 5
#define TLS_NLS_INC_5 6
#define TLS_NLS_INC_6 7
#define TLS_NLS_INC_7 8
#define TLS_NLS_INC_8 9
#define TLS_NLS_INC_9 10
#define TLS_NLS_INC_10 11
#define TLS_NLS_INC_11 12
#define TLS_NLS_INC_12 13
#define TLS_NLS_INC_13 14
#define TLS_NLS_INC_14 15
#define TLS_NLS_INC_15 16
#define TLS_NLS_INC_16 17
#define TLS_NLS_INC_17 18
#define TLS_NLS_INC_18 19
#define TLS_NLS_INC_19 20
#define TLS_NLS_INC_20 21
#define TLS_NLS_INC_21 22
#define TLS_NLS_INC_22 23
#define TLS_NLS_INC_23 24
#define TLS_NLS_INC_24 25
#define TLS_NLS_INC_25 26
#define TLS_NLS_INC_26 27
#define TLS_NLS_INC_27 28
#define TLS_NLS_INC_28 29
#define TLS_NLS_INC_29 30
#define TLS_NLS_INC_30 31
#define TLS_NLS_INC_31 32
#define TLS_NLS_INC_32 33
#define TLS_NLS_INC_33 34
#define TLS_NLS_INC_34 35
#define TLS_NLS_INC_35 36
#define TLS_NLS_INC_36 37
#define TLS_NLS_INC_37 38
#define TLS_NLS_INC_38 39
#define TLS_NLS_INC_39 40
#define TLS_NLS_INC_40 41
#define TLS_NLS_INC_41 42
#define TLS_NLS_INC_42 43
#define TLS_NLS_INC_43 44
#define TLS_NLS_INC_44 45
#define TLS_NLS_INC_45 46
#define TLS_NLS_INC_46 47
#define TLS_NLS_INC_47 48
#define TLS_NLS_INC_48 49
#define TLS_NLS_INC_49 50
#define TLS_NLS_INC_50 51
#define TLS_NLS_INC_51 52
#define TLS_NLS_INC_52 53
#define TLS_NLS_INC_53 54
#define TLS_NLS_INC_54 55
#define TLS_NLS_INC_55 56
#define TLS_NLS_INC_56 57
#define TLS_NLS_INC_57 58
#define TLS_NLS_INC_58 59
#define TLS_NLS_INC_59 60
#define TLS_NLS_INC_60 61
#define TLS_NLS_INC_61 62
#define TLS_NLS_INC_62 63
#define TLS_NLS_INC_63 64
#define TLS_NLS_INC_64 65
#define TLS_NLS_INC_65 66
#define TLS_NLS_INC_66 67
#define TLS_NLS_INC_67 68
#define TLS_NLS_INC_68 69
#define TLS_NLS_INC_69 70
#define TLS_NLS_INC_70 71
#define TLS_NLS_INC_71 72
#define TLS_NLS_INC_72 73
#define TLS_NLS_INC_73 74
#define TLS_NLS_INC_74 75
#define TLS_NLS_INC_75 76
#define TLS_NLS_INC_76 77
#define TLS_NLS_INC_77 78
#define TLS_NLS_INC_78 79
#define TLS_NLS_INC_79 80
#define TLS_NLS_INC_80 81
#define TLS_NLS_INC_81 82
#define TLS_NLS_INC_82 83
#define TLS_NLS_INC_83 84
#define TLS_NLS_INC_84 85
#define TLS_NLS_INC_85 86
#define TLS_NLS_INC_86 87
#define TLS_NLS_INC_87 88
#define TLS_NLS_INC_88 89
#define TLS_NLS_INC_89 90
#define TLS_NLS_INC_90 91
#define TLS_NLS_INC_91 92
#define TLS_NLS_INC_92 93
#define TLS_NLS_INC_93 94
#define TLS_NLS_INC_94 95
#define TLS_NLS_INC_95 96
#define TLS_NLS_INC_96 97
#define TLS_NLS_INC_97 98
#define TLS_NLS_INC_98 99
#define TLS_NLS_INC_99 100
#define TLS_NLS_INC_100 101
#define TLS_NLS_INC_101 102
#define TLS_NLS_INC_102 103
#define TLS_NLS_INC_103 104
#define TLS_NLS_INC_104 105
#define TLS_NLS_INC_105 106
#define TLS_NLS_INC_106 107
#define TLS_NLS_INC_107 108
#define TLS_NLS_INC_108 109
#define TLS_NLS_INC_109 110
#define TLS_NLS_INC_110 111
#define TLS_NLS_INC_111 112
#define TLS_NLS_INC_112 113
#define TLS_NLS_INC_113 114
#define TLS_NLS_INC_114 115
#define TLS_NLS_INC_115 116
#define TLS_NLS_INC_116 117
#define TLS_NLS_INC_117 118
#define TLS_NLS_INC_118 119
#define TLS_NLS_INC_119 120
#define TLS_NLS_INC_120 121
#define TLS_NLS_INC_121 122
#define TLS_NLS_INC_122 123
#define TLS_NLS_INC_123 124
#define TLS_NLS_INC_124 125
#define TLS_NLS_INC_125 126
#define TLS_NLS_INC_126 127
#define TLS_NLS_INC_127 128
#define TLS_NLS_INC_128 129
#define TLS_NLS_INC_129 130
#define TLS_NLS_INC_130 131
#define TLS_NLS_INC_131 132
#define TLS_NLS_INC_132 133
#define TLS_NLS_INC_133 134
#define TLS_NLS_INC_134 135
#define TLS_NLS_INC_135 136
#define TLS_NLS_INC_136 137
#define TLS_NLS_INC_137 138
#define TLS_NLS_INC_138 139
#define TLS_NLS_INC_139 140
#define TLS_NLS_INC_140 141
#define TLS_NLS_INC_141 142
#define TLS_NLS_INC_142 143
#define TLS_NLS_INC_143 144
#define TLS_NLS_INC_144 145
#define TLS_NLS_INC_145 146
#define TLS_NLS_INC_146 147
#define TLS_NLS_INC_147 148
#define TLS_NLS_INC_148 149
#define TLS_NLS_INC_149 150
#define TLS_NLS_INC_150 151
#define TLS_NLS_INC_151 152
#define TLS_NLS_INC_152 153
#define TLS_NLS_INC_153 154
#define TLS_NLS_INC_154 155
#define TLS_NLS_INC_155 156
#define TLS_NLS_INC_156 157
#define TLS_NLS_INC_157 158
#define TLS_NLS_INC_158 159
#define TLS_NLS_INC_159 160
#define TLS_NLS_INC_160 161
#define TLS_NLS_INC_161 162
#define TLS_NLS_INC_162 163
#define TLS_NLS_INC_163 164
#define TLS_NLS_INC_164 165
#define TLS_NLS_INC_165 166
#define TLS_NLS_INC_166 167
#define TLS_NLS_INC_167 168
#define TLS_NLS_INC_168 169
#define TLS_NLS_INC_169 170
#define TLS_NLS_INC_170 171
#define TLS_NLS_INC_171 172
#define TLS_NLS_INC_172 173
#define TLS_NLS_INC_173 174
#define TLS_NLS_INC_174 175
#define TLS_NLS_INC_175 176
#define TLS_NLS_INC_176 177
#define TLS_NLS_INC_177 178
#define TLS_NLS_INC_178 179
#define TLS_NLS_INC_179 180
#define TLS_NLS_INC_180 181
#define TLS_NLS_INC_181 182
#define TLS_NLS_INC_182 183
#define TLS_NLS_INC_183 184
#define TLS_NLS_INC_184 185
#define TLS_NLS_INC_185 186
#define TLS_NLS_INC_186 187
#define TLS_NLS_INC_187 188
#define TLS_NLS_INC_188 189
#define TLS_NLS_INC_189 190
#define TLS_NLS_INC_190 191
#define TLS_NLS_INC_191 192
#define TLS_NLS_INC_192 193
#define TLS_NLS_INC_193 194
#define TLS_NLS_INC_194 195
#define TLS_NLS_INC_195 196
#define TLS_NLS_INC_196 197
#define TLS_NLS_INC_197 198
#define TLS_NLS_INC_198 199
#define TLS_NLS_INC_199 200
#define TLS_NLS_INC_200 201
#define TLS_NLS_INC_201 202
#define TLS_NLS_INC_202 203
#define TLS_NLS_INC_203 204
#define TLS_NLS_INC_204 205
#define TLS_NLS_INC_205 206
#define TLS_NLS_INC_206 207
#define TLS_NLS_INC_207 208
#define TLS_NLS_INC_208 209
#define TLS_NLS_INC_209 210
#define TLS_NLS_INC_210 211
#define TLS_NLS_INC_211 212
#define TLS_NLS_INC_212 213
#define TLS_NLS_INC_213 214
#define TLS_NLS_INC_214 215
#define TLS_NLS_INC_215 216
#define TLS_NLS_INC_216 217
#define TLS_NLS_INC_217 218
#define TLS_NLS_INC_218 219
#define TLS_NLS_INC_219 220
#define TLS_NLS_INC_220 221
#define TLS_NLS_INC_221 222
#define TLS_NLS_INC_222 223
#define TLS_NLS_INC_223 224
#define TLS_NLS_INC_224 225
#define TLS_NLS_INC_225 226
#define TLS_NLS_INC_226 227
#define TLS_NLS_INC_227 228
#define TLS_NLS_INC_228 229
#define TLS_NLS_INC_229 230
#define TLS_NLS_INC_230 231
#define TLS_NLS_INC_231 232
#define TLS_NLS_INC_232 233
#define TLS_NLS_INC_233 234
#define TLS_NLS_INC_234 235
#define TLS_NLS_INC_235 236
#define TLS_NLS_INC_236 237
#define TLS_NLS_INC_237 238
#define TLS_NLS_INC_238 239
#define TLS_NLS_INC_239 240
#define TLS_NLS_INC_240 241
#define TLS_NLS_INC_241 242
#define TLS_NLS_INC_242 243
#define TLS_NLS_INC_243 244
#define TLS_NLS_INC_244 245
#define TLS_NLS_INC_245 246
#define TLS_NLS_INC_246 247
#define TLS_NLS_INC_247 248
#define TLS_NLS_INC_248 249
#define TLS_NLS_INC_249 250
#define TLS_NLS_INC_250 251
#define TLS_NLS_INC_251 252
#define TLS_NLS_INC_252 253
#define TLS_NLS_INC_253 254
#define TLS_NLS_INC_254 255
#define TLS_NLS_INC_255 256
#define TLS_NLS_INC(i) TLS_NLS_CAT(TLS_NLS_INC_, i)
#define TLS_NLS_EACH_1(m, i, a) m(i, a)
#define TLS_NLS_EACH_2(m, i, a, ...) m(i, a) TLS_NLS_EACH_1(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_3(m, i, a, ...) m(i, a) TLS_NLS_EACH_2(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_4(m, i, a, ...) m(i, a) TLS_NLS_EACH_3(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_5(m, i, a, ...) m(i, a) TLS_NLS_EACH_4(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_6(m, i, a, ...) m(i, a) TLS_NLS_EACH_5(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_7(m, i, a, ...) m(i, a) TLS_NLS_EACH_6(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_8(m, i, a, ...) m(i, a) TLS_NLS_EACH_7(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_9(m, i, a, ...) m(i, a) TLS_NLS_EACH_8(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_10(m, i, a, ...) m(i, a) TLS_NLS_EACH_9(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_11(m, i, a, ...) m(i, a) TLS_NLS_EACH_10(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_12(m, i, a, ...) m(i, a) TLS_NLS_EACH_11(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_13(m, i, a, ...) m(i, a) TLS_NLS_EACH_12(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_14(m, i, a, ...) m(i, a) TLS_NLS_EACH_13(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_15(m, i, a, ...) m(i, a) TLS_NLS_EACH_14(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_16(m, i, a, ...) m(i, a) TLS_NLS_EACH_15(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_17(m, i, a, ...) m(i, a) TLS_NLS_EACH_16(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_18(m, i, a, ...) m(i, a) TLS_NLS_EACH_17(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_19(m, i, a, ...) m(i, a) TLS_NLS_EACH_18(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_20(m, i, a, ...) m(i, a) TLS_NLS_EACH_19(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_21(m, i, a, ...) m(i, a) TLS_NLS_EACH_20(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_22(m, i, a, ...) m(i, a) TLS_NLS_EACH_21(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_23(m, i, a, ...) m(i, a) TLS_NLS_EACH_22(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_24(m, i, a, ...) m(i, a) TLS_NLS_EACH_23(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_25(m, i, a, ...) m(i, a) TLS_NLS_EACH_24(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_26(m, i, a, ...) m(i, a) TLS_NLS_EACH_25(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_27(m, i, a, ...) m(i, a) TLS_NLS_EACH_26(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_28(m, i, a, ...) m(i, a) TLS_NLS_EACH_27(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_29(m, i, a, ...) m(i, a) TLS_NLS_EACH_28(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_30(m, i, a, ...) m(i, a) TLS_NLS_EACH_29(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_31(m, i, a, ...) m(i, a) TLS_NLS_EACH_30(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_32(m, i, a, ...) m(i, a) TLS_NLS_EACH_31(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_33(m, i, a, ...) m(i, a) TLS_NLS_EACH_32(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_34(m, i, a, ...) m(i, a) TLS_NLS_EACH_33(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_35(m, i, a, ...) m(i, a) TLS_NLS_EACH_34(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_36(m, i, a, ...) m(i, a) TLS_NLS_EACH_35(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_37(m, i, a, ...) m(i, a) TLS_NLS_EACH_36(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_38(m, i, a, ...) m(i, a) TLS_NLS_EACH_37(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_39(m, i, a, ...) m(i, a) TLS_NLS_EACH_38(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_40(m, i, a, ...) m(i, a) TLS_NLS_EACH_39(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_41(m, i, a, ...) m(i, a) TLS_NLS_EACH_40(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_42(m, i, a, ...) m(i, a) TLS_NLS_EACH_41(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_43(m, i, a, ...) m(i, a) TLS_NLS_EACH_42(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_44(m, i, a, ...) m(i, a) TLS_NLS_EACH_43(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_45(m, i, a, ...) m(i, a) TLS_NLS_EACH_44(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_46(m, i, a, ...) m(i, a) TLS_NLS_EACH_45(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_47(m, i, a, ...) m(i, a) TLS_NLS_EACH_46(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_48(m, i, a, ...) m(i, a) TLS_NLS_EACH_47(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_49(m, i, a, ...) m(i, a) TLS_NLS_EACH_48(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_50(m, i, a, ...) m(i, a) TLS_NLS_EACH_49(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_51(m, i, a, ...) m(i, a) TLS_NLS_EACH_50(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_52(m, i, a, ...) m(i, a) TLS_NLS_EACH_51(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_53(m, i, a, ...) m(i, a) TLS_NLS_EACH_52(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_54(m, i, a, ...) m(i, a) TLS_NLS_EACH_53(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_55(m, i, a, ...) m(i, a) TLS_NLS_EACH_54(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_56(m, i, a, ...) m(i, a) TLS_NLS_EACH_55(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_57(m, i, a, ...) m(i, a) TLS_NLS_EACH_56(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_58(m, i, a, ...) m(i, a) TLS_NLS_EACH_57(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_59(m, i, a, ...) m(i, a) TLS_NLS_EACH_58(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_60(m, i, a, ...) m(i, a) TLS_NLS_EACH_59(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_61(m, i, a, ...) m(i, a) TLS_NLS_EACH_60(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_62(m, i, a, ...) m(i, a) TLS_NLS_EACH_61(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_63(m, i, a, ...) m(i, a) TLS_NLS_EACH_62(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_64(m, i, a, ...) m(i, a) TLS_NLS_EACH_63(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_65(m, i, a, ...) m(i, a) TLS_NLS_EACH_64(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_66(m, i, a, ...) m(i, a) TLS_NLS_EACH_65(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_67(m, i, a, ...) m(i, a) TLS_NLS_EACH_66(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_68(m, i, a, ...) m(i, a) TLS_NLS_EACH_67(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_69(m, i, a, ...) m(i, a) TLS_NLS_EACH_68(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_70(m, i, a, ...) m(i, a) TLS_NLS_EACH_69(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_71(m, i, a, ...) m(i, a) TLS_NLS_EACH_70(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_72(m, i, a, ...) m(i, a) TLS_NLS_EACH_71(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_73(m, i, a, ...) m(i, a) TLS_NLS_EACH_72(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_74(m, i, a, ...) m(i, a) TLS_NLS_EACH_73(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_75(m, i, a, ...) m(i, a) TLS_NLS_EACH_74(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_76(m, i, a, ...) m(i, a) TLS_NLS_EACH_75(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_77(m, i, a, ...) m(i, a) TLS_NLS_EACH_76(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_78(m, i, a, ...) m(i, a) TLS_NLS_EACH_77(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_79(m, i, a, ...) m(i, a) TLS_NLS_EACH_78(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_80(m, i, a, ...) m(i, a) TLS_NLS_EACH_79(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_81(m, i, a, ...) m(i, a) TLS_NLS_EACH_80(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_82(m, i, a, ...) m(i, a) TLS_NLS_EACH_81(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_83(m, i, a, ...) m(i, a) TLS_NLS_EACH_82(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_84(m, i, a, ...) m(i, a) TLS_NLS_EACH_83(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_85(m, i, a, ...) m(i, a) TLS_NLS_EACH_84(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_86(m, i, a, ...) m(i, a) TLS_NLS_EACH_85(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_87(m, i, a, ...) m(i, a) TLS_NLS_EACH_86(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_88(m, i, a, ...) m(i, a) TLS_NLS_EACH_87(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_89(m, i, a, ...) m(i, a) TLS_NLS_EACH_88(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_90(m, i, a, ...) m(i, a) TLS_NLS_EACH_89(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_91(m, i, a, ...) m(i, a) TLS_NLS_EACH_90(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_92(m, i, a, ...) m(i, a) TLS_NLS_EACH_91(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_93(m, i, a, ...) m(i, a) TLS_NLS_EACH_92(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_94(m, i, a, ...) m(i, a) TLS_NLS_EACH_93(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_95(m, i, a, ...) m(i, a) TLS_NLS_EACH_94(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_96(m, i, a, ...) m(i, a) TLS_NLS_EACH_95(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_97(m, i, a, ...) m(i, a) TLS_NLS_EACH_96(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_98(m, i, a, ...) m(i, a) TLS_NLS_EACH_97(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_99(m, i, a, ...) m(i, a) TLS_NLS_EACH_98(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_100(m, i, a, ...) m(i, a) TLS_NLS_EACH_99(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_101(m, i, a, ...) m(i, a) TLS_NLS_EACH_100(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_102(m, i, a, ...) m(i, a) TLS_NLS_EACH_101(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_103(m, i, a, ...) m(i, a) TLS_NLS_EACH_102(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_104(m, i, a, ...) m(i, a) TLS_NLS_EACH_103(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_105(m, i, a, ...) m(i, a) TLS_NLS_EACH_104(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_106(m, i, a, ...) m(i, a) TLS_NLS_EACH_105(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_107(m, i, a, ...) m(i, a) TLS_NLS_EACH_106(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_108(m, i, a, ...) m(i, a) TLS_NLS_EACH_107(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_109(m, i, a, ...) m(i, a) TLS_NLS_EACH_108(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_110(m, i, a, ...) m(i, a) TLS_NLS_EACH_109(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_111(m, i, a, ...) m(i, a) TLS_NLS_EACH_110(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_112(m, i, a, ...) m(i, a) TLS_NLS_EACH_111(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_113(m, i, a, ...) m(i, a) TLS_NLS_EACH_112(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_114(m, i, a, ...) m(i, a) TLS_NLS_EACH_113(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_115(m, i, a, ...) m(i, a) TLS_NLS_EACH_114(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_116(m, i, a, ...) m(i, a) TLS_NLS_EACH_115(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_117(m, i, a, ...) m(i, a) TLS_NLS_EACH_116(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_118(m, i, a, ...) m(i, a) TLS_NLS_EACH_117(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_119(m, i, a, ...) m(i, a) TLS_NLS_EACH_118(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_120(m, i, a, ...) m(i, a) TLS_NLS_EACH_119(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_121(m, i, a, ...) m(i, a) TLS_NLS_EACH_120(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_122(m, i, a, ...) m(i, a) TLS_NLS_EACH_121(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_123(m, i, a, ...) m(i, a) TLS_NLS_EACH_122(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_124(m, i, a, ...) m(i, a) TLS_NLS_EACH_123(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_125(m, i, a, ...) m(i, a) TLS_NLS_EACH_124(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_126(m, i, a, ...) m(i, a) TLS_NLS_EACH_125(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_127(m, i, a, ...) m(i, a) TLS_NLS_EACH_126(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_128(m, i, a, ...) m(i, a) TLS_NLS_EACH_127(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_129(m, i, a, ...) m(i, a) TLS_NLS_EACH_128(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_130(m, i, a, ...) m(i, a) TLS_NLS_EACH_129(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_131(m, i, a, ...) m(i, a) TLS_NLS_EACH_130(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_132(m, i, a, ...) m(i, a) TLS_NLS_EACH_131(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_133(m, i, a, ...) m(i, a) TLS_NLS_EACH_132(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_134(m, i, a, ...) m(i, a) TLS_NLS_EACH_133(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_135(m, i, a, ...) m(i, a) TLS_NLS_EACH_134(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_136(m, i, a, ...) m(i, a) TLS_NLS_EACH_135(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_137(m, i, a, ...) m(i, a) TLS_NLS_EACH_136(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_138(m, i, a, ...) m(i, a) TLS_NLS_EACH_137(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_139(m, i, a, ...) m(i, a) TLS_NLS_EACH_138(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_140(m, i, a, ...) m(i, a) TLS_NLS_EACH_139(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_141(m, i, a, ...) m(i, a) TLS_NLS_EACH_140(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_142(m, i, a, ...) m(i, a) TLS_NLS_EACH_141(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_143(m, i, a, ...) m(i, a) TLS_NLS_EACH_142(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_144(m, i, a, ...) m(i, a) TLS_NLS_EACH_143(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_145(m, i, a, ...) m(i, a) TLS_NLS_EACH_144(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_146(m, i, a, ...) m(i, a) TLS_NLS_EACH_145(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_147(m, i, a, ...) m(i, a) TLS_NLS_EACH_146(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_148(m, i, a, ...) m(i, a) TLS_NLS_EACH_147(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_149(m, i, a, ...) m(i, a) TLS_NLS_EACH_148(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_150(m, i, a, ...) m(i, a) TLS_NLS_EACH_149(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_151(m, i, a, ...) m(i, a) TLS_NLS_EACH_150(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_152(m, i, a, ...) m(i, a) TLS_NLS_EACH_151(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_153(m, i, a, ...) m(i, a) TLS_NLS_EACH_152(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_154(m, i, a, ...) m(i, a) TLS_NLS_EACH_153(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_155(m, i, a, ...) m(i, a) TLS_NLS_EACH_154(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_156(m, i, a, ...) m(i, a) TLS_NLS_EACH_155(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_157(m, i, a, ...) m(i, a) TLS_NLS_EACH_156(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_158(m, i, a, ...) m(i, a) TLS_NLS_EACH_157(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_159(m, i, a, ...) m(i, a) TLS_NLS_EACH_158(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_160(m, i, a, ...) m(i, a) TLS_NLS_EACH_159(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_161(m, i, a, ...) m(i, a) TLS_NLS_EACH_160(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_162(m, i, a, ...) m(i, a) TLS_NLS_EACH_161(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_163(m, i, a, ...) m(i, a) TLS_NLS_EACH_162(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_164(m, i, a, ...) m(i, a) TLS_NLS_EACH_163(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_165(m, i, a, ...) m(i, a) TLS_NLS_EACH_164(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_166(m, i, a, ...) m(i, a) TLS_NLS_EACH_165(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_167(m, i, a, ...) m(i, a) TLS_NLS_EACH_166(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_168(m, i, a, ...) m(i, a) TLS_NLS_EACH_167(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_169(m, i, a, ...) m(i, a) TLS_NLS_EACH_168(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_170(m, i, a, ...) m(i, a) TLS_NLS_EACH_169(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_171(m, i, a, ...) m(i, a) TLS_NLS_EACH_170(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_172(m, i, a, ...) m(i, a) TLS_NLS_EACH_171(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_173(m, i, a, ...) m(i, a) TLS_NLS_EACH_172(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_174(m, i, a, ...) m(i, a) TLS_NLS_EACH_173(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_175(m, i, a, ...) m(i, a) TLS_NLS_EACH_174(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_176(m, i, a, ...) m(i, a) TLS_NLS_EACH_175(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_177(m, i, a, ...) m(i, a) TLS_NLS_EACH_176(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_178(m, i, a, ...) m(i, a) TLS_NLS_EACH_177(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_179(m, i, a, ...) m(i, a) TLS_NLS_EACH_178(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_180(m, i, a, ...) m(i, a) TLS_NLS_EACH_179(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_181(m, i, a, ...) m(i, a) TLS_NLS_EACH_180(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_182(m, i, a, ...) m(i, a) TLS_NLS_EACH_181(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_183(m, i, a, ...) m(i, a) TLS_NLS_EACH_182(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_184(m, i, a, ...) m(i, a) TLS_NLS_EACH_183(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_185(m, i, a, ...) m(i, a) TLS_NLS_EACH_184(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_186(m, i, a, ...) m(i, a) TLS_NLS_EACH_185(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_187(m, i, a, ...) m(i, a) TLS_NLS_EACH_186(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_188(m, i, a, ...) m(i, a) TLS_NLS_EACH_187(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_189(m, i, a, ...) m(i, a) TLS_NLS_EACH_188(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_190(m, i, a, ...) m(i, a) TLS_NLS_EACH_189(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_191(m, i, a, ...) m(i, a) TLS_NLS_EACH_190(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_192(m, i, a, ...) m(i, a) TLS_NLS_EACH_191(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_193(m, i, a, ...) m(i, a) TLS_NLS_EACH_192(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_194(m, i, a, ...) m(i, a) TLS_NLS_EACH_193(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_195(m, i, a, ...) m(i, a) TLS_NLS_EACH_194(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_196(m, i, a, ...) m(i, a) TLS_NLS_EACH_195(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_197(m, i, a, ...) m(i, a) TLS_NLS_EACH_196(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_198(m, i, a, ...) m(i, a) TLS_NLS_EACH_197(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_199(m, i, a, ...) m(i, a) TLS_NLS_EACH_198(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_200(m, i, a, ...) m(i, a) TLS_NLS_EACH_199(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_201(m, i, a, ...) m(i, a) TLS_NLS_EACH_200(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_202(m, i, a, ...) m(i, a) TLS_NLS_EACH_201(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_203(m, i, a, ...) m(i, a) TLS_NLS_EACH_202(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_204(m, i, a, ...) m(i, a) TLS_NLS_EACH_203(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_205(m, i, a, ...) m(i, a) TLS_NLS_EACH_204(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_206(m, i, a, ...) m(i, a) TLS_NLS_EACH_205(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_207(m, i, a, ...) m(i, a) TLS_NLS_EACH_206(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_208(m, i, a, ...) m(i, a) TLS_NLS_EACH_207(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_209(m, i, a, ...) m(i, a) TLS_NLS_EACH_208(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_210(m, i, a, ...) m(i, a) TLS_NLS_EACH_209(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_211(m, i, a, ...) m(i, a) TLS_NLS_EACH_210(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_212(m, i, a, ...) m(i, a) TLS_NLS_EACH_211(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_213(m, i, a, ...) m(i, a) TLS_NLS_EACH_212(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_214(m, i, a, ...) m(i, a) TLS_NLS_EACH_213(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_215(m, i, a, ...) m(i, a) TLS_NLS_EACH_214(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_216(m, i, a, ...) m(i, a) TLS_NLS_EACH_215(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_217(m, i, a, ...) m(i, a) TLS_NLS_EACH_216(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_218(m, i, a, ...) m(i, a) TLS_NLS_EACH_217(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_219(m, i, a, ...) m(i, a) TLS_NLS_EACH_218(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_220(m, i, a, ...) m(i, a) TLS_NLS_EACH_219(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_221(m, i, a, ...) m(i, a) TLS_NLS_EACH_220(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_222(m, i, a, ...) m(i, a) TLS_NLS_EACH_221(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_223(m, i, a, ...) m(i, a) TLS_NLS_EACH_222(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_224(m, i, a, ...) m(i, a) TLS_NLS_EACH_223(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_225(m, i, a, ...) m(i, a) TLS_NLS_EACH_224(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_226(m, i, a, ...) m(i, a) TLS_NLS_EACH_225(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_227(m, i, a, ...) m(i, a) TLS_NLS_EACH_226(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_228(m, i, a, ...) m(i, a) TLS_NLS_EACH_227(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_229(m, i, a, ...) m(i, a) TLS_NLS_EACH_228(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_230(m, i, a, ...) m(i, a) TLS_NLS_EACH_229(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_231(m, i, a, ...) m(i, a) TLS_NLS_EACH_230(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_232(m, i, a, ...) m(i, a) TLS_NLS_EACH_231(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_233(m, i, a, ...) m(i, a) TLS_NLS_EACH_232(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_234(m, i, a, ...) m(i, a) TLS_NLS_EACH_233(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_235(m, i, a, ...) m(i, a) TLS_NLS_EACH_234(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_236(m, i, a, ...) m(i, a) TLS_NLS_EACH_235(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_237(m, i, a, ...) m(i, a) TLS_NLS_EACH_236(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_238(m, i, a, ...) m(i, a) TLS_NLS_EACH_237(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_239(m, i, a, ...) m(i, a) TLS_NLS_EACH_238(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_240(m, i, a, ...) m(i, a) TLS_NLS_EACH_239(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_241(m, i, a, ...) m(i, a) TLS_NLS_EACH_240(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_242(m, i, a, ...) m(i, a) TLS_NLS_EACH_241(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_243(m, i, a, ...) m(i, a) TLS_NLS_EACH_242(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_244(m, i, a, ...) m(i, a) TLS_NLS_EACH_243(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_245(m, i, a, ...) m(i, a) TLS_NLS_EACH_244(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_246(m, i, a, ...) m(i, a) TLS_NLS_EACH_245(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_247(m, i, a, ...) m(i, a) TLS_NLS_EACH_246(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_248(m, i, a, ...) m(i, a) TLS_NLS_EACH_247(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_249(m, i, a, ...) m(i, a) TLS_NLS_EACH_248(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_250(m, i, a, ...) m(i, a) TLS_NLS_EACH_249(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_251(m, i, a, ...) m(i, a) TLS_NLS_EACH_250(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_252(m, i, a, ...) m(i, a) TLS_NLS_EACH_251(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_253(m, i, a, ...) m(i, a) TLS_NLS_EACH_252(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_254(m, i, a, ...) m(i, a) TLS_NLS_EACH_253(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_EACH_255(m, i, a, ...) m(i, a) TLS_NLS_EACH_254(m, TLS_NLS_INC(i), __VA_ARGS__)
#define TLS_NLS_COUNT_(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127, a128, a129, a130, a131, a132, a133, a134, a135, a136, a137, a138, a139, a140, a141, a142, a143, a144, a145, a146, a147, a148, a149, a150, a151, a152, a153, a154, a155, a156, a157, a158, a159, a160, a161, a162, a163, a164, a165, a166, a167, a168, a169, a170, a171, a172, a173, a174, a175, a176, a177, a178, a179, a180, a181, a182, a183, a184, a185, a186, a187, a188, a189, a190, a191, a192, a193, a194, a195, a196, a197, a198, a199, a200, a201, a202, a203, a204, a205, a206, a207, a208, a209, a210, a211, a212, a213, a214, a215, a216, a217, a218, a219, a220, a221, a222, a223, a224, a225, a226, a227, a228, a229, a230, a231, a232, a233, a234, a235, a236, a237, a238, a239, a240, a241, a242, a243, a244, a245, a246, a247, a248, a249, a250, a251, a252, a253, a254, a255, N, ...) N
#define TLS_NLS_COUNT(...) TLS_NLS_COUNT_(__VA_ARGS__, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define TLS_NLS_EACH(m, ...) TLS_NLS_CAT(TLS_NLS_EACH_, TLS_NLS_COUNT(__VA_ARGS__))(m, 1, __VA_ARGS__)

#define SHARED_CONFIG(...) \
    static_assert(TLS_NLS_COUNT(__VA_ARGS__) <= 254, "At most 254 shared variables"); \
    TLS_NLS_EACH(TLS_NLS_DECL, __VA_ARGS__) \
    TLS_NLS_EACH(TLS_NLS_CHECK, __VA_ARGS__) \
    constexpr const void* tls_nls_configuration_addresses[] = { TLS_NLS_EACH(TLS_NLS_ADDRESS, __VA_ARGS__) }; \
    static_assert(shared::detail::uniqueAll(tls_nls_configuration_addresses, TLS_NLS_COUNT(__VA_ARGS__), 0), \
                  "Each shared variable must be configured once"); \
    extern "C" { \
        netls::kernel::Object nls_shared_objects[] = { TLS_NLS_EACH(TLS_NLS_ITEM, __VA_ARGS__) }; \
        extern const uint8_t nls_shared_count = TLS_NLS_COUNT(__VA_ARGS__); \
        uint8_t nls_shared_dirty[(TLS_NLS_COUNT(__VA_ARGS__) + 7u) / 8u]; \
        uint8_t nls_shared_bit_masks[TLS_NLS_COUNT(__VA_ARGS__) < 8u ? TLS_NLS_COUNT(__VA_ARGS__) : 8u]; \
        uint8_t nls_shared_first_bit[1u << (TLS_NLS_COUNT(__VA_ARGS__) < 4u ? TLS_NLS_COUNT(__VA_ARGS__) : 4u)]; \
        extern const uint8_t nls_shared_search_mask = TLS_NLS_COUNT(__VA_ARGS__) < 8u ? (uint8_t)((1u << (TLS_NLS_COUNT(__VA_ARGS__) & 7u)) - 1u) : 255u; \
        extern const uint8_t nls_shared_tail_mask = (uint8_t)((1u << (TLS_NLS_COUNT(__VA_ARGS__) & 7u)) - 1u); \
        extern const uint8_t nls_shared_configuration_owner = 0u; \
    } \
    extern "C" uint8_t \
    nls_shared_lookup(const void* tls_nls_value_address, uint8_t tls_nls_value_size) { \
        TLS_NLS_EACH(TLS_NLS_MATCH, __VA_ARGS__) \
        return 255u; \
    }
