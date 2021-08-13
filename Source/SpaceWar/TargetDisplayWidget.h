// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy_SpaceGalleon.h"
#include "Enemy_Hunter.h"
#include "Enemy_FreeFly.h"
#include "TargetDisplayWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEWAR_API UTargetDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//void SetOwnerEnemy(AEnemy_SpaceGalleon* Enemy) { OwnerEnemy_SpaceGalleon = Enemy; }		
	void SetOwnerEnemy(AActor* Enemy) { OwnerEnemy = Enemy; }

	UFUNCTION()
		void MoveArrow(float moveSpeed);
	UFUNCTION()
		void SetArrowInitPosition();

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakObjectPtr<AActor> OwnerEnemy;


	UPROPERTY(meta = (BindWidget))
		class UImage* targetDisplayArrow_Up;
	UPROPERTY(meta = (BindWidget))
		class UImage* targetDisplayArrow_Left;
	UPROPERTY(meta = (BindWidget))
		class UImage* targetDisplayArrow_Right;
	UPROPERTY(meta = (BindWidget))
		class UImage* targetDisplayArrow_Under;

private:

	UPROPERTY()
		float Init_UpPosition_X = 0;
	UPROPERTY()
		float Init_UnderPosition_X = 0;
	UPROPERTY()
		float Init_LeftPosition_Y = 0;
	UPROPERTY()
		float Init_RightPosition_Y = 0;
};
