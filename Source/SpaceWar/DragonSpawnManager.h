// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonSpawnManager.generated.h"

UCLASS()
class SPACEWAR_API ADragonSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADragonSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* VehicleSpawnPosition;

	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* EnemySpawnPosition;

	UPROPERTY()
		class AEnemy_Dragon* Dragon;
	UPROPERTY()
		class APlayer_Human* target;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class AVehicle_Dragon> DragonVehicleClass;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class AEnemy_Dragon> DragonEnemyClass;

	UPROPERTY()
		bool canStartTimer = false;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float spawnDelay = 7.0f;

	UPROPERTY()
		float currentSpawnDelay = 0.0f;

	UPROPERTY()
		bool isSpawnVehicle = false;
	UPROPERTY()
		bool isSpawnEnemy=false;
};
