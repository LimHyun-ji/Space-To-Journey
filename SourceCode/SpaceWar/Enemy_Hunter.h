// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceEnemy.h"
#include "Enemy_Hunter.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_Hunter : public ASpaceEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy_Hunter(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* firePosition1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;



	UPROPERTY()
		FVector dir = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float turnSpeed = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float speed = 1000;


	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float spread = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float collisionDamage = 20;

	UPROPERTY()
		bool isTrackPlayer = false;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float trackPlayerDistance = 50000;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float avoidTime = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float fireActivationDistance = 10000;
	UPROPERTY()
		float currentAvoidTime = 0;
	UPROPERTY()
		bool isAvoid = false;
	UPROPERTY()
		float avoidTurn_Pitch;
	UPROPERTY()
		float currentAvoidTurn_Pitch;
	UPROPERTY()
		float avoidTurn_Yaw;
	UPROPERTY()
		float currentAvoidTurn_Yaw;
	UPROPERTY()
		bool isFireActivation = false;

	//////////	FUNCTION

	UFUNCTION()
		void Move();
	UFUNCTION()
		void Turn();
	UFUNCTION()
		void Avoid();
	UFUNCTION()
		void Fire() override;

	UFUNCTION()
		void Dead();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollisionEnter_Overlab(AActor* OtherActor);


};
