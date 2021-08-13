// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UFUNCTION()
		void SetWidgetOwner(APlayer_Human* inputOwner);

	UFUNCTION()
		void SettingQuestText(FText inputNPCName = FText::FromString(""), FText inputTitle = FText::FromString(""), FText inputConversation = FText::FromString(""), FText inputGoalText = FText::FromString(""), FText inputGoalCount = FText::FromString(""));
	UFUNCTION(BlueprintCallable, Category = "Code")
		void ClickedCancel();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void ClickedAccept();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void ClickedClear();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void VisibleAcceptBtn(bool inputOnOff);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void VisibleClearBtn(bool inputOnOff);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* acceptBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* cancelBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* clearBtn;

private:
	UPROPERTY()
		class APlayer_Human* Owner;
	

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* GoalCount;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* GoalText;


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* title;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* conversation;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NPCName;
	
};
