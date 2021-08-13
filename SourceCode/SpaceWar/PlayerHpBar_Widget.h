// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCPP.h"
#include "PlayerHpBar_Widget.generated.h"

/**
 *
 */

//어쩌다 보니 전체 우주선 UI로 바뀜
UCLASS(Abstract)
class SPACEWAR_API UPlayerHpBar_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	//void SetOwnerEnemy(AEnemy_SpaceGalleon* Enemy) { OwnerEnemy_SpaceGalleon = Enemy; }		
	void SetOwner(APlayerCPP* player) { Owner = player; }

	void SetMessage(FString inputMessage);
	void SetReloadBarVisible(bool inputOnOff);
	void SetAimingPointVisible(uint8 isSpeedMode);	// 0 : normal , 1: speed  2 : hidden,
	
	UFUNCTION()
		void MissileCoolTimeDoneWork();
	UFUNCTION()
		void SetMissileCoolTimeDoneWork();
	
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

	TWeakObjectPtr<APlayerCPP> Owner;
	
	

private:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HpBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ReloadBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MissileCoolTimeBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* currentMagazine;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* maxMagazine;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* message;

	UPROPERTY(meta = (BindWidget))
		class UImage* inclination_Stick_Horizontal;
	UPROPERTY(meta = (BindWidget))
		class UImage* inclination_Stick_Vertical;

	UPROPERTY(meta = (BindWidget))
		class UImage* aimingPoint_Normal;
	UPROPERTY(meta = (BindWidget))
		class UImage* aimingPoint_Speed;

	UPROPERTY(meta = (BindWidget))
		class UImage* missileCoolTimeDoneImage;
	UPROPERTY()
		float missileCoolTimeDoneOpacity = 1;
	UPROPERTY()
		float missileCoolTimeDoneScale = 1;
	UPROPERTY()
		bool isMissileCoolTimeDoneOpacityWork = false;

	UPROPERTY(meta = (BindWidget))
		class UImage* pauseBG;

};
