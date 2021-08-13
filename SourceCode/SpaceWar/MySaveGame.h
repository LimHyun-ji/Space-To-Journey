// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		float currentSpaceShipHp;
	UPROPERTY()
		float SpaceShipFireRate;
	UPROPERTY()
		float SpaceShipDamage;
	UPROPERTY()
		float SpaceShipMaxMagazine;
	UPROPERTY()
		float SpaceShipMaxHp;
	UPROPERTY()
		bool isSpaceTutorialDone;

	UPROPERTY()
		bool isHumanTutorialDone;

	UPROPERTY()
		int money;

	UPROPERTY()
		bool isCatchDragon;

	UPROPERTY()
		FTransform spaceSpawnPosition;

	UPROPERTY()
		TArray <uint8> inventory_ItemNumber;
	UPROPERTY()
		TArray <uint8> inventory_ItemAmount;

	UPROPERTY()
		TArray <uint8> upgradeLevel;
};
