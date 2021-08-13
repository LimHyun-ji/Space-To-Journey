// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_Widget.h"
#include "Components/Image.h"
#include <Components/TextBlock.h>
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Texture2D.h"
#include "Inventory_ItemSlotWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UInventory_Widget::LoadInventory(TArray <uint8> LoadItemNumber, TArray <uint8> LoadItemAmount, int LoadMoney)
{

	inventory_ItemNumber.Init(0, inventoryScale);
	inventory_ItemAmount.Init(0, inventoryScale);
	
	uint8 count = 0;

	for (uint8 temp : LoadItemNumber)
	{
		inventory_ItemNumber[count] = temp;
		count++;
	}
	count = 0;
	for (uint8 temp : LoadItemAmount)
	{
		inventory_ItemAmount[count] = temp;
		count++;
	}

	money = LoadMoney;

	

	count = 0;
	for (int i = 0; i < inventoryScale; i++)
	{
		UInventory_ItemSlotWidget* tempItemSlotWidget = CreateWidget<UInventory_ItemSlotWidget>(GetWorld(), inventorySlotWidgetFactory);
		inventorySlotWidget.Add(tempItemSlotWidget);		
		inventorySlotWidget[count]->SetSlotImage(Item_IconArray[inventory_ItemNumber[i]]);
		inventorySlotWidget[count]->SetAmount(inventory_ItemAmount[i]);		

		inventorySlotWidget[count]->AddToViewport();

		UserUniformGrid->AddChild(inventorySlotWidget[count]);
		if (UUniformGridSlot* slot = Cast<UUniformGridSlot>(inventorySlotWidget[count]->Slot))
		{
			slot->SetColumn(count % 3);
			slot->SetRow(count / 3);
		}

		count++;
	}
						
	moneyText->SetText(FText::AsNumber(money));
}

void UInventory_Widget::SetInventory(uint8 UpdateInventorySlot)
{
	inventorySlotWidget[UpdateInventorySlot]->SetSlotImage(Item_IconArray[inventory_ItemNumber[UpdateInventorySlot]]);
	inventorySlotWidget[UpdateInventorySlot]->SetAmount(inventory_ItemAmount[UpdateInventorySlot]);
}

bool UInventory_Widget::RemoveInventory(uint8 removeItemNumber,uint8 removeCount)
{
	uint8 count = 0;
	bool isHaveItemNumber = false;

	for (uint8 temp : inventory_ItemNumber)
	{
		if (removeItemNumber == inventory_ItemNumber[count])
		{
			if (inventory_ItemAmount[count] < removeCount) return false;
			inventory_ItemAmount[count] -= removeCount;
			if (inventory_ItemAmount[count] == 0) inventory_ItemNumber[count] = 0;
			isHaveItemNumber = true;
			break;
		}
		count += 1;
	}

	if (!isHaveItemNumber)
	{
		return false;
	}

	SetInventory(count);
	return true;
}


void UInventory_Widget::AddInventory(uint8 AddItemNumber)
{
	uint8 count = 0;
	bool isAlreadyHaveItemNumber = false;

	for (uint8 temp : inventory_ItemNumber)
	{
		if (AddItemNumber == inventory_ItemNumber[count])
		{
			inventory_ItemAmount[count] += 1;
			isAlreadyHaveItemNumber = true;
			break;
		}
		count += 1;
	}

	if (!isAlreadyHaveItemNumber)
	{
		count = 0;

		for (uint8 emptySpace : inventory_ItemNumber)
		{
			if (emptySpace == 0)
			{
				inventory_ItemNumber[count] = AddItemNumber;				
				inventory_ItemAmount[count] += 1;
				break;
			}
			count += 1;
		}
	}


	SetInventory(count);
}

bool UInventory_Widget::AddMoney(int inputMoney)
{
	if (money += inputMoney >= 0)
	{
		money += inputMoney;
		moneyText->SetText(FText::AsNumber(money));
		return true;
	}
	
	return false;	
}

TArray <uint8> UInventory_Widget::GetInventory_ItemNumber()
{
	return inventory_ItemNumber;
}

TArray <uint8> UInventory_Widget::GetInventory_ItemAmount()
{
	return inventory_ItemAmount;
}

int UInventory_Widget::GetMoney()
{
	return money;
}


float UInventory_Widget::GetItemAmount(uint8 inputItemNumber)
{
	int8 count = -1;

	for (uint8 temp : inventory_ItemNumber)
	{
		count += 1;
		if (temp == inputItemNumber)
		{

			break;
		}		
	}

	if (count == -1) return 0;

	return inventory_ItemAmount[count];
}

void UInventory_Widget::SetInventoryVisible(bool isOnOff)
{
	if (!isOnOff)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	
}