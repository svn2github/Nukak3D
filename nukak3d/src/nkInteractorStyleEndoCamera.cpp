/**
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2007-2010 by Bioingenium Research Group.
 * Bogota - Colombia
 * All rights reserved.
 *
 * Author(s): Alexander Pinzón Fernández.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include "nkInteractorStyleEndoCamera.h"


vtkCxxRevisionMacro(nkInteractorStyleEndoCamera, "$Revision: 1.0 $");
vtkStandardNewMacro(nkInteractorStyleEndoCamera);

//----------------------------------------------------------------------------
nkInteractorStyleEndoCamera::nkInteractorStyleEndoCamera() 
{
  this->MotionFactor   = 10.0;
  this->RestoreUpVector    = 1;
  this->DefaultUpVector[0] = 0;
  this->DefaultUpVector[1] = 0;
  this->DefaultUpVector[2] = 1;
}

//----------------------------------------------------------------------------
nkInteractorStyleEndoCamera::~nkInteractorStyleEndoCamera() 
{
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnMouseMove() 
{ 
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];

  switch (this->State) 
    {
    case VTKIS_ROTENDO:
	  this->FindPokedRenderer(x, y);
      this->RotateEndo();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;

    case VTKIS_ROTATE:
      this->FindPokedRenderer(x, y);
      this->Rotate();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;

    case VTKIS_PAN:
      this->FindPokedRenderer(x, y);
      this->Pan();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;

    case VTKIS_DOLLY:
      this->FindPokedRenderer(x, y);
      this->Dolly();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;

    case VTKIS_SPIN:
      this->FindPokedRenderer(x, y);
      this->Spin();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    }
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnLeftButtonDown() 
{ 
  this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
                          this->Interactor->GetEventPosition()[1]);
  if (this->CurrentRenderer == NULL)
    {
    return;
    }
  
  if (this->Interactor->GetShiftKey()) 
    {
    if (this->Interactor->GetControlKey()) 
      {
      this->StartDolly();
      }
    else 
      {
      this->StartPan();
      }
    } 
  else 
    {
    if (this->Interactor->GetControlKey()) 
      {
      this->StartSpin();
      }
    else 
      {
      this->StartRotate();
      }
    }

}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnLeftButtonUp()
{
  switch (this->State) 
    {
    case VTKIS_DOLLY:
      this->EndDolly();
      break;

    case VTKIS_PAN:
      this->EndPan();
      break;

    case VTKIS_SPIN:
      this->EndSpin();
      break;

    case VTKIS_ROTATE:
      this->EndRotate();
      break;
    }
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnMiddleButtonDown() 
{
  this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
                          this->Interactor->GetEventPosition()[1]);
  if (this->CurrentRenderer == NULL)
    {
    return;
    }
  
  this->StartPan();
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnMiddleButtonUp()
{
  switch (this->State) 
    {
    case VTKIS_PAN:
      this->EndPan();
      break;
    }
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnRightButtonDown() 
{
	this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
							this->Interactor->GetEventPosition()[1]);
	if (this->CurrentRenderer == NULL)
    {
		return;
    }

    if (this->Interactor->GetControlKey()) 
	{
		this->StartRotEndo();
	}
	else
	{
		this->StartDolly();
    }  
  
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnRightButtonUp()
{
	switch (this->State) 
	{
		case VTKIS_DOLLY:
			this->EndDolly();
			break;
		case VTKIS_ROTENDO:
			this->EndRotEndo();
			break;
    }
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnMouseWheelForward() 
{
	
	this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
						  this->Interactor->GetEventPosition()[1]);
	if (this->CurrentRenderer == NULL)
	{
		return;
	}

	//this->StartDolly();
	//double factor = this->MotionFactor * 0.2 * this->MouseWheelMotionFactor;  
	//this->Dolly( pow((double)1.1, factor) );
	//this->EndDolly();

  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  this->StartDolly();
  vtkCamera* cam = this->CurrentRenderer->GetActiveCamera();
  cam->OrthogonalizeViewUp();
  double *Position = cam->GetPosition();
  double *FocalPoint = cam->GetFocalPoint();
  double vec1[3];
  vec1[0] = FocalPoint[0]-Position[0];
  vec1[1] = FocalPoint[1]-Position[1];
  vec1[2] = FocalPoint[2]-Position[2];
  this->MotionAlongVector(vec1,-0.1,cam);
  this->FinishCamera(cam);
  this->EndDolly();
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::OnMouseWheelBackward()
{
  this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], 
                          this->Interactor->GetEventPosition()[1]);
  if (this->CurrentRenderer == NULL)
    {
    return;
    }
  
  //this->StartDolly();
  //double factor = this->MotionFactor * -0.2 * this->MouseWheelMotionFactor;  
  //this->Dolly( pow((double)1.1, factor) );
  //this->EndDolly();

  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  this->StartDolly();
  vtkCamera* cam = this->CurrentRenderer->GetActiveCamera();
  //cam->OrthogonalizeViewUp();
  double *Position = cam->GetPosition();
  double *FocalPoint = cam->GetFocalPoint();
  double vec1[3];
  vec1[0] = FocalPoint[0]-Position[0];
  vec1[1] = FocalPoint[1]-Position[1];
  vec1[2] = FocalPoint[2]-Position[2];
  this->MotionAlongVector(vec1,0.1,cam);
  this->FinishCamera(cam);
  this->EndDolly();
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////

}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::Rotate()
{
  if (this->CurrentRenderer == NULL)
    {
    return;
    }
  
  vtkRenderWindowInteractor *rwi = this->Interactor;

  int dx = rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0];
  int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
  
  int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();

  double delta_elevation = -20.0 / size[1];
  double delta_azimuth = -20.0 / size[0];
  
  double rxf = (double)dx * delta_azimuth * this->MotionFactor;
  double ryf = (double)dy * delta_elevation * this->MotionFactor;
  
  vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
  camera->Azimuth(rxf);
  camera->Elevation(ryf);
  camera->OrthogonalizeViewUp();

  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }

  if (rwi->GetLightFollowCamera()) 
    {
    this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

  rwi->Render();
}
//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::RotateEndo()
{
  if (this->CurrentRenderer == NULL)
    {
    return;
    }
  
  vtkRenderWindowInteractor *rwi = this->Interactor;

  int dx = rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0];
  int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
  
  int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();

  double delta_elevation = -20.0 / size[1];
  double delta_azimuth = -20.0 / size[0];
  
  double rxf = (double)dx * delta_azimuth * this->MotionFactor;
  double ryf = (double)dy * delta_elevation * this->MotionFactor;
  
  vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();

  double radio = 5;
  //Devolverse al pivote con distancia R
  double *Position = camera->GetPosition();
  double *FocalPoint = camera->GetFocalPoint();
  double vec1[3];
  vec1[0] = FocalPoint[0]-Position[0];
  vec1[1] = FocalPoint[1]-Position[1];
  vec1[2] = FocalPoint[2]-Position[2];
  this->MotionAlongVector(vec1,radio,camera);

  //Rotar en el pivote
  camera->Azimuth(rxf);
  camera->Elevation(ryf);
  camera->OrthogonalizeViewUp();

  //Avanzar R
  Position = camera->GetPosition();
  FocalPoint = camera->GetFocalPoint();
  vec1[0] = FocalPoint[0]-Position[0];
  vec1[1] = FocalPoint[1]-Position[1];
  vec1[2] = FocalPoint[2]-Position[2];
  this->MotionAlongVector(vec1,-radio,camera);


  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }

  if (rwi->GetLightFollowCamera()) 
    {
    this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

  rwi->Render();
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::StartRotEndo() 
{
  if (this->State != VTKIS_NONE) 
    {
    return;
    }
  this->StartState(VTKIS_ROTENDO);
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::EndRotEndo() 
{
  if (this->State != VTKIS_ROTENDO) 
    {
    return;
    }
  this->StopState();
}
//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::Spin()
{
  if (this->CurrentRenderer == NULL)
    {
    return;
    }

  vtkRenderWindowInteractor *rwi = this->Interactor;

  double *center = this->CurrentRenderer->GetCenter();

  double newAngle = 
    atan2((double)rwi->GetEventPosition()[1] - (double)center[1],
          (double)rwi->GetEventPosition()[0] - (double)center[0]);

  double oldAngle = 
    atan2((double)rwi->GetLastEventPosition()[1] - (double)center[1],
          (double)rwi->GetLastEventPosition()[0] - (double)center[0]);
  
  newAngle *= vtkMath::RadiansToDegrees();
  oldAngle *= vtkMath::RadiansToDegrees();

  vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
  camera->Roll(newAngle - oldAngle);
  camera->OrthogonalizeViewUp();
      
  rwi->Render();
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::Pan()
{
  if (this->CurrentRenderer == NULL)
    {
    return;
    }

  vtkRenderWindowInteractor *rwi = this->Interactor;

  double viewFocus[4], focalDepth, viewPoint[3];
  double newPickPoint[4], oldPickPoint[4], motionVector[3];
  
  // Calculate the focal depth since we'll be using it a lot

  vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
  camera->GetFocalPoint(viewFocus);
  this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2], 
                              viewFocus);
  focalDepth = viewFocus[2];

  this->ComputeDisplayToWorld((double)rwi->GetEventPosition()[0], 
                              (double)rwi->GetEventPosition()[1],
                              focalDepth, 
                              newPickPoint);
    
  // Has to recalc old mouse point since the viewport has moved,
  // so can't move it outside the loop

  this->ComputeDisplayToWorld((double)rwi->GetLastEventPosition()[0],
                              (double)rwi->GetLastEventPosition()[1],
                              focalDepth, 
                              oldPickPoint);
  
  // Camera motion is reversed

  motionVector[0] = oldPickPoint[0] - newPickPoint[0];
  motionVector[1] = oldPickPoint[1] - newPickPoint[1];
  motionVector[2] = oldPickPoint[2] - newPickPoint[2];
  
  camera->GetFocalPoint(viewFocus);
  camera->GetPosition(viewPoint);
  camera->SetFocalPoint(motionVector[0] + viewFocus[0],
                        motionVector[1] + viewFocus[1],
                        motionVector[2] + viewFocus[2]);

  camera->SetPosition(motionVector[0] + viewPoint[0],
                      motionVector[1] + viewPoint[1],
                      motionVector[2] + viewPoint[2]);
      
  if (rwi->GetLightFollowCamera()) 
    {
    this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }
    
  rwi->Render();
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::Dolly()
{
	if (this->CurrentRenderer == NULL)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;
	double *center = this->CurrentRenderer->GetCenter();  
	int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
	//double dyf = this->MotionFactor * (double)(dy) / (double)(center[1]);
	//this->Dolly(pow((double)1.1, dyf));
	if (dy>0)
	{
		this->Dolly(pow((double)1.1, 0.1));
	}
	else
		this->Dolly(pow((double)1.1, -0.1));

	//double factor = this->MotionFactor * -0.2 * this->MouseWheelMotionFactor;  
	//this->Dolly( pow((double)1.1, factor) ); 
}

//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::Dolly(double factor)
{
  if (this->CurrentRenderer == NULL)
    {
    return;
    }
  
  vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
  if (camera->GetParallelProjection())
    {
    camera->SetParallelScale(camera->GetParallelScale() / factor);
    }
  else
    {
    camera->Dolly(factor);
    if (this->AutoAdjustCameraClippingRange)
      {
      this->CurrentRenderer->ResetCameraClippingRange();
      }
    }
  
  if (this->Interactor->GetLightFollowCamera()) 
    {
    this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }
  
  this->Interactor->Render();
}
//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::MotionAlongVector(double vector[3],double amount, vtkCamera* cam)
{
  double oldcampos[3], oldcamfoc[3];
  cam->GetPosition(oldcampos);
  cam->GetFocalPoint(oldcamfoc);
  // move camera and focus along DirectionOfProjection
  cam->SetPosition(
    oldcampos[0] - amount * vector[0],
    oldcampos[1] - amount * vector[1],
    oldcampos[2] - amount * vector[2]);
  cam->SetFocalPoint(
    oldcamfoc[0] - amount * vector[0],
    oldcamfoc[1] - amount * vector[1],
    oldcamfoc[2] - amount * vector[2]);
}
//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::FinishCamera(vtkCamera* cam)
{
    cam->OrthogonalizeViewUp();
    if (this->RestoreUpVector)
      {
      double delta[3];
      cam->GetViewUp(delta);
      double weight = vtkMath::Dot(this->DefaultUpVector,delta);
      // only correct up if we're close to it already...
      if (weight>0.3) {
        weight = 0.25*fabs(weight);
        delta[0] = delta[0] + (this->DefaultUpVector[0]-delta[0])*weight;
        delta[1] = delta[1] + (this->DefaultUpVector[1]-delta[1])*weight;
        delta[2] = delta[2] + (this->DefaultUpVector[2]-delta[2])*weight;
        cam->SetViewUp(delta);
      }
      }
    if (this->AutoAdjustCameraClippingRange)
      {
      this->CurrentRenderer->ResetCameraClippingRange();
      }
    if (this->Interactor->GetLightFollowCamera())
      {
      this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
      }
}
//----------------------------------------------------------------------------
void nkInteractorStyleEndoCamera::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "RestoreUpVector: "
     << this->RestoreUpVector << "\n";
  os << indent << "DefaultUpVector: "
     << this->DefaultUpVector[0] << " "
     << this->DefaultUpVector[1] << " "
     << this->DefaultUpVector[2] << "\n";
}

