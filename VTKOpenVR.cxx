#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkNew.h>
#include <vtkOpenVRCamera.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkOpenVRRenderer.h>
#include <vtkPolyDataMapper.h>

int main(int argc, char* argv[])
{
  vtkNew<vtkActor> actor;
  vtkNew<vtkOpenVRRenderer> renderer;
  vtkNew<vtkOpenVRRenderWindow> renderWindow;
  vtkNew<vtkOpenVRRenderWindowInteractor> iren;
  vtkNew<vtkOpenVRCamera> cam;

  renderWindow->AddRenderer(renderer.Get());
  renderer->AddActor(actor.Get());
  iren->SetRenderWindow(renderWindow.Get());
  renderer->SetActiveCamera(cam.Get());

  vtkNew<vtkConeSource> cone;
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(cone->GetOutputPort());
  actor->SetMapper(mapper.Get());

  renderer->ResetCamera();
  renderWindow->Render();
  iren->Start();
  return EXIT_SUCCESS;
}