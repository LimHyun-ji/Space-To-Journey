// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanetEnemy.h"
#include "EnemyBoss.generated.h"

UCLASS()
class SPACEWAR_API AEnemyBoss : public APlanetEnemy
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBoss(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
