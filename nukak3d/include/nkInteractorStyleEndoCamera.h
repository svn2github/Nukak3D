/** 
 * @file nkInteractorStyleEndoCamera.h
 * @brief Class for Endoscopy Simulator.
 * @details Implement all functions for Endoscopy Simulator.
 * @author Byron Pérez
 * @version 0.1
 * @date 01/07/2008 03:15 p.m.
*/

#ifndef __nkInteractorStyleEndoCamera_h
#define __nkInteractorStyleEndoCamera_h

#include "vtkInteractorStyle.h"
#include "vtkAbstractPropPicker.h"
#include "vtkOutputWindow.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#define mensajes 1
#define VTKIS_ROTENDO       10

//class VTK_RENDERING_EXPORT nkInteractorStyleEndoCamera : public vtkInteractorStyle
class nkInteractorStyleEndoCamera : public vtkInteractorStyle
{
public:
  static nkInteractorStyleEndoCamera *New();
  vtkTypeRevisionMacro(nkInteractorStyleEndoCamera,vtkInteractorStyle);
  void PrintSelf(ostream& os, vtkIndent indent);


  vtkSetMacro(RestoreUpVector,int);
  vtkGetMacro(RestoreUpVector,int);
  vtkBooleanMacro(RestoreUpVector,int);

  // Specify "up" (by default {0,0,1} but can be changed)
  vtkGetVectorMacro(DefaultUpVector,double,3);
  vtkSetVectorMacro(DefaultUpVector,double,3);

  // Description:
  // Event bindings controlling the effects of pressing mouse buttons
  // or moving the mouse.
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnMiddleButtonDown();
  virtual void OnMiddleButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();
  virtual void OnMouseWheelForward();
  virtual void OnMouseWheelBackward();

  // These methods for the different interactions in different modes
  // are overridden in subclasses to perform the correct motion. Since
  // they are called by OnTimer, they do not have mouse coord parameters
  // (use interactor's GetEventPosition and GetLastEventPosition)
  virtual void RotateEndo();
  virtual void Rotate();
  virtual void Spin();
  virtual void Pan();
  virtual void Dolly();
  
protected:
  nkInteractorStyleEndoCamera();
  ~nkInteractorStyleEndoCamera();

  double		MotionFactor;
  int           RestoreUpVector;
  double        DefaultUpVector[3];

  virtual void Dolly(double factor);
  virtual void MotionAlongVector(double vector[3], double amount, vtkCamera* cam);
  virtual void FinishCamera(vtkCamera* cam);

  void StartRotEndo();
  void EndRotEndo();
  //virtual void StartForwardFly();
  //virtual void EndForwardFly();

private:
  nkInteractorStyleEndoCamera(const nkInteractorStyleEndoCamera&);  // Not implemented.
  void operator=(const nkInteractorStyleEndoCamera&);  // Not implemented.
};

#endif
