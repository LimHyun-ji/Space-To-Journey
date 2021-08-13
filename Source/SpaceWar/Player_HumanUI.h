// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_Human.h"
#include "Player_HumanUI.generated.h"

/**
 *
 */
UCLASS(Abstract)
class SPACEWAR_API UPlayer_HumanUI : public UUserWidget
{
	GENERATED_BODY()

public:
	//void SetOwnerEnemy(AEnemy_SpaceGalleon* Enemy) { OwnerEnemy_SpaceGalleon = Enemy; }		
	void SetOwner(APlayer_Human* player) { Owner = player; }

	UFUNCTION(BlueprintCallable, Category = "Code")
		void GamePlayBtn_Clicked();
	UFUNCTION()
		void GamePauseUIVisible(bool OnOff);
	
	UFUNCTION(BlueprintCallable, Category = "Code")
		void TutorialBtn_Clicked();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void QuitBtn_Clicked();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* playBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* tutorialBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* quitBtn;

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakObjectPtr<APlayer_Human> Owner;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HpBar_1;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MPBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* currentMagazine;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* maxMagazine;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* currentHP;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* fullHP;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Pistol;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PistolBG;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Spike;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* SpikeBG;

	UPROPERTY(meta = (BindWidget))
		class UImage* pauseBG;

};
