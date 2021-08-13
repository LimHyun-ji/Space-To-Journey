// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_Human.h"
#include "HitWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UHitWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetOwner(APlayer_Human* player) { Owner = player; }
private:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakObjectPtr<APlayer_Human> Owner;

	UPROPERTY(meta = (BindWidget))
		class UImage* HitImage;
	UPROPERTY()
		float opacity=0.1f;
	UPROPERTY()
		bool isChange=true;
};
