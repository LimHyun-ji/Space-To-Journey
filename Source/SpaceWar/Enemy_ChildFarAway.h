// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Planet_EnemyParent.h"
#include "Enemy_ChildFarAway.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AEnemy_ChildFarAway : public APlanet_EnemyParent
{
	GENERATED_BODY()
public:
	AEnemy_ChildFarAway();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class AEnemy_FarAwayAttack> attackClass;

	virtual void AttackMove();
	virtual void ReturnMove();
	virtual void Attack();

};
