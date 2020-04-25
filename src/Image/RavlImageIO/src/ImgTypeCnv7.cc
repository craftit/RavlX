
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Types.hh"
#include "Ravl/Image/RGBValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/RealYUVValue.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/RGBcYUV.hh"

namespace RavlImageN
{

  void InitStdImageCnv7()
  {}

  static TypeNameC type1(typeid(ImageC<YUVValueC<float> >),"RavImageN::ImageC<YUVValueC<float> >");
  static TypeNameC type2(typeid(ImageC<RGBValueC<float> >),"RavImageN::ImageC<RGBValueC<float> >");

  ImageC<RGBValueC<float> > Image_ByteRGBValue2Image_RGBValueFloat(const ImageC<ByteRGBValueC> &img) {
    ImageC<RGBValueC<float> > ret(img.Frame());
    for(Array2dIter2C<RGBValueC<float> ,ByteRGBValueC> it(ret,img);it;it++)
      it.Data1() = RGBValueC<float>(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue());
    return ret;
  }

  // Float RGB to float YUV image.

  ImageC<YUVValueC<float> > FloatRGBImageCT2FloatYUVImageCT(const ImageC<RGBValueC<float> > &dat) {
    ImageC<YUVValueC<float> > ret(dat.Rectangle());
    for(Array2dIter2C<YUVValueC<float>,RGBValueC<float> > it(ret,dat);it.IsElm();it.Next()) {
      RealYUVValueC v(RealRGBValueC(it.Data2()));
      it.Data1().Set(v.Y(),v.U(),v.V());
    }
    return ret;
  }

  ImageC<YUVValueC<float> > Image_ByteRGBValue2Image_YUVValueFloat(const ImageC<ByteRGBValueC> &img) {
    ImageC<YUVValueC<float> > ret(img.Frame());
    for(Array2dIter2C<YUVValueC<float> ,ByteRGBValueC> it(ret,img);it;it++) {
      RealYUVValueC v(RealRGBValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue()));
      it.Data1().Set(v.Y(),v.U(),v.V());
    }
    return ret;
  }


  ImageC<RealYUVValueC > FloatYUVImageCT2RealYUVImageCT(const ImageC<YUVValueC<float> > &dat) {
    ImageC<RealYUVValueC > ret(dat.Rectangle());
    for(Array2dIter2C<RealYUVValueC,YUVValueC<float> > it(ret,dat);it.IsElm();it.Next()) {
      it.Data1() = RealYUVValueC(it.Data2()); // .Y(),it.Data2().U(),it.Data2().V()
    }
    return ret;
  }


  DP_REGISTER_CONVERSION_NAMED(Image_ByteRGBValue2Image_RGBValueFloat,1,
                               "ImageC<RGBValueC<float> > RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(FloatRGBImageCT2FloatYUVImageCT,1,
                               "ImageC<YUVValueC<float> > RavlImageN::Convert(const ImageC<RGBValueC<float> > &)");

  DP_REGISTER_CONVERSION_NAMED(Image_ByteRGBValue2Image_YUVValueFloat,1,
                               "ImageC<YUVValueC<float> > RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(FloatYUVImageCT2RealYUVImageCT,1.01,
                               "ImageC<RealYUVValueC > RavlImageN::Convert(const ImageC<YUVValueC<float> > &)");


}
