// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Planet_EnemyParent.h"
#include "Enemy_ChildAnimal.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AEnemy_ChildAnimal : public APlanet_EnemyParent
{
	GENERATED_BODY()

public:
	AEnemy_ChildAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* attackInteraction;

	UPROPERTY()
		bool setRotating = false;
	UPROPERTY()
		bool isTracking = false;
	UPROPERTY()
		FRotator randomGoalRotation;

	virtual void AttackMove();
	virtual void ReturnMove();

	UFUNCTION()
		void RandomMove();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnAttackOverlap_Begin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnAttackOverlap_End(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category="Code")
		bool GetIsTracking();
};
