// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager_FA.generated.h"

UCLASS()
class SPACEWAR_API ASpawnManager_FA : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnManager_FA();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* BoundBox;

	UPROPERTY(EditDefaultsOnly)
		float spawnAmount = 0;

	UPROPERTY(EditDefaultsOnly)
		float spawnCountDown = 5;
	UPROPERTY(EditDefaultsOnly)
		float countDownTemp = 0;
	UPROPERTY(EditAnywhere)
		FVector InitLocation;
	UPROPERTY(EditAnywhere)
		FRotator InitRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class AEnemy_FarAway> Enemy;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class AEnemy_FarAway* EnemyFind;

	UFUNCTION()
		void SpawnEnemy();
};
