// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_ItemSlotWidget.h"
#include "Components/Image.h"
#include <Components/TextBlock.h>
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"


void UInventory_ItemSlotWidget::SetSlotImage(UTexture2D* inputImage)
{
	itemImage->SetBrushFromTexture(inputImage);
}

void UInventory_ItemSlotWidget::SetAmount(uint8 inputAmount)
{
	if (inputAmount == 0) itemAmount->SetVisibility(ESlateVisibility::Hidden);
	else itemAmount->SetVisibility(ESlateVisibility::Visible);
		
	itemAmount->SetText(FText::AsNumber(inputAmount));
}