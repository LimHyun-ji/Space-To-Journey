// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Enemy_Animal.h"
#include "SpawnManager.generated.h"

UCLASS()
class SPACEWAR_API ASpawnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* BoundBox;

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
		TSubclassOf <class APlanet_EnemyParent> EnemyFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class APlanet_EnemyParent* EnemyFind;

	UFUNCTION()
		void SpawnEnemy();
};
