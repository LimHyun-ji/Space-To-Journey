// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDisplayWidget.h"
#include "Components/Image.h"

void UTargetDisplayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UTargetDisplayWidget::MoveArrow(float moveSpeed)
{
	targetDisplayArrow_Up->SetRenderTranslation(FVector2D(0, Init_UpPosition_X - moveSpeed));
	targetDisplayArrow_Under->SetRenderTranslation(FVector2D(0, Init_UnderPosition_X + moveSpeed));
	targetDisplayArrow_Left->SetRenderTranslation(FVector2D(Init_LeftPosition_Y  - moveSpeed, 0));
	targetDisplayArrow_Right->SetRenderTranslation(FVector2D(Init_RightPosition_Y + moveSpeed, 0));
}

void UTargetDisplayWidget::SetArrowInitPosition()
{
	targetDisplayArrow_Up->SetRenderTranslation(FVector2D(0, Init_UpPosition_X));
	targetDisplayArrow_Under->SetRenderTranslation(FVector2D(0, Init_UnderPosition_X));
	targetDisplayArrow_Left->SetRenderTranslation(FVector2D(Init_LeftPosition_Y, 0));
	targetDisplayArrow_Right->SetRenderTranslation(FVector2D(Init_RightPosition_Y, 0));
}