// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy_ThrowingBall.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_ThrowingBall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy_ThrowingBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* MeshComp;

	UPROPERTY()
		float lifeTime = 3.0f;
	UPROPERTY()
		float currentLifeTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int attackDamage;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float speed = 1000;


	UFUNCTION()
		void LifeTime();
	UFUNCTION()
		int GetAttackDamage();
	UFUNCTION()
		void Move();
};
