// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestListSlot.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UQuestListSlot : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION()
		void UpdateList(FText inputQuestName, FText inputQuestGoal);
	UFUNCTION()
		void SetQuestNumber(uint8 inputQuestNumber);
	UFUNCTION()
		uint8 GetQuestNumber();
private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* questName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* questGoal;

	UPROPERTY()
		uint8 questNumber = 0;
};
