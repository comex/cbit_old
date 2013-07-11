#pragma once
/*
PP_SWALLOW(...) -> nothing
PP_CHOOSE(arg, nonempty, empty)
PP_ID(args...) -> args
PP_UNPAREN((args...)) -> args
PP_APPLY(func, ctx, args...)
PP_APPLY2(func, ctx, args...) (same as APPLY but works within APPLY)
PP_UNWRAP(prefix, prefix2, ctx, bits) ->
    given ctx and KIND(args...),
    transforms into prefix##KIND(ctx, args)
    prefix2 must have a definition for each KIND:
    #define prefix2##KIND(...) (KIND, __VA_ARGS__)
PP_UNWRAP2(prefix, prefix2, ctx, bits) (ditto)
*/

#define PP_SWALLOW(...)

#define PP_CHOOSE(arg, nonempty, empty) PP_CHOOSE_P(PP_CHOOSE_C##arg empty, nonempty)
#define PP_CHOOSE_C ,
#define PP_CHOOSE_P_(_, func, ...) func
#define PP_CHOOSE_P(...) PP_CHOOSE_P_(__VA_ARGS__)

#define PP_ID(...) __VA_ARGS__
#define PP_UNPAREN(bits) PP_ID bits

// PP_UNWRAP
#define PP_UNWRAP(prefix, prefix2, ctx, bits) \
    PP_UNWRAP_(prefix, ctx, PP_UNPAREN(prefix2##bits))
#define PP_UNWRAP_(prefix, ctx, bits) \
    PP_UNWRAP__(prefix, ctx, bits)
#define PP_UNWRAP__(prefix, ctx, kind, ...) \
    prefix##kind(ctx, __VA_ARGS__)

// PP_UNWRAP2
#define PP_UNWRAP2(prefix, prefix2, ctx, bits) \
    PP_UNWRAP2_(prefix, ctx, PP_UNPAREN(prefix2##bits))
#define PP_UNWRAP2_(prefix, ctx, bits) \
    PP_UNWRAP2__(prefix, ctx, bits)
#define PP_UNWRAP2__(prefix, ctx, kind, ...) \
    prefix##kind(ctx, __VA_ARGS__)

// PP_APPLY
#define PP_APPLY(func, ctx, ...) PP_APPLY_(func, ctx, __VA_ARGS__)
#define PP_APPLY_(func, ctx, arg, ...) PP_CHOOSE(arg, PP_APPLY_99, PP_SWALLOW)(func, ctx, arg, __VA_ARGS__)
#define PP_APPLY_99(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_98, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_98(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_97, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_97(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_96, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_96(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_95, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_95(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_94, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_94(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_93, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_93(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_92, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_92(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_91, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_91(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_90, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_90(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_89, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_89(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_88, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_88(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_87, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_87(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_86, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_86(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_85, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_85(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_84, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_84(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_83, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_83(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_82, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_82(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_81, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_81(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_80, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_80(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_79, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_79(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_78, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_78(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_77, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_77(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_76, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_76(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_75, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_75(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_74, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_74(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_73, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_73(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_72, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_72(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_71, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_71(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_70, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_70(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_69, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_69(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_68, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_68(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_67, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_67(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_66, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_66(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_65, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_65(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_64, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_64(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_63, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_63(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_62, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_62(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_61, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_61(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_60, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_60(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_59, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_59(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_58, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_58(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_57, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_57(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_56, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_56(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_55, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_55(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_54, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_54(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_53, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_53(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_52, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_52(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_51, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_51(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_50, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_50(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_49, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_49(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_48, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_48(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_47, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_47(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_46, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_46(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_45, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_45(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_44, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_44(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_43, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_43(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_42, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_42(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_41, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_41(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_40, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_40(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_39, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_39(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_38, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_38(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_37, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_37(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_36, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_36(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_35, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_35(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_34, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_34(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_33, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_33(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_32, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_32(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_31, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_31(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_30, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_30(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_29, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_29(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_28, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_28(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_27, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_27(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_26, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_26(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_25, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_25(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_24, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_24(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_23, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_23(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_22, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_22(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_21, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_21(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_20, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_20(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_19, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_19(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_18, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_18(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_17, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_17(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_16, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_16(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_15, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_15(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_14, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_14(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_13, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_13(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_12, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_12(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_11, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_11(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_10, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_10(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_09, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_09(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_08, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_08(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_07, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_07(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_06, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_06(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_05, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_05(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_04, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_04(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_03, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_03(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_02, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_02(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY_01, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY_01(func, ctx, arg, arg2, ...) func(ctx, arg)

// PP_APPLY2: this should be identical to PP_APPLY
#define PP_APPLY2(func, ctx, ...) PP_APPLY2_(func, ctx, __VA_ARGS__)
#define PP_APPLY2_(func, ctx, arg, ...) PP_CHOOSE(arg, PP_APPLY2_99, PP_SWALLOW)(func, ctx, arg, __VA_ARGS__)
#define PP_APPLY2_99(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_98, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_98(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_97, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_97(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_96, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_96(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_95, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_95(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_94, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_94(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_93, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_93(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_92, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_92(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_91, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_91(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_90, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_90(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_89, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_89(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_88, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_88(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_87, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_87(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_86, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_86(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_85, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_85(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_84, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_84(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_83, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_83(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_82, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_82(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_81, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_81(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_80, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_80(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_79, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_79(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_78, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_78(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_77, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_77(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_76, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_76(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_75, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_75(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_74, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_74(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_73, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_73(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_72, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_72(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_71, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_71(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_70, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_70(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_69, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_69(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_68, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_68(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_67, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_67(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_66, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_66(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_65, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_65(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_64, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_64(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_63, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_63(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_62, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_62(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_61, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_61(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_60, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_60(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_59, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_59(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_58, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_58(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_57, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_57(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_56, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_56(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_55, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_55(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_54, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_54(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_53, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_53(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_52, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_52(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_51, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_51(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_50, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_50(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_49, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_49(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_48, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_48(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_47, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_47(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_46, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_46(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_45, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_45(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_44, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_44(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_43, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_43(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_42, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_42(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_41, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_41(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_40, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_40(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_39, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_39(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_38, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_38(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_37, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_37(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_36, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_36(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_35, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_35(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_34, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_34(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_33, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_33(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_32, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_32(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_31, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_31(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_30, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_30(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_29, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_29(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_28, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_28(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_27, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_27(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_26, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_26(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_25, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_25(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_24, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_24(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_23, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_23(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_22, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_22(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_21, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_21(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_20, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_20(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_19, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_19(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_18, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_18(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_17, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_17(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_16, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_16(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_15, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_15(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_14, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_14(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_13, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_13(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_12, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_12(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_11, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_11(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_10, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_10(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_09, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_09(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_08, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_08(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_07, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_07(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_06, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_06(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_05, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_05(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_04, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_04(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_03, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_03(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_02, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_02(func, ctx, arg, arg2, ...) func(ctx, arg) PP_CHOOSE(arg2, PP_APPLY2_01, PP_SWALLOW)(func, ctx, arg2, __VA_ARGS__)
#define PP_APPLY2_01(func, ctx, arg, arg2, ...) func(ctx, arg)
