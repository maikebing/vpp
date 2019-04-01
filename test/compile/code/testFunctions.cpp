// -----------------------------------------------------------------------------

#include <vppAll.hpp>

// -----------------------------------------------------------------------------

class KFunctionTestPipeline : public vpp::ComputePipelineConfig
{
public:
    KFunctionTestPipeline (
        const vpp::Device& hDevice );

    void fComputeShader ( vpp::ComputeShader* pShader );

private:
    void testScalarsVectors();
    void testMatrices();

private:
    vpp::computeShader d_computeShader;
};

// -----------------------------------------------------------------------------

KFunctionTestPipeline :: KFunctionTestPipeline (
    const vpp::Device& hDevice ) :
        d_computeShader ( this, { 32, 1, 1 }, & KFunctionTestPipeline::fComputeShader )
{
}

// -----------------------------------------------------------------------------

void KFunctionTestPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
{
    testScalarsVectors();
    testMatrices();
}

// -----------------------------------------------------------------------------

void KFunctionTestPipeline :: testScalarsVectors()
{
    using namespace vpp;

    // -------------------------------------------------------------------------

    bool constBool = true;
    int constInt = 7;
    unsigned int constUInt = 7u;
    float constFloat = 7.7f;
    double constDouble = 7.7;

    Bool argBool = true;
    Int argInt = 7;
    UInt argUInt = 7u;
    Float argFloat = 7.7f;
    Double argDouble = 7.7;

    BVec2 argBVec2 { true, false };
    IVec2 argIVec2 { 7, 8 };
    UVec2 argUVec2 { 7u, 8u };
    Vec2 argVec2 { 7.7f, 8.8f };
    DVec2 argDVec2 { 7.7, 8.8 };

    BVec3 argBVec3 { true, false, true };
    IVec3 argIVec3 { 7, 8, 9 };
    UVec3 argUVec3 { 7u, 8u, 9u };
    Vec3 argVec3 { 7.7f, 8.8f, 9.9f };
    DVec3 argDVec3 { 7.7, 8.8, 9.9 };

    BVec4 argBVec4 { true, false, true, false };
    IVec4 argIVec4 { 7, 8, 9, 10 };
    UVec4 argUVec4 { 7u, 8u, 9u, 10u };
    Vec4 argVec4 { 7.7f, 8.8f, 9.9f, 10.10f };
    DVec4 argDVec4 { 7.7, 8.8, 9.9, 10.0 };

    // -------------------------------------------------------------------------

    Float fRound0 = Round ( constFloat );
    Float fRound1 = Round ( argFloat );
    Vec2 fRound2 = Round ( argVec2 );
    Vec3 fRound3 = Round ( argVec3 );
    Vec4 fRound4 = Round ( argVec4 );

    Double dRound0 = Round ( constDouble );
    Double dRound1 = Round ( argDouble );
    DVec2 dRound2 = Round ( argDVec2 );
    DVec3 dRound3 = Round ( argDVec3 );
    DVec4 dRound4 = Round ( argDVec4 );

    Float fRoundEven0 = RoundEven ( constFloat );
    Float fRoundEven1 = RoundEven ( argFloat );
    Vec2 fRoundEven2 = RoundEven ( argVec2 );
    Vec3 fRoundEven3 = RoundEven ( argVec3 );
    Vec4 fRoundEven4 = RoundEven ( argVec4 );

    Double dRoundEven0 = RoundEven ( constDouble );
    Double dRoundEven1 = RoundEven ( argDouble );
    DVec2 dRoundEven2 = RoundEven ( argDVec2 );
    DVec3 dRoundEven3 = RoundEven ( argDVec3 );
    DVec4 dRoundEven4 = RoundEven ( argDVec4 );

    Float fTrunc0 = Trunc ( constFloat );
    Float fTrunc1 = Trunc ( argFloat );
    Vec2 fTrunc2 = Trunc ( argVec2 );
    Vec3 fTrunc3 = Trunc ( argVec3 );
    Vec4 fTrunc4 = Trunc ( argVec4 );

    Double dTrunc0 = Trunc ( constDouble );
    Double dTrunc1 = Trunc ( argDouble );
    DVec2 dTrunc2 = Trunc ( argDVec2 );
    DVec3 dTrunc3 = Trunc ( argDVec3 );
    DVec4 dTrunc4 = Trunc ( argDVec4 );

    Int iAbs0 = Abs ( constInt );
    Int iAbs1 = Abs ( argInt );
    IVec2 iAbs2 = Abs ( argIVec2 );
    IVec3 iAbs3 = Abs ( argIVec3 );
    IVec4 iAbs4 = Abs ( argIVec4 );

    Float fAbs0 = Abs ( constFloat );
    Float fAbs1 = Abs ( argFloat );
    Vec2 fAbs2 = Abs ( argVec2 );
    Vec3 fAbs3 = Abs ( argVec3 );
    Vec4 fAbs4 = Abs ( argVec4 );

    Double dAbs0 = Abs ( constDouble );
    Double dAbs1 = Abs ( argDouble );
    DVec2 dAbs2 = Abs ( argDVec2 );
    DVec3 dAbs3 = Abs ( argDVec3 );
    DVec4 dAbs4 = Abs ( argDVec4 );

    Int iSign0 = Sign ( constInt );
    Int iSign1 = Sign ( argInt );
    IVec2 iSign2 = Sign ( argIVec2 );
    IVec3 iSign3 = Sign ( argIVec3 );
    IVec4 iSign4 = Sign ( argIVec4 );

    Float fSign0 = Sign ( constFloat );
    Float fSign1 = Sign ( argFloat );
    Vec2 fSign2 = Sign ( argVec2 );
    Vec3 fSign3 = Sign ( argVec3 );
    Vec4 fSign4 = Sign ( argVec4 );

    Double dSign0 = Sign ( constDouble );
    Double dSign1 = Sign ( argDouble );
    DVec2 dSign2 = Sign ( argDVec2 );
    DVec3 dSign3 = Sign ( argDVec3 );
    DVec4 dSign4 = Sign ( argDVec4 );

    Float fFloor0 = Floor ( constFloat );
    Float fFloor1 = Floor ( argFloat );
    Vec2 fFloor2 = Floor ( argVec2 );
    Vec3 fFloor3 = Floor ( argVec3 );
    Vec4 fFloor4 = Floor ( argVec4 );

    Double dFloor0 = Floor ( constDouble );
    Double dFloor1 = Floor ( argDouble );
    DVec2 dFloor2 = Floor ( argDVec2 );
    DVec3 dFloor3 = Floor ( argDVec3 );
    DVec4 dFloor4 = Floor ( argDVec4 );

    Float fCeil0 = Ceil ( constFloat );
    Float fCeil1 = Ceil ( argFloat );
    Vec2 fCeil2 = Ceil ( argVec2 );
    Vec3 fCeil3 = Ceil ( argVec3 );
    Vec4 fCeil4 = Ceil ( argVec4 );

    Double dCeil0 = Ceil ( constDouble );
    Double dCeil1 = Ceil ( argDouble );
    DVec2 dCeil2 = Ceil ( argDVec2 );
    DVec3 dCeil3 = Ceil ( argDVec3 );
    DVec4 dCeil4 = Ceil ( argDVec4 );

    Float fFract0 = Fract ( constFloat );
    Float fFract1 = Fract ( argFloat );
    Vec2 fFract2 = Fract ( argVec2 );
    Vec3 fFract3 = Fract ( argVec3 );
    Vec4 fFract4 = Fract ( argVec4 );

    Double dFract0 = Fract ( constDouble );
    Double dFract1 = Fract ( argDouble );
    DVec2 dFract2 = Fract ( argDVec2 );
    DVec3 dFract3 = Fract ( argDVec3 );
    DVec4 dFract4 = Fract ( argDVec4 );

    Float fRadians0 = Radians ( constFloat );
    Float fRadians1 = Radians ( argFloat );
    Vec2 fRadians2 = Radians ( argVec2 );
    Vec3 fRadians3 = Radians ( argVec3 );
    Vec4 fRadians4 = Radians ( argVec4 );

    Float fDegrees0 = Degrees ( constFloat );
    Float fDegrees1 = Degrees ( argFloat );
    Vec2 fDegrees2 = Degrees ( argVec2 );
    Vec3 fDegrees3 = Degrees ( argVec3 );
    Vec4 fDegrees4 = Degrees ( argVec4 );

    Float fSin0 = Sin ( constFloat );
    Float fSin1 = Sin ( argFloat );
    Vec2 fSin2 = Sin ( argVec2 );
    Vec3 fSin3 = Sin ( argVec3 );
    Vec4 fSin4 = Sin ( argVec4 );

    Float fCos0 = Cos ( constFloat );
    Float fCos1 = Cos ( argFloat );
    Vec2 fCos2 = Cos ( argVec2 );
    Vec3 fCos3 = Cos ( argVec3 );
    Vec4 fCos4 = Cos ( argVec4 );

    Float fTan0 = Tan ( constFloat );
    Float fTan1 = Tan ( argFloat );
    Vec2 fTan2 = Tan ( argVec2 );
    Vec3 fTan3 = Tan ( argVec3 );
    Vec4 fTan4 = Tan ( argVec4 );

    Float fAsin0 = Asin ( constFloat );
    Float fAsin1 = Asin ( argFloat );
    Vec2 fAsin2 = Asin ( argVec2 );
    Vec3 fAsin3 = Asin ( argVec3 );
    Vec4 fAsin4 = Asin ( argVec4 );

    Float fAcos0 = Acos ( constFloat );
    Float fAcos1 = Acos ( argFloat );
    Vec2 fAcos2 = Acos ( argVec2 );
    Vec3 fAcos3 = Acos ( argVec3 );
    Vec4 fAcos4 = Acos ( argVec4 );

    Float fAtan0 = Atan ( constFloat );
    Float fAtan1 = Atan ( argFloat );
    Vec2 fAtan2 = Atan ( argVec2 );
    Vec3 fAtan3 = Atan ( argVec3 );
    Vec4 fAtan4 = Atan ( argVec4 );

    Float fSinh0 = Sinh ( constFloat );
    Float fSinh1 = Sinh ( argFloat );
    Vec2 fSinh2 = Sinh ( argVec2 );
    Vec3 fSinh3 = Sinh ( argVec3 );
    Vec4 fSinh4 = Sinh ( argVec4 );

    Float fCosh0 = Cosh ( constFloat );
    Float fCosh1 = Cosh ( argFloat );
    Vec2 fCosh2 = Cosh ( argVec2 );
    Vec3 fCosh3 = Cosh ( argVec3 );
    Vec4 fCosh4 = Cosh ( argVec4 );

    Float fTanh0 = Tanh ( constFloat );
    Float fTanh1 = Tanh ( argFloat );
    Vec2 fTanh2 = Tanh ( argVec2 );
    Vec3 fTanh3 = Tanh ( argVec3 );
    Vec4 fTanh4 = Tanh ( argVec4 );

    Float fAsinh0 = Asinh ( constFloat );
    Float fAsinh1 = Asinh ( argFloat );
    Vec2 fAsinh2 = Asinh ( argVec2 );
    Vec3 fAsinh3 = Asinh ( argVec3 );
    Vec4 fAsinh4 = Asinh ( argVec4 );

    Float fAcosh0 = Acosh ( constFloat );
    Float fAcosh1 = Acosh ( argFloat );
    Vec2 fAcosh2 = Acosh ( argVec2 );
    Vec3 fAcosh3 = Acosh ( argVec3 );
    Vec4 fAcosh4 = Acosh ( argVec4 );

    Float fAtanh0 = Atanh ( constFloat );
    Float fAtanh1 = Atanh ( argFloat );
    Vec2 fAtanh2 = Atanh ( argVec2 );
    Vec3 fAtanh3 = Atanh ( argVec3 );
    Vec4 fAtanh4 = Atanh ( argVec4 );

    Float fAtan20 = Atan2 ( constFloat, constFloat );
    Float fAtan21 = Atan2 ( argFloat, argFloat );
    Vec2 fAtan22 = Atan2 ( argVec2, argVec2 );
    Vec3 fAtan23 = Atan2 ( argVec3, argVec3 );
    Vec4 fAtan24 = Atan2 ( argVec4, argVec4 );

    Float fPow0 = Pow ( constFloat, constFloat );
    Float fPow1 = Pow ( argFloat, argFloat );
    Vec2 fPow2 = Pow ( argVec2, argVec2 );
    Vec3 fPow3 = Pow ( argVec3, argVec3 );
    Vec4 fPow4 = Pow ( argVec4, argVec4 );

    Float fExp0 = Exp ( constFloat );
    Float fExp1 = Exp ( argFloat );
    Vec2 fExp2 = Exp ( argVec2 );
    Vec3 fExp3 = Exp ( argVec3 );
    Vec4 fExp4 = Exp ( argVec4 );

    Float fLog0 = Log ( constFloat );
    Float fLog1 = Log ( argFloat );
    Vec2 fLog2 = Log ( argVec2 );
    Vec3 fLog3 = Log ( argVec3 );
    Vec4 fLog4 = Log ( argVec4 );

    Float fExp20 = Exp2 ( constFloat );
    Float fExp21 = Exp2 ( argFloat );
    Vec2 fExp22 = Exp2 ( argVec2 );
    Vec3 fExp23 = Exp2 ( argVec3 );
    Vec4 fExp24 = Exp2 ( argVec4 );

    Float fLog20 = Log2 ( constFloat );
    Float fLog21 = Log2 ( argFloat );
    Vec2 fLog22 = Log2 ( argVec2 );
    Vec3 fLog23 = Log2 ( argVec3 );
    Vec4 fLog24 = Log2 ( argVec4 );

    Float fSqrt0 = Sqrt ( constFloat );
    Float fSqrt1 = Sqrt ( argFloat );
    Vec2 fSqrt2 = Sqrt ( argVec2 );
    Vec3 fSqrt3 = Sqrt ( argVec3 );
    Vec4 fSqrt4 = Sqrt ( argVec4 );

    Double dSqrt0 = Sqrt ( constDouble );
    Double dSqrt1 = Sqrt ( argDouble );
    DVec2 dSqrt2 = Sqrt ( argDVec2 );
    DVec3 dSqrt3 = Sqrt ( argDVec3 );
    DVec4 dSqrt4 = Sqrt ( argDVec4 );

    Float fInverseSqrt0 = InverseSqrt ( constFloat );
    Float fInverseSqrt1 = InverseSqrt ( argFloat );
    Vec2 fInverseSqrt2 = InverseSqrt ( argVec2 );
    Vec3 fInverseSqrt3 = InverseSqrt ( argVec3 );
    Vec4 fInverseSqrt4 = InverseSqrt ( argVec4 );

    Double dInverseSqrt0 = InverseSqrt ( constDouble );
    Double dInverseSqrt1 = InverseSqrt ( argDouble );
    DVec2 dInverseSqrt2 = InverseSqrt ( argDVec2 );
    DVec3 dInverseSqrt3 = InverseSqrt ( argDVec3 );
    DVec4 dInverseSqrt4 = InverseSqrt ( argDVec4 );

    Int   iMin0 = Min ( constInt, constInt );
    Int   iMin1 = Min ( argInt, argInt );
    IVec2 iMin2 = Min ( argIVec2, argIVec2 );
    IVec3 iMin3 = Min ( argIVec3, argIVec3 );
    IVec4 iMin4 = Min ( argIVec4, argIVec4 );

    UInt  uMin0 = Min ( constUInt, constUInt );
    UInt  uMin1 = Min ( argUInt, argUInt );
    UVec2 uMin2 = Min ( argUVec2, argUVec2 );
    UVec3 uMin3 = Min ( argUVec3, argUVec3 );
    UVec4 uMin4 = Min ( argUVec4, argUVec4 );

    Float fMin0 = Min ( constFloat, constFloat );
    Float fMin1 = Min ( argFloat, argFloat );
    Vec2  fMin2 = Min ( argVec2, argVec2 );
    Vec3  fMin3 = Min ( argVec3, argVec3 );
    Vec4  fMin4 = Min ( argVec4, argVec4 );

    Double dMin0 = Min ( constDouble, constDouble );
    Double dMin1 = Min ( argDouble, argDouble );
    DVec2  dMin2 = Min ( argDVec2, argDVec2 );
    DVec3  dMin3 = Min ( argDVec3, argDVec3 );
    DVec4  dMin4 = Min ( argDVec4, argDVec4 );

    Int   iMax0 = Max ( constInt, constInt );
    Int   iMax1 = Max ( argInt, argInt );
    IVec2 iMax2 = Max ( argIVec2, argIVec2 );
    IVec3 iMax3 = Max ( argIVec3, argIVec3 );
    IVec4 iMax4 = Max ( argIVec4, argIVec4 );

    UInt  uMax0 = Max ( constUInt, constUInt );
    UInt  uMax1 = Max ( argUInt, argUInt );
    UVec2 uMax2 = Max ( argUVec2, argUVec2 );
    UVec3 uMax3 = Max ( argUVec3, argUVec3 );
    UVec4 uMax4 = Max ( argUVec4, argUVec4 );

    Float fMax0 = Max ( constFloat, constFloat );
    Float fMax1 = Max ( argFloat, argFloat );
    Vec2  fMax2 = Max ( argVec2, argVec2 );
    Vec3  fMax3 = Max ( argVec3, argVec3 );
    Vec4  fMax4 = Max ( argVec4, argVec4 );

    Double dMax0 = Max ( constDouble, constDouble );
    Double dMax1 = Max ( argDouble, argDouble );
    DVec2  dMax2 = Max ( argDVec2, argDVec2 );
    DVec3  dMax3 = Max ( argDVec3, argDVec3 );
    DVec4  dMax4 = Max ( argDVec4, argDVec4 );

    Int   iClamp0 = Clamp ( constInt, constInt, constInt );
    Int   iClamp1 = Clamp ( argInt, argInt, argInt );
    IVec2 iClamp2 = Clamp ( argIVec2, argIVec2, argIVec2 );
    IVec3 iClamp3 = Clamp ( argIVec3, argIVec3, argIVec3 );
    IVec4 iClamp4 = Clamp ( argIVec4, argIVec4, argIVec4 );

    UInt  uClamp0 = Clamp ( constUInt, constUInt, constUInt );
    UInt  uClamp1 = Clamp ( argUInt, argUInt, argUInt );
    UVec2 uClamp2 = Clamp ( argUVec2, argUVec2, argUVec2 );
    UVec3 uClamp3 = Clamp ( argUVec3, argUVec3, argUVec3 );
    UVec4 uClamp4 = Clamp ( argUVec4, argUVec4, argUVec4 );

    Float fClamp0 = Clamp ( constFloat, constFloat, constFloat );
    Float fClamp1 = Clamp ( argFloat, argFloat, argFloat );
    Vec2  fClamp2 = Clamp ( argVec2, argVec2, argVec2 );
    Vec3  fClamp3 = Clamp ( argVec3, argVec3, argVec3 );
    Vec4  fClamp4 = Clamp ( argVec4, argVec4, argVec4 );

    Double dClamp0 = Clamp ( constDouble, constDouble, constDouble );
    Double dClamp1 = Clamp ( argDouble, argDouble, argDouble );
    DVec2  dClamp2 = Clamp ( argDVec2, argDVec2, argDVec2 );
    DVec3  dClamp3 = Clamp ( argDVec3, argDVec3, argDVec3 );
    DVec4  dClamp4 = Clamp ( argDVec4, argDVec4, argDVec4 );

    Float fMix0 = Mix ( constFloat, constFloat, constFloat );
    Float fMix1 = Mix ( argFloat, argFloat, argFloat );
    Vec2  fMix2 = Mix ( argVec2, argVec2, argVec2 );
    Vec3  fMix3 = Mix ( argVec3, argVec3, argVec3 );
    Vec4  fMix4 = Mix ( argVec4, argVec4, argVec4 );

    Double dMix0 = Mix ( constDouble, constDouble, constDouble );
    Double dMix1 = Mix ( argDouble, argDouble, argDouble );
    DVec2  dMix2 = Mix ( argDVec2, argDVec2, argDVec2 );
    DVec3  dMix3 = Mix ( argDVec3, argDVec3, argDVec3 );
    DVec4  dMix4 = Mix ( argDVec4, argDVec4, argDVec4 );

    Float fStep0 = Step ( constFloat, constFloat );
    Float fStep1 = Step ( argFloat, argFloat );
    Vec2  fStep2 = Step ( argVec2, argVec2 );
    Vec3  fStep3 = Step ( argVec3, argVec3 );
    Vec4  fStep4 = Step ( argVec4, argVec4 );

    Double dStep0 = Step ( constDouble, constDouble );
    Double dStep1 = Step ( argDouble, argDouble );
    DVec2  dStep2 = Step ( argDVec2, argDVec2 );
    DVec3  dStep3 = Step ( argDVec3, argDVec3 );
    DVec4  dStep4 = Step ( argDVec4, argDVec4 );

    Float fSmoothStep0 = SmoothStep ( constFloat, constFloat, constFloat );
    Float fSmoothStep1 = SmoothStep ( argFloat, argFloat, argFloat );
    Vec2  fSmoothStep2 = SmoothStep ( argVec2, argVec2, argVec2 );
    Vec3  fSmoothStep3 = SmoothStep ( argVec3, argVec3, argVec3 );
    Vec4  fSmoothStep4 = SmoothStep ( argVec4, argVec4, argVec4 );

    Double dSmoothStep0 = SmoothStep ( constDouble, constDouble, constDouble );
    Double dSmoothStep1 = SmoothStep ( argDouble, argDouble, argDouble );
    DVec2  dSmoothStep2 = SmoothStep ( argDVec2, argDVec2, argDVec2 );
    DVec3  dSmoothStep3 = SmoothStep ( argDVec3, argDVec3, argDVec3 );
    DVec4  dSmoothStep4 = SmoothStep ( argDVec4, argDVec4, argDVec4 );

    Float fFma0 = Fma ( constFloat, constFloat, constFloat );
    Float fFma1 = Fma ( argFloat, argFloat, argFloat );
    Vec2  fFma2 = Fma ( argVec2, argVec2, argVec2 );
    Vec3  fFma3 = Fma ( argVec3, argVec3, argVec3 );
    Vec4  fFma4 = Fma ( argVec4, argVec4, argVec4 );

    Double dFma0 = Fma ( constDouble, constDouble, constDouble );
    Double dFma1 = Fma ( argDouble, argDouble, argDouble );
    DVec2  dFma2 = Fma ( argDVec2, argDVec2, argDVec2 );
    DVec3  dFma3 = Fma ( argDVec3, argDVec3, argDVec3 );
    DVec4  dFma4 = Fma ( argDVec4, argDVec4, argDVec4 );

    UInt ps48 = PackSnorm4x8 ( argVec4 );
    UInt pu48 = PackUnorm4x8 ( argVec4 );
    UInt ps216 = PackSnorm2x16 ( argVec2 );
    UInt pu216 = PackUnorm2x16 ( argVec2 );
    UInt ph216 = PackHalf2x16 ( argVec2 );
    Double pd232 = PackDouble2x32 ( argUVec2 );
    Vec2 us216 = UnpackSnorm2x16 ( argUInt );
    Vec2 uu216 = UnpackUnorm2x16 ( argUInt );
    Vec2 uh216 = UnpackHalf2x16 ( argUInt );
    Vec4 us48 = UnpackSnorm4x8 ( argUInt );
    Vec4 uu48 = UnpackUnorm4x8 ( argUInt );
    UVec2 ud232 = UnpackDouble2x32 ( argDouble );

    Float fDot2 = Dot ( argVec2, argVec2 );
    Float fDot3 = Dot ( argVec3, argVec3 );
    Float fDot4 = Dot ( argVec4, argVec4 );

    Double dDot2 = Dot ( argDVec2, argDVec2 );
    Double dDot3 = Dot ( argDVec3, argDVec3 );
    Double dDot4 = Dot ( argDVec4, argDVec4 );

    Float fLength0 = Length ( constFloat );
    Float fLength1 = Length ( argFloat );
    Float fLength2 = Length ( argVec2 );
    Float fLength3 = Length ( argVec3 );
    Float fLength4 = Length ( argVec4 );

    Double dLength0 = Length ( constDouble );
    Double dLength1 = Length ( argDouble );
    Double dLength2 = Length ( argDVec2 );
    Double dLength3 = Length ( argDVec3 );
    Double dLength4 = Length ( argDVec4 );

    Float fDistance2 = Distance ( argVec2, argVec2 );
    Float fDistance3 = Distance ( argVec3, argVec3 );
    Float fDistance4 = Distance ( argVec4, argVec4 );

    Double dDistance2 = Distance ( argDVec2, argDVec2 );
    Double dDistance3 = Distance ( argDVec3, argDVec3 );
    Double dDistance4 = Distance ( argDVec4, argDVec4 );

    Vec3 fCross3 = Cross ( argVec3, argVec3 );
    DVec3 dCross3 = Cross ( argDVec3, argDVec3 );

    Float fNormalize0 = Normalize ( constFloat );
    Float fNormalize1 = Normalize ( argFloat );
    Vec2 fNormalize2 = Normalize ( argVec2 );
    Vec3 fNormalize3 = Normalize ( argVec3 );
    Vec4 fNormalize4 = Normalize ( argVec4 );

    Double dNormalize0 = Normalize ( constDouble );
    Double dNormalize1 = Normalize ( argDouble );
    DVec2 dNormalize2 = Normalize ( argDVec2 );
    DVec3 dNormalize3 = Normalize ( argDVec3 );
    DVec4 dNormalize4 = Normalize ( argDVec4 );

    Float fFaceForward0 = FaceForward ( constFloat, constFloat, constFloat );
    Float fFaceForward1 = FaceForward ( argFloat, argFloat, argFloat );
    Vec2  fFaceForward2 = FaceForward ( argVec2, argVec2, argVec2 );
    Vec3  fFaceForward3 = FaceForward ( argVec3, argVec3, argVec3 );
    Vec4  fFaceForward4 = FaceForward ( argVec4, argVec4, argVec4 );

    Double dFaceForward0 = FaceForward ( constDouble, constDouble, constDouble );
    Double dFaceForward1 = FaceForward ( argDouble, argDouble, argDouble );
    DVec2  dFaceForward2 = FaceForward ( argDVec2, argDVec2, argDVec2 );
    DVec3  dFaceForward3 = FaceForward ( argDVec3, argDVec3, argDVec3 );
    DVec4  dFaceForward4 = FaceForward ( argDVec4, argDVec4, argDVec4 );

    Float fReflect0 = Reflect ( constFloat, constFloat );
    Float fReflect1 = Reflect ( argFloat, argFloat );
    Vec2  fReflect2 = Reflect ( argVec2, argVec2 );
    Vec3  fReflect3 = Reflect ( argVec3, argVec3 );
    Vec4  fReflect4 = Reflect ( argVec4, argVec4 );

    Double dReflect0 = Reflect ( constDouble, constDouble );
    Double dReflect1 = Reflect ( argDouble, argDouble );
    DVec2  dReflect2 = Reflect ( argDVec2, argDVec2 );
    DVec3  dReflect3 = Reflect ( argDVec3, argDVec3 );
    DVec4  dReflect4 = Reflect ( argDVec4, argDVec4 );

    Float fRefract0 = Refract ( constFloat, constFloat, constFloat );
    Float fRefract1 = Refract ( argFloat, argFloat, argFloat );
    Vec2  fRefract2 = Refract ( argVec2, argVec2, argFloat );
    Vec3  fRefract3 = Refract ( argVec3, argVec3, argFloat );
    Vec4  fRefract4 = Refract ( argVec4, argVec4, argFloat );

    Double dRefract0 = Refract ( constDouble, constDouble, constFloat );
    Double dRefract1 = Refract ( argDouble, argDouble, argFloat );
    DVec2  dRefract2 = Refract ( argDVec2, argDVec2, argFloat );
    DVec3  dRefract3 = Refract ( argDVec3, argDVec3, argFloat );
    DVec4  dRefract4 = Refract ( argDVec4, argDVec4, argFloat );

    Int   iFindLsb0 = FindLsb ( constInt );
    Int   iFindLsb1 = FindLsb ( argInt );
    IVec2 iFindLsb2 = FindLsb ( argIVec2 );
    IVec3 iFindLsb3 = FindLsb ( argIVec3 );
    IVec4 iFindLsb4 = FindLsb ( argIVec4 );

    UInt  uFindLsb0 = FindLsb ( constUInt );
    UInt  uFindLsb1 = FindLsb ( argUInt );
    UVec2 uFindLsb2 = FindLsb ( argUVec2 );
    UVec3 uFindLsb3 = FindLsb ( argUVec3 );
    UVec4 uFindLsb4 = FindLsb ( argUVec4 );

    Int   iFindMsb0 = FindMsb ( constInt );
    Int   iFindMsb1 = FindMsb ( argInt );
    IVec2 iFindMsb2 = FindMsb ( argIVec2 );
    IVec3 iFindMsb3 = FindMsb ( argIVec3 );
    IVec4 iFindMsb4 = FindMsb ( argIVec4 );

    UInt  uFindMsb0 = FindMsb ( constUInt );
    UInt  uFindMsb1 = FindMsb ( argUInt );
    UVec2 uFindMsb2 = FindMsb ( argUVec2 );
    UVec3 uFindMsb3 = FindMsb ( argUVec3 );
    UVec4 uFindMsb4 = FindMsb ( argUVec4 );

    Float fNMin0 = NMin ( constFloat, constFloat );
    Float fNMin1 = NMin ( argFloat, argFloat );
    Vec2  fNMin2 = NMin ( argVec2, argVec2 );
    Vec3  fNMin3 = NMin ( argVec3, argVec3 );
    Vec4  fNMin4 = NMin ( argVec4, argVec4 );

    Double dNMin0 = NMin ( constDouble, constDouble );
    Double dNMin1 = NMin ( argDouble, argDouble );
    DVec2  dNMin2 = NMin ( argDVec2, argDVec2 );
    DVec3  dNMin3 = NMin ( argDVec3, argDVec3 );
    DVec4  dNMin4 = NMin ( argDVec4, argDVec4 );

    Float fNMax0 = NMax ( constFloat, constFloat );
    Float fNMax1 = NMax ( argFloat, argFloat );
    Vec2  fNMax2 = NMax ( argVec2, argVec2 );
    Vec3  fNMax3 = NMax ( argVec3, argVec3 );
    Vec4  fNMax4 = NMax ( argVec4, argVec4 );

    Double dNMax0 = NMax ( constDouble, constDouble );
    Double dNMax1 = NMax ( argDouble, argDouble );
    DVec2  dNMax2 = NMax ( argDVec2, argDVec2 );
    DVec3  dNMax3 = NMax ( argDVec3, argDVec3 );
    DVec4  dNMax4 = NMax ( argDVec4, argDVec4 );

    Float fNClamp0 = NClamp ( constFloat, constFloat, constFloat );
    Float fNClamp1 = NClamp ( argFloat, argFloat, argFloat );
    Vec2  fNClamp2 = NClamp ( argVec2, argVec2, argVec2 );
    Vec3  fNClamp3 = NClamp ( argVec3, argVec3, argVec3 );
    Vec4  fNClamp4 = NClamp ( argVec4, argVec4, argVec4 );

    Double dNClamp0 = NClamp ( constDouble, constDouble, constDouble );
    Double dNClamp1 = NClamp ( argDouble, argDouble, argDouble );
    DVec2  dNClamp2 = NClamp ( argDVec2, argDVec2, argDVec2 );
    DVec3  dNClamp3 = NClamp ( argDVec3, argDVec3, argDVec3 );
    DVec4  dNClamp4 = NClamp ( argDVec4, argDVec4, argDVec4 );

    Int   iMod0 = Mod ( constInt, constInt );
    Int   iMod1 = Mod ( argInt, argInt );
    IVec2 iMod2 = Mod ( argIVec2, argIVec2 );
    IVec3 iMod3 = Mod ( argIVec3, argIVec3 );
    IVec4 iMod4 = Mod ( argIVec4, argIVec4 );

    UInt  uMod0 = Mod ( constUInt, constUInt );
    UInt  uMod1 = Mod ( argUInt, argUInt );
    UVec2 uMod2 = Mod ( argUVec2, argUVec2 );
    UVec3 uMod3 = Mod ( argUVec3, argUVec3 );
    UVec4 uMod4 = Mod ( argUVec4, argUVec4 );

    Float fMod0 = Mod ( constFloat, constFloat );
    Float fMod1 = Mod ( argFloat, argFloat );
    Vec2  fMod2 = Mod ( argVec2, argVec2 );
    Vec3  fMod3 = Mod ( argVec3, argVec3 );
    Vec4  fMod4 = Mod ( argVec4, argVec4 );

    Double dMod0 = Mod ( constDouble, constDouble );
    Double dMod1 = Mod ( argDouble, argDouble );
    DVec2  dMod2 = Mod ( argDVec2, argDVec2 );
    DVec3  dMod3 = Mod ( argDVec3, argDVec3 );
    DVec4  dMod4 = Mod ( argDVec4, argDVec4 );

    std::pair< Float, Float > fModf0 = Modf ( constFloat );
    std::pair< Float, Float > fModf1 = Modf ( argFloat );
    std::pair< Vec2, Vec2 > fModf2 = Modf ( argVec2 );
    std::pair< Vec3, Vec3 > fModf3 = Modf ( argVec3 );
    std::pair< Vec4, Vec4 > fModf4 = Modf ( argVec4 );

    std::pair< Double, Double > dModf0 = Modf ( constDouble );
    std::pair< Double, Double > dModf1 = Modf ( argDouble );
    std::pair< DVec2, DVec2 > dModf2 = Modf ( argDVec2 );
    std::pair< DVec3, DVec3 > dModf3 = Modf ( argDVec3 );
    std::pair< DVec4, DVec4 > dModf4 = Modf ( argDVec4 );

    Float fLdexp0 = Ldexp ( constFloat, constInt );
    Float fLdexp1 = Ldexp ( argFloat, argInt );
    Vec2  fLdexp2 = Ldexp ( argVec2, argIVec2 );
    Vec3  fLdexp3 = Ldexp ( argVec3, argIVec3 );
    Vec4  fLdexp4 = Ldexp ( argVec4, argIVec4 );

    Double dLdexp0 = Ldexp ( constDouble, constInt );
    Double dLdexp1 = Ldexp ( argDouble, argInt );
    DVec2  dLdexp2 = Ldexp ( argDVec2, argIVec2 );
    DVec3  dLdexp3 = Ldexp ( argDVec3, argIVec3 );
    DVec4  dLdexp4 = Ldexp ( argDVec4, argIVec4 );

    std::pair< Float, Int > fFrexp0 = Frexp ( constFloat );
    std::pair< Float, Int > fFrexp1 = Frexp ( argFloat );
    std::pair< Vec2, IVec2 > fFrexp2 = Frexp ( argVec2 );
    std::pair< Vec3, IVec3 > fFrexp3 = Frexp ( argVec3 );
    std::pair< Vec4, IVec4 > fFrexp4 = Frexp ( argVec4 );

    std::pair< Double, Int > dFrexp0 = Frexp ( constDouble );
    std::pair< Double, Int > dFrexp1 = Frexp ( argDouble );
    std::pair< DVec2, IVec2 > dFrexp2 = Frexp ( argDVec2 );
    std::pair< DVec3, IVec3 > dFrexp3 = Frexp ( argDVec3 );
    std::pair< DVec4, IVec4 > dFrexp4 = Frexp ( argDVec4 );

    Bool  fIsNaN0 = IsNaN ( constFloat );
    Bool  fIsNaN1 = IsNaN ( argFloat );
    BVec2 fIsNaN2 = IsNaN ( argVec2 );
    BVec3 fIsNaN3 = IsNaN ( argVec3 );
    BVec4 fIsNaN4 = IsNaN ( argVec4 );

    Bool  dIsNaN0 = IsNaN ( constDouble );
    Bool  dIsNaN1 = IsNaN ( argDouble );
    BVec2 dIsNaN2 = IsNaN ( argDVec2 );
    BVec3 dIsNaN3 = IsNaN ( argDVec3 );
    BVec4 dIsNaN4 = IsNaN ( argDVec4 );

    Bool  fIsInf0 = IsInf ( constFloat );
    Bool  fIsInf1 = IsInf ( argFloat );
    BVec2 fIsInf2 = IsInf ( argVec2 );
    BVec3 fIsInf3 = IsInf ( argVec3 );
    BVec4 fIsInf4 = IsInf ( argVec4 );

    Bool  dIsInf0 = IsInf ( constDouble );
    Bool  dIsInf1 = IsInf ( argDouble );
    BVec2 dIsInf2 = IsInf ( argDVec2 );
    BVec3 dIsInf3 = IsInf ( argDVec3 );
    BVec4 dIsInf4 = IsInf ( argDVec4 );

    Bool bIsAnyComponentTrue2 = IsAnyComponentTrue ( fIsInf2 );
    Bool bIsAnyComponentTrue3 = IsAnyComponentTrue ( fIsInf3 );
    Bool bIsAnyComponentTrue4 = IsAnyComponentTrue ( fIsInf4 );
    Bool bIsAllComponentsTrue2 = IsAllComponentsTrue ( fIsInf2 );
    Bool bIsAllComponentsTrue3 = IsAllComponentsTrue ( fIsInf3 );
    Bool bIsAllComponentsTrue4 = IsAllComponentsTrue ( fIsInf4 );

    Int   iBitCount0 = BitCount ( constInt );
    Int   iBitCount1 = BitCount ( argInt );
    IVec2 iBitCount2 = BitCount ( argIVec2 );
    IVec3 iBitCount3 = BitCount ( argIVec3 );
    IVec4 iBitCount4 = BitCount ( argIVec4 );

    Int  uBitCount0 = BitCount ( constUInt );
    Int  uBitCount1 = BitCount ( argUInt );
    IVec2 uBitCount2 = BitCount ( argUVec2 );
    IVec3 uBitCount3 = BitCount ( argUVec3 );
    IVec4 uBitCount4 = BitCount ( argUVec4 );

    Int  iBitReverse0 = BitReverse ( constInt );
    Int  iBitReverse1 = BitReverse ( argInt );
    IVec2 iBitReverse2 = BitReverse ( argIVec2 );
    IVec3 iBitReverse3 = BitReverse ( argIVec3 );
    IVec4 iBitReverse4 = BitReverse ( argIVec4 );

    UInt  uBitReverse0 = BitReverse ( constUInt );
    UInt  uBitReverse1 = BitReverse ( argUInt );
    UVec2 uBitReverse2 = BitReverse ( argUVec2 );
    UVec3 uBitReverse3 = BitReverse ( argUVec3 );
    UVec4 uBitReverse4 = BitReverse ( argUVec4 );
}

