// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_Human.h"
#include "QuestListWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UQuestListWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
		void AddList(FQuestStruct questData, uint8 inputQuestNumber);
	UFUNCTION()
		void RemoveList(int32 inputQuestNumber);
	UFUNCTION()
		void SetVisibleQuestList(bool isOnOff);

	UFUNCTION()
		void ListSort();
private:

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf<class UUserWidget>  questSlotFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TArray <class UQuestListSlot*> questSlot;

	UPROPERTY()
		int8 currentQuestCount = -1;	//-1일때 퀘스트 없는것
};
