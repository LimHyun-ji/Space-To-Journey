// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceEnemy.h"
#include "EnemyHpBar_Widget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEWAR_API UEnemyHpBar_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
		//void SetOwnerEnemy(AEnemy_SpaceGalleon* Enemy) { OwnerEnemy_SpaceGalleon = Enemy; }		
	void SetOwnerEnemy(ASpaceEnemy* Enemy) { OwnerEnemy = Enemy; }

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakObjectPtr<ASpaceEnemy> OwnerEnemy;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HpBar;

};