// -----------------------------------------------------------------------------

void KFunctionTestPipeline :: testMatrices()
{
    using namespace vpp;

    // IMat2 IMat3 IMat4 IMat4x2 IMat2x3 IMat4x3 IMat2x4 IMat3x4
    // UMat2 UMat3 UMat4 UMat3x2 UMat4x2 UMat2x3 UMat4x3 UMat2x4 UMat3x4

    Float argFloat = 5.5f;
    Vec2 argVec2 { 1.0f, 2.0f };
    Vec3 argVec3 { 1.0f, 2.0f, 3.0f };
    Vec4 argVec4 { 1.0f, 2.0f, 3.0f, 4.0f };

    Mat2 argMat2 { 1.0f, 2.0f, 3.0f, 4.0f };

    Mat3 argMat3
    {
        argFloat, argFloat, argFloat,
        argFloat, argFloat, argFloat,
        argFloat, argFloat, argFloat
    };

    Mat4 argMat4 { argVec4, argVec4, argVec4, argVec4 };

    Mat3x2 argMat3x2 { argVec2, argVec2, argVec2 };
    Mat4x2 argMat4x2 { argVec2, argVec2, argVec2, argVec2 };
    Mat2x3 argMat2x3 { argVec3, argVec3 };
    Mat4x3 argMat4x3 { argVec3, argVec3, argVec3, argVec3 };
    Mat2x4 argMat2x4 { argVec4, argVec4 };
    Mat3x4 argMat3x4 { argVec4, argVec4, argVec4 };

    // -----------------------------------------------------------------------------

    Double argDouble = 5.5;
    DVec2 argDVec2 { 1.0, 2.0 };
    DVec3 argDVec3 { 1.0, 2.0, 3.0 };
    DVec4 argDVec4 { 1.0, 2.0, 3.0, 4.0 };

    DMat2 argDMat2 { 1.0, 2.0, 3.0, 4.0 };

    DMat3 argDMat3
    {
        argDouble, argDouble, argDouble,
        argDouble, argDouble, argDouble,
        argDouble, argDouble, argDouble
    };

    DMat4 argDMat4 { argDVec4, argDVec4, argDVec4, argDVec4 };

    DMat3x2 argDMat3x2 { argDVec2, argDVec2, argDVec2 };
    DMat4x2 argDMat4x2 { argDVec2, argDVec2, argDVec2, argDVec2 };
    DMat2x3 argDMat2x3 { argDVec3, argDVec3 };
    DMat4x3 argDMat4x3 { argDVec3, argDVec3, argDVec3, argDVec3 };
    DMat2x4 argDMat2x4 { argDVec4, argDVec4 };
    DMat3x4 argDMat3x4 { argDVec4, argDVec4, argDVec4 };

    // -----------------------------------------------------------------------------

    Int argInt = 5;
    IVec2 argIVec2 { 1, 2 };
    IVec3 argIVec3 { 1, 2, 3 };
    IVec4 argIVec4 { 1, 2, 3, 4 };

    // -----------------------------------------------------------------------------

    UInt argUInt = 5u;
    UVec2 argUVec2 { 1u, 2u };
    UVec3 argUVec3 { 1u, 2u, 3u };
    UVec4 argUVec4 { 1u, 2u, 3u, 4u };

    // -------------------------------------------------------------------------

    Float fDeterminant2 = Determinant ( argMat2 );
    Float fDeterminant3 = Determinant ( argMat3 );
    Float fDeterminant4 = Determinant ( argMat4 );

    Double dDeterminant2 = Determinant ( argDMat2 );
    Double dDeterminant3 = Determinant ( argDMat3 );
    Double dDeterminant4 = Determinant ( argDMat4 );

    Mat2 fMatrixInverse2 = MatrixInverse ( argMat2 );
    Mat3 fMatrixInverse3 = MatrixInverse ( argMat3 );
    Mat4 fMatrixInverse4 = MatrixInverse ( argMat4 );

    DMat2 dMatrixInverse2 = MatrixInverse ( argDMat2 );
    DMat3 dMatrixInverse3 = MatrixInverse ( argDMat3 );
    DMat4 dMatrixInverse4 = MatrixInverse ( argDMat4 );

    Mat2 fTranspose2 = Transpose ( argMat2 );
    Mat3 fTranspose3 = Transpose ( argMat3 );
    Mat4 fTranspose4 = Transpose ( argMat4 );
    Mat2x3 fTranspose3x2 = Transpose ( argMat3x2 );
    Mat2x4 fTranspose4x2 = Transpose ( argMat4x2 );
    Mat3x2 fTranspose2x3 = Transpose ( argMat2x3 );
    Mat3x4 fTranspose4x3 = Transpose ( argMat4x3 );
    Mat4x2 fTranspose2x4 = Transpose ( argMat2x4 );
    Mat4x3 fTranspose3x4 = Transpose ( argMat3x4 );

    DMat2 dTranspose2 = Transpose ( argDMat2 );
    DMat3 dTranspose3 = Transpose ( argDMat3 );
    DMat4 dTranspose4 = Transpose ( argDMat4 );
    DMat2x3 dTranspose3x2 = Transpose ( argDMat3x2 );
    DMat2x4 dTranspose4x2 = Transpose ( argDMat4x2 );
    DMat3x2 dTranspose2x3 = Transpose ( argDMat2x3 );
    DMat3x4 dTranspose4x3 = Transpose ( argDMat4x3 );
    DMat4x2 dTranspose2x4 = Transpose ( argDMat2x4 );
    DMat4x3 dTranspose3x4 = Transpose ( argDMat3x4 );

    Mat2 fOuterProduct2 = OuterProduct ( argVec2, argVec2 );
    Mat3 fOuterProduct3 = OuterProduct ( argVec3, argVec3 );
    Mat4 fOuterProduct4 = OuterProduct ( argVec4, argVec4 );
    Mat2x3 fOuterProduct2x3 = OuterProduct ( argVec3, argVec2 );
    Mat2x4 fOuterProduct2x4 = OuterProduct ( argVec4, argVec2 );
    Mat3x2 fOuterProduct3x2 = OuterProduct ( argVec2, argVec3 );
    Mat3x4 fOuterProduct3x4 = OuterProduct ( argVec4, argVec3 );
    Mat4x2 fOuterProduct4x2 = OuterProduct ( argVec2, argVec4 );
    Mat4x3 fOuterProduct4x3 = OuterProduct ( argVec3, argVec4 );
}

// -----------------------------------------------------------------------------
