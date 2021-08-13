// Fill out your copyright notice in the Description page of Project Settings.


#include "HitWidget.h"
#include "Components/Image.h"

void UHitWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
 	if (isChange)
 	{
 		opacity += GetWorld()->DeltaTimeSeconds;
 	}
 	else
 		opacity -= GetWorld()->DeltaTimeSeconds;
 
 	if (opacity > 0.5 || opacity<0.1)
 	{
 		isChange = !isChange;
 	}
	HitImage->SetOpacity(opacity);
}
