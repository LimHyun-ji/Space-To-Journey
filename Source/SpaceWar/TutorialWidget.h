// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void SetOwner(class APlayerCPP* inputPlayer);
	UFUNCTION()
		void SetOwner_Human(class APlayer_Human* inputPlayer_Human);

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TArray <class UTexture2D*> TutorialPage;	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* previousBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* nextBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* skipBtn;

	UFUNCTION(BlueprintCallable, Category = "Code")
		void ClickedPreviousBtn();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void ClickedNextBtn();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void ClickedSkipBtn();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void SetCurrentPage_0();

private:
	UPROPERTY()
		uint8 currentPage = 0;

	UPROPERTY(meta = (BindWidget))
		class UImage* TutorialImage;

	UPROPERTY()
		class APlayerCPP* player;
	UPROPERTY()
		class APlayer_Human* player_Human;

	UPROPERTY()
		uint8 totalPage;
};
