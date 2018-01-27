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

// This example demostrates usage of the itk::BayesianClassifierImageFilter
// The input to this example is an itk::VectorImage that represents pixel
// memberships to 'n' classes.
//
// This image is conveniently generated by the BayesianClassifierInitializer.cxx
// example.
//
// The output of the filter is a label map (an image of unsigned char's) with
// pixel values indicating the classes they correspond to. Pixels with intensity 0
// belong to the 0th class, 1 belong to the 1st class etc. The classification
// is done by applying a Maximum decision rule to the posterior image.
//
// The filter allows you to specify a prior image as well, (although this is not
// done in this example). The prior image, if specified will be a itk::VectorImage
// with as many components as the number of classes. The posterior image is
// then generated by multiplying the prior image with the membership image. If
// the prior image is not specified, the posterior image is the same as the
// membership image.
//
// The filter optionally accepts a smoothingIterations argument. See the
// itk::BayesianClassifierImageFilter for details on how this affects the
// classification. The philosophy is that the filter allows you to iteratively
// smooth the posteriors prior to applying the decision rule. It is hoped
// that this would yield a better classification. The user will need to plug
// in his own smoothing filter. In this case, we specify a
// GradientAnisotropicDiffusionImageFilter.
//
// Example args:
//   Memberships.mhd Labelmap.png  3

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkBayesianClassifierImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char* argv[] )
{

  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile [smoothingIterations]" << std::endl;
    return EXIT_FAILURE;
    }

  // input parameters
  const char * membershipImageFileName  = argv[1];
  const char * labelMapImageFileName    = argv[2];

  // setup reader
  const unsigned int                                    Dimension = 2;
  using InputPixelType = float;
  using InputImageType = itk::VectorImage< InputPixelType, Dimension >;
  using ReaderType = itk::ImageFileReader< InputImageType >;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( membershipImageFileName );

  using LabelType = unsigned char;
  using PriorType = float;
  using PosteriorType = float;


  using ClassifierFilterType = itk::BayesianClassifierImageFilter<
                              InputImageType,LabelType,
                              PosteriorType,PriorType >;

  ClassifierFilterType::Pointer filter = ClassifierFilterType::New();


  filter->SetInput( reader->GetOutput() );

  if( argv[3] )
    {
    filter->SetNumberOfSmoothingIterations( atoi( argv[3] ));
    using ExtractedComponentImageType = ClassifierFilterType::ExtractedComponentImageType;
    using SmoothingFilterType = itk::GradientAnisotropicDiffusionImageFilter<
      ExtractedComponentImageType, ExtractedComponentImageType >;
    SmoothingFilterType::Pointer smoother = SmoothingFilterType::New();
    smoother->SetNumberOfIterations( 1 );
    smoother->SetTimeStep( 0.125 );
    smoother->SetConductanceParameter( 3 );
    filter->SetSmoothingFilter( smoother );
    }


  // SET FILTER'S PRIOR PARAMETERS
  // do nothing here to default to uniform priors
  // otherwise set the priors to some user provided values

  //
  // Setup writer.. Rescale the label map to the dynamic range of the
  // datatype and write it
  //
  using ClassifierOutputImageType = ClassifierFilterType::OutputImageType;
  using OutputImageType = itk::Image< unsigned char, Dimension >;
  using RescalerType = itk::RescaleIntensityImageFilter<
    ClassifierOutputImageType, OutputImageType >;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetInput( filter->GetOutput() );
  rescaler->SetOutputMinimum( 0 );
  rescaler->SetOutputMaximum( 255 );

  using WriterType = itk::ImageFileWriter< OutputImageType >;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( labelMapImageFileName );

  //
  // Write labelmap to file
  //
  writer->SetInput( rescaler->GetOutput() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception caught: " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  // Testing print
  filter->Print( std::cout );
  std::cout << "Test passed." << std::endl;

  return EXIT_SUCCESS;

}
