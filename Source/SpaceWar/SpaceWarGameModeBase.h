// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceWarGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class SPACEWAR_API ASpaceWarGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASpaceWarGameModeBase();
	virtual void InitGameState() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void FadeOutEffect(float FadeInOutTime, APlayerController* Controller);
	UFUNCTION()
		void FadeInEffect(float FadeInOutTime, APlayerController* Controller);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void LoadNewLevel(FName LevelName);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void LoadLevel(FName LevelName);
	UFUNCTION()
		void SaveData_SpaceShip();
	UFUNCTION()
		void LoadData_SpaceShip();
	UFUNCTION()
		void SaveData_Human();
	UFUNCTION()
		void LoadData_Human();
	UFUNCTION()
		void SwitchControllerMaster();
	UFUNCTION()
		void LoadLevelAnimation();
	UFUNCTION()
		void SettingAfterLoadLevel();


	UPROPERTY()
		TArray <uint8> testArray;
	


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = true))
		TSubclassOf<class UUserWidget> mainmenuWidgetFactory;
	UPROPERTY()
		class UUserWidget* mainmenuWidget;

	UPROPERTY()
		float currentSpaceShipHp;
	UPROPERTY()
		bool currentPlayerType;	//0 : 우주선, 1 : 인간
	UPROPERTY()
		class APlayerCPP* player_SpaceShip;
	
	UPROPERTY()
		class APlayer_Human* player_Human;

	UPROPERTY()
		class UMySaveGame* saveData;
	UPROPERTY()
		bool isLoadNewLevel;		

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowAccessPrivate = true))
		float duringDarkScene = 1.5;
	UPROPERTY()
		float currentDuringDarkScene = 0;
	UPROPERTY()
		bool isLoadAnimation = false;
	UPROPERTY()
		FName NextPlanetName;
	UPROPERTY()
		class APlayerController* MyController;

};
