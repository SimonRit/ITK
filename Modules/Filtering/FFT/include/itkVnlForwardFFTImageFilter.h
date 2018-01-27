/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkForwardFFTImageFilter.h"

#ifndef itkVnlForwardFFTImageFilter_h
#define itkVnlForwardFFTImageFilter_h

#include "vnl/algo/vnl_fft_base.h"

namespace itk
{
/** \class VnlForwardFFTImageFilter
 *
 * \brief VNL based forward Fast Fourier Transform.
 *
 * The input image size must be a multiple of combinations of 2s, 3s,
 * and/or 5s in all dimensions.
 *
 * \ingroup FourierTransform
 *
 * \sa ForwardFFTImageFilter
 * \ingroup ITKFFT
 *
 * \wiki
 * \wikiexample{SpectralAnalysis/VnlForwardFFTImageFilter,Compute the FFT of an image}
 * \wikiexample{SpectralAnalysis/CrossCorrelationInFourierDomain,Compute the cross-correlation of two images in the Fourier domain}
 * \endwiki
 */
template< typename TInputImage, typename TOutputImage=Image< std::complex<typename TInputImage::PixelType>, TInputImage::ImageDimension> >
class ITK_TEMPLATE_EXPORT VnlForwardFFTImageFilter:
  public ForwardFFTImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class type aliases. */
  using InputImageType = TInputImage;
  using InputPixelType = typename InputImageType::PixelType;
  using InputSizeType = typename InputImageType::SizeType;
  using InputSizeValueType = typename InputImageType::SizeValueType;
  using OutputImageType = TOutputImage;
  using OutputPixelType = typename OutputImageType::PixelType;

  using Self = VnlForwardFFTImageFilter;
  using Superclass = ForwardFFTImageFilter<  TInputImage, TOutputImage>;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VnlForwardFFTImageFilter,
               ForwardFFTImageFilter);

  /** Extract the dimensionality of the images. They are assumed to be
   * the same. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  SizeValueType GetSizeGreatestPrimeFactor() const override;

#ifdef ITK_USE_CONCEPT_CHECKING
  // Begin concept checking
  itkConceptMacro( ImageDimensionsMatchCheck,
                   ( Concept::SameDimension< InputImageDimension, OutputImageDimension > ) );
  // End concept checking
#endif

protected:
  VnlForwardFFTImageFilter() {}
  ~VnlForwardFFTImageFilter() override {}

  void GenerateData() override;

private:
  ITK_DISALLOW_COPY_AND_ASSIGN(VnlForwardFFTImageFilter);

  using SignalVectorType = vnl_vector< std::complex< InputPixelType > >;
};
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVnlForwardFFTImageFilter.hxx"
#endif

#endif
