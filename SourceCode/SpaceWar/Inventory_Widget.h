// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_Human.h"
#include "Inventory_Widget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEWAR_API UInventory_Widget : public UUserWidget
{
	GENERATED_BODY()

public:



	//void SetInventory(TArray <uint8> Inputinventory_ItemNumber, TArray <uint8> Inputinventory_ItemAmount);

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TArray <class UTexture2D*> Item_IconArray;

	UFUNCTION()
		void AddInventory(uint8 AddItemNumber);
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool RemoveInventory(uint8 removeItemNumber, uint8 removeCount);	//제거하려는 아이템 수가 충분하면 true를 리턴, 그렇지 않으면 제거하지 않고 false 리턴
	UFUNCTION()
		void SetInventory(uint8 UpdateInventorySlot);
	UFUNCTION()
		bool AddMoney(int inputMoney);

	UFUNCTION()
		TArray <uint8> GetInventory_ItemNumber();
	UFUNCTION()
		TArray <uint8> GetInventory_ItemAmount();
	UFUNCTION()
		int GetMoney();

	UPROPERTY()
		TArray <uint8> testArray;
	UFUNCTION()
		void LoadInventory(TArray <uint8> LoadItemNumber, TArray <uint8> LoadItemAmount, int LoadMoney);

	UFUNCTION()
		float GetItemAmount(uint8 inputItemNumber);

	UFUNCTION()
		void SetInventoryVisible(bool isOnOff);

protected:



private:
	//UPROPERTY()
		//TArray <class UImage> inventory_ItemImage;



	//==========================================

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* moneyText;

	UPROPERTY()
		TArray <uint8> inventory_ItemNumber;
	UPROPERTY()
		TArray <uint8> inventory_ItemAmount;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		uint8 inventoryScale = 20;	
	
	UPROPERTY()
		int money = 0;



	//==========================================
	
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess))
		TSubclassOf<class UUserWidget>  inventorySlotWidgetFactory;
	UPROPERTY()
		TArray <class UInventory_ItemSlotWidget*> inventorySlotWidget;
	UPROPERTY()
		class UInventory_ItemSlotWidget* inventorySlotWidget2;


	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess))
		FVector2D firstSlotOffset;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess))
		FVector2D slotTermOffset;

	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* UserCanvas;
	UPROPERTY(meta = (BindWidget))
		class UUniformGridPanel* UserUniformGrid;

};
