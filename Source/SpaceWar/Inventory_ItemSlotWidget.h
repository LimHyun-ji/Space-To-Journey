// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UInventory_ItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		void SetSlotImage(UTexture2D* inputImage);
		
	UFUNCTION()
		void SetAmount(uint8 inputAmount);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* itemSlotButton;


private:

	UPROPERTY(meta = (BindWidget))
		class UImage* itemImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* itemAmount;

	

};
