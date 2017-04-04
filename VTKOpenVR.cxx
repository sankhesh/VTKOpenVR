#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkConeSource.h>
#include <vtkDICOMImageReader.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkNew.h>
#include <vtkOpenVRCamera.h>
#include <vtkOpenVRCustomInteractorStyle.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPolyDataMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <Path to DICOM directory>" << std::endl;
    return EXIT_FAILURE;
  }

  // Setup the OpenVR rendering classes
  vtkNew<vtkActor> actor;
  vtkNew<vtkVolume> volume;
  vtkNew<vtkOpenVRRenderer> renderer;
  vtkNew<vtkOpenVRRenderWindow> renderWindow;
  vtkNew<vtkOpenVRRenderWindowInteractor> iren;
  vtkNew<vtkOpenVRCustomInteractorStyle> style;
  style->SetLeftButtonEvent( vtkOpenVRCustomInteractorStyle::PICK );
  vtkNew<vtkOpenVRCamera> cam;

  renderWindow->AddRenderer( renderer.Get() );
  renderer->AddActor( actor.Get() );
  //renderer->AddActor( volume.GetPointer() );
  iren->SetRenderWindow( renderWindow.Get() );
  iren->SetInteractorStyle( style.Get() );
  renderer->SetActiveCamera( cam.Get() );

  // Without the next line volume rendering in VR does not work
  renderWindow->SetMultiSamples( 0 );

  // Add a cone to the scene
  vtkNew<vtkConeSource> cone;
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection( cone->GetOutputPort() );
  actor->SetMapper( mapper.Get() );

  // Add a volume to the scene
  vtkNew<vtkDICOMImageReader> reader;
  reader->SetDirectoryName( argv[1] );
  reader->Update();

  vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;
  volumeMapper->SetInputConnection( reader->GetOutputPort() );
  volume->SetMapper( volumeMapper.GetPointer() );
  vtkNew<vtkVolumeProperty> volumeProperty;
  volume->SetProperty( volumeProperty.GetPointer() );

  vtkNew<vtkColorTransferFunction> color;
  color->AddRGBPoint( -3024, 0.00, 0.00, 0.00 );
  color->AddRGBPoint( -86.9, 0.00, 0.25, 1.00 );
  color->AddRGBPoint( 45.38, 1.00, 0.00, 0.00 );
  color->AddRGBPoint( 139.9, 1.00, 0.89, 0.89 );
  color->AddRGBPoint( 347.9, 1.00, 1.00, 0.25 );
  color->AddRGBPoint(  1224, 1.00, 1.00, 1.00 );
  color->AddRGBPoint(  3071, 0.83, 0.66, 1.00 );
  volumeProperty->SetColor( color.GetPointer() );
  vtkNew<vtkPiecewiseFunction> opacity;
  opacity->AddPoint( -3024, 0.00 );
  opacity->AddPoint( -86.9, 0.00 );
  opacity->AddPoint( 45.38, 0.17 );
  opacity->AddPoint( 139.9, 0.39 );
  opacity->AddPoint( 347.9, 0.51 );
  opacity->AddPoint(  1224, 0.59 );
  opacity->AddPoint(  3071, 0.61 );
  volumeProperty->SetScalarOpacity( opacity.GetPointer() );

  // Render
  renderer->ResetCamera();
  renderWindow->Render();
  iren->Start();

  return EXIT_SUCCESS;
}