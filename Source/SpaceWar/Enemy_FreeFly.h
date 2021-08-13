// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceEnemy.h"
#include "Enemy_FreeFly.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_FreeFly : public ASpaceEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy_FreeFly(const FObjectInitializer& ObjectInitializer);

	

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* firePosition1;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* firePosition2;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Move();
	UFUNCTION()
		void Turn();

	UFUNCTION()
		void Fire() override;


	UFUNCTION()
		void Dead();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollisionEnter_Overlab(AActor* OtherActor);

	


private:
	
	

	UPROPERTY()
		float targetDistance;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
		float spread = 5;

	UPROPERTY()
		FVector dir = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
		float speed = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
		float turnSpeed = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		float stopDistance = 3500;

	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
		float fireActivateDistance = 5000;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
		float trackActivateDistance = 30000;

	UPROPERTY()
		bool isFireActivation = false;
	UPROPERTY()
		bool isTrackActivation = false;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float collisionDamage = 50;

	/// HP°ü·Ã


	

};
